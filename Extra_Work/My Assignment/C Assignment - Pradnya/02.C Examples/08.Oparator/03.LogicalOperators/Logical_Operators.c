#include<stdio.h>

int main(void)
{
    int x;
    int y;
    int z;
    int result;
    

    printf("\n");
    printf("Enter 1. Numyer : ");
    scanf("%d", &x);

    printf("\n");
    printf("Enter 2. Numyer : ");
    scanf("%d", &y);
    
    
    printf("\n");
    printf("Enter 3. Numyer : ");
    scanf("%d", &z);
    
    printf("\n\n");
    printf("If Ans = 0, It is 'False'.\n");
    printf("If Ans = 1, It is 'True'.\n\n");

    result = (x <= y) && (y != z);
    printf("x = %d\ny = %d\nz = %d\n\n1. condition : \n(x <= y) && (y != z)\n(%d <= %d) && (%d != %d)\nAns : %d.\n", x, y, z, x, y, y, z, result);

    printf("--------------------------------------------------------------------");

    result = (y >= x) || (x == z);
    printf("\n\n2. condition : \n(y >= x) || (x == z)\n(%d >= %d) && (%d == %d)\nAns : %d.\n", y, x, x, z, result);

    printf("--------------------------------------------------------------------");

    result = !x;
    printf("\n\n3. condition : \n result = !x\nNone = !%d\nAns : %d.\n", x, result);

    printf("--------------------------------------------------------------------");
    result = !y;
    printf("\n\n4. condition : \n result = !y\nNone = !%d\nAns : %d.\n", y, result);

    printf("--------------------------------------------------------------------");

    result = !z;
    printf("\n\n5. condition : \n result = !z\nNone = !%d\nAns : %d.\n", z, result);

    printf("--------------------------------------------------------------------");

    result = (!(x <= y) && !(y != z));
    printf("\n\n6. condition : \n(x <= y) && (y != z)\n(%d <= %d) && (%d != %d)\nAns : %d.\n", x, y, y, z, result);

    printf("--------------------------------------------------------------------");
    result = !((y >= z) || (x == z));
    printf("\n\n7. condition : \n!((y >= x) || (x == z))\n!((%d <= %d) || (%d != %d))\nAns : %d.\n", y, x, x, z, result);

    return(0);

}
