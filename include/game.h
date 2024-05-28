void startGame();
void showBackground(int shiftY, int stage);
void loadBlock(int start_x, int start_y, int stage);
unsigned int *create_block_array(unsigned int current_block);
void create_block(unsigned int *block_array, int stage);
void load_character(int start_w, int start_h, int direction);
void *show_timer(unsigned int curret_time);
void show_die_character_fn(int start_w, int start_h, int direction, int is_jump);
void show_phase(int phase);
void show_jump(int start_w, int start_h, int direction);
void show_stage_clear(int stage);
int show_game_over_fn();
void all_clear();
void load_monster_for_complete_screen(unsigned int start_h);
unsigned int *create_monster_array(int *monster_position_array, unsigned int current_monster_index, int stage);
void create_bullet(int *monster_position_array, int current_bullet_w, int stage, unsigned int *monster_index);
void re_load_black(int start_w, int start_h, int img_w, int img_h);
void load_background_with_transition(unsigned int *block_array, int stage, int shiftY, int current_w_index, int direction);
struct screenData
{
    unsigned int current_value;
    unsigned int prev_value;
};

extern struct screenData screen[1024][768];

extern struct screenData screen_for_transition[1024][768];
