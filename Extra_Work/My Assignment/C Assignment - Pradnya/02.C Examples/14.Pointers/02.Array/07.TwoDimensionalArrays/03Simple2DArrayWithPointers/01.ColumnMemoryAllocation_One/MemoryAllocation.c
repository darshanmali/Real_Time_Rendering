#include<stdio.h>
#include<stdlib.h>

#define NUM_ROWS 5
#define NUM_COLOMN 3


int main(void)
{
  
    int *iArray[NUM_ROWS];
    int i_D, j_D;

    printf("\n\n");
    
    for ( i_D = 0; i_D < NUM_ROWS; i_D++)
    {

        iArray[i_D] = (int *)malloc(NUM_ROWS * sizeof(int ));
        if (iArray  == NULL)
        {
            printf("Failed to Allocate Memory to Row %d of 2D Integer Array !!! Exiting Now...\n\n", i_D);
            exit(0);
        }
        else
        {   
            printf("memory Allocation of %d Rows has Succeeded !!!\n\n ",i_D); 
        }
    }    
        
    for ( i_D = 0; i_D < NUM_ROWS; i_D++)
    {
        for (j_D = 0; j_D < NUM_COLOMN; j_D++)
        {
            iArray[i_D][j_D] = (i_D + 1) * (j_D + 1);
        }
        
    }
    printf("\n\n");
    printf("2D Integer Array  : \n\n");
    
    for ( i_D = 0; i_D < NUM_ROWS; i_D++)
    {
        for (j_D = 0; j_D < NUM_COLOMN; j_D++)
        {
            printf(" iArray[%d][%d] = %d\n", i_D, j_D, iArray[i_D][j_D]);
        }
        printf("\n\n");
    }
    printf("\n\n");
    for (i_D = (NUM_ROWS - 1); i_D >= 0; i_D--)
    {
       
        free(iArray[i_D]);
        printf("Memory Allocation To Row %d Has Been SuccessFully Freed !!!!\n\n", i_D);
        
    }
  
    return(0);
    
}


