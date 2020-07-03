#include<stdio.h>

int main(void)
{
    int iArray_D[5][3][2] = {{{9,18},{27,36},{45,54}},
    {{8,16},{24,32},{40,48}},
    {{7,14},{21,36},{35,42}},
    {{6,12},{18,36},{30,36}},
    {{5,10},{15,36},{25,30}}};

    int int_size_D;
    int iArray_size_D;
    int iArray_num_elements,iArray_Width_D,iArray_height_D, iArray_Depth_D;
    int i_D, j_D, k_D;

    printf("\n");
    
    int_size_D = sizeof(int);

    iArray_size_D = sizeof(iArray_D);
    printf("Size of Dimensional (3D) Integer Array Is = %d\n\n", iArray_size_D);
    
    iArray_Width_D = iArray_size_D  / sizeof(iArray_D[0]);
    printf("Number of row in Tow Dimentional (3D)Integer Array Is = %d\n\n", iArray_Width_D);

    iArray_height_D = sizeof(iArray_D[0]) / sizeof(iArray_D[0][0]);
   printf("Number of Columns In Two dimension (3D) Integer Array Is = %d \n\n", iArray_height_D);

    iArray_Depth_D = sizeof(iArray_D[0][0]) / int_size_D;
    printf("Number of Columns In Two dimension (3D) Integer Array Is = %d \n\n", iArray_Depth_D);

    iArray_num_elements = iArray_Width_D * iArray_height_D * iArray_Depth_D ;
    printf("Number of Columns In Two dimension (3D) Integer Array Is = %d \n\n", iArray_num_elements);

    printf("\n\n");
    printf("Elements In the 3D Array : \n\n");

    
    printf("\n\n");
    printf("Elements In Integer 3D array : \n\n");
    for (i_D = 0; i_D < iArray_Width_D; i_D++)
    {
        printf("*********** ROW  %d *********** \n",(i_D + 1));
        for (j_D = 0; j_D < iArray_height_D; j_D++)
        {
            printf("*********** COL  %d *********** \n",(j_D + 1));
            for (k_D = 0; k_D < iArray_Depth_D; k_D++)
            {
                printf("iArray [%d][%d][%d] : \n",i_D,j_D,iArray_D[i_D][j_D][k_D]);
            }
        printf("\n");
        }
        printf("\n\n");
    }
    
return(0);


}


