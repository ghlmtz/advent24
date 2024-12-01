/* Support for storing dynamically resizing arrays of pointers to objs */

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
    data->elements[data->length++] = (intptr_t)el; 
    if (data->length == data->capacity)
    { 
        data->elements = realloc(data->elements, data->el_size * data->capacity * 2); \
        data->capacity *= 2;
    }
}

void realloc_arr_free(ReallocArr *data)
{
    for (size_t i = 0; i < data->length; i++) {
        free((void *)data->elements[i]);
    }
    realloc_arr_smallfree(data);
}

void realloc_arr_smallfree(ReallocArr *data) 
{
    free(data->elements);
    free(data);
}