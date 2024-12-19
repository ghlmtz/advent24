#include "advent.h"
#include "dyn_arr.h"

DynArr *towels;
static int part1 = 0;

static int try_towels(char *design)
{
    if (*design == '\0')
        return 1;
    for (size_t i = 0; i < towels->length; i++)
    {
        char *towel = dyn_arr_get(towels, i);
        if(strncmp(towel, design, strlen(towel)) == 0)
            if(try_towels(design + strlen(towel)))
                return 1;
    }
    return 0;
}


static void parse_line(char *line)
{
    static int line_num = 0;
    if (line_num == 0)
    {
        char *add_me = strtok(line, ", ");
        while(add_me != NULL)
        {
            dyn_arr_add(towels, add_me);
            add_me = strtok(NULL, ", ");
        }
    }
    if (line_num > 1)
    {
        part1 += try_towels(line);
    }
    line_num++;
}

int day19()
{
    towels = dyn_arr_alloc(sizeof(char *));
    READ_INPUT("input");
    for_each_line(parse_line);
    printf("%d\n", part1);
    dyn_arr_free(towels);
    return 0;
}