#//mishaaskarov@gmail.com
CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -g -I.
LDFLAGS  = -lsfml-graphics -lsfml-window -lsfml-system

# application sources and objects
SRCS    = main.cpp HomeScreen.cpp Game.cpp Player.cpp Baron.cpp Governor.cpp Spy.cpp General.cpp Judge.cpp Merchant.cpp
OBJS    = main.o HomeScreen.o Game.o Player.o Baron.o Governor.o Spy.o General.o Judge.o Merchant.o

# single test source
TEST_SRC = test.cpp
TEST_OBJ = test.o

.PHONY: all clean

all: main test

main: $(OBJS)
	g++ $(CXXFLAGS) -o main $(OBJS) $(LDFLAGS)

test: Game.o Player.o Baron.o Governor.o Spy.o General.o Judge.o Merchant.o $(TEST_OBJ)
	g++ $(CXXFLAGS) -o test Game.o Player.o Baron.o Governor.o Spy.o General.o Judge.o Merchant.o test.o $(LDFLAGS)

# compile rules for each .cpp → .o
main.o:       main.cpp        ; g++ $(CXXFLAGS) -c main.cpp
HomeScreen.o: HomeScreen.cpp  ; g++ $(CXXFLAGS) -c HomeScreen.cpp
Game.o:       Game.cpp        ; g++ $(CXXFLAGS) -c Game.cpp
Player.o:     Player.cpp      ; g++ $(CXXFLAGS) -c Player.cpp
Baron.o:      Baron.cpp       ; g++ $(CXXFLAGS) -c Baron.cpp
Governor.o:   Governor.cpp    ; g++ $(CXXFLAGS) -c Governor.cpp
Spy.o:        Spy.cpp         ; g++ $(CXXFLAGS) -c Spy.cpp
General.o:    General.cpp     ; g++ $(CXXFLAGS) -c General.cpp
Judge.o:      Judge.cpp       ; g++ $(CXXFLAGS) -c Judge.cpp
Merchant.o:   Merchant.cpp    ; g++ $(CXXFLAGS) -c Merchant.cpp

test.o: test.cpp
	g++ $(CXXFLAGS) -c test.cpp -o test.o

clean:
	rm -f *.o main test

# CXX        := g++
# CXXFLAGS   := -std=c++17 -Wall -Wextra -pedantic -g
# LDFLAGS    := -lsfml-graphics -lsfml-window -lsfml-system

# SRCS := \
#     main.cpp \
#     HomeScreen.cpp \
#     Game.cpp \
#     Player.cpp \
#     Baron.cpp \
#     Governor.cpp \
#     Spy.cpp \
#     General.cpp \
#     Judge.cpp \
#     Merchant.cpp

# OBJS := $(SRCS:.cpp=.o)

# all: main

# main: $(OBJS)
# 	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)

# test: $(SRCS) test.cpp
#     g++ $(CXXFLAGS) $(filter-out main.cpp HomeScreen.cpp Game.cpp, $(SRCS)) test.cpp -o test
#     ./test

# %.o: %.cpp
# 	$(CXX) $(CXXFLAGS) -c $<

# clean:
# 	rm -f $(OBJS) main



