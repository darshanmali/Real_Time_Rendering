#include<stdio.h>

int main(void)
{
    int i_D, j_D;

    printf("\n\n");

    for (i_D = 1; i_D <= 10; i_D++)
    {
        printf("i_D = %d\n",i_D);
        printf("-------\n\n");
        for (j_D = 1; j_D <= 5; j_D++)
        {
            printf("\t j_D = %d\n",j_D);
        }
        printf("\n\n");
    }
    return (0);

    
}