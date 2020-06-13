#include<stdio.h>

int main(int argc, char *argv[],char *envp[])
{
    int MyAddition(int , int);
    int result_D;
    int a_D, b_D;
    
    printf("\n\n");
    printf("Enter Integer value For 'A' : ");
    scanf("%d", &a_D);

    printf("\n\n");
    printf("Enter Integer value for 'B' : ");
    scanf("%d", &b_D);

    result_D = MyAddition(a_D, b_D);
    
    printf("\n\n");
    printf("Sum of %d and %d = %d \n\n", a_D, b_D, result_D);

    return(0);
}

int MyAddition(int x, int y)
{
   int Sum_D;

    Sum_D = x + y;
    return(Sum_D);
}
