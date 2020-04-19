#define IMAGE_ORIGIN_BOTTOM 1
#define IMAGE_ORIGIN_RIGHT 2
#define IMAGE_MIRROR_X 4
#define IMAGE_MIRROR_Y 8

typedef struct image_s {
  unsigned int width;
  unsigned int height;
  const unsigned char* bits;
} image_t;

void image_draw(display_t* display,
                image_t* im,
                unsigned int x,
                unsigned int y,
                unsigned int flags);
void image_draw_center_x(display_t* display,
                         image_t* im,
                         unsigned int w,
                         unsigned int y,
                         unsigned int flags);
void image_draw_center_y(display_t* display,
                         image_t* im,
                         unsigned int x,
                         unsigned int h,
                         unsigned int flags);
void image_draw_center(display_t* display,
                       image_t* im,
                       unsigned int x,
                       unsigned int h,
                       unsigned int flags);
