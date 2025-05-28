CXX        := g++
CXXFLAGS   := -std=c++17 -Wall -Wextra -pedantic -g
LDFLAGS    := -lsfml-graphics -lsfml-window -lsfml-system

SRCS := \
    main.cpp \
    HomeScreen.cpp \
    Game.cpp \
    Player.cpp \
    Baron.cpp \
    Governor.cpp \
    Spy.cpp \
    General.cpp \
    Judge.cpp \
    Merchant.cpp

OBJS := $(SRCS:.cpp=.o)

all: main

main: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJS) main



# CXX := g++
# CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic -g
# LDFLAGS := -lsfml-graphics -lsfml-window -lsfml-system

# SRCS := main.cpp HomeScreen.cpp Game.cpp
# OBJS := $(SRCS:.cpp=.o)

# all: main

# main: $(OBJS)
# 	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)

# %.o: %.cpp
# 	$(CXX) $(CXXFLAGS) -c $<

# clean:
# 	rm -f $(OBJS) main


