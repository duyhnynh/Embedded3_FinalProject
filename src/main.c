#include "uart1.h"
#include "mbox.h"
#include "delay.h"
// #include "font.h"
#include "image.h"
#include "string.h"
#include "video.h"
#include "fb.h"

// #define SCR_HEIGH 768
// #define SCR_WIDTH 1024

#define MAX_CMD_SIZE 100
#define OS_NAME "Group2"
#define BG_IMAGE_WIDTH 1024
#define BG_IMAGE_HEIGHT 768
#define IMAGE_WIDTH 1920
#define IMAGE_HEIGHT 1080
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

//--------------------------------------------------------- Command -----------------------------------------------------------
// CommandList
void commandList()
{
    uart_puts("\n1. Show font size\n");
    uart_puts("   Please enter 'font'\n");
    uart_puts("2. Show a image\n");
    uart_puts("   Please enter 'image'\n");
    uart_puts("3. Show a video\n");
    uart_puts("   Please enter 'video'\n");
    uart_puts("4. Play a game\n");
    uart_puts("   Please enter 'game'\n");
    uart_puts("5. Clear screen\n");
    uart_puts("   Please enter 'clear'\n");
}
//---------------Background Image---------------//
void BackgroundImage()
{
    // Display the background image
    for (int y = 0; y < BG_IMAGE_HEIGHT; y++)
    {
        for (int x = 0; x < BG_IMAGE_WIDTH; x++)
        {
            drawPixelARGB32(x, y, image1[(y)*BG_IMAGE_WIDTH + (x)]);
        }
    }
}
//---------------welcome screen---------------//
void welcomeScreen()
{
    fb_init(1024, 768);
    // Display the background image
    BackgroundImage();
    // Display the course name and the OS NAME
    Introduction();

    // Wait for user to press a key to continue
    uart_puts("\nPress any key to continue...\n");
    uart_getc();
}
//---------------Introduction---------------//
void Introduction()
{
    drawString(200, 100, "Welcome to Group 2 OS", 0xFF00FF, 4);             // Magenta
    drawString(200, 500, "EEET2490 - Embedded Systems:", 0xFFFF99FF, 3);     // Pink
    drawString(150, 600, "Operating Systems and Interfacing", 0xFFFF99FF, 3);
}
//---------------font---------------//
void TeamName()
{
    drawString(200, 100, "Group 2", 0xFFFFA500, 5);                      // Orange
    drawString(200, 200, "Phan Trong Nguyen - s3927189", 0xFFFF99FF, 3); // Pink
    drawString(200, 300, "Hur Hyeonbin - s3740878", 0x0000FFFF, 3);      // Cyan
    drawString(200, 400, "Huynh Ngoc Duy - s3924704", 0x0000FF00, 3);    // Green
    drawString(200, 500, "Tran Thanh Tu - s3957386", 0xFFFF0000, 3);     // Red
}
//---------------video---------------//
void video()
{
    deleteImage(previousPosX, previousPosY, SCREEN_SIZE, SCREEN_SIZE);
    displayImage(350, 250, video1allArray[0], VID_WIDTH, VID_HEIGHT);
    for (int i = 1; i < FRAME; i++)
    {
        wait_msec(100000);

        displayImage(350, 250, video1allArray[i], VID_WIDTH, VID_HEIGHT);
    }
}
//---------------image---------------//
void image()
{
    int currentInputPosY = 0;
    fb_init(1024, 768);
    uart_puts("\nPress 's' to scroll down \n");
    uart_puts("Press 'w' to scroll up \n");
    uart_puts("Press 'a' to shift left \n");
    uart_puts("Press 'd' to shift left \n");
    uart_puts("Press ENTER to exit image! \n\n");
    // uart_puts(OS_NAME);
    // uart_puts(":> ");
    // index = 0;
    for (int y = 0; y < IMAGE_HEIGHT; y++)
    {
        for (int x = 0; x < IMAGE_WIDTH; x++)
        {
            drawPixelARGB32(x, y, image0[(y)*IMAGE_WIDTH + (x)]);
        }
    }

    char command = 0;

    while (1)
    {
        command = uart_getc();
        if (command == UP)
        {
            /* 31 times as i counted is the maximum number user can scroll down the image otherwise, it will started to overlap at the border top and bottom (left and right doesn't need as the image connected to itselft somehow? : D )*/
            if (currentInputPosY < 31 && currentInputPosY >= 0)
            {
                currentInputPosY += 1;
                previousPosY = currentPosY;
                currentPosY -= STEP; // scroll up by 10 pixels
                displayImage(currentPosX, currentPosY, image0, IMAGE_WIDTH, IMAGE_HEIGHT);
            }
        }
        else if (command == DOWN)
        {

            if (currentInputPosY <= 31 && currentInputPosY > 0)
            {
                currentInputPosY -= 1;
                previousPosY = currentPosY;
                currentPosY += STEP; // scroll down by 10 pixels
                displayImage(currentPosX, currentPosY, image0, IMAGE_WIDTH, IMAGE_HEIGHT);
            }
        }
        else if (command == LEFT)
        {
            previousPosX = currentPosX;
            currentPosX -= STEP; // scroll left by 10 pixels

            displayImage(currentPosX, currentPosY, image0, IMAGE_WIDTH, IMAGE_HEIGHT);
        }
        else if (command == RIGHT)
        {
            previousPosX = currentPosX;
            currentPosX += STEP; // scroll right by 10 pixels
            displayImage(currentPosX, currentPosY, image0, IMAGE_WIDTH, IMAGE_HEIGHT);
        }
        // Exit the image when user press ENTER
        if (command == 0xA)
        {
            break;
        }
    }
}
//--------------New Receive Method-----------------//

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
        command = strtok(cli_buffer, " ");

        /* Compare with supported commands and execute
         * ........................................... */
        if (strcmp(command, "font") == 0)
        {
            clear_screen();
            TeamName();
        }
        else if (strcmp(command, "image") == 0)
        {
            image();
        }
        else if (strcmp(command, "video") == 0)
        {
            video();
        }
        // else if (strcmp(command , "game") == 0 )
        // {
        //     game();
        // }
        else if (strcmp(command, "clear") == 0)
        {
            clear_screen();
            BackgroundImage();
            Introduction();
        }
        else
        {
            uart_puts("Invalid command. Please enter again!\n");
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
    //Display welcome screen
    welcomeScreen();
    // Print available command list
    commandList();
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