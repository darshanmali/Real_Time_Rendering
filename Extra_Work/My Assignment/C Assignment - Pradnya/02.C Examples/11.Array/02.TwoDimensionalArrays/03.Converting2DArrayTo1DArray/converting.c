#include<stdio.h>

#define NUM_ROWS 5
#define NUM_COLS 3

int main(void)
{
    int iArray_2D_D[NUM_ROWS][NUM_COLS];

    int iArray_1D_D[NUM_ROWS * NUM_COLS];

    int i_D, j_D;
    int num_D;


    printf("Enter Elements for 2D array : \n\n");
    for (i_D = 0; i_D < NUM_ROWS; i_D++)
    {
        printf("Enter ROW NUMBER %d :\n",(i_D + 1));
        for (j_D = 0; j_D < NUM_COLS; j_D++)
        {
            printf("Enter Elements %d : \n",(j_D + 1));
            scanf("%d",&num_D);
            iArray_2D_D[i_D][j_D] = num_D;
        }
        printf("\n\n");
    }
    
    printf("TWO= DIMENTIONAL (2D) Elements : \n\n");
    for (i_D = 0; i_D < NUM_ROWS; i_D++)
    {
        printf("*********** ROW  %d *********** \n",(i_D + 1));
        for (j_D = 0; j_D < NUM_COLS; j_D++)
        {
            printf("iArray_2D [%d][%d] : %d\n",i_D,j_D,iArray_2D_D[i_D][j_D]);
        }
        printf("\n\n");
    }

    for (i_D = 0; i_D < NUM_ROWS; i_D++)
    {
        for (j_D = 0; j_D < NUM_COLS; j_D++)
        {
            iArray_1D_D[(i_D * NUM_COLS) + j_D] = iArray_2D_D[i_D][j_D];
        }

    }
    
    printf("\n\n");
    printf("1D Array : ");
    for (i_D = 0; i_D < (NUM_ROWS * NUM_COLS); i_D++)
    {
        printf("iArray_1D [%d] : %d\n",i_D, iArray_1D_D[i_D]);
    }
    printf("\n\n");
    return(0);
}