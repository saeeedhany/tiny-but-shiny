CC = gcc
CFLAGS = -Wall -Wextra -g $(shell pkg-config --cflags sdl2)  
LDLIBS = $(shell pkg-config --libs sdl2)                     

TARGET = paint_project

SRCS = paint.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
