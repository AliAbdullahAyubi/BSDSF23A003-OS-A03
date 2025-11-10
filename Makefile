CC = gcc
CFLAGS = -Iinclude -Wall -g
LDFLAGS =
SRC = src/main.c src/shell.c src/execute.c
OBJ = $(SRC:.c=.o)
BIN = bin/myshell

all: $(BIN)

$(BIN): $(OBJ)
	mkdir -p bin
	$(CC) $(OBJ) -o $(BIN) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ) $(BIN)

.PHONY: all clean

