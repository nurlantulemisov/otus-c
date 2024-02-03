CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11
LDFLAGS = -lcurl -ljansson
SOURCES = ./src/main.c ./src/curl.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = myprogram

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) libccurl.a
	$(CC) $(CFLAGS) src/main.o -o $@ $(LDFLAGS) -Lsrc -lccurl

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

libccurl.a:
	ar cr src/libccurl.a src/curl.o

clean:
	rm -f $(OBJECTS) $(EXECUTABLE) src/libccurl.a
