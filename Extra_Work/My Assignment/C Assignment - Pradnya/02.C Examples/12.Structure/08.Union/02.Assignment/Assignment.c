#include<stdio.h>

union MyUnion
{
    int i_D;
    float f_D;
    double d_D;
    char c_D;
};

int main(void)
{
    union MyUnion u1, u2;

    printf("\n\n");
    printf("Member Of Union u1 Are : \n\n");

    u1.i_D = 6;
    u1.f_D = 1.2f;
    u1.d_D = 8.33333;
    u1.c_D = 'S';

    printf("u1.i = %d \n\n", u1.i_D);
    printf("u1.f = %f \n\n", u1.f_D);
    printf("u1.d = %lf \n\n", u1.d_D);
    printf("u1.c = %c \n\n", u1.c_D);

    printf("Address of members Of Union u1 Are :\n\n");
    printf("u1.i = %p \n\n", &u1.i_D);
    printf("u1.f = %p \n\n", &u1.f_D);
    printf("u1.d = %p \n\n", &u1.d_D);
    printf("u1.c = %p \n\n", &u1.c_D);

    printf("MyUnion u1 = %p\n\n",&u1);

    printf("\n\n");
    printf("Member of union u2 Are : \n\n");

    u2.i_D = 3;
    printf("u2.i_D  = %d\n\n", u2.i_D);

    u2.f_D = 4.5f;
    printf("u2.f_D  = %f\n\n", u2.f_D);
    
    u2.d_D = 4.5454455;
    printf("u2.d_D  = %lf\n\n", u2.d_D);
    
    u2.c_D = 'D';
    printf("u2.c_D  = %c\n\n", u2.c_D);

    printf(" Address of Member Of Union u2 Are :\n");
    printf("u2.i = %p \n\n", &u2.i_D);
    printf("u2.f = %p \n\n", &u2.f_D);
    printf("u2.d = %p \n\n", &u2.d_D);
    printf("u2.c = %p \n\n", &u2.c_D);

    printf("MyUnion u2 = %p \n\n", &u2);
    return (0);

}



