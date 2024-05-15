#include "command.h"
#include "mbox.h"
#include "uart1.h"
#include "fb.h"
#include "string.h"
#include "font.h"
// #include "font1.h"

#include "delay.h"
#include "image.h"

#define OS_NAME "Group2-OS"
#define SCR_HEIGH 500
#define SCR_WIDTH 700

#define CHAR_SIZE 224
#define CHAR_WIDTH 14
#define CHAR_HEIGTH 16
#define IMG0_HEIGH 300
#define IMG0_WIDTH 500
#define IMG1_HEIGH 796
#define IMG1_WIDTH 1024
#define VID_HEIGH 340
#define VID_WIDTH 600
#define ROW_STEP 40


// --------------------------- Scan/print(cursor) user input--------------------------------------------
// read the input stream => classify character => store in the temporary string and display on screen
// receive enter => end => break the temporary string into arguments stored in argument list
int input(int total_arg, char args_list[][50])
{

	char c = 0;				 // received character
	int receivedString_index = 0;	 // index of received string
	char received_str[1000]; // received string (from command)
	int arg_det = 0;		 // argument detector, raise if an argument is detected
	int num_arg = 0;			 // current number of argument
	int argumentString = 0;		 // index of argument string

	int current_char = 0; // number of received char, represent the position of the cursor in command line
	uart_puts("\n");
    uart_puts(OS_NAME);
    uart_puts(">");
	// 1. Scan the input from keyboard
	while (1)
	{
		c = uart_getc();

		// if backspace is pressed then the cursor go back 1 character
		if (c == 8)
		{
			// if user have not send any character yet => ignore backspace
			if (current_char == 0)
			{
				continue;
			}

			// mover cursor back 1 position
			uart_sendc(c);
			// replace the current character that the cursor points at by a " " (delete that character) then move cursor back to the " " position (wait for the user input new char)
			uart_puts("\x20\b");
			// move the index of the received string back 1 position
			receivedString_index--;
			// update the position of the cursor(move to the deleted position)
			current_char--;
			// delete last character in received string
			received_str[receivedString_index] = '\0';
			// get the next character
			continue;
		}

		// if not backspace then send the current char to the screen
		uart_sendc(c);

		// Check the newline char
		if (c == 10)
		{
			break;
		}

		// Check the newline char

		// if not backspace or newline char then store in the received string
		received_str[receivedString_index] = c;

		// update the position of the cursor(move to the next position)
		current_char++;
		// move to the next position in the received string
		receivedString_index++;
	}

	// End the received string
	received_str[receivedString_index] = '\0';

	// 2. Split the received string into argument and store in the argument list
	// Reset the received string index
	receivedString_index = 0;

	// reset argument list
	for (int i = 0; i < total_arg; i++)
	{
		for (int j = 0; j < 50; j++)
		{
			args_list[i][j] = '\0';
		}
	}

	// Ignore all space at the beginning
	while (received_str[receivedString_index] == ' ')
	{
		receivedString_index++;
	}

	// Loop through the received string
	while (received_str[receivedString_index] != '\0')
	{
		// if not " " then add to the argument list
		if (received_str[receivedString_index] != ' ')
		{
			// Add character to the current(num_arg) argument string
			args_list[num_arg][argumentString] = received_str[receivedString_index];
			arg_det = 0;			// reset argument detector
			argumentString++;			// move to the next position in the current argument string
			total_arg = num_arg + 1; // In scenario: arg1_arg2 => 2 arguments but num_arg = 1
		}

		// if " " is detected and argument detector haven't raised yet
		// This detector ensure the num_arg only increase by 1 for each argument even there are multiple " " between 2 arguments, Ex: arg1___arg2
		if (received_str[receivedString_index] == ' ' && !arg_det)
		{
			arg_det = 1;		// raise argument detector
			num_arg++;			// move to the next argument in the argument list
			argumentString = 0;		// reset the argument string index
			total_arg = num_arg; // In scenario: arg1_arg2_ => 2 arguments and num_arg = 2
		}

		// move to the next position in the received string
		receivedString_index++;
	}
	return total_arg;
}

// Find the command base on argument list
int compare_arg(char args_list[][50])
{
	static char command_list[][50] = {"font", "image", "image_large", "video", "game", "clear"};
	// Loop the string and compare each character
	for (int i = 0; i < 6; i++)
	{
		if (str_cmp(command_list[i], args_list[0]))
		{
			return i + 1;
		}
	}
	return 0;
}

// Based on the total argument sent by user and argument list => verify => exercute the task or print error message
void execute(int no_input_arg, char args_list[][50])
{
	clear_screen();
	// Extract the command store in argument list - index 0
	int corresponding_command = compare_arg(args_list);

	// Exercute the task correspoding to the command
	switch (corresponding_command)
	{
	case 1: // font
		if (no_input_arg == 1)
		{
			font();
		}
		else
		{
			uart_puts("Invalid font command. Please enter again!\n");
			break;
		}
		break;

	case 2: // image
		if (no_input_arg == 1)
		{
			draw_image();
		}
		else
		{
			uart_puts("Invalid image command. Please enter again!\n");
			break;
		}
		break;
    // case 3: // large image
	// 	if (no_input_arg == 1)
	// 	{
	// 		draw_image_large();
	// 	}
	// 	else
	// 	{
	// 		uart_puts("Invalid image command. Please enter again!\n");
	// 		break;
	// 	}
	// 	break;

	// case 4: // video
	// 	if (no_input_arg == 1)
	// 	{
	// 		video();
	// 	}
	// 	else
	// 	{
	// 		uart_puts("Invalid video command. Please enter again!\n");
	// 		break;
	// 	}
	// 	break;

	// case 5: // game
	// 	if (no_input_arg == 1)
	// 	{
	// 		uart_puts("\n\n-------------------------\n\n");
	// 		uart_puts("Press Enter to start the game. Enjoy!\n\n");
	// 		uart_puts("-------------------------\n\n");
	// 		menu_start();
	// 	}
	// 	else
	// 	{
	// 		uart_puts("Invalid video command. Please enter again!\n");
	// 		break;
	// 	}
	// 	break;

	case 6: // cls - clear monitor 
		clear_screen();
		break;

	default:
		uart_puts("Invalid command. Please enter again!\n");
		break;
	}
}

//--------------------------------------------------------- Command -----------------------------------------------------------
// commands
void commands()
{
	uart_puts("\n1. Show font size\n");
	uart_puts("   Please enter 'font'\n");
	uart_puts("2. Show a small image\n");
	uart_puts("   Please enter 'image'\n");
	uart_puts("3. Show a larger image\n");
	uart_puts("   Please enter 'image_large'\n");
	uart_puts("4. Show a video\n");
	uart_puts("   Please enter 'video'\n");
	uart_puts("5. Play a game\n");
	uart_puts("   Please enter 'game'\n");
	uart_puts("6. Clear screen\n");
	uart_puts("   Please enter 'clear'\n");
}

/////////////// font ///////////////////
// print character with 16x14 font (Apple II)
void printC_16x14(int x, int y, char color[], char input_char)
{
	// color list
	static char colors_list[][50] = {"pink", "red", "green", "yellow", "blue", "purple", "cyan", "white"};
	// text color list
	static unsigned int text_color_list[] = {0x00FF69B4,
											 0x00AA0000,
											 0x0000BB00,
											 0x00FFFF00,
											 0x000000CC,
											 0x00800080,
											 0x0000FFFF,
											 0x00FFFFFF};

	unsigned char bitmap[CHAR_SIZE];
	int row = 0, collum = 0;
	// the text color index
	int text_color = 0;
	for (int i = 0; i < 8; i++)
	{
		// find the corresponding color
		if (str_cmp(colors_list[i], color))
		{
			text_color = i;
			break;
		}
	}

	// Load bitmap of the input character into buffer
	if (font_get_glyph(input_char, bitmap, CHAR_SIZE))
	{
		// Loop through bitmap
		for (int i = 0; i < CHAR_SIZE; i++)
		{
			// if detect 0xFF (on) => fill the corresponding pixel with the input color
			// Because the bitmap is 1D array
			// => bitmap[16x14 = 224] = {0xFF,0x00,... end} and 1 pixel =0xFF/0x00 => read 14 pixel (CHAR_WIDTH)
			// => move to next row
			// Clear the pixel before print
			drawPixelARGB32(x + collum, y + row, 0x00000000);
			if (bitmap[i] == 0xFF)
			{
				drawPixelARGB32(x + collum, y + row, text_color_list[text_color]);
			}
			collum++; // move to next pixel
			// Reach 14 pixel => finish a row => move next row
			if (collum == CHAR_WIDTH)
			{
				collum = 0; // reset collum
				row++;		// move to next row
			}
		}
	}
}

// print string with 16x14 font (Apple II)
void printS_16x14(int x, int y, char color[], char *input_string)
{
	// location of the next character
	int next_char_x_position = 0, next_char_y_position = 0;

	while (*input_string != '\0')
	{
		// carriage return => move to starting point
		if (*input_string == '\r')
		{
			next_char_x_position = 0;
		}
		// newline => increase y by 20 pixels => does not overlap
		else if (*input_string == '\n')
		{
			next_char_x_position = 0;
			next_char_y_position += 20;
		}
		// tab = 4 spaces = charater witdth(14)*4
		else if (*input_string == '\t')
		{
			next_char_x_position += CHAR_WIDTH * 4;
		}
		// print normal character
		else
		{
			printC_16x14(x + next_char_x_position, y + next_char_y_position, color, *input_string);
			next_char_x_position += CHAR_WIDTH;
		}
		input_string++;
	}
}


// Clear screen 
void clear_screen()
{
	fb_init(1920, 1080);
	for (int i = 0; i < 1920; i++)
	{
		for (int j = 0; j < 1080; j++)
		{
			drawPixel(i, j, 0x00000000);
		}
	}
}

//--------------------------------------font---------------------------------//
void font()
{

	fb_init(SCR_WIDTH, SCR_HEIGH);
	printS_16x14(20, 100, "cyan", "Phan Trong Nguyen - s3927189");
	printS_16x14(20, 120, "yellow", "Hur Hyeonbin  - s3740878");
	printS_16x14(20, 140, "green", "Huynh Ngoc Duy - s3924704");
	printS_16x14(20, 160, "red", "Tran Thanh Tu   - s3957386");

	// Test all character from 33 to 126
	int x = 20, y = 300;
	int count = 0;
	for (int i = 33; i < 127; i++)
	{
		if (count == 30 || count == 60 || count == 90)
		{
			x = 20;
			y += 20;
		}
		printC_16x14(x, y, "pink", i);
		x += 14;
		count++;
	}

    // drawString(350, 200, "Phan Trong Nguyen", 0xFFFF99FF, 3);     // Pink color, zoom factor of 3
    // drawString(350, 300, "Hur Hyeonbin", 0x0000FFFF, 3); // Blue color, zoom factor of 3
    // drawString(350, 400, "Huynh Ngoc Duy", 0x0000FF00, 3);   // Green color, zoom factor of 3
    // drawString(350, 500, "Tran Thanh Tu", 0x00FFFF00, 3);    // Yellow color, zoom factor of 3
}

//--------------------------------------image---------------------------------//
void draw_image()
{

	fb_init(SCR_WIDTH, SCR_HEIGH);
	int offset_w = (SCR_WIDTH - IMG0_WIDTH) / 2;
	int offset_h = (SCR_HEIGH - IMG0_HEIGH) / 2;
	for (int y = offset_h; y < IMG0_HEIGH + offset_h; y++)
	{
		for (int x = offset_w; x < IMG0_WIDTH + offset_w; x++)
		{
			drawPixelARGB32(x, y, image0[(y - offset_h) * IMG0_WIDTH + (x - offset_w)]);
		}
	}
}