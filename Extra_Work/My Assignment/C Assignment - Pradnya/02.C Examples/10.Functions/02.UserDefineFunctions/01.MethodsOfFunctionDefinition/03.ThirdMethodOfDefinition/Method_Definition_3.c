#include<stdio.h>

int main(int argc, char *argv[],char *envp[])
{
    void MyAddition(int , int);
    int a_D, b_D;
    
    printf("\n\n");
    printf("Enter Integer value For 'A' : ");
    scanf("%d", &a_D);

    printf("\n\n");
    printf("Enter Integer value for 'B' : ");
    scanf("%d", &b_D);

    MyAddition(a_D, b_D);

    return(0);
}

void MyAddition(int x, int y)
{
   int Sum_D;

    Sum_D = x + y;

    printf("\n\n");
    printf("Sum of %d and %d = %d \n\n",x, y, Sum_D);

}
