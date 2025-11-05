#ifndef APP_H
#define APP_H

#define X_SIZE 1000
#define Y_SIZE 900

void sim_init();
void sim_fini();
int sim_rand();
void app();
void screen_put_pixel(int x, int y, int argb);
void screen_flush();
void dump_grid(int *grid_ptr);
void dump_reg(long long reg_val);
void dump_neighbours(int *neigh_ptr, int x, int y);

#endif // APP_H
