#include<stdio.h>

int main(void)
{
    int m, n, p;

    m = 6;
    n = 20;
    p = 30;

    printf("\n\n");
    if( m < n)
    {
        printf("Entering First if-Block...\n\n");
        printf("M Is Less Than N !!!\n\n");
    }
    printf("Done With First if-Block...\n\n");
    
    printf("\n\n");
    if(n != p)
    {
        printf("Entering Second if-Block...\n\n");
        printf("N Is Not Equal to p !!!\n\n");
    }
    else
    {
        printf("Entering Second Else-Block...\n\n");
        printf("N Is Equal to p !!!\n\n");
    }
    printf("Done With Second if-Else Pair...\n\n");
    
    return (0);

}

