#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include <assert.h>
#include "sim.h"

static SDL_Renderer *renderer = NULL;
static SDL_Window *window = NULL;
static uint32_t ticks = 0;

static const uint32_t FPS_MAX = 60;

void sim_init() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(X_SIZE, Y_SIZE, 0, &window, &renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    srand(time(NULL));
    screen_put_pixel(0, 0, 0);
}

void sim_fini() {
    SDL_Event event;
    while (true) {
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT) {
            break;
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void screen_flush() {
    SDL_PumpEvents();

    uint32_t curr_ticks = SDL_GetTicks() - ticks;
    if (curr_ticks < FPS_MAX) {
        SDL_Delay(FPS_MAX - curr_ticks);
    }
    SDL_RenderPresent(renderer);
}

void screen_put_pixel(int x, int y, int argb) {
    assert(x >= 0 && x < X_SIZE && "x coord out of range");
    assert(y >= 0 && y < Y_SIZE && "y coord out of range");

    uint8_t a = argb >> 24;
    uint8_t r = (argb >> 16) & 0xFF;
    uint8_t g = (argb >> 8) & 0xFF;
    uint8_t b = argb & 0xFF;
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderDrawPoint(renderer, x, y);
    ticks = SDL_GetTicks();
}

int sim_rand() {
    return rand();
}
