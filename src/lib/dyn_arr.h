#ifndef DYN_ARR_H
#define DYN_ARR_H

#include <stdlib.h>
#include <stdint.h>

typedef struct {
    size_t length; 
    size_t capacity;
    void *elements; 
    size_t el_size;
} DynArr;

/* Allocates a resizeable array with element length el_size. 
 * The memory must be freed later, perhaps with dyn_arr_free. */
DynArr *dyn_arr_alloc(size_t el_size);

void dyn_arr_add(DynArr *data, void *el);

/* Frees the data structure and element array */
void dyn_arr_free(DynArr *data);

#endif
