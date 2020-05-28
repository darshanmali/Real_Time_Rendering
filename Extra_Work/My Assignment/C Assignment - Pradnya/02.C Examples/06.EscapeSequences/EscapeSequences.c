#include<stdio.h>
int main(void)
{
    printf("\n\n");
    printf("Going on to next Line ...using \\n Escape sequence\n\n");
    printf("Demonstrating \t Horizontal \t Tab \t Using \t \\t Escape sequence\n\n");
    printf("This is double quoted output \" Done using  \\\" \\\" Escape sequence\n\n");
    printf("This is Single quoted output \' Done using  \\\' \\\' Escape sequence\n\n");
    printf("BACKSPACE Turned To BACKSPACE\b using Escape sequence\\b\n\n");

    printf("\r Demostarting Carriage Return using \\r Escape sequence\n");
    printf("Demostrating \r Carriage Return using \\r Escape sequence\n");
    printf("Demistrating carriage \r return using \\r Escape sequence\n\n");

    printf("Demostrating \x41 using \\xhh Escape sequence \n\n");
    

    printf("Demostrating \102 using \\ooo Escape sequence \n\n");
    return(0);
}