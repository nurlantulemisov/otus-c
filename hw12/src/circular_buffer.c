#include <circular_buffer.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

cir_buffer_t *create_cir_buffer(size_t max) {
  cir_buffer_t *buf = (cir_buffer_t *)malloc(sizeof(cir_buffer_t));
  if (buf == NULL) {
    perror("failed init cir_buffer_t");
    return NULL;
  }
  buf->max = max;

  buf->tasks = (task_t **)malloc(sizeof(task_t *) * max);
  if (buf->tasks == NULL) {
    perror("invalid malloc tasks");
    return NULL;
  }

  buf->head = 0;
  buf->tail = 0;
  buf->full = false;

  return buf;
}

bool cir_buffer_put(cir_buffer_t *buf, task_t *t) {
  if (buf->full) {
    return false;
  }

  buf->tasks[buf->head] = t;

  buf->head = (buf->head + 1) % buf->max;

  buf->full = buf->head == buf->tail;

  return true;
}

task_t *cir_buffer_get(cir_buffer_t *buf) {
  buf->full = false;

  task_t *t = buf->tasks[buf->tail];
  buf->tasks[buf->tail] = NULL;

  buf->tail = (buf->tail + 1) % buf->max;

  return t;
}

size_t cir_buffer_size(cir_buffer_t *buf) {
  if (buf->full) {
    return buf->max;
  }
  if (buf->head >= buf->tail) {
    return buf->head - buf->tail;
  }
  return buf->max + buf->head - buf->tail;
}

static void task_free(cir_buffer_t *buf, task_t **tasks) {
  if (tasks != NULL) {
    for (int i = 0; i < buf->max; i++) {
      if (tasks[i] != NULL) {
        free(tasks[i]->arg);
        free(tasks[i]);

        tasks[i] = NULL;
      }
    }
    free(tasks); // Освобождение памяти для массива tasks
  }
}

void cir_buffer_free(cir_buffer_t *buf) {
  if (buf != NULL) {
    task_free(buf, buf->tasks);
    free(buf);
  }
}
