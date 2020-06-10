#include<stdio.h>

int main(void)
{
    int i_D, j_D, k_D;

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
            printf("-----------\n\n");

            k_D = 1;
            do
            {
                printf("\t \t k = %d \n", k_D);
                k_D++;
            } while (k_D <= 3);
            printf("\n\n");            
            j_D++;
        } while (j_D <= 5);
        printf("\n\n");
        i_D++;

    }while (i_D <= 10);

    return(0);
}

