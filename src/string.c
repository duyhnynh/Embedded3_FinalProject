#include "string.h"

// Compare string
int str_cmp(char* str1, char* str2) {
	// Check length
	if (str_len(str1) != str_len(str2))
		return 0;
    // pointer for str1 and str1
    char* ptr1 = str1; 
	char* ptr2 = str2; 

    // loop through the given string
	while(*ptr1 != '\0' && *ptr2 != '\0') {
    // Compare each character, if not match return 0 => different
		if (*ptr1 != *ptr2)
			return 0;

        // move to next character
		ptr1++;
		ptr2++;
	}
    // return 1 => Compare match
	return 1;
}

// Measure string length
int str_len(char* str) {
	int i = 0;
    // loop through the given string
	while (str[i] != '\0') {
		i++;
	}
	return i;
}

// convert string to integer
int str2int(char* str)
{
    // number equivalent to the given string
    int num = 0;
    const int BASE = 10;
    // loop through the given string
    for (int i = 0; str[i] != '\0'; i++)
    // "123" => Loop 1: num = 0*10 + 49 - 48 = 1;
    // Loop 2: num = 1*10 + 50 - 48 = 12;
    // Loop 3: num = 12*10 + 51 - 48 = 123
        num = num * BASE  + str[i] - 48;

    return num;
}

// check if string is only digit?
int is_digit(char* str)
{   
    int i = 0;
    // loop through the given string
	while (str[i] != '\0') {
        if(str[i] < 48 || str[i] > 57)
        {
            return 0;
        }
		i++;
	}
    return 1;
}

// Clear string/buffer
void clr_buffer(char* str){
    while(*str != '\0'){
            *str = '\0';
            str++;
    }
}


// find substring in a main string
int sub_string (char* main_str, char *sub_str){
	   int i = 0;
	   while (*sub_str != '\0') {

        if(*sub_str != *main_str)
            return 0;
	   }
       	sub_str++;
	    main_str++;
        return 1;
}