#include <stddef.h>
#include <stdio.h>

#include "config.h"
#include "display.h"
#include "image.h"
#include "sprite.h"
#include "sprites.h"
#include "game.h"

#include "images/image_heart_empty.xbm"
#include "images/image_heart_full.xbm"
#include "images/image_heart_half.xbm"
#include "images/image_hunger_empty.xbm"
#include "images/image_hunger_full.xbm"
#include "images/image_hunger_half.xbm"

void game_init(game_t* g, display_t* display) {
  g->tick = 0;
  g->display = display;
  g->sprite = &rabbit_sprite_basic;
  g->energy_points = 10;
  g->hunger_points = 10;
  g->interest_points = 10;
  g->jump_frames = 0;
  g->activity_frames = 0;
  g->pos_x = DISPLAY_WIDTH / 2 + g->sprite->front_normal->width / 2;
  g->pos_y = 0;
  g->pos_f = FACING_FRONT;
}

void game_tick(game_t* g) {
  int sleeping = 0;
  image_t* image = NULL;

  g->tick++;

  if (!g->energy_points)
    sleeping = 1;

  if (g->activity_frames > 0)
    g->activity_frames -= 1;

  if (!g->activity_frames)
    g->pos_f = FACING_FRONT;

  // select image
  switch (g->pos_f) {
    case FACING_FRONT:
      if (sleeping)
        image = g->sprite->front_sleeping;
      else if ((g->tick % 9) == 0 || (g->tick % 15) == 0)
        image = g->sprite->front_lowered;
      else
        image = g->sprite->front_normal;
      break;
    case FACING_LEFT:
    case FACING_RIGHT:
      if (sleeping)
        image = g->sprite->side_sleeping;
      else if ((g->tick % 9) == 0 || (g->tick % 15) == 0)
        image = g->sprite->side_lowered;
      else
        image = g->sprite->side_normal;
      break;
  }

  // gravity
  if (g->jump_frames > 0) {
    g->jump_frames -= 1;
    g->pos_y += 1;
  } else if (g->pos_y > 0) {
    g->pos_y -= 1;
  } else if (g->pos_y < 0) {
    g->pos_y = 0;
  }

  // move
  switch (g->pos_f) {
    case FACING_FRONT:
      break;
    case FACING_LEFT:
      g->pos_x -= 1;
      break;
    case FACING_RIGHT:
      g->pos_x += 1;
      break;
  }

  // turn around
  if (g->pos_x < 0) {
    g->pos_x = 0;
    g->pos_f = FACING_RIGHT;
  } else if (g->pos_x > DISPLAY_WIDTH) {
    g->pos_x = DISPLAY_WIDTH;
    g->pos_f = FACING_LEFT;
  }

  // ---
  // begin rendering
  // ---

  g->display->clear(g->display);

  // draw hearts

  for (int i = 0; i < 3; i++) {
    if (g->interest_points >= i * 2 + 2) {
      if (g->interest_points > 0 || g->tick % FPS >= FPS / 2)
        image_draw(g->display, &image_heart_full,
                   DISPLAY_WIDTH - 8 * (2 - i), 0, IMAGE_ORIGIN_RIGHT);
    } else if (g->interest_points == i * 2 + 1) {
      if (g->interest_points > 0 || g->tick % FPS >= FPS / 2)
        image_draw(g->display, &image_heart_half,
                   DISPLAY_WIDTH - 8 * (2 - i), 0, IMAGE_ORIGIN_RIGHT);
    } else {
      if (g->interest_points > 0 || g->tick % FPS >= FPS / 2)
        image_draw(g->display, &image_heart_empty,
                   DISPLAY_WIDTH - 8 * (2 - i), 0, IMAGE_ORIGIN_RIGHT);
    }
  }

  // draw food

  for (int i = 0; i < 5; i++) {
    if (g->hunger_points >= i * 2 + 2) {
      if (g->hunger_points > 0 || g->tick % FPS >= FPS / 2)
        image_draw(g->display, &image_hunger_full, 8 * i, 0, 0);
    } else if (g->hunger_points == i * 2 + 1) {
      if (g->hunger_points > 0 || g->tick % FPS >= FPS / 2)
        image_draw(g->display, &image_hunger_half, 8 * i, 0, 0);
    } else {
      if (g->hunger_points > 0 || g->tick % FPS >= FPS / 2)
        image_draw(g->display, &image_hunger_empty, 8 * i, 0, 0);
    }
  }

  // draw rabbit

  if (g->energy_points == 0) {
    image_draw_center_x(g->display, image, DISPLAY_WIDTH, DISPLAY_HEIGHT,
                        IMAGE_ORIGIN_BOTTOM);
  } else {
    switch (g->pos_f) {
      case FACING_LEFT:
        image_draw(g->display, image,
                   (int)(((double)(g->pos_x) / 100) * (double)(DISPLAY_WIDTH)),
                   DISPLAY_HEIGHT - g->pos_y, IMAGE_ORIGIN_BOTTOM);
        break;
      case FACING_RIGHT:
        image_draw(g->display, image,
                   (int)(((double)(g->pos_x) / 100) * (double)(DISPLAY_WIDTH)),
                   DISPLAY_HEIGHT - g->pos_y,
                   IMAGE_ORIGIN_BOTTOM | IMAGE_MIRROR_X);
        break;
      case FACING_FRONT:
        image_draw(g->display, image,
                   (int)(((double)(g->pos_x) / 100) * (double)(DISPLAY_WIDTH)),
                   DISPLAY_HEIGHT - g->pos_y, IMAGE_ORIGIN_BOTTOM);
        break;
    }
  }

  g->display->flush(g->display);

  // ---
  // end rendering
  // ---
}

void game_action(game_t* g, int action) {
  g->activity_frames += 16;

  switch (action) {
    case ACTION_POKE:
      if (g->pos_f == FACING_FRONT)
        g->pos_f = FACING_LEFT;
      break;
    case ACTION_FACE_FORWARD:
      g->pos_f = FACING_FRONT;
      break;
    case ACTION_FACE_LEFT:
      g->pos_x = g->pos_x - 1;
      g->pos_f = FACING_LEFT;
      break;
    case ACTION_FACE_RIGHT:
      g->pos_x = g->pos_x + 1;
      g->pos_f = FACING_RIGHT;
      break;
    case ACTION_JUMP:
      switch (g->pos_f) {
        case FACING_FRONT:
          // ???
          break;
        case FACING_RIGHT:
        case FACING_LEFT:
          if (g->pos_y == 0)
            g->jump_frames = 2;
          break;
      }
      break;
    case ACTION_RESET_SPRITE:
      g->sprite = &rabbit_sprite_basic;
      break;
    case ACTION_RANDOM_SPRITE:
      g->sprite = &rabbit_sprite_hat;
      break;
  }
}
