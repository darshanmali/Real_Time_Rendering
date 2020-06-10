#include<stdio.h>

int main(void)
{
    int i_D, j_D;

    printf("\n\n");

    printf("Printing Digits 1 to 10 and 10 to 100 : \n\n");

    i_D = 1;
    j_D = 10;

    do 
    {
        printf("\t %d \t %d \n",i_D, j_D);
        i_D++;
        j_D += 10;
    }while(i_D <= 10, j_D <= 100);
    printf("\n\n");

    return(0);
}

