/* Support for storing dynamically resizing arrays of pointers to objs */
#include <string.h>

#include "dyn_arr.h"

DynArr *dyn_arr_alloc(size_t el_size)
{
    DynArr *data = malloc(sizeof(DynArr));
    data->length = 0;
    data->capacity = 64;
    data->el_size = el_size;
    data->elements = malloc(el_size * data->capacity);
    return data;
}

void dyn_arr_add(DynArr *data, void *el)
{
    memcpy((void *)((intptr_t)data->elements + data->length++ * data->el_size), el, data->el_size);
    if (data->length == data->capacity)
    { 
        data->elements = realloc(data->elements, data->el_size * data->capacity * 2); \
        data->capacity *= 2;
    }
}

void dyn_arr_free(DynArr *data) 
{
    free(data->elements);
    free(data);
}