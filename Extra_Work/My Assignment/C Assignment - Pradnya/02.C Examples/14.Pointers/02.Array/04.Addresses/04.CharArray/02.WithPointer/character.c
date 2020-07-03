#include<stdio.h>

int main(void)
{
    char cArray_D[10];
    char *ptr_cArray = NULL;
    int i_D;

    for ( i_D = 0; i_D < 10; i_D++)
        cArray_D[i_D] = (char)(i_D + 65);

    ptr_cArray = cArray_D;

    printf("\n\n");
    printf("Elements Of The Character Array : \n\n");

    for ( i_D = 0; i_D < 10; i_D++)
        printf("cArray_D[%d] = %c\n", i_D, *(ptr_cArray + i_D));
    
    printf("\n\n");
    printf("Elements of the Character Array : \n\n");
    
    for ( i_D = 0; i_D < 10; i_D++)
        printf("cArray_D[%d] = %c        \t\t Address = %p\n", i_D, *(ptr_cArray + i_D), (ptr_cArray + i_D));
    
    printf("\n\n");
    
    return(0);
}


