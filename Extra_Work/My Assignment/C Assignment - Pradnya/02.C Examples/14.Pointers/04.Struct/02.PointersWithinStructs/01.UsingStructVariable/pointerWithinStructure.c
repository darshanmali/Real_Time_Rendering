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
    struct MyData data;

    data.i_D = 9;
    data.ptr_i_D = &data.i_D;

    data.f_D = 11.45f;
    data.ptr_f_D = &data.f_D;

    data.d_D = 45.67544;
    data.ptr_d_D = &data.d_D;

    printf("\n\n");
    printf("i_D = %d\n",*(data.ptr_i_D));
    printf("ANddress of 'i_D' = %p", data.ptr_i_D);
    
    printf("\n\n");
    printf("f_D = %f\n",*(data.ptr_f_D));
    printf("ANddress of 'f_D' = %p", data.ptr_f_D);
    
    printf("\n\n");
    printf("d_D = %lf\n",*(data.ptr_d_D));
    printf("ANddress of 'd_D' = %p", data.ptr_d_D);

    return(0);
    
}
