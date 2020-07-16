#include<stdio.h>
#include<stdlib.h>

#define NUM_ROWS 5
#define NUM_COLOMN 3


int main(void)
{
    int iArray[NUM_ROWS][NUM_COLOMN];
    int i_D, j_D;
    int *ptr_iArray_Row = NULL;

    for ( i_D = 0; i_D < NUM_ROWS; i_D++)
    {
        ptr_iArray_Row = iArray[i_D];

        for (j_D = 0; j_D < NUM_COLOMN; j_D++)
        {
            *(ptr_iArray_Row + j_D) = (i_D + 1) * (j_D + 1);
        }
        
    }
    printf("\n\n");
    printf("2D Integer Array Element Along With Address : \n\n");
    
    for ( i_D = 0; i_D < NUM_ROWS; i_D++)
    {
        ptr_iArray_Row = iArray[i_D];
        for (j_D = 0; j_D < NUM_COLOMN; j_D++)
        {
            printf("(ptr_iArray_Row + %d) = %d \t\t At Address : %p\n", j_D, *(ptr_iArray_Row + j_D), (ptr_iArray_Row + j_D));
        }
        printf("\n\n");
    }
    return(0);
    
}


