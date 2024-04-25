#include "hashtable.h"
#include <assert.h>
#include <curl/curl.h>
#include <stdbool.h>
#include <stdio.h>

// Тестирование создания и освобождения хэш-таблицы
void test_create_and_free_hash_table() {
  hash_t *table = create_hash_t(10);
  assert(table != NULL);
  assert(table->cap == 10);
  assert(table->size == 0);

  hash_t_free(table);
}

// Тестирование операции вставки элемента в хэш-таблицу
void test_hash_table_put() {
  hash_t *table = create_hash_t(10);
  hash_t_put(table, "key1", 10);
  hash_t_put(table, "key2", 20);

  uint64_t value;
  bool result = hash_t_get(table, "key1", &value);
  assert(result == true);
  assert(value == 10);

  result = hash_t_get(table, "key2", &value);
  assert(result == true);
  assert(value == 20);

  hash_t_free(table);
}

// Тестирование операции получения элемента из хэш-таблицы
void test_hash_table_get() {
  hash_t *table = create_hash_t(10);
  hash_t_put(table, "key1", 10);
  hash_t_put(table, "key2", 20);

  uint64_t value;
  bool result = hash_t_get(table, "key1", &value);
  assert(result == true);
  assert(value == 10);

  result = hash_t_get(table, "key3", &value);
  assert(result == false);

  hash_t_free(table);
}

void test_hash_table_inc() {

  hash_t *table = create_hash_t(10);

  hash_t_inc(table,
             "/-%D1%87%D1%82%D0%BE-%D1%82%D0%B0%D0%BA%D0%BE%D0%B5-%D0%BD%D0%B5-"
             "%D0%B6%D1%83%D0%B6%D0%B6%D0%B8%D1%82-%D0%B8-%D0%B2-%D0%B6%D0%BE%"
             "D0%BF%D1%83-%D0%BD%D0%B5-%D0%BB%D0%B5%D0%B7%D0%B5%D1%82/",
             1);
  hash_t_inc(table,
             "/-%D1%87%D1%82%D0%BE-%D1%82%D0%B0%D0%BA%D0%BE%D0%B5-%D0%BD%D0%B5-"
             "%D0%B6%D1%83%D0%B6%D0%B6%D0%B8%D1%82-%D0%B8-%D0%B2-%D0%B6%D0%BE%"
             "D0%BF%D1%83-%D0%BD%D0%B5-%D0%BB%D0%B5%D0%B7%D0%B5%D1%82/",
             1);

  uint64_t value;
  bool result = hash_t_get(
      table,
      "/-%D1%87%D1%82%D0%BE-%D1%82%D0%B0%D0%BA%D0%BE%D0%B5-%D0%BD%D0%B5-"
      "%D0%B6%D1%83%D0%B6%D0%B6%D0%B8%D1%82-%D0%B8-%D0%B2-%D0%B6%D0%BE%"
      "D0%BF%D1%83-%D0%BD%D0%B5-%D0%BB%D0%B5%D0%B7%D0%B5%D1%82/",
      &value);
  assert(result == true);
  assert(value == 2);

  hash_t_free(table);
}
