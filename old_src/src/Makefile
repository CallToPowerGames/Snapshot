# Snapshot makefile

all: Snapshot

CFLAGS=-fPIC -g -Wall `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`
INCLUDE = 
FREE_LIBS = 

Snapshot: snapshot.o
	$(CXX) $(INCLUDE) $(CFLAGS) $? -o Snapshot $(LIBS) $(FREE_LIBS)

snapshot.o: snapshot.cpp libimgfilter.hpp
	$(CXX) -c $(CFLAGS) snapshot.cpp -o snapshot.o

clean:
	rm -rf *.o Snapshot.dSYM

clean-all: clean
	rm -rf Snapshot *.exe
