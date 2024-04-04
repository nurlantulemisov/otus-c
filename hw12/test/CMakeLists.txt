cmake_minimum_required(VERSION 3.5)

project(CircularBufferTest)

# Подключаем исполняемый файл с тестами
add_executable(test_circular_buffer test_circular_buffer.c)

# Подключаем статическую библиотеку с реализацией кольцевого буфера и функций
target_link_libraries(test_circular_buffer MyLibs)

# Включаем использование CTest
enable_testing()

# Добавляем тест, который запускает исполняемый файл с тестами
add_test(NAME circular_buffer_tests COMMAND test_circular_buffer)

# Добавляем тестовые файлы
file(COPY test_circular_buffer.c DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

