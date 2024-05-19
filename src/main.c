#include "uart1.h"
#include "mbox.h"
#include "delay.h"
// #include "font.h"
#include "image.h"
#define IMAGE_WIDTH 1024
#define IMAGE_HEIGHT 768
#define SREEN_SIZE 2000 // Define the dimension to clear a larger area than the image itself
#define STEP 10         // Define the number of pixels to move the image with each key press
#define UP 'w'
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'
// #define VIDEO_WIDTH 320
// #define VIDEO_HEIGHT 240
int currentPosX = 0;  // Current horizontal position of the image
int currentPosY = 0;  // Current vertical position of the image
int previousPosX = 0; // Previous horizontal position of the image
int previousPosY = 0; // Previous vertical position of the image

void cli()
{
    char command = uart_getc();
    if (command == UP)
    {
        previousPosY = currentPosY;
        currentPosY -= STEP; // scroll up by 10 pixels
        deleteImage(currentPosX, previousPosY, SREEN_SIZE, SREEN_SIZE);
        displayImage(currentPosX, currentPosY, image0, IMAGE_WIDTH, IMAGE_HEIGHT);
    }
    else if (command == DOWN)
    {
        previousPosY = currentPosY;
        currentPosY += STEP; // scroll down by 10 pixels
        deleteImage(previousPosX, previousPosY, SREEN_SIZE, SREEN_SIZE);
        displayImage(currentPosX, currentPosY, image0, IMAGE_WIDTH, IMAGE_HEIGHT);
    }
    else if (command == LEFT)
    {
        previousPosX = currentPosX;
        currentPosX -= STEP; // scroll down by 10 pixels
        deleteImage(previousPosX, previousPosY, SREEN_SIZE, SREEN_SIZE);
        displayImage(currentPosX, currentPosY, image0, IMAGE_WIDTH, IMAGE_HEIGHT);
    }
    else if (command == RIGHT)
    {
        previousPosX = currentPosX;
        currentPosX += STEP; // scroll down by 10 pixels
        deleteImage(previousPosX, previousPosY, SREEN_SIZE, SREEN_SIZE);
        displayImage(currentPosX, currentPosY, image0, IMAGE_WIDTH, IMAGE_HEIGHT);
    }
}

void main()
{
    // set up serial console
    uart_init();

    // Initialize frame buffer
    fb_init();
    // font
    drawString(350, 100, "Group 2 : ", 0xFFFFFFFF, 4);        // White
    drawString(200, 200, "Phan Trong Nguyen", 0xFFFF99FF, 3); // Pink
    drawString(200, 300, "Hur Hyeonbin", 0x0000FFFF, 3);      // Cyan
    drawString(200, 400, "Huynh Ngoc Duy", 0x0000FF00, 3);    // Green
    drawString(200, 500, "Tran Thanh Tu", 0xFFFF0000, 3);     // Red
    // image
    displayImage(currentPosX, currentPosY, image0, IMAGE_WIDTH, IMAGE_HEIGHT); // starting position (x,y), image, width, height

    // echo everything back
    while (1)
    {
        cli();
    }
}