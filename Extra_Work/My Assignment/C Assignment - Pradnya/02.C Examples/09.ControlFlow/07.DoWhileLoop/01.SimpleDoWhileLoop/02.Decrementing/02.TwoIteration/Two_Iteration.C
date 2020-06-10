#include<stdio.h>

int main(void)
{
    int i_D, j_D;

    printf("\n\n");

    printf("Printing Digits 10 to 1 and 100 to 10 : \n\n");

    i_D = 10;
    j_D = 100;

    do 
    {
        printf("\t %d \t %d \n",i_D, j_D);
        i_D--;
        j_D -= 10;
    }while(i_D >= 1, j_D >= 10);
    printf("\n\n");

    return(0);
}
