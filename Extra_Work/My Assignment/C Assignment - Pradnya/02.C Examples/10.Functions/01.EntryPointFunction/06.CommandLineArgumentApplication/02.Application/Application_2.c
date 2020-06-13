#include<stdio.h>
#include<stdlib.h>

int main(int argc, char *argv[],char *envp[])
{
    int i_D;

    if(argc != 4)
    {
        printf("\n\n");
        printf("Invalid Usage !!! Existing Now ....\n\n");
        printf("Usage : CommandLine Application <First Name> <Second Name> <Surname>\n\n");
        exit(0);
    }

    printf("\n\n");
    printf("Your Full Name IS : ");
    for (i_D = 1; i_D < argc; i_D++)
    {
        printf("%s ", argv[i_D]);
    }

    printf("\n\n");
    return(0);

}

