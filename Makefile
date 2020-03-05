PROJECT_NAME := test
CXX := g++
LD := g++
INCLUDE := \
	-Ilib
OUTPUT_DIR := build
CPP_SRCS := $(wildcard main.cpp lib/*.cpp tools/genMaze/cppCode/*.cpp)
CXX_FLAGS := -O1 -g -std=c++17 -Wall

OBJS := $(patsubst %,$(OUTPUT_DIR)/%, $(CPP_SRCS:.cpp=.o))


.PHONY: all clean

all: $(OUTPUT_DIR) $(OUTPUT_DIR)/$(PROJECT_NAME).out

$(OUTPUT_DIR):
	@mkdir $(OUTPUT_DIR)

$(OUTPUT_DIR)/$(PROJECT_NAME).out: $(OBJS)
	@echo "LD $@"
	@$(LD) -Wl,-Map,$(@:.out=.map) -o $@ $^

$(OUTPUT_DIR)/%.o: %.cpp
	@echo "CXX $<"
	@if [ ! -d $(dir $@) ]; then mkdir -p $(dir $@); fi
	@$(CXX) $(CXX_FLAGS) $(INCLUDE) -MMD -MP  -MT$@ -o $@  -c $<
#test.out: main.cpp
#	$(CXX) $(CXX_FLAGS) $(CPP_SRCS) $(INCLUDE) -o $@ main.cpp
clean:
	@rm -rf $(OUTPUT_DIR)/*
