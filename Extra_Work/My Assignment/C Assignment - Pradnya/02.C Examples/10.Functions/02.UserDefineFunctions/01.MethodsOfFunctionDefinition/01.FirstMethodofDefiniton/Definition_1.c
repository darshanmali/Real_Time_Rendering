#include<stdio.h>

int main(int argc, char *argv[], char *envp[])
{
    void MyAddition(void);

    MyAddition();
    return(0);
}

void MyAddition(void)
{
    int a_D, b_D, sum_D;

    printf("\n\n");
    printf("Enter Integer Value For 'A' : ");
    scanf("%d",&a_D);

    printf("\n\n");
    printf("Enter Integer Value For 'B' : ");
    scanf("%d",&b_D);

    sum_D = a_D + b_D;

    printf("\n\n");
    printf("Sum of %d and %d = %d", a_D, b_D, sum_D);    

}

