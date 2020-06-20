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
    
    int i_D;

    printf("In-Line Initialization And Peace-meal Display Of Element Of Array 'iArray[]' : \n");

    int_size_D = sizeof(int);
    iArray_size_D = sizeof(iArray_D);
    iArray_num_elements_D = iArray_size_D / int_size_D;

    for (i_D = 0; i_D < iArray_num_elements_D; i_D++)
    {
        printf("iArray[%d] (Element %d) = %d\n", i_D, (i_D + 1), iArray_D[i_D]);
    }
    
    printf("size of Data type 'int'                           = %d bytes\n",int_size_D);
    printf("Number of Elements In 'int' Array 'iArray[]'      = %d Bytes\n",iArray_num_elements_D);
    printf("size of Array 'iArray[]' (%d Elements * %d Bytes) = %d bytes\n\n", iArray_num_elements_D, int_size_D,iArray_size_D); 

    //*********fArray[]*********

    printf("In-Line Initialization And Peace-meal Display Of Element Of Array 'fArray[]' : \n");

    float_size_D = sizeof(float);
    fArray_size_D = sizeof(fArray_D);
    fArray_num_elements_D = fArray_size_D / float_size_D;
    
    i_D = 0;
    while(i_D < fArray_num_elements_D)
    {
        printf("fArray[%d] (Element %d) = %f\n", i_D, (i_D + 1), fArray_D[i_D]);
        i_D++;
    }

    printf("size of Data type 'float'                           = %d bytes\n",float_size_D);
    printf("Number of Elements In 'float' Array 'fArray[]'      = %d Elements\n",fArray_num_elements_D);
    printf("size of Array 'fArray[]' (%d Elements * %d Bytes) = %d bytes\n\n", fArray_num_elements_D, float_size_D, fArray_size_D); 
    

    //cArray[]
    
    printf("In-Line Initialization And Peace-meal Display Of Element Of Array 'cArray[]' : \n");
    char_size_D = sizeof(char);
    cArray_size_D = sizeof(cArray_D);
    cArray_num_elements_D = cArray_size_D / char_size_D;

    i_D = 0;
    do
    {
        printf("cArray[%d] (Element %d) = %c\n", i_D, ( i_D + 1), cArray_D[i_D]);
        i_D++;
    } while (i_D < cArray_num_elements_D);
    
    printf("size of Data type 'char'                           = %d bytes\n",char_size_D);
    printf("Number of Elements In 'char' Array 'cArray[]'      = %d Elements\n",cArray_num_elements_D);
    printf("size of Array 'cArray[]' (%d Elements * %d Bytes) = %d bytes", cArray_num_elements_D, char_size_D, cArray_size_D); 
    
    return(0);

}


