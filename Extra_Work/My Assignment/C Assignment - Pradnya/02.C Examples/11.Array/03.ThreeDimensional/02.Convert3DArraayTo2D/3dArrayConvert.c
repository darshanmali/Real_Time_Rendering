#include<stdio.h>

#define NUM_ROWS 5
#define NUM_COLS 3
#define DEPTH 2

int main(void)
{
    int iArray_D[NUM_ROWS][NUM_COLS][DEPTH] = {{{9,18},{27,36},{45,54}},
    {{8,16},{24,32},{40,48}},
    {{7,14},{21,36},{35,42}},
    {{6,12},{18,36},{30,36}},
    {{5,10},{15,36},{25,30}}};

    int i_D, j_D, k_D;

    int iArray_1D_D[NUM_ROWS * NUM_COLS];

    printf("\n\n");
    printf("Enter Elements for 3D array : \n\n");
    for (i_D = 0; i_D < NUM_ROWS; i_D++)
    {
        printf("*********** ROW  %d *********** \n",(i_D + 1));
        for (j_D = 0; j_D < NUM_COLS; j_D++)
        {
            printf("*********** COL  %d *********** \n",(j_D + 1));
            for (k_D = 0; k_D < NUM_COLS; k_D++)
            {
                printf("iArray [%d][%d][%d] : \n",i_D,j_D,iArray_D[i_D][j_D][k_D]);
            }
        printf("\n");
        }
        printf("\n");
    }
    
   for (i_D = 0; i_D < NUM_ROWS; i_D++)
    {
        for (j_D = 0; j_D < NUM_COLS; j_D++)
        {
            for (k_D = 0; k_D < NUM_COLS; k_D++)
            {
                iArray_1D_D[(i_D * NUM_COLS * DEPTH) + (j_D * DEPTH) + k_D] = iArray_D[i_D][j_D][k_D];    
            }
        }
    }

    printf("\n\n");
    printf("1D Array : ");
    for (i_D = 0; i_D < (NUM_ROWS * NUM_COLS * DEPTH); i_D++)
    {
        printf("iArray_1D[%d] : %d\n",i_D, iArray_1D_D[i_D]);
    }
    return(0);
}


