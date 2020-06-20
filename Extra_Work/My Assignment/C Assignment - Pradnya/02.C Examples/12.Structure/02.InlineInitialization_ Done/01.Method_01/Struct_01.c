#include<stdio.h>

struct MyData
{
    int i_D;
    float f_D;
    double d_D;
    char c_D;
}data = {50, 5.7f, 11.67890, 'D'};

int main(void)
{

    printf("\n\n");
    printf("Data Members of 'struct MyData' are : \n\n");
    printf("I = %d\n", data.i_D);
    printf("F = %f\n", data.f_D);
    printf("D = %lf\n", data.d_D);
    printf("C = %c\n\n", data.c_D);

    return(0);
}

