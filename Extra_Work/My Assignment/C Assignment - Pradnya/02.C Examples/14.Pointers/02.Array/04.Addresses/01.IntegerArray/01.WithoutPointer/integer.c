#include<stdio.h>

int main(void)
{
    int iArray_D[10];
    int i_D;

    for ( i_D = 0; i_D < 10; i_D++)
        iArray_D[i_D] = (i_D + 1) * 3;

    printf("\n\n");
    printf("Elements Of The Integers Array : \n\n");

    for ( i_D = 0; i_D < 10; i_D++)
        printf("iArray_D[%d] = %d\n", i_D, iArray_D[i_D]);
    
    printf("\n\n");
    printf("Elements of the ietegers Array : \n\n");
    
    for ( i_D = 0; i_D < 10; i_D++)
        printf("iArray_D[%d] = %d \t\t Address = %p\n", i_D, iArray_D[i_D], &iArray_D[i_D]);
    
    printf("\n\n");
return(0);
}


