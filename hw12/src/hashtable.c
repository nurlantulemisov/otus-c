#include <hashtable.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/_types/_size_t.h>

hash_t *create_hash_t(size_t sz) {
  hash_t *t = (hash_t *)malloc(sizeof(hash_t));
  t->cap = sz;
  t->size = 0;
  t->buckets = (bucket_t **)malloc(sizeof(bucket_t *) * sz);
  t->mus = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * sz);
  for (size_t i = 0; i < sz; i++) {
    pthread_mutex_init(&t->mus[i], NULL);
  }
  return t;
}

static uint64_t hash(hash_t *t, const char *key) {
  uint64_t hash = 0;
  for (int i = 0; key[i] != '\0'; i++) {
    hash = hash * 31 + key[i];
  }
  return hash % t->cap;
}

void hash_t_put(hash_t *t, const char *key, uint64_t value) {
  uint32_t bucket_id = hash(t, key);

  pthread_mutex_lock(&t->mus[bucket_id]);

  bucket_t *current = t->buckets[bucket_id];
  while (current != NULL) {
    if (strncmp(current->key, key, strlen(current->key)) == 0) {
      current->val = value;
      pthread_mutex_unlock(&t->mus[bucket_id]);

      return;
    }
    if (current->next == NULL)
      break;
    current = current->next;
  }

  bucket_t *buc = (bucket_t *)malloc(sizeof(bucket_t));
  buc->key = strndup(key, strlen(key));
  buc->val = value;
  if (current == NULL) {
    t->buckets[bucket_id] = buc;
  } else {
    current->next = buc;
  }

  pthread_mutex_unlock(&t->mus[bucket_id]);

  return;
}

void hash_t_inc(hash_t *t, const char *key) {
  uint32_t bucket_id = hash(t, key);

  pthread_mutex_lock(&t->mus[bucket_id]);

  bucket_t *current = t->buckets[bucket_id];
  while (current != NULL) {
    if (strncmp(current->key, key, strlen(current->key)) == 0) {
      current->val++;
      pthread_mutex_unlock(&t->mus[bucket_id]);

      return;
    }
    if (current->next == NULL)
      break;
    current = current->next;
  }

  bucket_t *buc = (bucket_t *)malloc(sizeof(bucket_t));
  buc->key = strndup(key, strlen(key));
  buc->val = 1;
  if (current == NULL) {
    t->buckets[bucket_id] = buc;
  } else {
    current->next = buc;
  }

  pthread_mutex_unlock(&t->mus[bucket_id]);

  return;
}

bool hash_t_get(hash_t *t, const char *key, uint64_t *value) {
  uint32_t bucket_id = hash(t, key);
  pthread_mutex_lock(&t->mus[bucket_id]);

  if (t->buckets[bucket_id] == NULL)
    return false;

  bucket_t *current = t->buckets[bucket_id];
  while (current != NULL) {
    if (strncmp(current->key, key, strlen(current->key)) == 0) {
      *value = current->val;
      pthread_mutex_unlock(&t->mus[bucket_id]);
      return true;
    }
    current = current->next;
  }

  pthread_mutex_unlock(&t->mus[bucket_id]);
  return false;
}

void print_hash_t(hash_t *t) {
  if (t == NULL) {
    printf("Hash table is NULL\n");
    return;
  }

  printf("Printing hash table values:\n");

  for (size_t i = 0; i < t->cap; ++i) {
    bucket_t *current_bucket = t->buckets[i];
    while (current_bucket != NULL) {
      printf("Key: %s, Value: %llu\n", current_bucket->key,
             current_bucket->val);
      current_bucket = current_bucket->next;
    }
  }
}

void hash_t_free(hash_t *t) {
  for (size_t i = 0; i < t->cap; i++) {
    bucket_t *current = t->buckets[i];
    while (current != NULL) {
      bucket_t *next = current->next;
      free((void *)current->key);
      free(current);
      current = next;
    }
    // Уничтожаем мьютексы
    pthread_mutex_destroy(&t->mus[i]);
  }
  free(t->buckets);
  free(t->mus);
  free(t);
}
