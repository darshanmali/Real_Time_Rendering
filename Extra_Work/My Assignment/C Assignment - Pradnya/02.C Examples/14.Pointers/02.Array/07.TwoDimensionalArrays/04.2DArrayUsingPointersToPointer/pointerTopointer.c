#include<stdio.h>
#include<stdlib.h>

int main(void)
{
  
    int **iArray_D = NULL;
    int i_D, j_D;
    int num_rows_D, num_columns_D;

    printf("\n\n");
    printf("Enter The Number Of Rows : ");
    scanf("%d", &num_rows_D);
    
    printf("Enter The Number Of Column : ");
    scanf("%d", &num_columns_D);


    
    printf("\n\n");
    printf("******************** MEMORY ALLCATION TO 2D INTEGERS ARRAY *******************\n\n");
    iArray_D = (int **)malloc(num_rows_D  * sizeof(int *));
    if (iArray_D  == NULL)
        {
            printf("Failed to Allocate Memory to Row %d of 2D Integer Array !!! Exiting Now...\n\n", i_D);
            exit(0);
        }
        else
        {   
            printf("memory Allocation to %d Rows has Succeeded !!!\n\n ",num_rows_D); 
        }


    for ( i_D = 0; i_D < num_rows_D; i_D++)
    {
        iArray_D[i_D] = (int *)malloc(num_columns_D  * sizeof(int ));
        if (iArray_D[i_D]  == NULL)
        {
            printf("Failed to Allocate Memory to Row %d of 2D Integer Array !!! Exiting Now...\n\n", i_D);
            exit(0);
        }
        else
        {   
            printf("memory Allocation of %d Rows has Succeeded !!!\n\n ",i_D); 
        }
        
        
    }    
        
    for ( i_D = 0; i_D < num_rows_D; i_D++)
    {
       
        for (j_D = 0; j_D < num_columns_D; j_D++)
        {
            iArray_D[i_D][j_D] = (i_D * 1) + (j_D * 1);
        }
        
    }

     for (i_D = 0; i_D < num_rows_D; i_D++)
    {
        printf(" base Address of Row %d : Array_D[%d] = %p \t At Address : %p\n", i_D, i_D, iArray_D[i_D], &iArray_D[i_D]);
    }

    printf("\n\n");
    for ( i_D = 0; i_D < num_rows_D; i_D++)
    {
        for (j_D = 0; j_D < num_columns_D; j_D++)
        {
            printf(" iArray[%d][%d] = %d  \t At Address :%p\n", i_D, j_D, iArray_D[i_D][j_D], &iArray_D[i_D][j_D]);
        }
        printf("\n\n");
    }

    for (i_D = (num_rows_D - 1); i_D >= 0; i_D--)
    {
        if (iArray_D[i_D])
        {
            free(iArray_D[i_D]);
            iArray_D[i_D] = NULL;
            printf("Memory Allocation To Row %d Has Been SuccessFully Freed !!!!\n\n", i_D);
        
        }
        
    }
  
  if (iArray_D)
    {
        free(iArray_D);
        iArray_D = NULL;
        printf("Memory Allocation To Row %d Has Been SuccessFully Freed !!!!\n\n", i_D);
    
    }
    return(0);
    
}


