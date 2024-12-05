#include "advent.h"
#include "realloc_arr.h"

static size_t n_rules;
static char **lines;

struct rule {
    long key;
    long *values;
    size_t n_values;
};
static ReallocArr *rules;
static ReallocArr *this_key;

static int phase = 0;
static int part1 = 0;
static int part2 = 0;

int rule_comp(const void *p, const void *q)
{
    struct rule *x = (struct rule *)p;
    struct rule *y = *(struct rule **)q;

    if (x->key < y->key)
        return -1;
    else if (x->key > y->key)
        return 1;

    return 0;
}

int rule_exists(long page, long after)
{
    struct rule search = {.key = page};
    struct rule **found = bsearch(&search, rules->elements, rules->length, rules->el_size, rule_comp);
    if (found == NULL)
        return 0;
    if (bsearch_long(&after, (*found)->values, (*found)->n_values) != NULL)
        return 1;
    return 0;
}

int comparitor(const void *p, const void *q)
{
    int x = *(const int *)p;
    int y = *(const int *)q;

    if (rule_exists(x, y))
        return 1;
    else if (rule_exists(y, x))
        return -1;
    return 0;
}

void parse_line(char *line)
{
    long *nums;
    static int line_idx = 0;
    static struct rule *rule = NULL;
    if (strlen(line) < 2)
    {
        int index = -1;
        phase = 1;
        sort_strings(lines, n_rules);
        for (size_t i = 0; i < n_rules; i++) {
            scan_longs(lines[i], &nums);
            if (rule == NULL || nums[0] != rule->key)
            {
                index++;
                if (index != 0)
                {
                    rule->n_values = this_key->length;
                    rule->values = malloc(sizeof(long) * rule->n_values);
                    memcpy(rule->values, this_key->elements, rule->n_values * this_key->el_size);
                    sort_asc(rule->values, rule->n_values);
                    realloc_arr_add(rules, rule);
                }
                rule = malloc(sizeof(struct rule));
                rule->key = nums[0];
                this_key->length = 0;
            }
            realloc_arr_add(this_key, (void *)nums[1]);
            free(nums);
        }
        rule->n_values = this_key->length;
        rule->values = malloc(sizeof(long) * rule->n_values);
        memcpy(rule->values, this_key->elements, rule->n_values * this_key->el_size);
        sort_asc(rule->values, rule->n_values);
        realloc_arr_add(rules, rule);
    }
    else if (phase == 0)
    {
        lines[line_idx++] = line;
        return;
    }
    else
    {
        int n = scan_longs(line, &nums);
        int mid = nums[(n-1)/2];
        for (int i = n - 1; i > 0; i--)
        {
            for (int j = 0; j < i; j++)
            {
                if (rule_exists(nums[i], nums[j]))
                {
                    mid = 0;
                    break;
                }
            }
            if (mid == 0)
                break;
        }
        part1 += mid;
        if (mid == 0)
        {
            qsort(nums, n, sizeof(long), comparitor);
            part2 += nums[(n-1)/2];
        }
        free(nums);
    }
}

int day5()
{
    READ_INPUT("input");

    n_rules = count_to_blank(0) - 1;
    lines = malloc(sizeof(char *) * n_rules);

    rules = realloc_arr_alloc(sizeof(struct rule *));
    this_key = realloc_arr_alloc(sizeof(unsigned long));

    for_each_line(parse_line);

    printf("%d\n", part1);
    printf("%d\n", part2);

    realloc_arr_smallfree(this_key);
    for (size_t i = 0; i < rules->length; i++)
    {
        struct rule *r = (struct rule *)rules->elements[i];
        free(r->values);
    }
    realloc_arr_free(rules);
    free(lines);

    return 0;
}