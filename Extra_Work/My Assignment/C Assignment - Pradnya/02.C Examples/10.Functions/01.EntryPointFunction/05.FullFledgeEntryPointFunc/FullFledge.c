#include<stdio.h>

int main(int argc, char *argv[], char *envp[])
{
    int i_D;
    printf("\n\n");

    printf("Hello World!!!!\n\n");

    printf("Number of arguments Passed to this Program Are :\n\n");

    for(i_D = 0; i_D < argc; i_D++)
    {
        printf("Commande Line Argument Number %d = %s\n", (i_D + 1),argv[i_D]);
    }
    printf("\n\n");

    printf("First 20 Environment Variables passed to This Program Are : \n\n");
    for (i_D = 0; i_D < 20; i_D++)
    {
        printf("Environment Variable Nubmer %d = %s \n", (i_D + 1), envp[i_D]);
    }
    printf("\n\n");
    return(0);
}


