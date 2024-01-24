CC = gcc
SRC = $(wildcard src/*.c) $(wildcard src/*/*.c)
OBJ = $(SRC:.c=.o)
BIN = bti
CFLAGS = -Wall -Wextra -Werror -pedantic -ansi

%.o: %.c
	$(CC) $(CFLAGS) -g -c $< -o $@

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $(BIN)

.PHONY: valgrind clean
valgrind: $(BIN)
	@valgrind -s --leak-check=full ./$(BIN)
clean:
	@rm $(OBJ)
	@rm $(BIN)
