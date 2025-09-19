#include "sim.h"

static const int WHITE = 0xFFFFFFFF;
static const int GREEN = 0xFF00FF00;
static const int RED = 0xFFFF0000;
static const int BLUE = 0xFF0000FF;
static const int BLACK = 0xFF000000;

int get_alive_neighbours(int (*grid)[X_SIZE][Y_SIZE], int x, int y) {
    int alive_num = 0;

    int neighbours[8][2] = {
        {x - 1, y - 1},
        {x - 1, y},
        {x - 1, y + 1},
        {x, y - 1},
        {x, y + 1},
        {x + 1, y - 1},
        {x + 1, y},
        {x + 1, y + 1},
    };

    for (int i = 0; i < 8; ++i) {
        int neigh_x = neighbours[i][0];
        int neigh_y = neighbours[i][1];

        if (neigh_x >= 0 && neigh_x < X_SIZE &&
            neigh_y >= 0 && neigh_y < Y_SIZE) {
            alive_num += (int)(!!((*grid)[neigh_x][neigh_y]));
        }
    }

    return alive_num;
}

int get_cell_color(int cell_val) {
    if (cell_val <= 0) {
        return BLACK;
    } else if (cell_val == 1) {
        return GREEN;
    } else if (cell_val == 2) {
        return BLUE;
    } else if (cell_val == 3) {
        return RED;
    } else if (cell_val >= 4) {
        return WHITE;
    }

    return BLACK;
}

void app() {
    int grid[X_SIZE][Y_SIZE] = {};
    for (int i = 0; i < X_SIZE; ++i) {
        for (int j = 0; j < Y_SIZE; ++j) {
            int rand = sim_rand() % 3;
            if (rand) {
                grid[i][j] = 0;
            } else {
                grid[i][j] = 1;
            }
        }
    }

    for (int step = 0; step < 100; ++step) {
        int new_grid[X_SIZE][Y_SIZE] = {};

        for (int i = 0; i < X_SIZE; ++i) {
            for (int j = 0; j < Y_SIZE; ++j) {
                new_grid[i][j] = grid[i][j];
            }
        }

        for (int i = 0; i < X_SIZE; ++i) {
            for (int j = 0; j < Y_SIZE; ++j) {
                int cell = grid[i][j];
                int alive_num = get_alive_neighbours(&grid, i, j);
                if (cell && alive_num < 2) {
                    new_grid[i][j] = 0;
                } else if (cell && (alive_num == 3 || alive_num == 2)) {
                    new_grid[i][j] = cell + 1;
                } else if (cell && alive_num > 3) {
                    new_grid[i][j] = 0;
                } else if (!cell && alive_num == 3) {
                    new_grid[i][j] = 1;
                }
            }
        }

        for (int i = 0; i < X_SIZE; ++i) {
            for (int j = 0; j < Y_SIZE; ++j) {
                grid[i][j] = new_grid[i][j];
            }
        }

        for (int i = 0; i < X_SIZE; ++i) {
            for (int j = 0; j < Y_SIZE; ++j) {
                int argb = get_cell_color(grid[i][j]);
                screen_put_pixel(i, j, argb);
            }
        }
        screen_flush();
    }
}


