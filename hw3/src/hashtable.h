#include <stdlib.h>
#include <stdbool.h>

typedef struct
{
    const char *key;
    uint32_t v;
} bucket;

typedef struct
{
    size_t size;
    size_t cap;
    bucket *values;
    uint32_t collision;
} HTable;

HTable* new_table(size_t sz, uint32_t collision);
size_t hash(HTable* t, const char* key);
bool put(HTable* t, const char* key, uint32_t value);
uint32_t get(HTable* t, const char* key);
void clear(HTable* t);
void iterate(HTable* t, void (*callback)(const char*, uint32_t));
