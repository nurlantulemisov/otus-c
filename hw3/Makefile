CC = gcc CFLAGS = -Wall - Wextra - Wpedantic - std =
  c11 SOURCES =./ src / main.c./ src / hashtable.c OBJECTS = $(SOURCES
							       :.c =.o)
    EXECUTABLE = myprogram LIBRARY = src
					 / libhashtable.a

					     all : $(EXECUTABLE)

						     $(EXECUTABLE)
    : $(OBJECTS) $(LIBRARY) $(CC)
	$(OBJECTS) - Lsrc - lhashtable
				       - o $ @

					   $(LIBRARY)
    : src / hashtable.o ar cr $ @$ ^

				       .c.o : $(CC) $(CFLAGS) - c $ < -o $ @

									clean
    : rm - f $(OBJECTS) $(EXECUTABLE) $(LIBRARY)
