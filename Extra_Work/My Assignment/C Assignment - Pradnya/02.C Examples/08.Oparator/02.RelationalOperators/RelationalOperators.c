#include<stdio.h>

int main(void)
{
    int x;
    int y;
    int result;
    

    printf("\n\n");
    printf("Enter 1. Numyer : ");
    scanf("%d", &x);

    printf("\n\n");
    printf("Enter 2. Numyer : ");
    scanf("%d", &y);
    
    printf("\n\n");
    printf("If Ans = 0, It is 'False'.\n");
    printf("If Ans = 1, It is 'True'.\n\n");

    result = (x < y);
    printf("(x < y)  X = %d Is Less Than y = %d                         \t Answer = %d\n", x, y, result);

    result = (x > y);
    printf("(x > y)  X = %d Is Greater Than y = %d                       \t Answer = %d\n", x, y, result);

    result = (x <= y);
    printf("(x <= y)  X = %d Is Less Than or Equal to y = %d              \t Answer = %d\n", x, y, result);

    result =  (x >= y);
    printf("(x >= y)  X = %d Is Greater Than or Equal to y = %d            \t Answer = %d\n", x, y, result);

    result = (x == y);
    printf("(x == y)  X = %d Is Equal To y = %d                             \t Answer = %d\n", x, y, result);
    
    result = (x != y);
    printf("(x != y)  X = %d Is NOT Equal To y = %d                         \t Answer = %d\n", x, y, result);
    
    return (0);
    

}