#include<stdio.h>

int main(int argc, char *argv[], char *envp[])
{
    int iArray_D[5][3] = {{ 1, 2, 3}, { 2, 4, 6}, { 3, 6, 9}, { 4, 8, 12}, { 5, 10, 15}};
    int int_size_D;
    int iArray_size_D;
    int iArray_num_elemets_D, iArray_num_rows_D, iArray_num_columns_D;

    printf("\n");
    
    int_size_D = sizeof(int);

    iArray_size_D = sizeof(iArray_D);
    printf("Size of Dimensional (2D) Integer Array Is = %d\n\n", iArray_size_D);
    
    iArray_num_rows_D = iArray_size_D  / sizeof(iArray_D[0]);
    printf("Number of row in Tow Dimentional (2D) Integer Array Is = %d\n\n", iArray_num_rows_D);

    iArray_num_columns_D = sizeof(iArray_D[0]) / int_size_D;
    printf("Number of Columns In Two dimension (2D) Integer Array Is = %d \n\n", iArray_num_columns_D);

    iArray_num_elemets_D = iArray_num_rows_D * iArray_num_columns_D;
    printf("Number of Columns In Two dimension (2D) Integer Array Is = %d \n\n", iArray_num_elemets_D);

    printf("\n\n");
    printf("Elements In the 2D Array : \n\n");

    
    for (int i_D = 0; i_D < iArray_num_rows_D; i_D++)
    {
        for (int j_D = 0; j_D < iArray_num_columns_D; j_D++)
        {
            printf("iArray_D[%d][%d] = %d\n",i_D, j_D, iArray_D[i_D][j_D]);        
        }
        
    }
    
return(0);


}


