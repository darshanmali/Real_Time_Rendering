#include<stdio.h>

int main(void)
{
    //variable declaration
    int i = 5;
    float f = 3.9f;
    double d = 8.041997;
    char c = 'A';

    //code
    printf("\n\n");

    printf("i = %d\n", i);
    printf("f = %f\n", f);
    printf("d = %lf\n", d);
    printf("c = %c\n", c);

    printf("\n");

    i = 43;
    f = 6.54f;
    d = 26.5645;
    c = 'D';

    printf("i = %d\n", i);
    printf("f = %f\n", f);
    printf("d = %lf\n", d);
    printf("c = %c\n", c);

    return(0);
}