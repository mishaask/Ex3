CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic -g
LDFLAGS := -lsfml-graphics -lsfml-window -lsfml-system

SRCS := main.cpp HomeScreen.cpp Game.cpp
OBJS := $(SRCS:.cpp=.o)

all: main

main: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJS) main


# # Makefile

# all: main

# main: main.o HomeScreen.o
# 	g++ main.o HomeScreen.o -o main $(shell pkg-config --libs sfml-graphics sfml-window sfml-system)

# main.o: main.cpp HomeScreen.hpp
# 	g++ -c main.cpp $(shell pkg-config --cflags sfml-graphics sfml-window sfml-system)

# HomeScreen.o: HomeScreen.cpp HomeScreen.hpp
# 	g++ -c HomeScreen.cpp $(shell pkg-config --cflags sfml-graphics sfml-window sfml-system)

# clean:
# 	rm -f *.o main
