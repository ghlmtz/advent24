#ifndef HASHMAP_H
#define HASHMAP_H

#define HASH_SIZE 4096

#include "dyn_arr.h"

struct hash_ptr
{
    void *data;
    struct hash_ptr *next;
};

typedef struct hash_map
{
    unsigned (*hash_func)(void *);
    int (*equal_func)(void *, void *);
    size_t el_size;
    struct hash_ptr *storage[HASH_SIZE];
    DynArr *all_ptrs;
    size_t count;
} HashMap;

HashMap *hash_init(unsigned (*hash_func)(void *),
                   int (*equal_func)(void *, void *), size_t el_size);
void *hash_exists(HashMap *hash_map, void *element);
void *hash_add(HashMap *hash_map, void *element);
void *hash_del(HashMap *hash_map, void *element);
void hash_free(HashMap *hash_map);
void hash_flush(HashMap *hash_map);
void *hash_iterate(HashMap *hash_map);

#endif
