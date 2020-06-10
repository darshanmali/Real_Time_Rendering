#include<stdio.h>


int main(void)
{
    int a;

    printf("\n\n");

    a = 10;
    if(a)
    {
        printf("IF Block Exists and value is : %d.\n\n", a);

    }
    a = -11;
    if(a)
    {
        printf("IF Block Exists and value is : %d.\n\n", a);
        
    }
    
    a = 0;
    if(a)
    {
        printf("IF Block Exists and value is : %d.\n\n", a);
        
    }
    printf("All Three if-statement Are Done \n\n");

    return (0);

}
