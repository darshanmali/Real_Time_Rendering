#include<stdio.h>

#define NUM_ELEMTES 10

int main(void)
{
    int iArray[NUM_ELEMTES];
    int i_D, num_D, sum_D = 0;

    printf("\n\n");
    printf("Enter Integers Elements For Array : \n\n");
    for (i_D = 0; i_D < NUM_ELEMTES; i_D++)
    {
        scanf("%d", &num_D);
        iArray[i_D] = num_D;
    }

    for (i_D = 0; i_D < NUM_ELEMTES; i_D++)
    {
        sum_D = sum_D + iArray[i_D];
    }
    printf("\n\n");
    printf("Sum Of All Elemetes Of Array = %d\n\n",sum_D);

    return (0);
    
}


