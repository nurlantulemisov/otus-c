#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include "circular_buffer.h"

#include <pthread.h>
#include <stdbool.h>

typedef struct {
  pthread_t *threads;
  size_t cap;
  cir_buffer_t *b;
  pthread_mutex_t mu;
  pthread_cond_t full; // waiting for new task
  pthread_cond_t ready;

  bool stop; // for stopping
} thread_pool_t;

thread_pool_t *
create_pool(size_t num_threads, cir_buffer_t *buf);

void
add_to_pool(thread_pool_t *t_pool, char *filename, callback cb);

void
stop_pool(thread_pool_t *t_pool);

#endif // !THREAD_POOL_H
