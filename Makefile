CC = gcc
CFLAGS = -Wall -Wextra -I./include
LDFLAGS = -lncurses

SRC_DIR = ./src
OBJ_DIR = ./obj
BIN_DIR = ./bin

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))
EXECUTABLE = $(BIN_DIR)/tic_tac_toe

$(EXECUTABLE): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -rf $(OBJ_DIR)/*.o $(EXECUTABLE)
