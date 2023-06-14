CC = gcc
CFLAGS = -Wall -Wextra -g

# list of source files
SRCS = main.c compress.c decompress.c huffman.c heap.c

# name of the executable
TARGET = program

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

clean:
	del $(TARGET).exe