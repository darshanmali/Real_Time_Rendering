#include<stdio.h>

int main(int argc, char *argv[],char *envp[])
{
    unsigned int num_D;

    void recursive(unsigned int);

    printf("\n\n");
    printf("Enter Any Number : \n\n");
    scanf("%u",&num_D);

    printf("\n\n");
    printf("OutPut of recursive function : \n\n");

    recursive(num_D);

    printf("\n\n");
    return(0);
    
}

void recursive(unsigned int n)
{
    printf("n = %d\n",n);

    if (n > 0)
    {
        recursive(n - 1);
    }
    
}


