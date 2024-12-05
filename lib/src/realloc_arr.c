/* Support for storing dynamically resizing arrays of pointers to objs */
#include <string.h>

#include "realloc_arr.h"

ReallocArr *realloc_arr_alloc(size_t el_size)
{
    ReallocArr *data = malloc(sizeof(ReallocArr));
    data->length = 0;
    data->capacity = 64;
    data->el_size = el_size;
    data->elements = malloc(el_size * data->capacity);
    return data;
}

void realloc_arr_add(ReallocArr *data, void *el)
{
    memcpy((void *)((intptr_t)data->elements + data->length++ * data->el_size), el, data->el_size);
    if (data->length == data->capacity)
    { 
        data->elements = realloc(data->elements, data->el_size * data->capacity * 2); \
        data->capacity *= 2;
    }
}

void realloc_arr_free(ReallocArr *data) 
{
    free(data->elements);
    free(data);
}