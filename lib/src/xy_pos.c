/* Code for dealing with cartesian coordinates */
#include <stdlib.h>

#include "xy_pos.h"
#include "hashmap.h"

const XY_POS xy_dirs[] = {{.x=0, .y=-1}, {.x=1, .y=0}, {.x=0, .y=1}, {.x=-1, .y=0} };

int manhattan(XY_POS *z, XY_POS *w)
{
    return abs(z->x - w->x) + abs(z->y - w->y);
}

unsigned xy_pos_hash(void *p) 
{
    XY_POS *z = (XY_POS *)p;
    return (HASH_SIZE + (z->x * 57 + z->y * 31) % HASH_SIZE) % HASH_SIZE;
}

int xy_pos_eq(void *p, void *q)
{
    XY_POS *z = (XY_POS *)p;
    XY_POS *w = (XY_POS *)q;
    return z->x == w->x && z->y == w->y;
}

XY_POS *xy_pos_add(XY_POS *dest, const XY_POS *src)
{
    dest->x += src->x;
    dest->y += src->y;
    return dest;
}

/* For sorting / searching XY_POS objects */
int xy_pos_comp(const void *p, const void *q)
{
    XY_POS *a = (XY_POS *)p;
    XY_POS *b = (XY_POS *)q;

    if (a->y < b->y)
        return -1;
    else if (a->y > b->y)
        return 1;
    else if (a->x < b->x)
        return -1;
    else if (a->x > b->x)
        return 1;
    else
        return 0;
}
