#include<stdio.h>

int main(void)
{
    int a;
    int b;
    int x;

    printf("\n\n");
    printf("Enter A Number : ");
    scanf("%d",&a);

    printf("\n\n");
    printf("Enter B Number : ");
    scanf("%d",&b);

    x = a;
    a += b;
    printf("Addition of A = %d and B = %d is : %d\n", x, b, a);

    x = a;
    a -= b;
    printf("Substraction of A = %d and B = %d is : %d\n", x, b, a);
    
    x = a;
    a *= b;
    printf("Multipliction of A = %d and B = %d is : %d\n", x, b, a);
    
    x = a;
    a /= b;
    printf("Division of A = %d and B = %d Quotient  is : %d\n", x, b, a);
    
    x = a;
    a %= b;
    printf("Division of A = %d and B = %d Remainder is : %d\n", x, b, a);
    

    printf("\n\n");

    return(0);
    
}