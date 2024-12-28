#include "advent.h"
#include "hashmap.h"
#include "dyn_arr.h"

typedef struct {
    int key;
    int idx1;
    int idx2;
    int op;
    int value;
} IntHash;

static int int_hash_eq(void *p, void *q)
{
    IntHash *a = (IntHash *)p;
    IntHash *b = (IntHash *)q;
    return a->key == b->key;
}

static unsigned int_hash_hash(void *p) 
{
    return ((IntHash *)p)->key % HASH_SIZE;
}

HashMap *values;
DynArr *zeds;

static int key(char *name)
{
    return name[0] * 256 * 256 + name[1] * 256 + name[2];
}

static void parse_line(char *line)
{
    static int get_values = 1;
    if (get_values)
    {
        if (strlen(line) < 2)
            get_values = 0;
        else {
            IntHash add_me = { .key = key(line),
            .value = line[5] - '0'};
            hash_add(values, &add_me);
        }
    }
    else
    {
        IntHash add_me = {.value = -1};
        char *group = strtok(line, " ");
        add_me.idx1 = key(group);
        group = strtok(NULL, " ");
        switch (group[0])
        {
            case 'A':
                add_me.op = 0;
                break;
            case 'O':
                add_me.op = 1;
                break;
            case 'X':
                add_me.op = 2;
                break;
        }
        group = strtok(NULL, " ");
        add_me.idx2 = key(group);
        strtok(NULL, " ");
        group = strtok(NULL, " ");
        add_me.key = key(group);
        hash_add(values, &add_me);
        if (group[0] == 'z')
        {
            dyn_arr_add(zeds, &add_me);
        }
    }
}

int find_z(int idx)
{
    IntHash search = {.key = idx};
    IntHash *el = hash_exists(values, &search);
    if (el->value != -1)
        return el->value;
    int result1 = find_z(el->idx1);
    int result2 = find_z(el->idx2);
    switch (el->op)
    {
        case 0:
            return result1 & result2;
        case 1:
            return result1 | result2;
        case 2:
            return result1 ^ result2;
    }
}

int day24()
{
    READ_INPUT("input");
    values = hash_init(int_hash_hash, int_hash_eq, sizeof(IntHash));
    zeds = dyn_arr_alloc(sizeof(IntHash *));
    for_each_line(parse_line);
    long output = 0;
    for (size_t i = 0; i < zeds->length; i++)
    {
        long bit = find_z('z' * 256 * 256 + ('0' + i / 10) * 256 + ('0' + i % 10));
        output |= bit << i;
    }
    printf("%ld\n", output);
    hash_free(values);
    dyn_arr_free(zeds);
    return 0;
}