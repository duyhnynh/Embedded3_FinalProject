#include "string.h"

// Custom strcmp function
int strcmp(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }
    return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}
// Custom strncmp function
int strncmp(const char *s1, const char *s2, int n)
{
    while (n-- && *s1 && *s2)
    {
        if (*s1 != *s2)
        {
            return (*(unsigned char *)s1 - *(unsigned char *)s2);
        }
        s1++;
        s2++;
    }
    return 0;
}

// Measure string length
int str_len(char *str)
{
    int i = 0;
    // loop through the given string
    while (str[i] != '\0')
    {
        i++;
    }
    return i;
}

// Custom strcpy function
void strcpy(char *dest, const char *src)
{
    while (*src)
    {
        *dest++ = *src++;
    }
    *dest = '\0';
}