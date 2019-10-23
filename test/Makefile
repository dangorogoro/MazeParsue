CXX := g++
INCLUDE := \
	-I../../MazeParsue
C_SRCS := $(wildcard ../*.cpp  ../tools/genMaze/cppCode/*.cpp)
test.out: main.cpp
	g++ -O0 -g -std=c++17 $(C_SRCS) $(INCLUDE) -o $@ main.cpp -Wall
clean:
	rm test.out
