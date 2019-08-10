CXX := g++
INCLUDE := \
	-I../../MazeParsue
C_SRCS := $(wildcard ../*.cpp)
test.out: main.cpp
	g++ -O0 -g -std=c++17 $(C_SRCS) $(INCLUDE) -o $@ main.cpp
