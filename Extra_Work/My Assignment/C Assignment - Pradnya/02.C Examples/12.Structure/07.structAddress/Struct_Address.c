#include<stdio.h>

struct MyData
{
    int i_D;
    float f_D;
    double d_D;
    char c_D;
};

int main(void)
{
    struct MyData data;

    data.i_D = 30;
    data.d_D = 11.45f;
    data.f_D = 1.2995;
    data.c_D = 'D';

    printf("\n\n");
    printf("Data Members Of 'Struct MyData' Are : \n\n");
    printf("i = %d\n", data.i_D);
    printf("f = %f\n", data.f_D);
    printf("d = %lf\n", data.d_D);
    printf("c = %c\n", data.c_D);

    printf("\n\n");
    printf("Address Of Data Members of 'struct Mydata' Are : \n\n");
    printf("'i_D' occupies Address From %p\n", &data.i_D);
    printf("'f_D' occupies Address From %p\n", &data.f_D);
    printf("'d_D' occupies Address From %p\n", &data.d_D);
    printf("'c_D' occupies Address %p\n", &data.c_D);

    printf("Starting Address of 'struct MyData' Variable 'data' = %p\n\n",&data);
    return(0);
}


