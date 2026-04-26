CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = mysh
SRC = src/mysh.c

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)