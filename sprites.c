#include "display.h"
#include "image.h"
#include "sprite.h"

#include "images/image_rabbit_basic_front_lowered.xbm"
#include "images/image_rabbit_basic_front_normal.xbm"
#include "images/image_rabbit_basic_front_sleeping.xbm"
#include "images/image_rabbit_basic_side_lowered.xbm"
#include "images/image_rabbit_basic_side_normal.xbm"
#include "images/image_rabbit_basic_side_sleeping.xbm"
#include "images/image_rabbit_hat_front_lowered.xbm"
#include "images/image_rabbit_hat_front_normal.xbm"
#include "images/image_rabbit_hat_front_sleeping.xbm"
#include "images/image_rabbit_hat_side_lowered.xbm"
#include "images/image_rabbit_hat_side_normal.xbm"
#include "images/image_rabbit_hat_side_sleeping.xbm"

#define DEFINE_RABBIT_SPRITE(name, image)                                     \
  rabbit_sprite_t rabbit_sprite_##name = {                                    \
      &image##_front_normal, &image##_front_lowered, &image##_front_sleeping, \
      &image##_side_normal,  &image##_side_lowered,  &image##_side_sleeping};

DEFINE_RABBIT_SPRITE(basic, image_rabbit_basic);
DEFINE_RABBIT_SPRITE(hat, image_rabbit_hat);
