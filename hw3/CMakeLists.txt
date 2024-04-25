cmake_minimum_required(VERSION 3.10) project(myprogram C)

  set(CMAKE_C_STANDARD 11)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic")

#Add your source files here
      set(SOURCES./ src / main.c./ src / hashtable.c)

#Create an executable
	add_executable(myprogram ${SOURCES})

#Create a library
	  add_library(hashtable STATIC./ src / hashtable.c)

#Link the executable with the library
	    target_link_libraries(myprogram PRIVATE hashtable)
