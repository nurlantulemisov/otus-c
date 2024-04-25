CC = gcc CFLAGS = -Wall - Wextra - Wpedantic - std = c11 LDFLAGS =
  -ljansson - lcurl - Lsrc
  - lccurl SOURCES =./ src / main.c./ src / curl.c OBJECTS = $(SOURCES
							       :.c =.o)
    EXECUTABLE = myprogram LIBRARY = src
					 / libccurl.a

					     all : $(EXECUTABLE)

						     $(EXECUTABLE)
    : $(OBJECTS) $(LIBRARY) $(CC) $(CFLAGS) $(OBJECTS)
				       - o $ @$(LDFLAGS)

					     $(LIBRARY)
    : src / curl.o ar cr $ @$ ^

				       .c.o : $(CC) $(CFLAGS) - c $ < -o $ @

									clean
    : rm - f $(OBJECTS) $(EXECUTABLE) $(LIBRARY)
