#include<stdio.h>
int main(void)
{
    printf("\n\n");
    printf("********************************************************");
    printf("\n\n");

    printf("Hello World !!!\n\n");
    
    int a = 13;
    printf("Integer Decimal value of 'a' = %d\n", a);
    printf("Integer Octal value of 'a' = %o\n", a);
    printf("Integer Hexadecimal value of 'a' (Hexadecimal Letters In Lower case) = %x\n", a);
    printf("Integer Hexadecimal value of 'a' (Hexadecimal Letter In Lower Case ) = %X\n\n", a);

    char ch = 'D';
    printf("Character ch = %c\n",ch);
    char str[] = "AstroMedicomp's Real Time Rendering Batch 3.0 (2020-2021)";
    printf("String str = %s\n\n",str);

    long num = 30121995L; 
    printf("Long Integer = %ld\n\n", num);

    unsigned int b = 7;
    printf("unsigned Integer 'b' = %u\n\n", b);

    float f_num = 3012.1995f;
    printf("Floating Point Number With just %%f 'f_num' = %f\n",f_num);
    printf("Floating Point Number with %%4.2f 'f_num' = %4.2f\n", f_num);
    printf("Floating Point Number With %%6.5f 'f_num' = %6.5f\n\n", f_num);

    double d_pi = 3.14159265358979323846;

    printf("Double Precision Folating Point Number Without Exponential =%g\n", d_pi);
    printf("Double Precision Floating Point Number With Exponential (Lower Case) = %e\n", d_pi);
    printf("Double Precision Floating point Number With Exponential (Upper Case) = %E\n\n", d_pi);
    printf("Double Hexadecimal Value of 'd_pi' (Hexadecimal Letters In Lower Case) = %a\n", d_pi);
    printf("Double Hexadecimal value of 'd_pi' (Hexadecimal Letters In Lower case) = %A\n\n", d_pi);

    printf("****************************************************************");
    printf("\n\n");
    return(0);

}