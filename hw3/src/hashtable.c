#include <stdlib.h>
#include <stdbool.h>
#include <strings.h>
#include <stdio.h>

typedef struct
{
    char *key;
    int32_t v;
} bucket;

typedef struct
{
    size_t size;
    size_t cap;
    bucket *values;
    uint32_t collision;
} HTable;

/**
 * Выделяет память и инициализирует новую хэш-таблицу.
 *
 * @param sz Размер хэш-таблицы.
 * @param collision Параметр столкновения, определяющий максимальное количество коллизий при добавлении элементов.
 * @return Указатель на новую хэш-таблицу, или NULL в случае ошибки выделения памяти.
 */
HTable *new_table(size_t sz, uint32_t collision)
{
    HTable *p = malloc(sizeof(HTable));
    p->size = sz;
    p->cap = 0;
    p->collision = collision;
    bucket *ptr = (bucket *)malloc(sz * sizeof(bucket));
    if (!ptr)
    {
        return NULL;
    }
    memset(ptr, 0, sz * sizeof(bucket));
    p->values = ptr;
    return p;
}

size_t hash(HTable *t, char *key)
{
    uint32_t h = 0;
    size_t keyLength = strlen(key);
    for (size_t i = 0; i < keyLength; i++)
    {
        h = h * 31 + key[i];
    }
    return h % t->size;
}

bool grow_bucket_size(HTable *t)
{
    size_t old_size = t->size;
    bucket *oldBuckets = t->values;
    t->size *= 2;
    bucket *ptr = (bucket *)malloc(t->size * sizeof(bucket));
    if (!ptr)
    {
        return false;
    }
    memset(ptr, 0, t->size * sizeof(bucket));

    for (size_t i = 0; i < old_size; i++)
    {
        if (oldBuckets[i].key != NULL)
        {
            size_t idx = hash(t, oldBuckets[i].key);
            ptr[idx].key = oldBuckets[i].key;
            ptr[idx].v = oldBuckets[i].v;
        }
    }

    t->values = ptr;

    free(oldBuckets);

    return true;
}

int32_t get(HTable *t, char *key)
{
    size_t index = hash(t, key);
    size_t i = 0;

    for (i = 0; i < t->collision; i++)
    {
        if (t->values[index].key != NULL && strncmp(t->values[index].key, key, strlen(key)) == 0)
        {
            return t->values[index].v;
        }
        index = (index + 1) % t->size;
    }

    return -1;
}

bool insert(HTable *t, char *key, int32_t value, size_t index)
{
    size_t i = 0;

    for (i = 0; i < t->collision; i++)
    {
        if (t->values[index].key == NULL)
        {
            t->values[index].v = value;
            char *ptr = (char *)malloc((strlen(key) + 1) * sizeof(char));
            if (!ptr)
            {
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

bool replace_value(HTable *t, char *key, int32_t value, size_t index)
{
    size_t i = 0;

    for (i = 0; i < t->collision; i++)
    {
        if (t->values[index].key != NULL && strncmp(t->values[index].key, key, strlen(key)) == 0)
        {
            t->values[index].v = value;

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
 * @return true, если элемент был успешно добавлен, и false, если добавление не удалось.
 *
 * Если при добавлении элемента количество элементов в хэш-таблице превышает выделенную память,
 * происходит перераспределение памяти с помощью функции grow_bucket_size.
 * При этом выделяется новый блок памяти большего размера, копируются все существующие элементы в новый блок памяти,
 * а старый блок памяти освобождается.
 */
bool put(HTable *t, char *key, int32_t value)
{
    size_t index = hash(t, key);
    int32_t old_v = get(t, key);

    if (old_v != -1)
    {
        return replace_value(t, key, value, index);
    }

    if (t->cap + 1 > t->size && !grow_bucket_size(t))
    {
        return false;
    }

    return insert(t, key, value, index);
}

void iterate(HTable *t, void (*callback)(char *, int32_t))
{
    for (size_t i = 0; i < t->size; i++)
    {
        if (t->values[i].key != NULL)
        {
            callback(t->values[i].key, t->values[i].v);
        }
    }
}

void clear(HTable *t)
{
    free(t->values);
    free(t);
}
