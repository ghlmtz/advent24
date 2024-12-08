#include <stdlib.h>
#include <string.h>

#include "hashmap.h"

/* For structs that don't need to be freed */
void hash_dummy_free(void *p) { (void)p; }

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
                   void (*free_func)(void *))
{
    HashMap *hash_map = malloc(sizeof(HashMap));

    memset(hash_map->storage, 0, HASH_SIZE * sizeof(struct hash_ptr *));

    hash_map->hash_func = hash_func;
    hash_map->equal_func = equal_func;
    hash_map->free_func = free_func;

    return hash_map;
}

// Maybe return something if the key's already there
int hash_add(HashMap *hash_map, void *element)
{
    if (!hash_exists(hash_map, element)) {
        int key = hash_map->hash_func(element);
        struct hash_ptr *head = hash_map->storage[key];
        struct hash_ptr *new_ptr = malloc(sizeof(struct hash_ptr));

        new_ptr->data = element;
        new_ptr->next = head;

        hash_map->storage[key] = new_ptr;

        return 0;
    }

    return 1;
}

// Returns element that matches
void *hash_del(HashMap *hash_map, void *element) {
    if (!hash_exists(hash_map, element))
        return NULL;
    else {
        int key = hash_map->hash_func(element);
        struct hash_ptr *cur = hash_map->storage[key];
        struct hash_ptr *prev = NULL;
        void *data;

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
    int i;
    for (i = 0; i < HASH_SIZE; i++) {
        struct hash_ptr *head = hash_map->storage[i];
        struct hash_ptr *next;

        while (head != NULL) {
            next = head->next;
            hash_map->free_func(head->data);
            free(head);
            head = next;
        }
    }
    
    free(hash_map);
}

void hash_flush(HashMap *hash_map) {
    int i;
    for (i = 0; i < HASH_SIZE; i++) {
        struct hash_ptr *head = hash_map->storage[i];
        struct hash_ptr *next;

        while (head != NULL) {
            next = head->next;
            hash_map->free_func(head->data);
            free(head);
            head = next;
        }
    }
    
    memset(hash_map->storage, 0, HASH_SIZE * sizeof(struct hash_ptr *));
}

HashMap *hash_iterate_map;
int hash_iterate_idx;
struct hash_ptr *hash_iterate_ptr;

void *hash_iterate(HashMap *hash_map) {
    void *return_ptr;

    if (hash_map != NULL)
    {
        hash_iterate_map = hash_map;
        hash_iterate_idx = 0;
        hash_iterate_ptr = hash_map->storage[0];
    }

    if (hash_iterate_idx == HASH_SIZE)
        return NULL;

    while (hash_iterate_ptr == NULL) {
        hash_iterate_idx++;
        if (hash_iterate_idx == HASH_SIZE)
            return NULL;
        hash_iterate_ptr = hash_iterate_map->storage[hash_iterate_idx];
    }

    return_ptr = hash_iterate_ptr->data;
    hash_iterate_ptr = hash_iterate_ptr->next;

    return return_ptr;
}

/* Returns total number of data members in hash */
size_t hash_length(HashMap *hash_map)
{
    size_t count = 0;
    for (int i = 0; i < HASH_SIZE; i++)
    {
        struct hash_ptr *head = hash_map->storage[i];
        while (head != NULL)
        {
            count++;
            head = head->next;
        }
    }
    return count;
}