#ifndef XY_POS_H
#define XY_POS_H

typedef struct xy_pos {
    int x;
    int y;
} XY_POS;

int manhattan(XY_POS *z, XY_POS *w);
unsigned xy_pos_hash(void *p);
int xy_pos_eq(void *z, void *w);
static inline void xy_pos_add(XY_POS *dest, const XY_POS *src)
{
    dest->x += src->x;
    dest->y += src->y;
}
int xy_pos_comp(const void *p, const void *q);

#endif
