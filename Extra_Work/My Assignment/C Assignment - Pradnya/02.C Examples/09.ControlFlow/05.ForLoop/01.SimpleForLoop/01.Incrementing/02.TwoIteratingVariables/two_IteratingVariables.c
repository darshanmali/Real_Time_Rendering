#include<stdio.h>
int main(void)
{
    int i_D, j_D;

    printf("\n\n");
    printf("Printing Digits 1 to 10 : \n\n");

    for(i_D = 1, j_D = 10; i_D <= 10, j_D <= 100; i_D++, j_D += 10)
    {
        printf("\t %d \t %d\n", i_D,j_D);
    }

    printf("\n\n");

    return(0);

}