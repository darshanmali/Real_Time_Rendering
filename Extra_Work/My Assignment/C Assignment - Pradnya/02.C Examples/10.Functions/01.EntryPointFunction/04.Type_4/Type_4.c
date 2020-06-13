#include<stdio.h>

int main(int argc, char *argv[])
{
    int i_D;
    printf("\n\n");
    printf("Hello World !!!\n");
    printf("Number of Comand Line Arguments = %d\n\n", argc);

    printf("Command Line  Argument passed To this Program Are : \n\n");
    for(i_D = 0; i_D < argc; i_D++)
    {
        printf("Command Line Argument Number %d = %s\n", (i_D + 1), argv[i_D]);

    }
    printf("\n\n");

    return (0);
}
