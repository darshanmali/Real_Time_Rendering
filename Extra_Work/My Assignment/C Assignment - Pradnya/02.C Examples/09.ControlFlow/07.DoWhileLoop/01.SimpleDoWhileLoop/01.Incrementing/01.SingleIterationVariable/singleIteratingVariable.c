#include<stdio.h>

int main(void)
{
    int i_D;

    printf("\n\n");

    printf("Printing Digits 1 to 10 : \n\n");

    i_D = 1;
    do 
    {
        printf("\t %d \n",i_D);
        i_D++;
    }while(i_D <= 10);
    printf("\n\n");

    return(0);
}

