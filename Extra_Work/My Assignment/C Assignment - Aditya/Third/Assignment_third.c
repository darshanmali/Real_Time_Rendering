#include<stdio.h>

int main(int argc, char *argv[])
{
    int i = 0;
    printf("Please Enter Number :");
    scanf("%d",&i);

    if( i < 0 )
    {
        printf("I is Negative number : %d", i);
    }
    else if( i > 0)
    {
        printf("i is Positive number : %d", i);
    }
    else
    {
        printf("I is Zero : %d", i);
    }
    
    return (0);
}