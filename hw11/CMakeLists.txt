cmake_minimum_required(VERSION 3.10)

  project(mmap LANGUAGES C)

    set(CMAKE_C_STANDARD 11) set(CMAKE_C_STANDARD_REQUIRED True)
      set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
	  "${CMAKE_CURRENT_SOURCE_DIR}/build/bin")

	add_compile_options(-Wall - Wextra - Wpedantic)

	  add_executable(mmap src / main.c)
