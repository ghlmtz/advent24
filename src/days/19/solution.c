#include "advent.h"
#include "dyn_arr.h"
#include "hashmap.h"

DynArr *towels;
static long part1 = 0;
static long part2 = 0;

struct subdesign
{
    char *design;
    long arrange;
};
HashMap *designs;

static int _strcmp(void *p, void *q)
{ 
    return strcmp(*(char **)p, *(char **)q) == 0; 
}

unsigned subdesign_hash(void *p)
{
    struct subdesign *z = (struct subdesign *)p;
    return (HASH_SIZE + (z->design[0] * 57 + z->design[1] * 31 + z->design[2] * 17) % HASH_SIZE) % HASH_SIZE;
}

static long try_towels(char *design)
{
    long ret = 0;
    struct subdesign test = {.design = design};
    struct subdesign *subdesign = hash_exists(designs, &test);
    if (subdesign != NULL)
        return subdesign->arrange;
    if (*design == '\0')
        return 1;
    for (size_t i = 0; i < towels->length; i++)
    {
        char *towel = DYN_ARR_GET(char *, towels, i);
        if (towel[0] != design[0])
            continue;
        int cmp = strncmp(towel, design, strlen(towel));
        if (cmp == 0)
            ret += try_towels(design + strlen(towel));
        else if (cmp > 0)
            break;
    }
    test.arrange = ret;
    hash_add(designs, &test);
    return ret;
}

static void parse_line(char *line)
{
    static int line_num = 0;
    if (line_num == 0)
    {
        char *add_me = strtok(line, ", ");
        while(add_me != NULL)
        {
            dyn_arr_add(towels, &add_me);
            add_me = strtok(NULL, ", ");
        }
        sort_strings(towels->elements, towels->length);
    }
    if (line_num > 1)
    {
        long result = try_towels(line);
        part2 += result;
        if (result > 0) part1++;
    }
    line_num++;
}

int day19()
{
    designs = hash_init(subdesign_hash, _strcmp, sizeof(struct subdesign));
    towels = dyn_arr_alloc(sizeof(char *));
    READ_INPUT("input");
    for_each_line(parse_line);
    printf("%ld\n", part1);
    printf("%ld\n", part2);
    hash_free(designs);
    dyn_arr_free(towels);
    return 0;
}