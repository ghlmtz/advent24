#ifndef REALLOC_ARR_H
#define REALLOC_ARR_H

#include <stdlib.h>
#include <stdint.h>

typedef struct {
    size_t length; 
    size_t capacity;
    intptr_t *elements; 
    size_t el_size;
} ReallocArr;

/* Allocates a resizeable array with element length el_size. 
 * The memory must be freed later, perhaps with realloc_arr_free. */
ReallocArr *realloc_arr_alloc(size_t el_size);

void realloc_arr_add(ReallocArr *data, void *el);

/* Frees the data structure, element array, and individual elements. */
void realloc_arr_free(ReallocArr *data);

/* Frees the data structure and element array. Skips the individual elements. */
void realloc_arr_smallfree(ReallocArr *data);

#endif