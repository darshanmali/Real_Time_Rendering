#include<stdio.h>

struct MyData
{
    int i_D;
    float f_D;
    double d_D;
};

int main(void)
{
    int i_size_D;
    int f_size_D;
    int d_size_D;
    int struct_Mydata_size;
    int pointer_to_MyData_size;

    struct MyData *pData = NULL;
    printf("\n\n");

    pData = (struct MyData *)malloc(sizeof(struct MyData));
    if (pData == NULL)
    {
        printf("failed to Allcation memmory to struct MyData !!! Exitting Now ...\n\n");
        exit(0);
    }
    else
    {
        printf("SuccessFull Allocation memory To 'struct Mydata' !!! \n\n");
    }
    pData->i_D = 30;
    pData->f_D = 11.45f;
    pData->i_D = 1.2345;

    printf("\n\n");
    printf("DATA MEMBERs of 'struct MyData' Are : \n\n");
    printf("i = %d \n", pData->i_D);
    printf("f = %f \n", pData->f_D);
    printf("d = %lf\n", pData->d_D);

    i_size_D = sizeof(pData->i_D);
    f_size_D = sizeof(pData->f_D);
    d_size_D = sizeof(pData->d_D);

    printf("\n\n");
    printf("Size (in bytes) of data members of 'struct MyData' Are :\n\n");
    printf("size of 'i_D' = %d bytes \n", i_size_D);
    printf("size of 'f_D' = %d bytes \n", f_size_D);
    printf("size of 'd_D' = %d bytes \n", d_size_D);

    struct_Mydata_size = sizeof(struct MyData);
    pointer_to_MyData_size = sizeof(struct MyData *);


    printf("\n\n");
    printf("size of 'struct MyData' : %d Bytes\n\n", struct_Mydata_size);
    printf("size of pointer to 'struct MyData' : %d bytes\n\n", pointer_to_MyData_size);

    if (pData)
    {
        free(pData);
        pData = NULL;
        printf("Memory Allocation to 'struct MyData' has been successfully freed ...!!!\n\n ");
    }
    return(0);
    
}