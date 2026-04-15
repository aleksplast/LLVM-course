#ifndef APP_H
#define APP_H

#define X_SIZE 1000
#define Y_SIZE 900

void sim_init();
void sim_fini();
void app();

#ifdef __riscvivii__
int sim_rand();
void screen_put_pixel(int x, int y, int argb);
void screen_flush();
#endif

#endif // APP_H
