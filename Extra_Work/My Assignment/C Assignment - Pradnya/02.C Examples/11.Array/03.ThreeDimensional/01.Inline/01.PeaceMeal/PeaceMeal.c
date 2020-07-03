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

    
    printf("*********** ROW  1 *********** \n"); 
    printf("*********** COL  1 *********** \n");
    printf("iArray [0][0][0] : %d\n",iArray_D[0][0][0]);
    printf("iArray [0][0][1] : %d\n",iArray_D[0][0][1]);
    printf("\n");
    
    printf("*********** COL  2 *********** \n");
    printf("iArray [0][1][0] : %d\n",iArray_D[0][1][0]);
    printf("iArray [0][1][1] : %d\n",iArray_D[0][1][1]);
    printf("\n");
    
    printf("*********** COL  3 *********** \n");
    printf("iArray [0][2][0] : %d\n",iArray_D[0][2][0]);
    printf("iArray [0][2][1] : %d\n",iArray_D[0][2][1]);
    printf("\n");
    
    printf("*********** ROW  2 *********** \n"); 
    printf("*********** COL  1 *********** \n");
    printf("iArray [1][0][0] : %d\n",iArray_D[1][0][0]);
    printf("iArray [1][0][1] : %d\n",iArray_D[1][0][1]);
    printf("\n");
    
    printf("*********** COL  2 *********** \n");
    printf("iArray [0][1][0] : %d\n",iArray_D[0][1][0]);
    printf("iArray [0][1][1] : %d\n",iArray_D[0][1][1]);
    printf("\n");

      printf("*********** COL  3 *********** \n");
    printf("iArray [0][2][0] : %d\n",iArray_D[0][2][0]);
    printf("iArray [0][2][1] : %d\n",iArray_D[0][2][1]);
    printf("\n");
    
    
    printf("*********** ROW  3 *********** \n"); 
    printf("*********** COL  1 *********** \n");
    printf("iArray [2][0][0] : %d\n",iArray_D[2][0][0]);
    printf("iArray [2][0][1] : %d\n",iArray_D[2][0][1]);
    printf("\n");
    
    printf("*********** COL  2 *********** \n");
    printf("iArray [2][1][0] : %d\n",iArray_D[2][1][0]);
    printf("iArray [2][1][1] : %d\n",iArray_D[2][1][1]);
    printf("\n");

      printf("*********** COL  3 *********** \n");
    printf("iArray [2][2][0] : %d\n",iArray_D[2][2][0]);
    printf("iArray [2][2][1] : %d\n",iArray_D[2][2][1]);
    printf("\n");
    
    
    printf("*********** ROW  4 *********** \n"); 
    printf("*********** COL  1 *********** \n");
    printf("iArray [3][0][0] : %d\n",iArray_D[3][0][0]);
    printf("iArray [3][0][1] : %d\n",iArray_D[3][0][1]);
    printf("\n");
    
    printf("*********** COL  2 *********** \n");
    printf("iArray [3][1][0] : %d\n",iArray_D[3][1][0]);
    printf("iArray [3][1][1] : %d\n",iArray_D[3][1][1]);
    printf("\n");

      printf("*********** COL  3 *********** \n");
    printf("iArray [3][2][0] : %d\n",iArray_D[3][2][0]);
    printf("iArray [3][2][1] : %d\n",iArray_D[3][2][1]);
    printf("\n");
    
    printf("*********** ROW  5 *********** \n"); 
    printf("*********** COL  1 *********** \n");
    printf("iArray [4][0][0] : %d\n",iArray_D[4][0][0]);
    printf("iArray [4][0][1] : %d\n",iArray_D[4][0][1]);
    printf("\n");
    
    printf("*********** COL  2 *********** \n");
    printf("iArray [4][1][0] : %d\n",iArray_D[4][1][0]);
    printf("iArray [4][1][1] : %d\n",iArray_D[4][1][1]);
    printf("\n");

      printf("*********** COL  3 *********** \n");
    printf("iArray [4][2][0] : %d\n",iArray_D[4][2][0]);
    printf("iArray [4][2][1] : %d\n",iArray_D[4][2][1]);
    printf("\n");
    
return(0);


}


