#include<stdio.h>

#define NUM_ELEMTES 10

int main(void)
{
    int iArray[NUM_ELEMTES];
    int i_D, num_D;

    printf("\n\n");
    printf("Enter Integers Elements For Array : \n\n");
    for (i_D = 0; i_D < NUM_ELEMTES; i_D++)
    {
        scanf("%d", &num_D);
        iArray[i_D] = num_D;
    }

    printf("\n\n");
    printf("Even numbers Amongst the Array Elements Are : \n\n");
    for (i_D = 0; i_D < NUM_ELEMTES; i_D++)
    {
        if((iArray[i_D] % 2) == 0)
            printf("%d\n", iArray[i_D]);

    }

    printf("\n\n");
    printf("Odd Numbers Amongst the Array Elements Are : \n\n");
    for (i_D = 0; i_D < NUM_ELEMTES; i_D++)
    {
        if((iArray[i_D] % 2) != 0)
            printf("%d\n", iArray[i_D]);
    }
    return (0);
    
}

