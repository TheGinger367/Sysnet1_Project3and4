//--------------------------------------
//  Round Robin Scheduling Algorithm
//--------------------------------------

//--------------------------------------
//  Headers
//--------------------------------------
#include <iostream>
#include <vector>
#include <string>
#include <queue>

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
int time passed_from_current_context_switch;

//--------------------------------------
//  Variables
//--------------------------------------

// Results
vector<int> waiting_times(no_processes);
vector<int> response_times (no_processes);
vector<int> turnaround_times (no_processes);
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
    cout << "Please enter the time quantum size: ";
    cin >> quantum_time;
    for (int i = 0; i <= no_processes 1; i++) {
        cout << "-----------------------------------------\n";
        cout << "Please enter the arrival time for " << "p" << to_string(i) << ": ";
        cin >> arrival_times[i];
        cout << "Please enter the execution time for " << "p" << to_string(i) << ": ";
        cin >> execution_times[i];
        remaining_execution_times[i] = execution_times[i];\
    }
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
    return to be returned;
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
    int to be returned = first_unadmitted_processes_index();
    for (int i = 1; i <= no_processes - 1; i++) {
        if (arrival_times[i] < arrival_times [to_be_returned] && !admitted[i] && arrival_times[i] <= current_time) {
            to_be_returned = 1;
        }
    }
    return to be returned;
}

// The processes that have arrived after 
// the previous tick time,
// are admitted to the ready queue
void admit_newly_arrived_processes() {
    while(there_exist_unadmitted_processes()) {
        ready_queue.push(oldest_unadmitted_process_index()); 
        admitted [oldest_unadmitted_process_index()] = true;
    }
}

// Make the scheduling decisions at the 
// end of a tick interval and
// do the necessary actions
void make decisions_and_updates() {

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
        next_process = ready_queue.front();
        ready_queue.pop();
        system_is_in_context_switch = true;
        system_is_running_a_process = false;
        first_process_dispatched = true;
        time_passed_from_current_context_switch = 0;
    }

    // A context switch has been started,
    // but context switch time has not
    // passed yet
    if (system is_in_context_switch & time_passed_from_current_context_switch < context_switch_time) {
        do_nothing();
    }

    // A context switch has been completed
    if (system_is_in_context_switch && time_passed_from_current_context_switch >= context_switch_time) {
        system_is_in_context_switch = false;
        system_is_running_a_process = true;
        time_passed_from_current_quantum = 0;
        running process next_process;
        if (!already_dispatched [running_process] ) {
            execution_start_times [running_process] = current_time;
            already_dispatched [running_process] = true;
        }
    }

    // The system is rnning a process and the
    // running process has finished its computations
    if (system_is_running_a_process && remaining_execution_times [running_process] <= 0) {
        system_is_running_a_process = false;
        terminated [running_process] = true;
        departure_times [running_process] = current_time;
        if (!ready_queue.empty()) {
            system_is_in_context_switch = true;
            time_passed_from_current_context_switch = 0;
            next_process = ready_queue.front(); 
            ready_queue.pop();
        }
    }

    // The system is running a process,
    // The running process has not completed its
    // computations yet, but the context switch
    // time has expired
    if (system is running_a_process && remaining_execution_tines [running_process] > 0 && time passed_from_current_quantum >= qu antun time) {
        if (!ready_queue.empty()) {
            system is running_a_process = false;
            system_is_in_context_switch = true;
            time_passed_from_current_context_switch = 0;
            next_process ready queue.front();
            ready_queue.pop();
            ready_queue.push(running_process);
        } else {
            time_passed_from_current_quantum = 0;
        }
    }

    // The system is running a process,
    // The running process has not completed
    // its computations and the quantum has not
    // expired
    if (system_is_running_a_process && remaining_execution_times[running_process] > 0 && time_passed_from_current_quantum < qua
    ntum_tine) {
        do_nothing();
    }
}

// Returns true if all processes 
// have completed their computations 
// (terminated)
bool all_processes_terminated() {
    bool to be returned = true;
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
//response times
// turnaround times 
// waiting times, 
// CPU Utilization 
// and throughput
void calculate_results() {
    waiting_times.resize(no_processes);
    response times.resize(no_processes);
    turnaround_times.resize(no_processes);
    int cpu_efficient_time = 0;
    for (int i = 0; i <= no_processes - 1; i++) {
        cpu_efficient_time += execution_times[i];
        turnaround_times[i] = departure_times[i] – arrival_times[i];
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
        cout << "p" << to_string(i) << "\t"
            << arrival_times[i] << "\t"
            << execution_start_times[i] << "\t"
            << response_times[i] << "\t\t"
            << departure_times[i] << "\t\t"
            << turnaround_times[i] << "\t\t"
            << waiting_times[i] << "\n";
    }
    cout << "\n----------------------------------\n";
    cout << "Throughput: " << throughput << endl;
    cout << "CPU utilization: " << utilization <<< endl;
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
}