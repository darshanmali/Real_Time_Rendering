#include<stdio.h>

struct MyData
{
    int i_D;
    float f_D;
    double d_D;
    char c_D;
}data;

int main(void)
{
    int i_size;
    int f_size;
    int d_size;
    int c_size;
    int struct_MyData_size;

    data.i_D = 25;
    data.f_D = 66.45f;
    data.d_D = 2.4567;
    data.c_D = 'D';

    printf("\n\n");
    printf("Data Members Of 'Strut MyData' Are :\n\n");
    printf("i = %d\n", data.i_D);
    printf("f = %f\n", data.f_D);
    printf("d = %lf\n", data.d_D);
    printf("c = %c\n", data.c_D);

    i_size = sizeof(data.i_D);
    f_size = sizeof(data.f_D);
    d_size = sizeof(data.d_D);
    c_size = sizeof(data.c_D);

    printf("\n\n");
    printf("Size (in bytes) of Data Members Of 'Strut MyData' Are : \n\n");
    printf("size of 'i' = %d bytes\n", i_size);
    printf("size of 'f' = %d bytes\n", f_size);
    printf("size of 'd' = %d bytes\n", d_size);
    printf("size of 'c' = %d bytes\n", c_size);

    struct_MyData_size = sizeof(struct MyData);

    printf("\n\n");
    printf("size of 'struct MyData' : %d Bytes\n\n", struct_MyData_size);
    return (0);
    
}