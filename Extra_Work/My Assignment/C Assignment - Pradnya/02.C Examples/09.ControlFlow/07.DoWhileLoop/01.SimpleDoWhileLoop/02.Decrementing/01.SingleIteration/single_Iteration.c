#include<stdio.h>

int main(void)
{
    int i_D;

    printf("\n\n");

    printf("Printing Digits 10 to 1 : \n\n");

    i_D = 10;

    do 
    {
        printf("\t %d \n",i_D);
        i_D--;
    }while(i_D >= 1);
    printf("\n\n");

    return(0);
}
