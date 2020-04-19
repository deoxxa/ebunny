#define FACING_FRONT 0
#define FACING_LEFT 1
#define FACING_RIGHT 2

typedef struct game_s {
  unsigned int tick;
  display_t* display;
  rabbit_sprite_t* sprite;
  unsigned int energy_points;
  unsigned int hunger_points;
  unsigned int interest_points;
  unsigned int jump_frames;
  unsigned int activity_frames;
  int pos_x;
  int pos_y;
  unsigned int pos_f;
} game_t;

#define ACTION_POKE 0
#define ACTION_FACE_FORWARD 1
#define ACTION_FACE_LEFT 2
#define ACTION_FACE_RIGHT 3
#define ACTION_JUMP 4
#define ACTION_RESET_SPRITE 5
#define ACTION_RANDOM_SPRITE 6

void game_init(game_t* g, display_t* display);
void game_tick(game_t* g);
void game_action(game_t* g, int action);
