# Makefile for compiling the tic_tac_toe program

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -I./include

# Linker flags
LDFLAGS = -lncurses

# Directories
SRC_DIR = ./src
OBJ_DIR = ./obj
BIN_DIR = ./bin

# Source files
SRC = $(wildcard $(SRC_DIR)/*.c)

# Object files
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

# Executable file
EXECUTABLE = $(BIN_DIR)/tic_tac_toe

# Rule to build the executable
$(EXECUTABLE): $(OBJ) | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Rule to build object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create object directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Create bin directory if it doesn't exist
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Clean rule
.PHONY: clean
clean:
	rm -rf $(OBJ_DIR)/*.o $(EXECUTABLE)
