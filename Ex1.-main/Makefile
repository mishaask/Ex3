#mishaaskarov@gmail.com

CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra 

.PHONY: all Main runtest valgrind clean

all: main test_runner

main: Graph.o Algorithms.o main.o
	$(CXX) $(CXXFLAGS) -o main Graph.o Algorithms.o main.o

test_runner: Graph.o Algorithms.o test.o
	$(CXX) $(CXXFLAGS) -o test_runner Graph.o Algorithms.o test.o

Graph.o: Graph.cpp
	$(CXX) $(CXXFLAGS) -c Graph.cpp -o Graph.o

Algorithms.o: Algorithms.cpp
	$(CXX) $(CXXFLAGS) -c Algorithms.cpp -o Algorithms.o

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

test.o: test.cpp
	$(CXX) $(CXXFLAGS) -c test.cpp -o test.o

Main: main
	./main

test: test_runner
	./test_runner

valgrind: main
	valgrind --leak-check=full ./main

clean:
	rm -f main test_runner main.o Graph.o Algorithms.o test.o
