#include <stdlib.h>
#include <stdbool.h>

#ifndef hashtable
#define hashtable

typedef struct {
    size_t size;
    size_t cap;
    int32_t* values;
    uint32_t collision;
} HTable;

HTable* new_table(size_t sz, uint32_t collision);
size_t hash(HTable* t, char* key);
bool put(HTable* t, char* key, int32_t value);
int32_t get(HTable* t, char* key);
void clear(HTable* t);
void iterate(HTable* t, void (*callback)(char*, int32_t));

#endif
