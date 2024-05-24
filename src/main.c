#include "uart1.h"
#include "mbox.h"
#include "delay.h"
// #include "font.h"
#include "image.h"
#include "string.h"
#include "video.h"
#include "fb.h"

#define MAX_CMD_SIZE 100
#define OS_NAME "Group2"
#define IMAGE_WIDTH 1024
#define IMAGE_HEIGHT 768
#define SCREEN_SIZE 2000 // Define the dimension to clear a larger area than the image itself
#define STEP 10          // Define the number of pixels to move the image with each key press
#define UP 'w'
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'
#define VID_WIDTH 320
#define VID_HEIGHT 240
int currentPosX = 0;  // Current horizontal position of the image
int currentPosY = 0;  // Current vertical position of the image
int previousPosX = 0; // Previous horizontal position of the image
int previousPosY = 0; // Previous vertical position of the image

// font
void TeamName()
{
    drawString(200, 100, "Group 2", 0xFFFFA500, 5);           // Orange
    drawString(200, 200, "Phan Trong Nguyen", 0xFFFF99FF, 3); // Pink
    drawString(200, 300, "Hur Hyeonbin", 0x0000FFFF, 3);      // Cyan
    drawString(200, 400, "Huynh Ngoc Duy", 0x0000FF00, 3);    // Green
    drawString(200, 500, "Tran Thanh Tu", 0xFFFF0000, 3);     // Red
}
void commandList()
{
    uart_puts("\n1. Show font size\n");
    uart_puts("   Please enter 'font'\n");
    uart_puts("2. Show a small image\n");
    uart_puts("   Please enter 'image'\n");
    uart_puts("3. Show a larger image\n");
    uart_puts("   Please enter 'image large'\n");
    uart_puts("4. Show a video\n");
    uart_puts("   Please enter 'video'\n");
    uart_puts("5. Play a game\n");
    uart_puts("   Please enter 'game'\n");
    uart_puts("6. Clear screen\n");
    uart_puts("   Please enter 'cls'\n");
}
//--------------New Receive Method-----------------//
int cus_strcmp(const char *s1, const char *s2)
{
    while (*s1 && *s2 && *s1 == *s2)
    {
        ++s1;
        ++s2;
    }
    return *s1 - *s2;
}
char *cus_strtok(char *str, const char *delim)
{
    static char *p = (char *)0;
    if (*str != '\0')
        p = str;

    if (p == (char *)0 || *p == '\0')
        return (char *)0;

    char *start = p;

    while (*p != '\0')
    {
        const char *d = delim;
        while (*d != '\0')
        {
            if (*p == *d)
            {
                *p = '\0';
                ++p;
                if (*p == '\0')
                    p = (char *)0;
                return start;
            }
            ++d;
        }
        ++p;
    }

    return start;
}

void cli()
{

    static char cli_buffer[MAX_CMD_SIZE];
    static int index = 0;
    static char *command;
    char c = uart_getc();
    if (c != '\b')
    {
        uart_sendc(c);
    }
    // put into a buffer until got new line or get backspace character
    if (c != '\n' && c != '\b')
    {
        cli_buffer[index] = c; // Store into the buffer
        index++;
    }

    // if user typein backspace it will delete the output and shift the index --
    if (c == '\b')
    {
        if (index > 0)
        {
            uart_puts("\b \b");
            index--;
            cli_buffer[index] = '\0';
        }
    }

    else if (c == '\n')
    {

        cli_buffer[index] = '\0';
        command = cus_strtok(cli_buffer, " ");

        /* Compare with supported commands and execute
         * ........................................... */
        // HELP FUNCTION
        if (cus_strcmp(command, "font") == 0)
        {
            TeamName();
        }
        else if (cus_strcmp(command, "video") == 0)
        {
            deleteImage(previousPosX, previousPosY, SCREEN_SIZE, SCREEN_SIZE);
            displayImage(350, 250, video1allArray[0], VID_WIDTH, VID_HEIGHT);
            for (int i = 1; i < FRAME; i++)
            {
                wait_msec(100000);
                deleteImage(350, 250, VID_WIDTH, VID_HEIGHT);
                displayImage(350, 250, video1allArray[i], VID_WIDTH, VID_HEIGHT);
            }
        }
        else if (cus_strcmp(command, "clear") == 0)
        {
            fb_init(1920, 1080);
            for (int i = 0; i < 1920; i++)
            {
                for (int j = 0; j < 1080; j++)
                {
                    drawPixelARGB32(i, j, 0x00000000);
                }
            }
        }

        //-----image-----//
        if (command == UP)
        {
            previousPosY = currentPosY;
            currentPosY -= STEP; // scroll up by 10 pixels
            deleteImage(currentPosX, previousPosY, SCREEN_SIZE, SCREEN_SIZE);
            displayImage(currentPosX, currentPosY, image0, IMAGE_WIDTH, IMAGE_HEIGHT);
        }
        else if (command == DOWN)
        {
            previousPosY = currentPosY;
            currentPosY += STEP; // scroll down by 10 pixels
            deleteImage(previousPosX, previousPosY, SCREEN_SIZE, SCREEN_SIZE);
            displayImage(currentPosX, currentPosY, image0, IMAGE_WIDTH, IMAGE_HEIGHT);
        }
        else if (command == LEFT)
        {
            previousPosX = currentPosX;
            currentPosX -= STEP; // scroll down by 10 pixels
            deleteImage(previousPosX, previousPosY, SCREEN_SIZE, SCREEN_SIZE);
            displayImage(currentPosX, currentPosY, image0, IMAGE_WIDTH, IMAGE_HEIGHT);
        }
        else if (command == RIGHT)
        {
            previousPosX = currentPosX;
            currentPosX += STEP; // scroll down by 10 pixels
            deleteImage(previousPosX, previousPosY, SCREEN_SIZE, SCREEN_SIZE);
            displayImage(currentPosX, currentPosY, image0, IMAGE_WIDTH, IMAGE_HEIGHT);
        }

        uart_puts(OS_NAME);
        uart_puts(":> ");
        index = 0;
    }
}
void main()
{
    // set up serial console
    uart_init();
    // Initialize frame buffer with specific width and height
    fb_init(1024, 768);
    // Initialize frame buffer
    // font
    TeamName();
    // Print OS line
    uart_puts("\n");
    uart_puts(OS_NAME);
    uart_puts(":> ");
    // echo everything back
    while (1)
    {
        cli();
    }
}