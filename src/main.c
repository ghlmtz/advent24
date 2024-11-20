#include <stdio.h>
#include <stdlib.h>

#include <sysexits.h>
#include <unistd.h>

#define ARRAY_SIZE(...) (sizeof(__VA_ARGS__) / sizeof(*(__VA_ARGS__)))

static const char *all_days[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25"};

typedef int (SolFunc)(void);

extern SolFunc day1;
extern SolFunc day2;
extern SolFunc day3;
extern SolFunc day4;
extern SolFunc day5;
extern SolFunc day6;
extern SolFunc day7;
extern SolFunc day8;
extern SolFunc day9;
extern SolFunc day10;
extern SolFunc day11;
extern SolFunc day12;
extern SolFunc day13;
extern SolFunc day14;
extern SolFunc day15;
extern SolFunc day16;
extern SolFunc day17;
extern SolFunc day18;
extern SolFunc day19;
extern SolFunc day20;
extern SolFunc day21;
extern SolFunc day22;
extern SolFunc day23;
extern SolFunc day24;
extern SolFunc day25;

static SolFunc *solutions[] = {&day1, &day2, &day3, &day4, &day5, &day6, &day7, &day8, &day9, &day10,
                                &day11, &day12, &day13, &day14, &day15, &day16, &day17, &day18,
                                &day19, &day20, &day21, &day22, &day23, &day24, &day25};

void fetch_input(void)
{
    if (access("input", R_OK)) 
    {
        printf("Fetching input:\n");
        int result = system("../../bin/fetch_input.sh");
        printf("Done.\n");
        if (result) 
        {
            perror(NULL);
            exit(EX_NOINPUT);
        }
    }
}

void run_day(const char *day)
{
    printf("Running Day %s:\n", day);
    chdir("src");
    chdir(day);
    fetch_input();
    int day_num = atoi(day);
    if (day_num < 1 || day_num > 25) 
    {
        exit(EX_DATAERR);
    }
    solutions[day_num - 1]();
    chdir("../..");
}

int main(int argc, char *argv[]) 
{
    if (argc < 2) 
    {
        printf("This will run ALL 25 days! Press any key to continue...\n");
        fflush(stdin);
        getchar();
        for (unsigned int i = 0; i < ARRAY_SIZE(all_days); i++) 
        {
            run_day(all_days[i]);
        }
    } 
    else 
    {
        for (int i = 1; i < argc; i++) 
        {
            run_day(argv[i]);
        }
    }
       
}