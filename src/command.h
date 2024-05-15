// --------------------------- Scan/print(cursor) user input--------------------------------------------
void options();
int input(int total_arg, char args_list[][50]);
int compare_arg(char args_list[][50]);
void execute(int no_input_arg, char args_list[][50]);

// font
void printC_16x14(int x, int y, char color[], char input_char);
void printS_16x14(int x, int y, char color[], char *input_string);
// void font();

// clear
void clear_screen();

// image
// void draw_image(int x, int y, const unsigned long *image0, int width, int height);
void draw_image();
void draw_image_large ();

// video
void video();