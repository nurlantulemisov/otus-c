#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/_pthread/_pthread_mutex_t.h>

struct bucket_t {
  const char *key;
  uint64_t val;
  struct bucket_t *next;
};

typedef struct bucket_t bucket_t;

typedef struct {
  size_t size;
  size_t cap;
  bucket_t **buckets;
  pthread_mutex_t *mus;
} hash_t;

hash_t *create_hash_t(size_t sz);
void hash_t_put(hash_t *t, const char *key, uint64_t value);
void hash_t_inc(hash_t *t, const char *key);
bool hash_t_get(hash_t *t, const char *key, uint64_t *value);
void hash_t_free(hash_t *t);
void print_hash_t(hash_t *t);

#endif // !HASHTABLE_H
