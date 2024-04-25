cmake_minimum_required(VERSION 3.10)

  project(logger LANGUAGES C)

    set(CMAKE_C_STANDARD 11) set(CMAKE_C_STANDARD_REQUIRED True)
      set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

	add_compile_options(-Wall - Wextra - Wpedantic)

	  add_library(my - logger STATIC src / logger.c)

	    add_executable(logger src / main.c)

	      target_link_libraries(logger PUBLIC my - logger)
