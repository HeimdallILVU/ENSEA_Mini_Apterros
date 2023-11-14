CC := gcc
CFLAGS := -Wall -Werror -I./include 
LDFLAGS := -lbcm2835 -lpthread -lm -lwiringPi -lrt

SRC := main.c src/heimdall.c src/heimdall_fast.c  # Add your source files here
OBJ := $(SRC:.c=.o)

TARGET := main

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)