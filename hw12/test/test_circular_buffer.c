#include "circular_buffer.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

// Тестирование создания кольцевого буфера
void test_create_cir_buffer() {
  size_t max = 5;
  cir_buffer_t *buf = create_cir_buffer(max);
  assert(buf != NULL); // Проверяем, что буфер создан успешно
  assert(buf->tasks != NULL); // Проверяем, что массив заданий создан
  assert(buf->head == 0); // Проверяем, что head равен 0
  assert(buf->tail == 0); // Проверяем, что tail равен 0
  assert(buf->max ==
         max); // Проверяем, что максимальный размер установлен корректно
  assert(!buf->full); // Проверяем, что буфер не полон
  cir_buffer_free(buf);
}

// Тестирование добавления и извлечения заданий из кольцевого буфера
void test_buffer_operations() {
  size_t max = 3;
  cir_buffer_t *buf = create_cir_buffer(max);
  assert(buf != NULL);

  task_t task1 = {.function = NULL, .arg = NULL};
  task_t task2 = {.function = NULL, .arg = NULL};

  // Проверяем добавление задания в буфер
  bool success = cir_buffer_put(buf, &task1);
  assert(success); // Ожидаем успешное добавление задания
  assert(buf->head == 1); // После добавления одного задания, head должен
                          // указывать на следующий элемент

  // Проверяем добавление второго задания
  success = cir_buffer_put(buf, &task2);
  assert(success); // Ожидаем успешное добавление задания
  assert(buf->head == 2); // После добавления второго задания, head должен
                          // указывать на следующий элемент

  // Проверяем извлечение задания из буфера
  task_t *retrieved_task = cir_buffer_get(buf);
  assert(retrieved_task ==
         &task1); // Ожидаем получить первое добавленное задание
  assert(buf->tail == 1); // После извлечения одного задания, tail должен
                          // указывать на следующий элемент

  // Проверяем извлечение второго задания
  retrieved_task = cir_buffer_get(buf);
  assert(retrieved_task ==
         &task2); // Ожидаем получить второе добавленное задание
  assert(buf->tail == 2); // После извлечения второго задания, tail должен
                          // указывать на следующий элемент

  // Проверяем, что буфер пуст после извлечения всех заданий
  assert(!cir_buffer_get(
      buf)); // Ожидаем, что извлечение из пустого буфера вернет NULL

  cir_buffer_free(buf);
}

void test_buffer_operations_full_buffer() {
  size_t max = 3;
  cir_buffer_t *buf = create_cir_buffer(max);
  assert(buf != NULL);

  task_t task1 = {.function = NULL, .arg = NULL};
  task_t task2 = {.function = NULL, .arg = NULL};
  task_t task3 = {.function = NULL, .arg = NULL};
  task_t task4 = {.function = NULL, .arg = NULL};

  // Проверяем добавление задания в буфер
  bool success = cir_buffer_put(buf, &task1);
  assert(success); // Ожидаем успешное добавление задания
  success = cir_buffer_put(buf, &task2);
  assert(success); // Ожидаем успешное добавление задания
  success = cir_buffer_put(buf, &task3);
  assert(success); // Ожидаем успешное добавление задания

  // Пытаемся добавить ещё одно задание, не должно быть успешно из-за превышения
  // максимальной вместимости
  success = cir_buffer_put(buf, &task4);
  assert(!success); // Ожидаем неуспешное добавление задания из-за превышения
                    // максимальной вместимости

  cir_buffer_free(buf);
}

int main() {
  test_create_cir_buffer();
  test_buffer_operations();
  test_buffer_operations_full_buffer();
  printf("All tests passed successfully!\n");
  return 0;
}
