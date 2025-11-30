CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall
RR_OBJS = round_robin.o
FCFS_OBJS = FCFS.o
SRTF_OBJS = SRTF.o

all: round_robin

round_robin: $(RR_OBJS)
	$(CXX) $(CXXFLAGS) -o round_robin $(RR_OBJS)

FCFS: $(FCFS_OBJS)
	$(CXX) $(CXXFLAGS) -o FCFS $(FCFS_OBJS)

SRTF: $(SRTF_OBJS)
	$(CXX) $(CXXFLAGS) -o SRTF $(SRTF_OBJS)

round_robin.o: round_robin.cpp
FCFS.o: FCFS.cpp
SRTF.o: SRTF.cpp

clean:
	rm -f *.o round_robin FCFS SRTF
