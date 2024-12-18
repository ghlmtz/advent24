typedef struct grid
{
    int rows;
    int cols;
    int empty;
    int *data;
} GRID;

GRID *init_grid(int rows, int cols, int empty);
GRID *init_grid_buffer(char *buffer, int empty);
GRID *grid_copy(GRID *old_grid);
int get_grid(GRID *grid, int row, int col);
void set_grid(GRID *grid, int row, int col, int value);
void free_grid(GRID *grid);
void print_grid(GRID *grid);
#define get_grid_xy(grid, xy_pos) get_grid(grid, (xy_pos).y, (xy_pos).x)
#define set_grid_xy(grid, xy_pos, value) set_grid(grid, (xy_pos).y, (xy_pos).x, value)