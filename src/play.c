#include "../include/mbox.h"
#include "../include/uart1.h"
#include "../include/fb.h"
#include "../include/utils.h"
#include "../include/game.h"
#include "../include/main.h"

unsigned int first_block;  // first block in each stage
unsigned int *block_array; // the random block array
unsigned int game_start;   // if game is started, change to 1

unsigned int current_w_index; // character's start w index
unsigned int current_h_index; // character's start h index

unsigned int step;       // if it reach 12, background is changed
int game_over_flag;      // if user die, change to 1
unsigned int ms_counter; // check the time
unsigned int is_jump;    // is the character jumping? 0 = no jumping, 1 = is jumping

unsigned int timer;
unsigned int phase;
unsigned int stage_start_flag; // stage start flag

unsigned int is_load_flag;
int shiftY;
int stage;
int direction; // 1 is right, 0 is left
unsigned int *monster_index;
unsigned int current_bullet_w;

int monster_array_value;
unsigned int monster_position_array[12];
int die_by_bullet;

int escape_game;
int immutal = 0;

void all_clear_fn();

void game_start_fn();

void game_init_fn();

int is_die_check(int current_character, int current_block);

void start_new_stage(int stage);

void pause_mode();
void welcome_screen();

void game_play()
{
    // set up serial console
    uart_init();
    // Initialize frame buffer
    // framebf_init();
    fb_init(1024, 768);
    // all_clear_fn();
    unsigned char c;
    escape_game = 0;

    while (1)
    {
        if (game_start == 1)
        {
            // load items logic
            if (stage_start_flag == 0)
            {
                show_stage_clear(stage);
                stage_start_flag = 1;
                start_new_stage(stage);
            }
            if (immutal == 1)
            {
                game_over_flag = 0;
            }

            if (step == 0 && is_load_flag == 0 && game_over_flag == 0)
            {
                is_load_flag = 1;
                if (stage == 1)
                {
                    showBackground(shiftY, stage);
                    create_block(block_array, stage);
                    monster_index = create_monster_array(monster_position_array, monster_index, stage);
                }
                else if (stage == 2)
                {
                    showBackground(shiftY, stage);
                    create_block(block_array, stage);
                    monster_index = create_monster_array(monster_position_array, monster_index, stage);
                }
                else if (stage == 3)
                {
                    showBackground(shiftY, stage);
                    create_block(block_array, stage);
                    monster_index = create_monster_array(monster_position_array, monster_index, stage);
                }

                show_phase(phase);
            }
            if (is_jump == 0 && game_over_flag == 0)
            {
                load_character(current_w_index, current_h_index, direction);
            }

            set_wait_timer(1, 10);
            c = getUart();
            set_wait_timer(0, 0);
            ms_counter++;

            show_timer(timer);

            // move logic
            if (is_jump == 1)
            {
                is_jump = 0;
                if (direction == 1)
                {
                    current_w_index += 38;
                    current_h_index -= 17;
                }
                else if (direction == 0)
                {
                    current_w_index -= 38;
                    current_h_index -= 17;
                }

                show_jump(current_w_index, current_h_index, direction);
                wait_msec_game(100);

                re_load_background(current_w_index, current_h_index, 70, 130);
            }

            else if (is_jump == 0)
            {
                if (c == 'a')
                {
                    step += 1;
                    re_load_background(current_w_index, current_h_index, 70, 120);
                    current_w_index -= 37;
                    current_h_index -= 40;
                    direction = 0;
                    is_jump = 1;
                    // re load at here
                }

                else if (c == 'd')
                {
                    step += 1;
                    re_load_background(current_w_index, current_h_index, 70, 120);
                    current_w_index += 37;
                    current_h_index -= 40;
                    direction = 1;
                    is_jump = 1;
                    // re load at here
                }
                else if (c == ' ')
                {
                    uart_puts("\" space \" pressed \n");
                    pause_mode();
                    re_load_background(450, 300, 200, 50);
                }
            }

            if (step == 12 && is_jump == 0)
            {
                is_load_flag = 0;
                step = 0;
                phase++;
                shiftY += 100;
                current_h_index = 708 - 120;
                current_bullet_w = 63;
                block_array = create_block_array(block_array[12]);
                // load_background_with_transition(block_array, stage, shiftY, current_w_index, direction);
            }

            // die logic
            if (step != 0)
            {
                if (is_jump == 1)
                {
                    if (direction == 0)
                    {
                        game_over_flag = is_die_check(current_w_index - 38, block_array[step]);
                    }
                    else if (direction == 1)
                    {
                        game_over_flag = is_die_check(current_w_index + 38, block_array[step]);
                    }
                }
                else
                {
                    if (direction == 0)
                    {
                        game_over_flag = is_die_check(current_w_index, block_array[step]);
                    }
                    else if (direction == 1)
                    {
                        game_over_flag = is_die_check(current_w_index, block_array[step]);
                    }
                }

                if (game_over_flag == 1)
                {
                    uart_puts("\nfalling off from the block..\n");
                }
            }
            die_by_bullet = is_shot_fatal_check();
            if (die_by_bullet == 1)
            {
                game_over_flag = 1;
            }
            if (game_over_flag == 0)
            {
                if (stage == 1)
                {
                    if (ms_counter % 10 == 0)
                    {
                        create_bullet(monster_position_array, current_bullet_w, stage, monster_index);
                        current_bullet_w += 20;
                        if (current_bullet_w == 963)
                        {
                            current_bullet_w = 63;
                        }
                    }
                }
                else if (stage == 2)
                {
                    if (ms_counter % 5 == 0)
                    {
                        create_bullet(monster_position_array, current_bullet_w, stage, monster_index);
                        current_bullet_w += 20;
                        if (current_bullet_w == 963)
                        {
                            current_bullet_w = 63;
                        }
                    }
                }
                else if (stage == 3)
                {
                    if (ms_counter % 2 == 0)
                    {
                        create_bullet(monster_position_array, current_bullet_w, stage, monster_index);
                        current_bullet_w += 20;
                        if (current_bullet_w == 963)
                        {
                            current_bullet_w = 63;
                        }
                    }
                }

                if (ms_counter == 100)
                {
                    ms_counter = 0;
                    if (timer == 1)
                    {
                        uart_puts("\nTime out!!\n");

                        game_over_flag = 1;
                    }
                    timer -= 1;
                }
                // move logic
                if (shiftY == 0)
                {
                    is_load_flag = 0;
                    shiftY = -700;
                    stage++;
                    stage_start_flag = 0;
                    if (stage == 1)
                    {
                        uart_puts("\nStage 1 : Jungle start!\n");
                    }
                    else if (stage == 2)
                    {
                        uart_puts("\nStage 2 : Antarctica start!\n");
                    }
                    else if (stage == 3)
                    {
                        uart_puts("\nStage 3 : Volcano start!\n");
                    }
                    if (stage == 4)
                    {
                        all_clear_fn();
                    }
                }
            }

            if (game_over_flag == 1 && immutal == 0) // game over
            {

                show_die_character_fn(current_w_index, current_h_index, direction, is_jump);
                if (die_by_bullet == 1)
                {
                    for (int i = 0; i < 11; i++)
                    {
                        if (monster_index[i] == 1)
                        {
                            load_bullet(current_bullet_w, monster_position_array[i], stage);
                        }
                    }
                }
                wait_msec_game(600);
                escape_game = show_game_over_fn();
                game_over_flag = 0;
                game_start = 0;
            }
        }

        else if (game_start == 0)
        {
            game_init_fn();
            game_start = 1;
            game_start_fn();
            block_array = create_block_array(first_block);
        }

        if (escape_game == 1)
        {
            game_init_fn();
            break;
        }
    }
    task1();
}

int is_die_check(int current_character, int current_block)
{

    if (current_block == current_character)
    {
        return 0;
    }
    else
    {
        return 1; // 1
    }
}

void game_start_fn()
{
    startGame();
    welcome_screen();

    uart_puts("\nPress \"Enter\" to start the game !");
    uart_puts("\nPress \"a\" to move left top block");
    uart_puts("\nPress \"d\" to move right top block");

    uart_puts("\nLet's move to top\n");

    while (1)
    {
        char c = uart_getc();
        if (c == '\n')
        {
            break;
        }
    }
}

void game_init_fn()
{
    die_by_bullet = 0;
    current_bullet_w = 63;
    first_block = 399;
    current_w_index = 399;
    current_h_index = 588;
    step = 0;
    game_over_flag = 0;
    ms_counter = 0;
    timer = 25;
    phase = 1;
    shiftY = -700;
    stage = 1;
    direction = 1;
    is_load_flag = 0;
    is_jump = 0;
    stage_start_flag = 0;
    int x = 1024;
    int y = 768;

    monster_array_value = 651;

    for (int i = 0; i < y; i++)
    {
        for (int j = 0; j < x; j++)
        {
            drawPixelARGB32(j, i, 0x000000);
        }
    }

    for (int i = 0; i < 11; i++)
    {
        monster_position_array[i] = monster_array_value;
        monster_array_value -= 57;
    }
}

void start_new_stage(int stage)
{
    if (stage == 1)
    {
        timer = 30;
        phase = 1;
    }
    else if (stage == 2)
    {
        timer = 25;
        phase = 1;
    }
    else if (stage == 3)
    {
        timer = 20; // 15
        phase = 1;  // 1
    }
}

void all_clear_fn()
{
    all_clear();
    uart_puts("\nYOU CLEAR THE GAME!\n");
    int start_w = 100;
    int start_h = 600;
    int bullet_w = 100;
    unsigned char c;
    load_monster_for_complete_screen(700);
    drawString(300, 400, "Press r to restart the game", 0x0000BB00, 2);
    drawString(300, 500, "Press \"q\" to escape the game", 0x0000FF, 2);

    while (1)
    {
        set_wait_timer(1, 10);
        c = getUart();
        set_wait_timer(0, 0);
        ms_counter++;

        if (c == 'r')
        {
            game_init_fn();
            break;
        }

        else if (c == 'q')
        {
            escape_game = 1;
            break;
        }

        if (ms_counter == 50)
        {
            if (start_w == 100)
            {
                re_load_black(870, start_h, 70, 130);
            }
            re_load_black(start_w, start_h, 70, 130);
            start_w += 35;
            start_h -= 60;
            load_character(start_w, start_h, 1);
        }

        else if (ms_counter == 100)
        {
            re_load_black(start_w, start_h, 70, 120);
            start_w += 35;
            start_h += 60;
            show_jump(start_w, start_h, 1);
            ms_counter = 0;
        }

        if (ms_counter % 4 == 0)
        {
            re_load_black(bullet_w, 700, 50, 50);
            bullet_w += 10;
            load_bullet(bullet_w, 700, 1);
            if (bullet_w > 900)
            {
                re_load_black(bullet_w, 700, 50, 50);
                bullet_w = 100;
            }
        }

        if (start_w == 870)
        {
            start_w = 100;
        }
    }
}

void pause_mode()
{
    uart_puts("\nEntered to Pause mode\n");

    drawString(450, 300, "PAUSE", 0x00AA0000, 4);
    static char currentCommand[100];
    int charIndex = 0;
    while (1)
    {
        char c = uart_getc();
        if (c == ' ')
        {
            uart_puts("\nEscape Pause mode\n");
            break;
        }

        else if (c != ' ' && c != '\n')
        {
            if (c == 127)
            {
                if (charIndex > 0)
                {
                    currentCommand[charIndex] = '\0';
                    uart_sendc('\b');
                    uart_sendc(' ');
                    uart_sendc('\b');
                    charIndex--;
                }
            }
            else
            {
                uart_sendc(c);
                currentCommand[charIndex] = c;
                charIndex++;
            }
        }
        else if (c == '\n')
        {
            uart_sendc(c);

            currentCommand[charIndex] = '\0';
            if (my_strncmp(currentCommand, "stage_?", 7))
            {
                uart_puts("current stage is ");
                uart_sendi(stage);
                uart_puts("\n");
            }
            else if (my_strncmp(currentCommand, "block_?", 7))
            {
                int num_of_block = phase * 11 + 1 - (12 - step);
                uart_puts("you went up ");
                uart_sendi(num_of_block);
                uart_puts(" blocks \n");
            }
            else if (my_strncmp(currentCommand, "character_?", 11))
            {
                uart_puts("character's current x coordinate : ");
                uart_sendi(current_w_index);
                uart_puts("\n");
                uart_puts("character's current y coordinate : ");
                uart_sendi(current_h_index);
                uart_puts("\n");
            }
            else if (my_strncmp(currentCommand, "stage=", 6))
            {
                char *destination_stage = my_strstr(currentCommand, "=");
                destination_stage += 1;

                if (my_strncmp(destination_stage, "1", 1) == 1)
                {
                    stage = 1;
                    stage_start_flag = 0;
                    is_load_flag = 0;
                    shiftY = -700;
                    step = 0;
                    current_h_index = 708 - 120;
                    game_over_flag = 0;
                    current_w_index = block_array[0];
                    uart_puts("\n Successfully jumped to stage 1 \n");
                }
                else if (my_strncmp(destination_stage, "2", 1) == 1)
                {
                    stage = 2;
                    stage_start_flag = 0;
                    is_load_flag = 0;
                    shiftY = -700;
                    current_h_index = 708 - 120;
                    step = 0;
                    game_over_flag = 0;
                    current_w_index = block_array[0];
                    uart_puts("\n Successfully jumped to stage 2 \n");
                }
                else if (my_strncmp(destination_stage, "3", 1) == 1)
                {
                    stage = 3;
                    stage_start_flag = 0;
                    is_load_flag = 0;
                    shiftY = -700;
                    current_h_index = 708 - 120;
                    game_over_flag = 0;
                    step = 0;
                    current_w_index = block_array[0];

                    uart_puts("\n Successfully jumped to stage 3 \n");
                }
                else
                {
                    uart_puts("Available stages are only 1, 2, and 3 \n");
                }
            }
            else if (my_strncmp(currentCommand, "uart_config", 11))
            {
                uart_puts("current uart is uart 1 \n");
                uart_puts("baud rate = 115200 \n");
                uart_puts("data bit  = 8 \n");
                uart_puts("handshaking  = off \n");
            }
            else if (my_strncmp(currentCommand, "complete_screen", 15))
            {
                all_clear_fn();
            }

            else
            {
                uart_puts("\"");
                for (int i = 0; i < charIndex; i++)
                {
                    uart_sendc(currentCommand[i]);
                }

                uart_puts("\" is not supported command \n");
            }
            charIndex = 0;
        }
    }
}

int is_shot_fatal_check()
{
    int die_flag;
    for (int i = 0; i < 11; i++)
    {
        if (monster_index[i] == 1)
        {
            die_flag = is_character_die_by_bullet(current_bullet_w, monster_position_array[i], current_w_index, current_h_index);
            if (die_flag == 1)
            {
                uart_puts("\nMonster kills you!\n");

                return 1;
            }
        }
    }

    return 0;
}

int is_character_die_by_bullet(int bullet_w, int bullet_h, int character_w, int character_h)
{
    if (bullet_h - 63 != character_h)
    {
        return 0;
    }
    else
    {
        if (bullet_w + 30 > character_w && bullet_w + 50 < character_w + 70)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
}

void welcome_screen()
{
    uart_puts("\n\n\n\n\n");
    uart_puts(
        " _____       __ _        _           \n"
        "|_   _|    / _(_)     (_) |          \n"
        "  | |  __ | |_ _ _ __  _| |_ _ _   _ \n"
        "  | | |_ \\|  _| | '_ \\| | __  | | | |  \n"
        " _| |_| | | | | | | | | | | |_| |_| |  \n"
        "|_____|_| |_|_| |_|_| |_|_|\\__|\\__, |  \n"
        "    | |      (_)                __/ |  \n"
        " ___| |_ __ _ _ _ __ ___ __ _ _|___/__ \n"
        "/ __| __/ _` | | '__/ __/ _` / __|/ _ \\\n"
        "\\__ \\ || (_| | | | | (_| (_| \\__ \\  __/\n"
        "|___/\\__\\__,_|_|_|  \\___\\__,_|___/\\___|\n");
    uart_puts("\n\n\n");
}
