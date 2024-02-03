CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11
SOURCES = ./src/main.c ./src/hashtable.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = myprogram

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) libhashtable.a
	$(CC) $(OBJECTS) -L. -lhashtable -o $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

libhashtable.a:
	ar cr libhashtable.a src/hashtable.o

clean:
	rm -f $(OBJECTS) $(EXECUTABLE) libhashtable.a
