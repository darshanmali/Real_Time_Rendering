#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>

int main(int argc, char *argv[],char *envp[])
{
    int i_D;
    int num_D;
    int sum_D = 0;

    if(argc == 1)
    {
        printf("\n\n");
        printf("No Number Given to Addition !!! Existing Now ....\n\n");
        printf("Usage : CommandLine Application <First Number> <Second Number> .....\n\n");
        exit(0);
    }

    printf("\n\n");
    printf("Sum of all Integer Command Line Argument is : \n\n");
    for (i_D = 1; i_D < argc; i_D++)
    {
        num_D = atoi(argv[i_D]);
        sum_D += num_D;        
    }

    printf("sum = %d \n\n", sum_D);

    return(0);

}

