#include "hashtable.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

/**
 * Выделяет память и инициализирует новую хэш-таблицу.
 *
 * @param sz Размер хэш-таблицы.
 * @param collision Параметр столкновения, определяющий максимальное количество
 * коллизий при добавлении элементов.
 * @return Указатель на новую хэш-таблицу, или NULL в случае ошибки выделения
 * памяти.
 */
HTable *
new_table(size_t sz, uint32_t collision) {
  HTable *p = malloc(sizeof(HTable));
  p->size = sz;
  p->cap = 0;
  p->collision = collision;
  bucket *ptr = (bucket *) malloc(sz * sizeof(bucket));
  if(!ptr) {
    return NULL;
  }
  memset(ptr, 0, sz * sizeof(bucket));
  p->values = ptr;
  return p;
}

size_t
hash(HTable *t, const char *key) {
  uint32_t h = 0;
  size_t keyLength = strlen(key);
  for(size_t i = 0; i < keyLength; i++) {
    /*
	Умножение на 31 в данной хэш-функции является распространенной практикой
       в алгоритмах хэширования строк. Выбор числа 31 основан на эмпирических
       данных и характеристиках английского языка. Это простое нечетное простое
       число, и умножение на нечетное простое число помогает более эффективно
       распределять символы, уменьшая количество коллизий в хэш-таблице.
    */
    h = h * 31 + key[i];
  }
  return h % t->size;
}

bool
grow_bucket_size(HTable *t) {
  size_t old_size = t->size;
  bucket *oldBuckets = t->values;
  t->size *= 2;
  bucket *ptr = (bucket *) malloc(t->size * sizeof(bucket));
  if(!ptr) {
    return false;
  }
  memset(ptr, 0, t->size * sizeof(bucket));

  for(size_t i = 0; i < old_size; i++) {
    if(oldBuckets[i].key != NULL) {
      size_t idx = hash(t, oldBuckets[i].key);
      ptr[idx].key = oldBuckets[i].key;
      ptr[idx].v = oldBuckets[i].v;
    }
  }

  t->values = ptr;

  free(oldBuckets);

  return true;
}

size_t
find_index(HTable *t, const char *key, size_t initialIndex) {
  size_t index = initialIndex;

  for(size_t i = 0; i < t->collision; i++) {
    const char *curr_key = t->values[index].key;
    if(curr_key != NULL && strncmp(curr_key, key, strlen(curr_key)) == 0) {
      return index;
    }
    index = (index + 1) % t->size;
  }

  return SIZE_MAX; // Indicate not found
}

bool
replace_value(HTable *t, const char *key, int32_t value, size_t index) {
  size_t foundIndex = find_index(t, key, index);

  if(foundIndex != SIZE_MAX) {
    t->values[foundIndex].v = value;
    return true;
  }

  return false;
}

uint32_t
get(HTable *t, const char *key) {
  size_t index = hash(t, key);
  size_t foundIndex = find_index(t, key, index);

  if(foundIndex != SIZE_MAX) {
    return t->values[foundIndex].v;
  }

  return UINT32_MAX;
}

bool
insert(HTable *t, const char *key, int32_t value, size_t index) {
  size_t i = 0;

  for(i = 0; i < t->collision; i++) {
    if(t->values[index].key == NULL) {
      t->values[index].v = value;
      char *ptr = (char *) malloc((strlen(key) + 1) * sizeof(char));
      if(!ptr) {
	printf("cant alloc %s\n", key);
	return false;
      }
      strcpy(ptr, key);
      t->values[index].key = ptr;
      t->cap++;

      return true;
    }
    index = (index + 1) % t->size;
  }

  return false;
}

/**
 * Добавляет элемент или заменяет значение в хэш-таблице.
 *
 * @param t Указатель на хэш-таблицу.
 * @param key Ключ элемента.
 * @param value Значение элемента.
 * @return true, если элемент был успешно добавлен, и false, если добавление не
 * удалось.
 *
 * Если при добавлении элемента количество элементов в хэш-таблице превышает
 * выделенную память, происходит перераспределение памяти с помощью функции
 * grow_bucket_size. При этом выделяется новый блок памяти большего размера,
 * копируются все существующие элементы в новый блок памяти, а старый блок
 * памяти освобождается.
 */
bool
put(HTable *t, const char *key, uint32_t value) {
  size_t index = hash(t, key);
  uint32_t old_v = get(t, key);

  if(old_v < UINT32_MAX) {
    return replace_value(t, key, value, index);
  }

  if(t->cap + 1 > t->size && !grow_bucket_size(t)) {
    return false;
  }

  return insert(t, key, value, index);
}

void
iterate(HTable *t, void (*callback)(const char *, uint32_t)) {
  for(size_t i = 0; i < t->size; i++) {
    if(t->values[i].key != NULL) {
      callback(t->values[i].key, t->values[i].v);
    }
  }
}

void
clear(HTable *t) {
  free(t->values);
  free(t);
}
