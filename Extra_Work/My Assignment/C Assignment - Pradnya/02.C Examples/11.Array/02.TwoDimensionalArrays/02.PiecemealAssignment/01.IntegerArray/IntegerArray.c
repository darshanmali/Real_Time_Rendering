#include<stdio.h>

int main(int argc, char *argv[], char *envp[])
{
    int iArray_D[3][5];
    int int_size_D;
    int iArray_size_D;
    int iArray_num_elemets_D, iArray_num_rows_D, iArray_num_columns_D;
    int i_D, j_D;
    
    int_size_D = sizeof(int);

    iArray_size_D = sizeof(iArray_D);
    printf("Size of Dimensional (2D) Integer Array Is = %d\n\n", iArray_size_D);
    
    iArray_num_rows_D = iArray_size_D  / sizeof(iArray_D[0]);
    printf("Number of row in Tow Dimentional (2D) Integer Array Is = %d\n\n", iArray_num_rows_D);

    iArray_num_columns_D = sizeof(iArray_D[0]) / int_size_D;
    printf("Number of Columns In Two dimension (2D) Integer Array Is = %d \n\n", iArray_num_columns_D);

    iArray_num_elemets_D = iArray_num_rows_D * iArray_num_columns_D;
    printf("Number of Elements In Two dimension (2D) Integer Array Is = %d \n\n", iArray_num_elemets_D);

    printf("\n\n");
    printf("Elements In the 2D Array : \n\n");

    iArray_D[0][0] = 21;
    iArray_D[0][1] = 34;
    iArray_D[0][2] = 45;
    iArray_D[0][3] = 67;
    iArray_D[0][4] = 67;

    iArray_D[1][0] = 76;
    iArray_D[1][1] = 87;
    iArray_D[1][2] = 56;
    iArray_D[1][3] = 67;
    iArray_D[1][4] = 78;

    iArray_D[2][0] = 98;
    iArray_D[2][1] = 43;
    iArray_D[2][2] = 48;
    iArray_D[2][3] = 78;
    iArray_D[2][4] = 13;

    iArray_D[3][0] = 54;
    iArray_D[3][1] = 56;
    iArray_D[3][2] = 67;
    iArray_D[3][3] = 89;
    iArray_D[3][4] = 70;

    iArray_D[4][0] = 34;
    iArray_D[4][1] = 22;
    iArray_D[4][2] = 11;
    iArray_D[4][3] = 33;
    iArray_D[4][4] = 44;

    for (i_D = 0; i_D < iArray_num_rows_D; i_D++)
    {
        printf("*******ROW %d*******\n",(i_D + 1));
        for (j_D = 0; j_D < iArray_num_columns_D; j_D++)
        { 
            printf("iArray[%d][%d] = %d\n", i_D, j_D, iArray_D[i_D][j_D]);

        }
        printf("\n\n");        
    }
    return (0);
    

}
