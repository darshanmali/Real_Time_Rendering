#include<stdio.h>

int main(int argc, char *argv[],char *envp[])
{
    int iArray_D[] = {29, 49, 76, 89, 21, 34, 12, 67, 56, 95};
    int int_size_D;
    int iArray_size_D;
    int iArray_num_elements_D;

    float fArray_D[] = {2.6f, 4.2f, 3.9f, 5.2f, 2.1f, 3.4f, 1.2f, 6.7f, 5.6f, 9.5f};
    int float_size_D;
    int fArray_size_D;
    int fArray_num_elements_D;
    
    char cArray_D[] = {'A', 'R', 'S', 'H', 'F', 'C', 'X', 'Q', 'O','H', 'C', 'W','P'};
    int char_size_D;
    int cArray_size_D;
    int cArray_num_elements_D;
    
    printf("In-Line Initialization And Peace-meal Display Of Element Of Array 'iArray[]' : \n");
    printf("iArray[0] (1st Element) = %d\n", iArray_D[0]);
    printf("iArray[1] (2nd Element) = %d\n", iArray_D[1]);
    printf("iArray[2] (3rd Element) = %d\n", iArray_D[2]);
    printf("iArray[3] (4rd Element) = %d\n", iArray_D[3]);
    printf("iArray[4] (5th Element) = %d\n", iArray_D[4]);
    printf("iArray[5] (6th Element) = %d\n", iArray_D[5]);
    printf("iArray[6] (7th Element) = %d\n", iArray_D[6]);
    printf("iArray[7] (8th Element) = %d\n", iArray_D[7]);
    printf("iArray[8] (9th Element) = %d\n", iArray_D[8]);
    printf("iArray[9] (10th Element) = %d\n", iArray_D[9]);

    int_size_D = sizeof(int);
    iArray_size_D = sizeof(iArray_D);
    iArray_num_elements_D = iArray_size_D / int_size_D;
    printf("size of Data type 'int'                           = %d bytes\n",int_size_D);
    printf("Number of Elements In 'int' Array 'iArray[]'      = %d Bytes\n",iArray_num_elements_D);
    printf("size of Array 'iArray[]' (%d Elements * %d Bytes) = %d bytes", iArray_num_elements_D, int_size_D,iArray_size_D); 

    //*********fArray[]*********

    printf("\n\n");
    printf("In-Line Initialization And Peace-meal Display Of Element Of Array 'fArray[]' : \n");
    printf("fArray[0] (1st Element) = %f\n", fArray_D[0]);
    printf("fArray[1] (2nd Element) = %f\n", fArray_D[1]);
    printf("fArray[2] (3rd Element) = %f\n", fArray_D[2]);
    printf("fArray[3] (4rd Element) = %f\n", fArray_D[3]);
    printf("fArray[4] (5th Element) = %f\n", fArray_D[4]);
    printf("fArray[5] (6th Element) = %f\n", fArray_D[5]);
    printf("fArray[6] (7th Element) = %f\n", fArray_D[6]);
    printf("fArray[7] (8th Element) = %f\n", fArray_D[7]);
    printf("fArray[8] (9th Element) = %f\n", fArray_D[8]);
    printf("fArray[9] (10th Element) = %f\n\n", fArray_D[9]);

    float_size_D = sizeof(float);
    fArray_size_D = sizeof(fArray_D);
    fArray_num_elements_D = fArray_size_D / float_size_D;
    printf("size of Data type 'float'                           = %d bytes\n",float_size_D);
    printf("Number of Elements In 'float' Array 'fArray[]'      = %d Elements\n",fArray_num_elements_D);
    printf("size of Array 'fArray[]' (%d Elements * %d Bytes) = %d bytes\n\n", fArray_num_elements_D, float_size_D, fArray_size_D); 
    

    //cArray[]
    
    printf("In-Line Initialization And Peace-meal Display Of Element Of Array 'cArray[]' : \n");
    printf("cArray[0] (1st Element) = %c\n", cArray_D[0]);
    printf("cArray[1] (2nd Element) = %c\n", cArray_D[1]);
    printf("cArray[2] (3rd Element) = %c\n", cArray_D[2]);
    printf("cArray[3] (4rd Element) = %c\n", cArray_D[3]);
    printf("cArray[4] (5th Element) = %c\n", cArray_D[4]);
    printf("cArray[5] (6th Element) = %c\n", cArray_D[5]);
    printf("cArray[6] (7th Element) = %c\n", cArray_D[6]);
    printf("cArray[7] (8th Element) = %c\n", cArray_D[7]);
    printf("cArray[8] (9th Element) = %c\n", cArray_D[8]);
    printf("cArray[9] (10th Element) = %c\n", cArray_D[9]);
    printf("cArray[10] (11th Element) = %c\n", cArray_D[10]);
    printf("cArray[11] (12th Element) = %c\n", cArray_D[11]);
    printf("cArray[12] (13th Element) = %c\n\n", cArray_D[12]);

    char_size_D = sizeof(char);
    cArray_size_D = sizeof(cArray_D);
    cArray_num_elements_D = cArray_size_D / char_size_D;
    printf("size of Data type 'char'                           = %d bytes\n",char_size_D);
    printf("Number of Elements In 'char' Array 'cArray[]'      = %d Elements\n",cArray_num_elements_D);
    printf("size of Array 'cArray[]' (%d Elements * %d Bytes) = %d bytes", cArray_num_elements_D, char_size_D, cArray_size_D); 
    
       return(0);

}


