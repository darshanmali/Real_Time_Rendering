#include<stdio.h>

int main(void)
{
    double dArray_D[10];
    int i_D;

    for ( i_D = 0; i_D < 10; i_D++)
        dArray_D[i_D] = (float)(i_D + 1) * 1.5f;

    printf("\n\n");
    printf("Elements Of The Double Array : \n\n");

    for ( i_D = 0; i_D < 10; i_D++)
        printf("dArray_D[%d] = %lf\n", i_D, dArray_D[i_D]);
    
    printf("\n\n");
    printf("Elements of the Double Array : \n\n");
    
    for ( i_D = 0; i_D < 10; i_D++)
        printf("dArray_D[%d] = %lf      \t\t Address = %p\n", i_D, dArray_D[i_D], &dArray_D[i_D]);
    
    printf("\n\n");
    
    return(0);
}


