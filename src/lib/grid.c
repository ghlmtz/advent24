#include "advent.h"
#include "grid.h"

/* static 2D array, returns a sentinel value when accessing beyond limits */

GRID *init_grid(int rows, int cols, int empty) {
    GRID *grid = malloc(sizeof(GRID));
    grid->rows = rows;
    grid->cols = cols;
    grid->data = malloc(sizeof(int) * rows * cols);
    grid->empty = empty;

    return grid;
}

/* Automatically init grid based on what's in buffer */
GRID *init_grid_buffer(char *buffer, int empty)
{
    GRID *grid = malloc(sizeof(GRID));
    grid->rows = count_to_blank(1) - 1;
    grid->cols = char_index(buffer, '\n');
    grid->data = malloc(sizeof(int) * grid->rows * grid->cols);
    grid->empty = empty;

    return grid;
}

int get_grid(GRID *grid, int row, int col) {
    if (row < 0 || row >= grid->rows || col < 0 || col >= grid->cols)
        return grid->empty;
    return grid->data[row * grid->cols + col];
}

void set_grid(GRID *grid, int row, int col, int value) {
    grid->data[row * grid->cols + col] = value;
}

void free_grid(GRID *grid) {
    free(grid->data);
    free(grid);
}

void print_grid(GRID *grid) {
    int i, j;
    for(i = 0; i < grid->rows; i++) {
        for(j = 0; j < grid->cols; j++) {
            printf("%c", get_grid(grid, i, j));
        }
        printf("\n");
    }
}
