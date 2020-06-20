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

    printf("************** Row 1 **************\n");
    printf("iArray[0][0] = %d\n", iArray_D[0][0]);
    printf("iArray[0][1] = %d\n", iArray_D[0][1]);
    printf("iArray[0][2] = %d\n", iArray_D[0][2]);

    printf("\n\n");
    
    printf("************** Row 2 **************\n");
    printf("iArray[1][0] = %d\n", iArray_D[1][0]);
    printf("iArray[1][1] = %d\n", iArray_D[1][1]);
    printf("iArray[1][2] = %d\n", iArray_D[1][2]);

    printf("\n\n");

    printf("************** Row 3 **************\n");
    printf("iArray[2][0] = %d\n", iArray_D[2][0]);
    printf("iArray[2][1] = %d\n", iArray_D[2][1]);
    printf("iArray[2][2] = %d\n", iArray_D[2][2]);

    printf("\n\n");

    printf("************** Row 4 **************\n");
    printf("iArray[3][0] = %d\n", iArray_D[3][0]);
    printf("iArray[3][1] = %d\n", iArray_D[3][1]);
    printf("iArray[3][2] = %d\n", iArray_D[3][2]);

    printf("\n\n");

    printf("************** Row 5 **************\n");
    printf("iArray[4][0] = %d\n", iArray_D[4][0]);
    printf("iArray[4][1] = %d\n", iArray_D[4][1]);
    printf("iArray[4][2] = %d\n", iArray_D[4][2]);
}
