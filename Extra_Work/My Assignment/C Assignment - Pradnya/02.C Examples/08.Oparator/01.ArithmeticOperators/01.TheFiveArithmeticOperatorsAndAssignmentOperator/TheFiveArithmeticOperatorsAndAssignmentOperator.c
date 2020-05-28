#include<stdio.h>

int main(void)
{
    int a;
    int b;
    int result;

    printf("\n\n");
    printf("Enter A Number : ");
    scanf("%d",&a);

    printf("\n\n");
    printf("Enter B Number : ");
    scanf("%d",&b);
    
    printf("\n\n");
    
    result = a + b;
    printf("Addition of A : %d and B : %d is = %d\n", a, b, result);

    result = a - b;
    printf("Subtraction of A : %d and B : %d is = %d\n", a, b, result);

    result = a * b;
    printf("Multiplication of A : %d and B : %d is = %d\n", a, b, result);

    result = a / b;
    printf("Division of A : %d and B : %d Quotient is = %d\n", a, b, result);

    result = a % b;
    printf("Division of A : %d and B : %d Reminder is = %d\n", a, b, result);

    printf("\n\n");

    return(0);
}