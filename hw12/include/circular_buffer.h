#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <stdbool.h>
#include <stddef.h>

typedef void (*callback)(void *);

typedef struct {
  void (*function)(
    void *arg); // Указатель на функцию, которая должна быть выполнена
  void *arg;	// Аргумент для передачи в функцию
} task_t;

typedef struct {
  task_t **tasks;
  size_t head;
  size_t tail;
  size_t max;
  bool full;
} cir_buffer_t;

cir_buffer_t *
create_cir_buffer(size_t max);

void
cir_buffer_free(cir_buffer_t *buf);

bool
cir_buffer_put(cir_buffer_t *buf, task_t *t);

task_t *
cir_buffer_get(cir_buffer_t *buf);

size_t
cir_buffer_size(cir_buffer_t *buf);

#endif // !CIRCULAR_BUFFER_H
