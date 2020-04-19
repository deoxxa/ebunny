#define DISPLAY_WIDTH 84
#define DISPLAY_HEIGHT 48

struct display_s;

typedef struct display_s {
  void* data;
  void (*clear)(struct display_s* d);
  void (*set_pixel)(struct display_s* d, int x, int y, char v);
  void (*flush)(struct display_s* d);
  int flip;
  int x, y;
} display_t;

void display_init(display_t* d);
void display_flip(display_t* d, int flip);
void display_move(display_t* d, int x, int y);
void display_putc_5x8(display_t* d, char ch);
void display_puts_5x8(display_t* d, const char* s);
void display_putc_3x5(display_t* d, char ch);
void display_puts_3x5(display_t* d, const char* s);
