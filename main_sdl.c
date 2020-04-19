#include <SDL2/SDL.h>

#include "config.h"
#include "display.h"
#include "image.h"
#include "sprite.h"
#include "game.h"

SDL_Window* window;
SDL_Surface* surface;

void display_sdl_clear(display_t* d) {
  SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0xee, 0xee, 0xee));
}

void display_sdl_set_pixel(display_t* d, int x, int y, char v) {
  if ((x < 0) || (x >= DISPLAY_WIDTH) || (y < 0) || (y >= DISPLAY_HEIGHT)) {
    return;
  }

  if (d->flip) {
    x = DISPLAY_WIDTH - x - 1;
    y = DISPLAY_HEIGHT - y - 1;
  }

  SDL_Rect r = {.x = x * 5, .y = y * 5, .w = 5, .h = 5};

  if (v) {
    SDL_FillRect(surface, &r, SDL_MapRGB(surface->format, 0x00, 0x00, 0x00));
  } else {
    SDL_FillRect(surface, &r, SDL_MapRGB(surface->format, 0xee, 0xee, 0xee));
  }
}

void display_sdl_flush(display_t* d) {
  SDL_UpdateWindowSurface(window);
}

Uint32 tick_cb(Uint32 iv, void* p) {
  static int counter = 0;
  counter++;
  game_t* g = p;
  if (!(counter % 31)) {
    if (!(counter % 7)) {
      game_action(g, ACTION_FACE_LEFT);
    } else if (!(counter % 8)) {
      game_action(g, ACTION_FACE_RIGHT);
    } else if (g->pos_x > DISPLAY_WIDTH / 2) {
      game_action(g, ACTION_FACE_LEFT);
    } else {
      game_action(g, ACTION_FACE_RIGHT);
    }
  }
  if (!(counter % 75)) {
    game_action(g, ACTION_JUMP);
  }
  if (!(counter % 1053)) {
    game_action(g, ACTION_RANDOM_SPRITE);
  }
  if (!(counter % 259)) {
    game_action(g, ACTION_RESET_SPRITE);
  }
  game_tick(g);
  return iv;
}

int main(int argc, char* argv[]) {
  if (SDL_Init(SDL_INIT_VIDEO) == -1) {
    SDL_Log("Unable to initialize SDL: %s\n", SDL_GetError());
    return 1;
  }

  atexit(SDL_Quit);

  window =
      SDL_CreateWindow("simulator", SDL_WINDOWPOS_UNDEFINED_DISPLAY(1),
                       SDL_WINDOWPOS_UNDEFINED_DISPLAY(1), DISPLAY_WIDTH * 5,
                       DISPLAY_HEIGHT * 5, SDL_WINDOW_SHOWN);
  if (window == 0) {
    SDL_Log("Unable to create window: %s\n", SDL_GetError());
    return 1;
  }

  surface = SDL_GetWindowSurface(window);
  if (surface == 0) {
    SDL_Log("Error: unable to get surface: %s\n", SDL_GetError());
    return 1;
  }

  display_t display = {.clear = display_sdl_clear,
                       .set_pixel = display_sdl_set_pixel,
                       .flush = display_sdl_flush};

  game_t g;
  game_init(&g, &display);

  SDL_AddTimer(1000 / FPS, tick_cb, &g);

  int quit = 0;

  SDL_Event ev;
  while (!quit) {
    SDL_WaitEvent(&ev);

    switch (ev.type) {
      case SDL_QUIT: {
        quit = 1;
        break;
      }
      case SDL_KEYDOWN: {
        switch (ev.key.keysym.sym) {
          case SDLK_SPACE:
            game_action(&g, ACTION_POKE);
            break;
          case SDLK_LEFT:
            game_action(&g, ACTION_FACE_LEFT);
            break;
          case SDLK_RIGHT:
            game_action(&g, ACTION_FACE_RIGHT);
            break;
          case SDLK_DOWN:
            game_action(&g, ACTION_FACE_FORWARD);
            break;
          case SDLK_UP:
            game_action(&g, ACTION_JUMP);
            break;
          case SDLK_s:
            game_action(&g, ACTION_RANDOM_SPRITE);
            break;
          case SDLK_f:
            display_flip(&display, display.flip ? 0 : 1);
            break;
        }

        break;
      }
    }
  }

  SDL_DestroyWindow(window);

  // save to flash

  return 0;
}