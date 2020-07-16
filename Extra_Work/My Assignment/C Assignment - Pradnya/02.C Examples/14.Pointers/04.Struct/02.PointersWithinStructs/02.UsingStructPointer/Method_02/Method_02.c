#include<stdio.h>

struct MyData
{
    int i_D;
    int *ptr_i_D;
    
    float f_D;
    float *ptr_f_D;

    double d_D;
    double *ptr_d_D;
};

int main(int Argc, char *argv[], char *envp[])
{
    struct MyData *ptrData_D = NULL;

    printf("\n\n");
    ptrData_D = (struct MyData *)malloc(sizeof(struct MyData));
    if (ptrData_D == NULL)
    {
        printf("Failed to Alllocate Memory for 'struct MyData'  !!! Exitting Now...\n\n");
        exit(0);
    }
    else
        printf("succesfully Allocated memory to 'struct MyData' !!!\n\n");    

    ptrData_D->i_D = 7;
    ptrData_D->ptr_i_D = &(ptrData_D->i_D);
    
    ptrData_D->f_D = 3.54;
    ptrData_D->ptr_f_D = &(ptrData_D->f_D);
    
    ptrData_D->d_D = 73.5658;
    ptrData_D->ptr_d_D = &(ptrData_D->d_D);    

    printf("\n\n");
    printf("i_D = %d\n",*(ptrData_D->ptr_i_D));
    printf("ANddress of 'i_D' = %p", ptrData_D->ptr_i_D);
    
    printf("\n\n");
    printf("f_D = %f\n",*(ptrData_D->ptr_f_D));
    printf("ANddress of 'f_D' = %p",ptrData_D->ptr_f_D);
    
    printf("\n\n");
    printf("d_D = %lf\n",*(ptrData_D->ptr_d_D));
    printf("ANddress of 'd_D' = %p\n\n", ptrData_D->ptr_d_D);

    if (ptrData_D)
    {
        free(ptrData_D);
        ptrData_D = NULL;
        printf("Meemory Allocated By you is SuccessFully freed..!!!!\n\n");
    }
    
    return(0);
    
}
