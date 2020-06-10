#include<stdio.h>

int main(void)
{
    int i_D, j_D;

    printf("\n\n");

    i_D = 1;
    do 
    {
        printf("i = %d\n",i_D);
        printf("-----------\n\n");
        j_D =1;
        do
        {
            printf("\t j = %d \n", j_D);
            j_D++;
        } while (j_D <= 5);
        i_D++;
        printf("\n\n");

    }while (i_D <= 10);

    return(0);
}

