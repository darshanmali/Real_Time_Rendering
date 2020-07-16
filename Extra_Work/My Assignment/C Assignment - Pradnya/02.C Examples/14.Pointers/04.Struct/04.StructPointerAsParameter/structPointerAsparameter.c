#include<stdio.h>

struct MyData
{
    int i_D;
    float f_D;
    double d_D;
};

int main(void)
{
   void changevalues(struct MyData *);

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
    pData->i_D = 87;
    pData->f_D = 21.45f;
    pData->d_D = 1.2345;

    printf("\n\n");
    printf("DATA MEMBERs of 'struct MyData' Are : \n\n");
    printf("i = %d \n", pData->i_D);
    printf("f = %f \n", pData->f_D);
    printf("d = %lf\n", pData->d_D);
  
    changevalues(pData);

    printf("\n\n");
    printf("DATA MEMBERs of 'struct MyData' Are : \n\n");
    printf("i = %d \n", pData->i_D);
    printf("f = %f \n", pData->f_D);
    printf("d = %lf\n", pData->d_D);
    
    if (pData)
    {
        free(pData);
        pData = NULL;
        printf("Memory Allocation to 'struct MyData' has been successfully freed ...!!!\n\n ");
    }
    return(0);
    
}

void changevalues(struct MyData *pParam_Data)
{
    pParam_Data->i_D = 5;
    pParam_Data->f_D = 29.12f;
    pParam_Data->d_D = 44.4567;
}

