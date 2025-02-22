CC = gcc
CFLAGS = -Wall -Wno-error -O2

SRCDIR = src
BUILDDIR = build

SRCS = $(SRCDIR)/main.c $(SRCDIR)/cache.c $(SRCDIR)/config.c $(SCRDIR)/memory.c
OBJS = $(BUILDDIR)/main.o $(BUILDDIR)/cache.o $(BUILDDIR)/config.o $(BUILDDIR)/memory.o
TARGET = $(BUILDDIR)/cache_sim

all: $(TARGET)
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(TARGET): $(BUILDDIR) $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(BUILDDIR)

.PHONY: all clean

