#include "display.h"
#include "image.h"

void image_draw(display_t* display,
                image_t* im,
                unsigned int x,
                unsigned int y,
                unsigned int flags) {
  if (flags & IMAGE_ORIGIN_BOTTOM) {
    y -= im->height;
  }
  if (flags & IMAGE_ORIGIN_RIGHT) {
    x -= im->width;
  }

  for (int px = 0; px < im->width; px++) {
    for (int py = 0; py < im->height; py++) {
      int bx = px;
      int by = py;

      if (flags & IMAGE_MIRROR_X) {
        bx = im->width - px - 1;
      }
      if (flags & IMAGE_MIRROR_Y) {
        by = im->height - py - 1;
      }

      display->set_pixel(
          display, x + px, y + py,
          im->bits[(by * (im->width / 8 + ((im->width % 8) != 0))) + (bx / 8)] &
              (1 << (bx % 8)));
    }
  }
}

void image_draw_center_x(display_t* display,
                         image_t* im,
                         unsigned int w,
                         unsigned int y,
                         unsigned int flags) {
  image_draw(display, im, w / 2 - im->width / 2, y, flags);
}

void image_draw_center_y(display_t* display,
                         image_t* im,
                         unsigned int x,
                         unsigned int h,
                         unsigned int flags) {
  image_draw(display, im, x, h / 2 - im->height / 2, flags);
}

void image_draw_center(display_t* display,
                       image_t* im,
                       unsigned int w,
                       unsigned int h,
                       unsigned int flags) {
  image_draw(display, im, w / 2 - im->width / 2, h / 2 - im->height / 2, flags);
}
