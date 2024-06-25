# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++23 -Wall

# Target executable
TARGET = ladder_logic

# Source files
SRCS = main.cpp LadderLogicParser.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)

# Rule to build the target executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Rule to build object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to clean the build directory
clean:
	rm -f $(OBJS) $(TARGET)

# Rule to run the program with default arguments
run: $(TARGET)
	./$(TARGET) logic.txt

# Rule to run the program with custom arguments
run-custom: $(TARGET)
	./$(TARGET) logic.txt -n 5

.PHONY: all clean run run-custom
