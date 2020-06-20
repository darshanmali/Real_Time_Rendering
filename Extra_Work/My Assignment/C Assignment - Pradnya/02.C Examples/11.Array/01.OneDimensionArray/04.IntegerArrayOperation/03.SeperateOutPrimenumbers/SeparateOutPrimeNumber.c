#include<stdio.h>

#define NUM_ELEMTES 10

int main(void)
{
    int iArray[NUM_ELEMTES];
    int i_D, j_D, num_D, count_D = 0;

    printf("\n\n");
    printf("Enter Integers Elements For Array : \n\n");
    for (i_D = 0; i_D < NUM_ELEMTES; i_D++)
    {
        scanf("%d", &num_D);
        if(num_D < 0)
            num_D = -1 * num_D;
        
        iArray[i_D] = num_D;
    }

    printf("\n\n");
    printf("Array Elements Are : \n\n");
    for (i_D = 0; i_D < NUM_ELEMTES; i_D++)
    {
        printf("%d\n", iArray[i_D]);
    }

    printf("\n\n");
    printf("Prime Numbers Amongst the Array Elements Are : \n\n");
    for (i_D = 0; i_D < NUM_ELEMTES; i_D++)
    {
        for (j_D = 1; j_D <= iArray[i_D]; j_D++)
        {
            if((iArray[i_D] % j_D) == 0)
                count_D++;
        }
        
        if (count_D == 2)
            printf("%d\n",iArray[i_D]);
        
        count_D = 0;
    }
    return (0);
    
}

