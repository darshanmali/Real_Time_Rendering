#include<stdio.h>
#include<stdlib.h>

#define NUM_ROWS 5
#define NUM_COLOMN 3


int main(void)
{
  
    int i_D, j_D;
    int **ptr_iArray = NULL;

    printf("\n\n");
    ptr_iArray = (int **)malloc(NUM_ROWS * sizeof(int *));

    if (ptr_iArray  == NULL)
    {
        printf("Memory Allocation To The 1D Array of base Address of %d Rows failed !!! Exiting Now...\n\n", NUM_ROWS);
        exit(0);
    }
    else
    {
        printf("memory Allocation To the 1D Array base Address of %d Rows has Succeeded !!!\n\n ",NUM_ROWS); 
    }
 
    for ( i_D = 0; i_D < NUM_ROWS; i_D++)
    {
        ptr_iArray[i_D] = (int *)malloc(NUM_COLOMN * sizeof(int));

        if (ptr_iArray == NULL)
        {
            printf("Memory Allocation To The  Column of Row %d failed !!! Exiting Now...\n\n", i_D);
            exit(0);
        }
        else
        {
            printf("Memory Allocation To The  Column of Row %d has Succeeded !!!\n\n", i_D);
        }
    }    
        
    for ( i_D = 0; i_D < NUM_ROWS; i_D++)
    {
        for (j_D = 0; j_D < NUM_COLOMN; j_D++)
        {
            *(*(ptr_iArray +i_D) + j_D) = (i_D + 1) * (j_D + 1);
        }
        
    }
    printf("\n\n");
    printf("2D Integer Array Element Along With Address : \n\n");
    
    for ( i_D = 0; i_D < NUM_ROWS; i_D++)
    {
        for (j_D = 0; j_D < NUM_COLOMN; j_D++)
        {
            printf(" ptr_iArray[%d][%d] = %d \t\t At Address &ptr_Array[%d][%d]: %p\n", i_D, j_D, ptr_iArray[i_D][j_D], i_D, j_D, &ptr_iArray [i_D][j_D]);
        }
        printf("\n\n");
    }

    for (i_D = (NUM_ROWS - 1); i_D >= 0; i_D--)
    {
        if (*(ptr_iArray + i_D))
        {
            free(*(ptr_iArray + i_D));
            *(ptr_iArray + i_D) = NULL;
            printf("Memory Allocation To Row %d Has Been SuccessFully Freed !!!!\n\n", i_D);
        }
        
    }
    
    if (ptr_iArray)
    {
        free(ptr_iArray);
        ptr_iArray = NULL;
        printf("Memory Allocation To ptr_iArray Has Been SuccessFully Freed !!!\n\n");
    }
    
    return(0);
    
}


