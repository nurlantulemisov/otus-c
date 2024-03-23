cmake_minimum_required(VERSION 3.10)

project(UsdFileSize LANGUAGES C)

# Поиск пакета GTK4
find_package(PkgConfig REQUIRED)
pkg_check_modules(CONFUSE REQUIRED libconfuse)

# Указываем путь к заголовочным файлам GTK4
include_directories(${CONFUSE_INCLUDE_DIRS})

# # Указываем путь к библиотекам GTK4
link_directories(${CONFUSE_LIBRARY_DIRS})

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

add_compile_options(-Wall -Wextra -Wpedanti)

add_executable(usd src/main.c)

target_link_libraries(usd ${CONFUSE_LIBRARIES}) 

