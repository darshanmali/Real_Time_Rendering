#include<stdio.h>

int main(void)
{
    int m,n;
    char ch_01, ch_02;

    int a, result_int;
    float f, result_float;

    int i_explicit;
    float f_explicit;

    printf("\n\n");

    m = 70;
    ch_01 = m;
    printf("M =%d\n", m);
    printf("character 1 (after ch_01 = m) = %c \n\n",ch_01);

    ch_02 = 'Q';
    n = ch_02;
    printf("Character 2 = %c\n", ch_02);
    printf("N (after N = ch_02) = %d\n\n", n);

    a = 10;
    f = 7.8f;
    result_float = a + f;
    printf("Integer a = %d And Floating - Point Number %f Added Gives Floating-point Sum  = %f\n\n",a,f,result_float);

    result_int = a + f;
    printf("Integer a = %d And Floating - Point Number %f Added Gives Ineger Sum  = %f\n\n",a,f,result_int);


    f_explicit = 30.12121f;
    i_explicit = (int)f_explicit;
    printf("Floating Point Type casted Explicitly = %f\n", f_explicit);   

    printf("Integer after Type casted Explicitly %f = %d\n\n", f_explicit,i_explicit);

    return (0);

}