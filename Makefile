CC=gcc
CFLAGS=-Iinclude
SRC=src/main.c
OBJ=$(SRC:.c=.o)
TARGET=vm

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

clean:
	rm -f src/*.o $(TARGET)
