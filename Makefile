CC = gcc
CFLAGS = -Wall -Werror -O2

SRCDIR = src

SRCS = $(SRCDIR)/main.c $(SRCDIR)/cache.c $(SRCDIR)/config.c
OBJS = main.o cache.o config.o
TARGET = cache_sim

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean

