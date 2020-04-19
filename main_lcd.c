#include <stdint.h>
#include <unistd.h>
#include <wiringPi.h>

#include "config.h"
#include "display.h"
#include "image.h"
#include "sprite.h"
#include "game.h"
#include "pcd8544.h"

void display_lcd_clear(display_t* d) {
  pcd8544_clear();
}

void display_lcd_set_pixel(display_t* d, int x, int y, char v) {
  if ((x < 0) || (x >= DISPLAY_WIDTH) || (y < 0) || (y >= DISPLAY_HEIGHT)) {
    return;
  }

  if (d->flip) {
    x = DISPLAY_WIDTH - x - 1;
    y = DISPLAY_HEIGHT - y - 1;
  }

  pcd8544_set_pixel(x, y, v);
}

void display_lcd_flush(display_t* d) {
  pcd8544_display();
}

void tick_cb(game_t* g) {
  static int counter = 0;
  counter++;

  if (!(counter % 31)) {
    if (!(counter % 7))
      game_action(g, ACTION_FACE_LEFT);
    else if (!(counter % 8))
      game_action(g, ACTION_FACE_RIGHT);
    else if (g->pos_x > DISPLAY_WIDTH / 2)
      game_action(g, ACTION_FACE_LEFT);
    else
      game_action(g, ACTION_FACE_RIGHT);
  }
  if (!(counter % 75))
    game_action(g, ACTION_JUMP);
  if (!(counter % 1053))
    game_action(g, ACTION_RANDOM_SPRITE);
  if (!(counter % 259))
    game_action(g, ACTION_RESET_SPRITE);

  game_tick(g);
}

int main(int argc, char* argv[]) {
  wiringPiSetup();

  pcd8544_init(65);

  display_t display = {.clear = display_lcd_clear,
                       .set_pixel = display_lcd_set_pixel,
                       .flush = display_lcd_flush};

  display_flip(&display, 1);

  game_t g;
  game_init(&g, &display);

  while (1) {
    tick_cb(&g);
    usleep(1000000 / FPS);
  }

  return 0;
}