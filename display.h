#define DISPLAY_WIDTH 84
#define DISPLAY_HEIGHT 48

struct display_s;

typedef struct display_s {
  void* data;
  void (*clear)(struct display_s* d);
  void (*set_pixel)(struct display_s* d, int x, int y, char v);
  void (*flush)(struct display_s* d);
} display_t;
