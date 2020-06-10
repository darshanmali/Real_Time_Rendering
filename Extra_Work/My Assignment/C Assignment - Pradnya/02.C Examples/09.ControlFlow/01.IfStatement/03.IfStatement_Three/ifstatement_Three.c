#include<stdio.h>

int main(void)
{

    int n;

    printf("\n\n");

    printf("Enter Value for 'n' : ");
    scanf("%d", &n);

    if(n < 0)
    {
        printf("n = %d is smaller then 0 (Negative).\n\n", n);
    }
    if((n > 0) && (n <= 100))
    {
        printf("n = %d Is Between 0 And 100 \n\n", n);
    }
    if((n > 100) && (n <= 200))
    {
        printf("n = %d Is Between 100 And 200 \n\n", n);
    }
    if((n > 200) && (n < 300))
    {
        printf("n = %d Is Between 200 And 300 \n\n", n);
    }
    if((n > 300) && (n < 400))
    {
        printf("n = %d Is Between 300 And 400 \n\n", n);
    }
    if((n > 400) && (n < 500))
    {
        printf("n = %d Is Between 400 And 500 \n\n", n);
    }
    if(n > 500)
    {
        printf("n = %d Is Greater then 500 \n\n", n);
    }

    return (0);

}

