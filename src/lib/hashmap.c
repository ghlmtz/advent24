#include <stdlib.h>
#include <string.h>

#include "hashmap.h"

void *hash_exists(HashMap *hash_map, void *element) {
    unsigned key = hash_map->hash_func(element);

    struct hash_ptr *head = hash_map->storage[key];

    while(head != NULL) {
        if (hash_map->equal_func(element, head->data)) {
            return head->data;
        }
        head = head->next;
    }

    return NULL;
}

HashMap *hash_init(unsigned (*hash_func)(void *),
                   int (*equal_func)(void *, void *),
                   size_t el_size)
{
    HashMap *hash_map = malloc(sizeof(HashMap));

    memset(hash_map->storage, 0, HASH_SIZE * sizeof(struct hash_ptr *));

    hash_map->hash_func = hash_func;
    hash_map->equal_func = equal_func;
    hash_map->el_size = el_size;

    hash_map->all_ptrs = dyn_arr_alloc(sizeof(struct hash_ptr *));

    hash_map->count = 0;

    return hash_map;
}

/* Returns existing element if already there */
void *hash_add(HashMap *hash_map, void *element)
{
    void *existing;
    if ((existing = hash_exists(hash_map, element)) == NULL) {
        int key = hash_map->hash_func(element);
        struct hash_ptr *head = hash_map->storage[key];
        struct hash_ptr *new_ptr = malloc(sizeof(struct hash_ptr));

        new_ptr->data = malloc(hash_map->el_size);
        memcpy(new_ptr->data, element, hash_map->el_size);
        new_ptr->next = head;

        hash_map->storage[key] = new_ptr;
        dyn_arr_add(hash_map->all_ptrs, &new_ptr);

        hash_map->count++;

        return NULL;
    }

    return existing;
}

/* Returns element that matches -- data must be freed */
void *hash_del(HashMap *hash_map, void *element) {
    if (!hash_exists(hash_map, element))
        return NULL;
    else {
        int key = hash_map->hash_func(element);
        struct hash_ptr *cur = hash_map->storage[key];
        struct hash_ptr *prev = NULL;
        void *data;

        hash_map->count--;

        while (!hash_map->equal_func(element, cur->data)) {
            prev = cur;
            cur = cur->next;
        }

        if (prev == NULL)
            hash_map->storage[key] = cur->next;
        else
            prev->next = cur->next;

        data = cur->data;
        free(cur);

        return data;
    }
}

void hash_free(HashMap *hash_map) {
    size_t i;
    for (i = 0; i < hash_map->all_ptrs->length; i++)
    {
        struct hash_ptr *el = *(struct hash_ptr **)dyn_arr_get(hash_map->all_ptrs, i);
        if (el != NULL)
        {
            free(el->data);
            free(el);
        }
    }
    dyn_arr_free(hash_map->all_ptrs);
    
    free(hash_map);
}

void hash_flush(HashMap *hash_map) {
    size_t i;
    for (i = 0; i < hash_map->all_ptrs->length; i++)
    {
        struct hash_ptr *el = DYN_ARR_GET(struct hash_ptr *, hash_map->all_ptrs, i);
        if (el != NULL)
        {
            free(el->data);
            free(el);
        }
    }
    memset(hash_map->all_ptrs->elements, 0, hash_map->all_ptrs->length * hash_map->all_ptrs->el_size);
    hash_map->all_ptrs->length = 0;
    hash_map->count = 0;
    
    memset(hash_map->storage, 0, HASH_SIZE * sizeof(struct hash_ptr *));
}

HashMap *hash_iterate_map;
size_t hash_iterate_idx;
struct hash_ptr *hash_iterate_ptr;

void *hash_iterate(HashMap *hash_map) {
    void *return_ptr;

    if (hash_map != NULL)
    {
        hash_iterate_map = hash_map;
        hash_iterate_idx = 0;
        hash_iterate_ptr = DYN_ARR_GET(struct hash_ptr *, hash_iterate_map->all_ptrs, hash_iterate_idx);
    }

    if (hash_iterate_idx == hash_iterate_map->all_ptrs->length)
        return NULL;

    while (hash_iterate_ptr == NULL) {
        hash_iterate_idx++;
        if (hash_iterate_idx == hash_iterate_map->all_ptrs->length)
            return NULL;
        hash_iterate_ptr = 
            DYN_ARR_GET(struct hash_ptr *, hash_iterate_map->all_ptrs, hash_iterate_idx);
    }


    return_ptr = hash_iterate_ptr->data;
    hash_iterate_idx++;
    hash_iterate_ptr = 
        DYN_ARR_GET(struct hash_ptr *, hash_iterate_map->all_ptrs, hash_iterate_idx);

    return return_ptr;
}
