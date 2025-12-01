//--------------------------------------
//  SRTF (converted from your FCFS)
//  Minimal changes only
//--------------------------------------

//--------------------------------------
//  Headers
//--------------------------------------
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <limits>

using namespace std;

//--------------------------------------
//  Variables
//--------------------------------------

// Problem Definition
int no_processes;
int tick;
int context_switch_time;
int quantum_time;
vector<int> arrival_times;
vector<int> execution_times;

//--------------------------------------
//  Variables
//--------------------------------------

// Scheduling Environment
queue<int> ready_queue;
int running_process;
int next_process;
vector<int> remaining_execution_times;
vector<int> execution_start_times;
vector<int> departure_times;
vector<int> running_times;
vector<int> last_got_cpu;
vector<bool> admitted;
vector<bool> already_dispatched;
vector<bool> terminated;
bool first_process_dispatched;
bool system_is_in_context_switch;
bool system_is_running_a_process;
int time_passed_from_current_quantum;
int time_passed_from_current_context_switch;

//--------------------------------------
//  Variables
//--------------------------------------

// Results
vector<int> waiting_times;
vector<int> response_times;
vector<int> turnaround_times;
float utilization;
float throughput;
int current_time;
int total_time;

//--------------------------------------
//  Auxiliary Functions
//--------------------------------------

// Read the number of processes, 
// Tick, context Switch, Quantum 
// Arrival times, execution times 
void read_problem_definition() {
    cout << "Please enter the number of processes: ";
    cin >> no_processes;
    arrival_times.resize(no_processes);
    execution_times.resize(no_processes);
    remaining_execution_times.resize(no_processes);
    execution_start_times.resize(no_processes);
    departure_times.resize(no_processes);
    running_times.resize(no_processes);
    last_got_cpu.resize(no_processes);
    cout << "-----------------------------------------\n";
    cout << "Please enter the tick interval: ";
    cin >> tick;
    cout << "Please enter the context switch time: ";
    cin >> context_switch_time;
    for (int i = 0; i <= no_processes - 1; i++) {
        cout << "-----------------------------------------\n";
        cout << "Please enter the arrival time for " << "p" << to_string(i) << ": ";
        cin >> arrival_times[i];
        cout << "Please enter the execution time for " << "p" << to_string(i) << ": ";
        cin >> execution_times[i];
        remaining_execution_times[i] = execution_times[i];
    }
    int max = execution_times[0];
    for (int i = 1; i < execution_times.size(); i++)
        if (execution_times[i] > max)
            max = execution_times[i];
    quantum_time = max;
}

// Initializations
void get_ready_to_schedule() {
    current_time = 0;
    running_process = -1;
    first_process_dispatched = false;
    system_is_in_context_switch = false;
    system_is_running_a_process = false;
    time_passed_from_current_quantum = 0;
    time_passed_from_current_context_switch = 0;
    admitted.resize(no_processes, false);
    already_dispatched.resize(no_processes, false);
    terminated.resize(no_processes, false);

    waiting_times.clear();
    response_times.clear();
    turnaround_times.clear();
    waiting_times.resize(no_processes);
    response_times.resize(no_processes);
    turnaround_times.resize(no_processes);
}

// For scheduling cases where
// the scheduler should do nothing
void do_nothing() {}

// Update current time,
// remaining execution times,
// time passed from current quantum,
// time passed from current context switch 
// at the end of each tick interval
void update_time() {
    current_time += tick;
    if (system_is_running_a_process) {
        running_times [running_process] += tick;
        remaining_execution_times [running_process] -= tick;
        time_passed_from_current_quantum += tick;
    }
    if (system_is_in_context_switch) {
        time_passed_from_current_context_switch += tick;
    }
}

// Returns true if there is any
// arrived process, not admitted to the ready queue
bool there_exist_unadmitted_processes() {
    bool to_be_returned = false;
    for (int i = 0; i <= no_processes - 1; i++) {
        if (arrival_times[i] <= current_time && !admitted[i]) {
            to_be_returned = true;
            break;
        }
    }
    return to_be_returned;
}

// The index of the first process 
// whose entry in admitted vector 
// is false
int first_unadmitted_processes_index() {
    for (int i = 0; i <= no_processes - 1; i++) {
        if (arrival_times[i] <= current_time && !admitted[i]) {
            return i;
        }
    }
    return no_processes;
}

// The index of the unadmitted process 
// that has arrived earliest (among all 
// unadmitted processes)
int oldest_unadmitted_process_index() {
    int to_be_returned = first_unadmitted_processes_index();
    if (to_be_returned == no_processes) return to_be_returned;
    for (int i = 0; i <= no_processes - 1; i++) {
        if (!admitted[i] && arrival_times[i] <= current_time && arrival_times[i] < arrival_times [to_be_returned]) {
            to_be_returned = i;
        }
    }
    return to_be_returned;
}

// ---------------------------
// New/changed helper functions
// ---------------------------

// Find index of process with shortest remaining execution time
// among running process and processes in ready_queue.
// Returns -1 if nothing exists.
int find_shortest_overall() {
    int shortest = -1;
    int shortest_time = 9999999;

    // Check running process
    if (system_is_running_a_process && running_process >= 0 && !terminated[running_process]) {
        shortest = running_process;
        shortest_time = remaining_execution_times[running_process];
    }

    // Check ready queue
    queue<int> temp = ready_queue;
    while (!temp.empty()) {
        int p = temp.front();
        temp.pop();
        if (!terminated[p] && remaining_execution_times[p] < shortest_time) {
            shortest = p;
            shortest_time = remaining_execution_times[p];
        }
    }

    return shortest;
}

// Remove and return the process in ready_queue with the shortest remaining time.
// If ready_queue empty, returns -1.
int remove_shortest_from_ready_queue() {
    if (ready_queue.empty()) return -1;
    int shortest = ready_queue.front();
    queue<int> temp;
    ready_queue.pop();

    while (!ready_queue.empty()) {
        int p = ready_queue.front();
        ready_queue.pop();
        if (remaining_execution_times[p] < remaining_execution_times[shortest]) {
            temp.push(shortest);
            shortest = p;
        } else {
            temp.push(p);
        }
    }
    ready_queue = temp;
    return shortest;
}

// The processes that have arrived after 
// the previous tick time,
// are admitted to the ready queue
void admit_newly_arrived_processes() {
    while(there_exist_unadmitted_processes()) {
        int newp = oldest_unadmitted_process_index();
        if (newp == no_processes) break;
        ready_queue.push(newp);
        admitted[newp] = true;

        if (system_is_running_a_process) {
            int shortest = find_shortest_overall();

            if (shortest != running_process && shortest != -1) {
                system_is_running_a_process = false;
                system_is_in_context_switch = true;
                time_passed_from_current_context_switch = 0;

                if (!terminated[running_process]) {
                    ready_queue.push(running_process);
                }

                queue<int> new_q;
                bool removed = false;
                while (!ready_queue.empty()) {
                    int p = ready_queue.front();
                    ready_queue.pop();
                    if (!removed && p == shortest) {
                        removed = true;
                        continue;
                    }
                    new_q.push(p);
                }
                ready_queue = new_q;
                next_process = shortest;
            }
        }
    }
}

// Make the scheduling decisions at the 
// end of a tick interval and
// do the necessary actions
void make_decisions_and_updates() {

    // If no process is running,
    // No context switch is taking place,
    // and ready que is empty,
    // but other processe are yet to arrive
    if (ready_queue.empty() && !system_is_in_context_switch && !system_is_running_a_process) {
        do_nothing();
    }

    // When the system needs to dispatch the very
    // first process
    // (Some processes are in the ready queue and
    // no process has been dispatched yet
    if (!ready_queue.empty() && !first_process_dispatched) {
        next_process = remove_shortest_from_ready_queue();
        system_is_in_context_switch = true;
        system_is_running_a_process = false;
        first_process_dispatched = true;
        time_passed_from_current_context_switch = 0;
    }

    // A context switch has been started,
    // but context switch time has not
    // passed yet
    if (system_is_in_context_switch && time_passed_from_current_context_switch < context_switch_time) {
        do_nothing();
    }

    // A context switch has been completed
    if (system_is_in_context_switch && time_passed_from_current_context_switch >= context_switch_time) {
        system_is_in_context_switch = false;
        system_is_running_a_process = true;
        time_passed_from_current_quantum = 0;
        running_process = next_process;
        if (!already_dispatched [running_process] ) {
            execution_start_times [running_process] = current_time;
            already_dispatched [running_process] = true;
        }
    }

    // The system is running a process and the
    // running process has finished its computations
    if (system_is_running_a_process && remaining_execution_times [running_process] <= 0) {
        system_is_running_a_process = false;
        terminated [running_process] = true;
        departure_times [running_process] = current_time;
        if (!ready_queue.empty()) {
            system_is_in_context_switch = true;
            time_passed_from_current_context_switch = 0;
            next_process = remove_shortest_from_ready_queue(); 
        }
    }

    // The system is running a process,
    // The running process has not completed its
    // computations yet, but the context switch
    // time has expired (quantum expired)
    if (system_is_running_a_process && remaining_execution_times [running_process] > 0 && time_passed_from_current_quantum >= quantum_time) {
        if (!ready_queue.empty()) {
            system_is_running_a_process = false;
            system_is_in_context_switch = true;
            time_passed_from_current_context_switch = 0;
            next_process = remove_shortest_from_ready_queue();
            // push the old running back to ready queue
            ready_queue.push(running_process);
        } else {
            time_passed_from_current_quantum = 0;
        }
    }

    // The system is running a process,
    // The running process has not completed
    // its computations and the quantum has not
    // expired
    if (system_is_running_a_process && remaining_execution_times[running_process] > 0 && time_passed_from_current_quantum < quantum_time) {
        do_nothing();
    }
}

// Returns true if all processes 
// have completed their computations 
// (terminated)
bool all_processes_terminated() {
    bool to_be_returned = true;
    for (int i = 0; i <= no_processes - 1; i++) {
        if (!terminated[i]) {
            to_be_returned = false;
            break;
        }
    }
    return to_be_returned;
}

// The scheduling function 
void scehdule() {
    while(true) {
        admit_newly_arrived_processes();
        make_decisions_and_updates();
        update_time();
        if (all_processes_terminated()) {
            break;
        }
    }
}

// Calculate cpu efficient time, 
// response times
// turnaround times 
// waiting times, 
// CPU Utilization 
// and throughput
void calculate_results() {
    total_time = current_time;
    waiting_times.resize(no_processes);
    response_times.resize(no_processes);
    turnaround_times.resize(no_processes);
    int cpu_efficient_time = 0;
    for (int i = 0; i <= no_processes - 1; i++) {
        cpu_efficient_time += execution_times[i];
        turnaround_times[i] = departure_times[i] - arrival_times[i];
        waiting_times[i] = turnaround_times[i] - execution_times[i];
        response_times[i] = execution_start_times[i] - arrival_times[i];
    }
    throughput = (float)no_processes / total_time;
    utilization = (float)cpu_efficient_time / total_time;
}


// Output the results in a tabulated format
void print_results() {
    cout << "\nProcess\tArrival\tStart\tResponse\tDeparture\tTurnaround\tWaiting\n";
    for (int i = 0; i < no_processes; i++) {
        cout << "P" << to_string(i) << "\t"
            << arrival_times[i] << "\t"
            << execution_start_times[i] << "\t"
            << response_times[i] << "\t\t"
            << departure_times[i] << "\t\t"
            << turnaround_times[i] << "\t\t"
            << waiting_times[i] << "\n";
    }
    cout << "\n----------------------------------\n";
    cout << "Throughput: " << throughput << endl;
    cout << "CPU utilization: " << utilization << endl;
}

//--------------------------------------
//  main() function
//--------------------------------------

int main() {
    read_problem_definition();
    get_ready_to_schedule();
    scehdule();
    calculate_results();
    print_results();
    return 0;
}
