CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall
RR_OBJS = round_robin.o
FCFS_OBJS = FCFS.o

all: round_robin

round_robin: $(RR_OBJS)
	$(CXX) $(CXXFLAGS) -o round_robin $(RR_OBJS)

FCFS: $(FCFS_OBJS)
	$(CXX) $(CXXFLAGS) -o FCFS $(FCFS_OBJS)

round_robin.o: round_robin.cpp
FCFS.o: FCFS.cpp

clean:
	rm -f *.o round_robin FCFS
