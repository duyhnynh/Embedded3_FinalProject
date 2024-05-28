#include "../include/utils.h"
// #include "./font.h"
// #include "./framebf.h"
#include "../include/fb.h"
#include "../include/uart1.h"
// #include "./game.h"
#include "../include/game.h"

#define screen_width 1024
#define screen_height 768

struct screenData screen[1024][768];
struct screenData screen_for_transition[1024][768];

#define background_og_height 948
#define background_og_width 177
#define background_scaleX (1024.0 / background_og_width)
#define background_scaled_width screen_width
#define background_scaled_height (background_og_height * background_scaleX)
// void walking_character()
// {

//     int char_w;
//     int char_h;
//     int char_w_coor;
//     int char_h_coor;

//     for (int w = char_w_coor; w < char_w_coor + char_w; w++)
//     {
//         for (int h = char_h_coor; h < char_h_coor + char_h; h++)
//         {
//             unsigned int attr;
//             attr = epd_bitmap_right_stand[(h - char_h_coor) * char_w + (w - char_w_coor)];
//             if (attr != 0x00000000)
//             {
//                 drawPixelARGB32(w, h, attr);
//             }
//         }
//     }
// }

void re_load_black(int start_w, int start_h, int img_w, int img_h)
{
    for (int w = start_w; w < start_w + img_w; w++)
    {
        for (int h = start_h; h < start_h + img_h; h++)
        {
            drawPixelARGB32(w, h, 0x000000);
        }
    }
}

void create_bullet(int *monster_position_array, int current_bullet_w, int stage, unsigned int *monster_index)
{

    // int monster_array_value = 651;
    // unsigned int monster_position_array[12];
    // for (int i = 0; i < 11; i++)
    // {
    //     monster_position_array[i] = monster_array_value;
    //     monster_array_value -= 57;
    // }

    for (int i = 0; i < 11; i++)
    {
        if (monster_index[i] == 1)
        {
            if (current_bullet_w == 63)
            {
                re_load_background(963, monster_position_array[i], 50, 50);
            }

            re_load_background(current_bullet_w, monster_position_array[i], 50, 50);

            load_bullet(current_bullet_w + 20, monster_position_array[i], stage);
        }
    }
    // if (current_bullet_w == 1020)
    // {
    //     current_bullet_w = 63;
    // }
    // else
    // {
    //     current_bullet_w += 20;
    // }

    // return current_bullet_w;
}

void load_bullet(int current_bullet_w, int current_bullet_h, int stage)
{
    int bullet_w = 50;
    int bullet_h = 50;

    for (int w = current_bullet_w; w < current_bullet_w + bullet_w; w++)
    {
        for (int h = current_bullet_h; h < current_bullet_h + bullet_h; h++)
        {
            unsigned int attr;
            if (stage == 1)
            {
                attr = epd_bitmap_stage1_ball[(h - current_bullet_h) * bullet_w + (w - current_bullet_w)];
            }
            else if (stage == 2)
            {
                attr = epd_bitmap_stage2_ball[(h - current_bullet_h) * bullet_w + (w - current_bullet_w)];
            }
            else if (stage == 3)
            {
                attr = epd_bitmap_stage3_ball[(h - current_bullet_h) * bullet_w + (w - current_bullet_w)];
            }

            if (attr != 0x00000000)
            {
                drawPixelARGB32(w, h, attr);
            }
        }
    }
}

void all_clear()
{
    int img_w = 100;
    int img_h = 99;

    int right_start_w = 700;
    int right_start_h = 300;

    int left_start_w = 200;
    int left_start_h = 300;

    int x = 1024;
    int y = 768;

    for (int i = 0; i < y; i++)
    {
        for (int j = 0; j < x; j++)
        {
            drawPixelARGB32(j, i, 0x000000);
        }
    }

    for (int i = left_start_w; i < left_start_w + img_w; i++)
    {
        for (int j = left_start_h; j < left_start_h + img_h; j++)
        {
            unsigned int attr;
            attr = epd_bitmap_fire_left[(j - left_start_h) * img_w + (i + left_start_w)];
            if (attr != 0x00000000)
            {
                drawPixelARGB32(i, j, attr);
            }
        }
    }

    for (int i = right_start_w; i < right_start_w + img_w; i++)
    {
        for (int j = right_start_h; j < right_start_h + img_h - 12; j++)
        {
            unsigned int attr;
            attr = epd_bitmap_fire_right[(j - right_start_h) * img_w + (i + right_start_w)];
            if (attr != 0x00000000)
            {
                drawPixelARGB32(i, j, attr);
            }
        }
    }

    drawString(420, 300, "Complete", 0x00AA0000, 3);
    // load_monster_for_complete_screen(600);
}

void startGame()
{
    drawString(330, 200, "Infinity stair", 0x00AA0000, 3);
    drawString(340, 400, "Press Enter to start", 0x0000BB00, 2);
}

int show_game_over_fn()
{
    int x = 1024;
    int y = 768;

    for (int i = 0; i < y; i++)
    {
        wait_msec(1);
        for (int j = 0; j < x; j++)
        {
            drawPixelARGB32(j, i, 0x000000);
        }
    }

    drawString(400, 200, "Game Over", 0x00AA0000, 3);
    drawString(300, 400, "Press \"r\" to restart the game", 0x0000BB00, 2);
    drawString(300, 600, "Press \"q\" to escape the game", 0x0000FF, 2);

    while (1)
    {
        unsigned char c = uart_getc();
        if (c == 'r')
        {
            break;
            return 0;
        }
        else if (c == 'q')
        {
            return 1;
        }
    }
}

void showBackground(int shiftY, int stage)
{
    int originalHeight = 948;                   // 상단 절반의 원래 높이
    int originalWidth = 177;                    // 원본 가로 크기
    float scaleX = 1024.0 / originalWidth;      // 가로 스케일 비율
    int scaledWidth = 1024;                     // 스케일된 가로 크기, 화면을 꽉 채움
    int scaledHeight = originalHeight * scaleX; // 스케일된 세로 크기

    // int yOffset = (768 - scaledHeight) / 2; // 세로 중앙 정렬을 위한 Y 오프셋
    // int shiftY = -350;                      // 아래로 이동시킬 픽셀 수
    for (int y = 0; y < 768; y++)
    {
        for (int x = 0; x < 1024; x++)
        {
            int srcY = y / scaleX - shiftY;
            int srcX = x / scaleX;                     // 원본 이미지에서의 X 인덱스
            int index = (srcY * originalWidth + srcX); // 원본 이미지의 RGB888 데이터 인덱스

            unsigned int attr;
            if (stage == 1)
            {
                attr = epd_bitmap_stage1[index];
            }
            else if (stage == 2)
            {
                attr = epd_bitmap_stage2[index];
            }
            else if (stage == 3)
            {
                attr = epd_bitmap_stage3[index];
            }
            screen[x][y].prev_value = attr;
            drawPixelARGB32(x, y, attr); // Y 오프셋을 적용하여 중앙에 이미지를 그림
        }
    }
}

void loadBlock(int start_x, int start_y, int stage)
{
    int img_w = 75;
    int img_h = 40;

    for (int y = start_y; y < start_y + img_h; y++)
    {

        for (int x = start_x; x < start_x + img_w; x++)
        {
            unsigned int attr;
            if (stage == 1)
            {
                attr = epd_bitmap_stage_1_block[(y - start_y) * img_w + (x - start_x)];
            }
            else if (stage == 2)
            {
                attr = epd_bitmap_stage_2_block[(y - start_y) * img_w + (x - start_x)];
            }
            else if (stage == 3)
            {
                attr = epd_bitmap_stage_3_block[(y - start_y) * img_w + (x - start_x)];
            }
            screen[x][y].prev_value = attr;
            drawPixelARGB32(x, y, attr);
        }
    }
}

void load_character(int start_w, int start_h, int direction)
{
    // uart_sendi(start_h);
    int character_w = 70;
    int character_h = 120;
    // if (direction == 1)
    // {
    //     re_load_background(start_w - 38, start_h + 17, character_w, 130);
    // }
    // else if (direction == 0)
    // {
    //     re_load_background(start_w + 38, start_h + 17, character_w, 130);
    // }
    for (int h = start_h; h < start_h + character_h; h++)
    {
        for (int w = start_w; w < start_w + character_w; w++)
        {
            unsigned int attr;
            if (direction == 1)
            {
                attr = epd_bitmap_right_stand[(h - start_h) * character_w + (w - start_w)];
            }
            else if (direction == 0)
            {
                attr = epd_bitmap_left_stand[(h - start_h) * character_w + (w - start_w)];
            }
            if (attr != 0x00000000)
            {
                drawPixelARGB32(w, h, attr);
            }
        }
    }
}

void show_die_character_fn(int start_w, int start_h, int direction, int is_jump)
{
    int character_w = 110;
    int character_h = 69;

    if (is_jump == 1)
    {
        if (direction == 1)
        {
            re_load_background(start_w - 75, start_h + 57, 70, 130);
            start_w += 50;
        }
        else if (direction == 0)
        {
            re_load_background(start_w + 75, start_h + 57, 70, 130);
            start_w -= 50;
        }
    }
    else
    {
        re_load_background(start_w, start_h, 70, 130);
    }

    start_h += 100;
    for (int h = start_h; h < start_h + character_h; h++)
    {
        for (int w = start_w; w < start_w + character_w; w++)
        {
            unsigned int attr;
            if (direction == 1)
            {
                attr = epd_bitmap_right_die[(h - start_h) * character_w + (w - start_w)];
            }
            else if (direction == 0)
            {
                attr = epd_bitmap_left_die[(h - start_h) * character_w + (w - start_w)];
            }
            if (attr != 0x00000000)
            {
                drawPixelARGB32(w, h, attr);
            }
        }
    }
}

unsigned int *create_block_array(unsigned int current_block)
{
    static unsigned int block_array[13];
    block_array[0] = current_block;
    for (int i = 1; i < 13; i++)
    {
        int temp = block_array[i - 1];

        if (temp - 75 < 100)
        {
            block_array[i] = temp + 75;
        }
        else if (temp + 75 > 900)
        {
            block_array[i] = temp - 75;
        }
        else
        {
            int n = generateRandomBit();
            if (n == 1)
            {
                block_array[i] = temp + 75;
            }
            else if (n == 0)
            {
                block_array[i] = temp - 75;
            }
        }
    }
    return block_array;
}

unsigned int *create_monster_array(int *monster_position_array, unsigned int current_monster_index, int stage)
{
    static unsigned int moster_index_arr[12];
    for (int i = 0; i < 11; i++)
    {
        moster_index_arr[i] = 0;
    }

    if (stage == 1)
    {
        for (int i = 0; i < 3; i++)
        {
            int monster_index = 0;
            for (int j = 0; j < 10; j++)
            {
                int n = generateRandomBit();
                monster_index += n;
            }
            moster_index_arr[monster_index] = 1;
        }
    }

    else if (stage == 2)
    {
        for (int i = 0; i < 5; i++)
        {
            int monster_index = 0;
            for (int j = 0; j < 10; j++)
            {
                int n = generateRandomBit();
                monster_index += n;
            }
            moster_index_arr[monster_index] = 1;
        }
    }

    else if (stage == 3)
    {
        for (int i = 0; i < 7; i++)
        {
            int monster_index = 0;
            for (int j = 0; j < 10; j++)
            {
                int n = generateRandomBit();
                monster_index += n;
            }
            moster_index_arr[monster_index] = 1;
        }
    }
    // uart_puts("complete load monster arr \n");

    create_monster(monster_position_array, moster_index_arr, stage);

    return moster_index_arr;
}

void create_monster(int *monster_position_array, unsigned int *monster_index, int stage)
{
    // int monster_array_value = 651;
    // unsigned int monster_position_array[12];
    // for (int i = 0; i < 11; i++)
    // {
    //     monster_position_array[i] = monster_array_value;
    //     monster_array_value -= 57;
    // }

    for (int i = 0; i < 11; i++)
    {
        // uart_puts("complete create monster \n");

        if (monster_index[i] == 1)
        {
            load_monster(monster_position_array[i], stage);
        }
    }
}
void load_monster_for_complete_screen(unsigned int start_h)
{
    int monster_w = 53;
    int monster_h = 53;
    for (int w = 10; w < 10 + monster_w; w++)
    {
        for (int h = start_h; h < start_h + monster_h; h++)
        {
            unsigned int attr;
            attr = epd_bitmap_stage1_monster[(h - start_h) * monster_w + (w - 10)];
            if (attr != 0x00000000)
            {
                drawPixelARGB32(w, h, attr);
            }
        }
    }
}

void load_monster(unsigned int start_h, int stage)
{
    int monster_w = 53;
    int monster_h = 53;
    if (stage == 1)
    {
        for (int w = 10; w < 10 + monster_w; w++)
        {
            for (int h = start_h; h < start_h + monster_h; h++)
            {
                unsigned int attr;
                attr = epd_bitmap_stage1_monster[(h - start_h) * monster_w + (w - 10)];
                if (attr != 0x00000000)
                {
                    drawPixelARGB32(w, h, attr);
                }
            }
        }
    }
    else if (stage == 2)
    {
        for (int w = 10; w < 10 + monster_w; w++)
        {
            for (int h = start_h; h < start_h + monster_h; h++)
            {
                unsigned int attr;
                attr = epd_bitmap_stage2_monster[(h - start_h) * monster_w + (w - 10)];
                if (attr != 0x00000000)
                {
                    drawPixelARGB32(w, h, attr);
                }
            }
        }
    }
    else if (stage == 3)
    {
        for (int w = 10; w < 10 + monster_w; w++)
        {
            for (int h = start_h; h < start_h + monster_h; h++)
            {
                unsigned int attr;
                attr = epd_bitmap_stage3_monster[(h - start_h) * monster_w + (w - 10)];
                if (attr != 0x00000000)
                {
                    drawPixelARGB32(w, h, attr);
                }
            }
        }
    }
}

void create_block(unsigned int *block_array, int stage)
{
    int h = 708;
    for (int i = 0; i < 13; i++)
    {
        loadBlock(block_array[i], h, stage);
        h -= 57;
    }
}

void *show_timer(unsigned int curret_time)
{

    char str[3];
    int i = 0;

    if (curret_time < 10)
    {
        str[i++] = '0';
    }

    if (curret_time == 0)
    {
        str[i++] = '0';
    }
    else
    {
        int temp = curret_time;
        int digit_count = 0;

        while (temp > 0)
        {
            temp /= 10;
            digit_count++;
        }

        for (int j = digit_count - 1; j >= 0; j--)
        {
            str[i + j] = (curret_time % 10) + '0';
            curret_time /= 10;
        }
        i += digit_count;
    }

    str[i] = '\0';

    re_load_background(20, 20, 70, 40);
    drawString(20, 20, str, 0x00AA0000, 3);
}

void show_phase(int phase)
{
    // drawString(330, 200, "Infinity stair", 0x00AA0000, 3);
    char currnet_phase = phase + '0';
    char str[3];
    str[0] = currnet_phase;
    str[1] = '/';
    str[2] = '7';
    uart_puts(str[0]);
    uart_puts("\n");
    uart_puts(str[1]);
    uart_puts("\n");
    uart_puts(str[2]);
    uart_puts("\n");
    drawString(900, 20, str, 0x00AA0000, 3);
}

void show_jump(int start_w, int start_h, int direction)
{
    int character_w = 70;
    int character_h = 130;

    for (int h = start_h; h < start_h + character_h; h++)
    {
        for (int w = start_w; w < start_w + character_w; w++)
        {
            unsigned int attr;
            if (direction == 1)
            {
                attr = epd_bitmap_right_jump[(h - start_h) * character_w + (w - start_w)];
            }
            else if (direction == 0)
            {
                attr = epd_bitmap_left_jump[(h - start_h) * character_w + (w - start_w)];
            }
            if (attr != 0x00000000)
            {
                drawPixelARGB32(w, h, attr);
            }
        }
    }
}

void re_load_background(unsigned int start_w, unsigned int start_h, int img_w, int img_h)
{

    if (start_h == -22 || start_h == -79 || start_h == -39)
    {
        start_h = 0;
    }
    for (int w = start_w; w < start_w + img_w; w++)
    {
        for (int h = start_h; h < start_h + img_h; h++)
        {
            unsigned int attr = screen[w][h].prev_value;
            drawPixelARGB32(w, h, attr);
        }
    }
}

void show_stage_clear(int stage)
{

    int x = 1024;
    int y = 768;

    for (int i = 0; i < y; i++)
    {
        for (int j = 0; j < x; j++)
        {
            drawPixelARGB32(j, i, 0x000000);
        }
    }

    if (stage == 1)
    {

        drawString(360, 200, "Stage 1 Jungle", 0x00FF00, 3);
        drawString(290, 400, "Press \"Enter\" to start the game", 0x0000BB00, 2);
    }
    else if (stage == 2)
    {
        drawString(320, 200, "Stage 2 Antarctica", 0x0000FF, 3);
        drawString(280, 400, "Press \"Enter\" to start the game", 0x0000FF, 2);
    }
    else if (stage == 3)
    {
        drawString(290, 200, "Fianl Stage Volcano", 0xFF0000, 3);
        drawString(250, 400, "Press \"Enter\" to restart the game", 0xFF0000, 2);
    }
    unsigned char c;

    while (1)
    {
        c = uart_getc();
        if (c == '\n')
        {
            break;
        }
    }

    uart_puts("escape show stage \n");
}

void load_background_with_transition(unsigned int *block_array, int stage, int shiftY, int current_w_index, int direction)
{
    store_background_for_transition(shiftY, stage);
    store_block_for_transition(block_array, stage);
    store_character_for_transition(current_w_index, 708 - 120, direction);
    // wait_msec(1000);
    for (int i = 10; i > 0; i--)
    {

        for (int h = 0; h < 768 - i * (768 / 10); h++)
        {
            for (int w = 0; w < 1024; w++)
            {
                unsigned int attr = screen_for_transition[w][768 - (768 - i * (768 / 10)) + h].current_value;
                drawPixelARGB32(w, h, attr);
            }
        }
        uart_puts(" complete transition\n");
    }
}

void store_background_for_transition(int shiftY, int stage)
{
    int originalHeight = 948;                   // 상단 절반의 원래 높이
    int originalWidth = 177;                    // 원본 가로 크기
    float scaleX = 1024.0 / originalWidth;      // 가로 스케일 비율
    int scaledWidth = 1024;                     // 스케일된 가로 크기, 화면을 꽉 채움
    int scaledHeight = originalHeight * scaleX; // 스케일된 세로 크기

    // int yOffset = (768 - scaledHeight) / 2; // 세로 중앙 정렬을 위한 Y 오프셋
    // int shiftY = -350;                      // 아래로 이동시킬 픽셀 수
    for (int y = 0; y < 768; y++)
    {
        for (int x = 0; x < 1024; x++)
        {
            int srcY = y / scaleX - shiftY;
            int srcX = x / scaleX;                     // 원본 이미지에서의 X 인덱스
            int index = (srcY * originalWidth + srcX); // 원본 이미지의 RGB888 데이터 인덱스

            unsigned int attr;
            if (stage == 1)
            {
                attr = epd_bitmap_stage1[index];
            }
            else if (stage == 2)
            {
                attr = epd_bitmap_stage2[index];
            }
            else if (stage == 3)
            {
                attr = epd_bitmap_stage3[index];
            }

            screen_for_transition[x][y].current_value = attr;
        }
    }
}
void store_block_for_transition(unsigned int *block_array, int stage)
{
    int h = 708;
    for (int i = 0; i < 13; i++)
    {
        store_block_for_transition2(block_array[i], h, stage);
        h -= 57;
    }
}
void store_block_for_transition2(int start_x, int start_y, int stage)
{
    for (int i = 0; i < 13; i++)
    {
        int img_w = 75;
        int img_h = 40;

        for (int y = start_y; y < start_y + img_h; y++)
        {
            for (int x = start_x; x < start_x + img_w; x++)
            {
                unsigned int attr;
                if (stage == 1)
                {
                    attr = epd_bitmap_stage_1_block[(y - start_y) * img_w + (x - start_x)];
                }
                else if (stage == 2)
                {
                    attr = epd_bitmap_stage_2_block[(y - start_y) * img_w + (x - start_x)];
                }
                else if (stage == 3)
                {
                    attr = epd_bitmap_stage_3_block[(y - start_y) * img_w + (x - start_x)];
                }
                screen_for_transition[x][y].current_value = attr;
            }
        }
    }
}
void store_character_for_transition(int start_w, int start_h, int direction)
{
    int character_w = 70;
    int character_h = 120;

    for (int h = start_h; h < start_h + character_h; h++)
    {
        for (int w = start_w; w < start_w + character_w; w++)
        {
            unsigned int attr;
            if (direction == 1)
            {
                attr = epd_bitmap_right_stand[(h - start_h) * character_w + (w - start_w)];
            }
            else if (direction == 0)
            {
                attr = epd_bitmap_left_stand[(h - start_h) * character_w + (w - start_w)];
            }
            if (attr != 0x00000000)
            {
                screen_for_transition[w][h].current_value = attr;
            }
        }
    }
}

// void shoot_Ball(unsigned int *array)