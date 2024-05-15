#include "uart1.h" 
#include "mbox.h" 
#include "string.h"
// #include "font1.h"

void main() { 
    // set up serial console 
    uart_init(); 
    // // Print commands
    commands();
    // // argument list(assumpt that the maximum argument is 10) => store argument sent by user
    char args_list[50][50];
    // // number of argument sent by user
    int number_input_arg;

    while(1) {
        // drawString(350, 200, "Phan Trong Nguyen", 0xFFFF99FF, 3);     // Pink color, zoom factor of 3
        // drawString(350, 300, "Hur Hyeonbin", 0x0000FFFF, 3); // Blue color, zoom factor of 3
        // drawString(350, 400, "Huynh Ngoc Duy", 0x0000FF00, 3);   // Green color, zoom factor of 3
        // drawString(350, 500, "Tran Thanh Tu", 0x00FFFF00, 3);    // Yellow color, zoom factor of 3
        // // read the input stream => classify character => store in the temporary string and display on screen 
        // // receive enter => end => break the temporary string into arguments stored in argument list
        number_input_arg = input(50, args_list);
        // // Based on the total argument sent by user and argument list => verify => exercute the task or print error message
        execute(number_input_arg, args_list);
    }
} 