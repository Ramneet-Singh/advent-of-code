CC=clang++
CFLAGS=-std=c++17 -Wall -Wextra -Werror -Wpedantic -Wno-unused-parameter -Wno-unused-variable -Wno-unused-private-field -Wno-unused-function -I include

# All files in src directory
SRCS=$(wildcard src/*.cpp)
# All files in include directory with .h or .hpp
INCS=$(wildcard include/*.h) $(wildcard include/*.hpp)
# Object files for each source file in bin directory
OBJS=$(patsubst src/%.cpp, bin/%.o, $(SRCS))
# Name of the executable in bin directory
EXECUTABLE=bin/prog

# Rule to build the executable in bin directory
$(EXECUTABLE): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

# Rule to compile each source file into an object file in bin directory
bin/%.o: src/%.cpp $(INCS)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

# Rule to clean the object files and the executable in bin directory
clean:
	rm -f $(OBJS) $(EXECUTABLE)