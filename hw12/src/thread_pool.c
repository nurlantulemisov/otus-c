#include "thread_pool.h"
#include "circular_buffer.h"
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static void *thread_executor(void *args);

thread_pool_t *create_pool(size_t num_threads, cir_buffer_t *buf) {
  thread_pool_t *t = (thread_pool_t *)malloc(sizeof(thread_pool_t));

  t->threads = (pthread_t *)malloc(sizeof(pthread_t) * num_threads);
  t->cap = num_threads;
  t->stop = false;
  t->b = buf;

  pthread_mutex_init(&t->mu, NULL);
  pthread_cond_init(&t->full, NULL);
  pthread_cond_init(&t->ready, NULL);

  for (int i = 0; i < num_threads; i++) {
    pthread_create(&t->threads[i], NULL, (void *)thread_executor, (void *)t);
  }

  return t;
}

void executeTask(task_t *task) {
  task->function(task->arg);
  // printf("Task %s is executed by thread %ld\n", (char *)task->arg,
  // pthread_self());
}

static void *thread_executor(void *args) {
  thread_pool_t *pool = (thread_pool_t *)args;
  while (1) {
    pthread_mutex_lock(&pool->mu);
    while (cir_buffer_size(pool->b) == 0 && !pool->stop) {
      pthread_cond_wait(&pool->ready, &pool->mu); // wait signal for exec task
    }

    if (pool->stop) {
      pthread_mutex_unlock(&pool->mu);
      pthread_exit(NULL);
    }

    task_t *t = cir_buffer_get(pool->b);
    if (t == NULL) {
      pthread_mutex_unlock(&pool->mu);
      pthread_exit(NULL);
    }

    pthread_cond_signal(&pool->full);
    pthread_mutex_unlock(&pool->mu);

    executeTask(t);
    free(t->arg);
    free(t);
  }
}

void add_to_pool(thread_pool_t *t_pool, char *s) {
  pthread_mutex_lock(&t_pool->mu);

  while (t_pool->b->full) {
    printf("buffer full. waiting\n");
    pthread_cond_wait(&t_pool->full, &t_pool->mu);
  }

  task_t *task = (task_t *)malloc(sizeof(task_t));
  task->function = (void *)printf;
  task->arg = (void *)s;

  if (!cir_buffer_put(t_pool->b, task)) {
    printf("should be never");
    free(task->arg);
    free(task); // todo подумать
    pthread_mutex_unlock(&t_pool->mu);
    return;
  }

  pthread_mutex_unlock(&t_pool->mu);
  pthread_cond_signal(&t_pool->ready);
}

void stop_pool(thread_pool_t *t_pool) {
  pthread_mutex_lock(&t_pool->mu);
  t_pool->stop = true;
  pthread_cond_broadcast(&t_pool->ready);
  pthread_mutex_unlock(&t_pool->mu);

  for (int i = 0; i < t_pool->cap; ++i) {
    pthread_join(t_pool->threads[i], NULL);
  }

  free(t_pool->threads);
  cir_buffer_free(t_pool->b);

  pthread_mutex_destroy(&t_pool->mu);
  pthread_cond_destroy(&t_pool->ready);
  pthread_cond_destroy(&t_pool->full);

  free(t_pool);
}
