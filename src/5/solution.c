#include "advent.h"

struct rule
{
    int page;
    int after;
    struct rule *next;
};
struct rule *rules = NULL;
struct rule *tail = NULL;
static int phase = 0;
static int part1 = 0;
static int part2 = 0;

int rule_exists(int page, int after)
{
    struct rule *rule = rules;
    while(rule != NULL)
    {
        if (rule->page == page && rule->after == after)
            return 1;
        rule = rule->next;
    }
    return 0;
}

void rule_breaker(int *nums, int n)
{
    int mid = 0;
    while (mid == 0) {
        mid = nums[(n-1)/2];
        for (int i = n - 1; i > 0; i--)
        {
            for (int j = 0; j < i; j++)
            {
                if (rule_exists(nums[i], nums[j]))
                {
                    int tmp = nums[i];
                    nums[i] = nums[j];
                    nums[j] = tmp;
                    mid = 0;
                    break;
                }
            }
            if (mid == 0)
                break;
        }
    }
    part2 += mid;
}

void parse_line(char *line)
{
    int *nums;
    if (strlen(line) < 2)
    {
        phase = 1;
        return;
    }
    if (phase == 0)
    {
        scan_ints(line, &nums);
        if (rules == NULL) {
            rules = malloc(sizeof(struct rule));
            tail = rules;
        }
        else
        {
            tail->next = malloc(sizeof(struct rule));
            tail = tail->next;
        }
        tail->next = NULL;
        tail->page = nums[0];
        tail->after = nums[1];
    }
    else
    {
        int n = scan_ints(line, &nums);
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
        if (mid == 0)
            rule_breaker(nums, n);
        part1 += mid;
    }
    free(nums);
}

int day5()
{
    READ_INPUT("input");
    for_each_line(parse_line);
    printf("%d\n", part1);
    printf("%d\n", part2);

    tail = rules->next;
    free(rules);
    while (tail != NULL)
    {
        rules = tail;
        tail = tail->next;
        free(rules);
    }

    return 0;
}