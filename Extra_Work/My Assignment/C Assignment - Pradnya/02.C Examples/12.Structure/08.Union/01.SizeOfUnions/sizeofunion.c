#include<stdio.h>

struct MyStruct
{
    int i_D;
    float f_D;
    double d_D;
    char c_D;
};

union MyUnion
{
    int i_D;
    float f_D;
    double d_D;
    char c_D;
};

int main(void)
{
    struct MyStruct s;
    union MyUnion u;

    printf("\n\n");
    printf("Size Of MyStruct = %lu\n", sizeof(s));
    printf("\n\n");
    printf("Size Of MyUnion = %lu\n", sizeof(u));
    return (0);

}

