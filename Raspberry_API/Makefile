CC := gcc
CFLAGS := -Wextra -I./include 
LDFLAGS := -lbcm2835

SRC := main.c $(wildcard src/*.c)   # Add your source files here
OBJ := $(SRC:.c=.o)

TARGET := main

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
