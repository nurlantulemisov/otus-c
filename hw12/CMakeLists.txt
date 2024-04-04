cmake_minimum_required(VERSION 3.10)

project(mmap LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build/bin")

include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB SOURCES "src/*.c")
add_library(MyLibs STATIC ${SOURCES})

add_subdirectory(${PROJECT_SOURCE_DIR}/test) # Для тестов

add_compile_options(-Wall -Wextra -Wpedantic)

add_executable(logparser src/main.c)
target_link_libraries(logparser MyLibs)


