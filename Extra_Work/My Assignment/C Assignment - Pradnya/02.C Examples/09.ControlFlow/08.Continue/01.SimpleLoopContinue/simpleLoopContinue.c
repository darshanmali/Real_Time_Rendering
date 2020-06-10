#include<stdio.h>

int main(void)
{
    int i_D;

    printf("\n\n");

    printf("Printing Even number From 0 to 100 : \n\n");

    for(i_D = 1; i_D <= 100; i_D++)
    {
        if(i_D % 2 != 0)
            continue;
        else
            printf("\t%d\n",i_D);
    }
    printf("\n\n");
    return(0);
}
