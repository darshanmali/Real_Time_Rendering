#include<stdio.h>

struct MyData
{
    int *ptr_i_D;
    int i_D;

    float *ptr_f_D;
    float f_D;

    double *ptr_d_D;
    double d_D;
};

int main(void)
{
    struct MyData *PtrData_D = NULL;

    printf("\n\n");
    PtrData_D = (struct MyData *)malloc(sizeof(struct MyData));
    if (PtrData_D  == NULL)
    {
        printf("Failed to Allocate the Memory to 'struct Mydata' !!! Exitting Now...\n\n");
        exit(0);
    }
    else
        printf("SuccessFully Allocated memory to 'struct MyData' !!!!\n\n");

    (*PtrData_D).i_D = 9;
    (*PtrData_D).ptr_i_D = &(*PtrData_D).i_D;

    (*PtrData_D).f_D = 11.45f;
    (*PtrData_D).ptr_f_D = &(*PtrData_D).f_D;

    (*PtrData_D).d_D = 45.67544;
    (*PtrData_D).ptr_d_D = &(*PtrData_D).d_D;

    printf("\n\n");
    printf("i_D = %d\n",*((*PtrData_D).ptr_i_D));
    printf("ANddress of 'i_D' = %p", (*PtrData_D).ptr_i_D);
    
    printf("\n\n");
    printf("f_D = %f\n",*((*PtrData_D).ptr_f_D));
    printf("ANddress of 'f_D' = %p", (*PtrData_D).ptr_f_D);
    
    printf("\n\n");
    printf("d_D = %lf\n",*((*PtrData_D).ptr_d_D));
    printf("ANddress of 'd_D' = %p\n\n", (*PtrData_D).ptr_d_D);

    if (PtrData_D)
    {
        free(PtrData_D);
        PtrData_D = NULL;
        printf("Meemory Allocated By you is SuccessFully freed..!!!!\n\n");
    }
    
    return(0);
    
}

