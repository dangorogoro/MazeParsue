CXX := g++
INCLUDE := \
	-I../../MazeParsue
C_SRCS := $(wildcard ../*.cpp  ../tools/genMaze/cppCode/*.cpp)
test.out: main.cpp
	g++ -O0 -g -std=c++14 $(C_SRCS) $(INCLUDE) -o $@ main.cpp -std=c++17
clean:
	rm test.out
