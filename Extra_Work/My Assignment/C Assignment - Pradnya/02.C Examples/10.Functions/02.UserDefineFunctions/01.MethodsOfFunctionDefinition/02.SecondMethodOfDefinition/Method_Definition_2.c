#include<stdio.h>

int main(int argc, char *argv[],char *envp[])
{
    int MyAdditon(void);
    int result_D;

    result_D = MyAddition();

    printf("\n\n");
    printf("Sum = %d \n\n", result_D);

    return(0);
}

int MyAddition(void)
{
    int a_D, b_D, Sum_D;

    printf("\n\n");
    printf("Enter Integer value For 'A' : ");
    scanf("%d", &a_D);

    printf("\n\n");
    printf("Enter Integer value for 'B' : ");
    scanf("%d", &b_D);

    Sum_D = a_D + b_D;

    return (Sum_D);
}

