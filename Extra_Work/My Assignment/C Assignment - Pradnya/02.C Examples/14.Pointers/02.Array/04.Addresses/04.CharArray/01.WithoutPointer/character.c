#include<stdio.h>

int main(void)
{
    char cArray_D[10];
    int i_D;

    for ( i_D = 0; i_D < 10; i_D++)
        cArray_D[i_D] = (char)(i_D + 65);

    printf("\n\n");
    printf("Elements Of The Character Array : \n\n");

    for ( i_D = 0; i_D < 10; i_D++)
        printf("cArray_D[%d] = %c\n", i_D, cArray_D[i_D]);
    
    printf("\n\n");
    printf("Elements of the character Array : \n\n");
    
    for ( i_D = 0; i_D < 10; i_D++)
        printf("cArray_D[%d] = %c      \t\t Address = %p\n", i_D, cArray_D[i_D], &cArray_D[i_D]);
    
    printf("\n\n");
    
    return(0);
}


