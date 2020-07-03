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
    printf("Member of struct Are  : \n\n");

    s.i_D = 7;
    s.f_D = 4.5f;
    s.d_D = 3.6776;
    s.c_D = 'D';

    printf("s.i = %d \n\n", s.i_D);
    printf("s.f = %f \n\n", s.f_D);
    printf("s.d = %lf\n\n", s.d_D);
    printf("s.c = %c \n\n", s.c_D);

    printf("Address of members Of Struct s Are :\n\n");
    printf("s.i = %p \n\n", &s.i_D);
    printf("s.f = %p \n\n", &s.f_D);
    printf("s.d = %p \n\n", &s.d_D);
    printf("s.c = %p \n\n", &s.c_D);

    printf("My struct s = %p\n\n", &s);

    printf("MyUnion u1 = %p\n\n",&u);

    printf("\n\n");
    printf("Member of union u Are : \n\n");

    u.i_D = 5;
    printf("u.i_D  = %d\n\n", u.i_D);

    u.f_D = 2.7f;
    printf("u.f_D  = %f\n\n", u.f_D);
    
    u.d_D = 6.87695;
    printf("u.d_D  = %lf\n\n", u.d_D);
    
    u.c_D = 'D';
    printf("u.c_D  = %c\n\n", u.c_D);

    printf(" Address of Member Of Union u Are :\n");
    printf("u.i = %p \n\n", &u.i_D);
    printf("u.f = %p \n\n", &u.f_D);
    printf("u.d = %p \n\n", &u.d_D);
    printf("u.c = %p \n\n", &u.c_D);

    printf("MyUnion u = %p \n\n", &u);

    return(0);
}

