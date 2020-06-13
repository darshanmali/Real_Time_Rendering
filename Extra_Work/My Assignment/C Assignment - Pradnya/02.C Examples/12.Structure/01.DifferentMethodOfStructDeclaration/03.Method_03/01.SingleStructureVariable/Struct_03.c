#include<stdio.h>


int main(void)
{
    struct MyData
    {
        int i_D;
        float f_D;
        double d_D;
    
    }data_D;

    int i_size;
    int f_size;
    int d_size;
    
    int struct_MyData_size;

    data_D.i_D = 15;
    data_D.f_D = 32.95f;
    data_D.d_D = 9.6578;

    printf("\n\n");
    printf("Data Members Of 'Strut MyData' Are :\n\n");
    printf("i = %d\n", data_D.i_D);
    printf("f = %f\n", data_D.f_D);
    printf("d = %lf\n", data_D.d_D);

    i_size = sizeof(data_D.i_D);
    f_size = sizeof(data_D.f_D);
    d_size = sizeof(data_D.d_D);

    printf("\n\n");
    printf("Size (in bytes) of Data Members Of 'Strut MyData' Are : \n\n");
    printf("size of 'i' = %d bytes\n", i_size);
    printf("size of 'f' = %d bytes\n", f_size);
    printf("size of 'd' = %d bytes\n", d_size);

    struct_MyData_size = sizeof(struct MyData);

    printf("\n\n");
    printf("size of 'struct MyData' : %d Bytes\n\n", struct_MyData_size);
    return (0);
    
}

