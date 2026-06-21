CXX = g++
CXXFLAGS = -std=c++17 -Wall

all: main tests

main: main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o main

tests: tests.cpp
	$(CXX) $(CXXFLAGS) -O2 tests.cpp -o tests

run-main: main
	./main

run-tests: tests
	./tests

clean:
	rm -f main tests

.PHONY: all run-main run-tests clean