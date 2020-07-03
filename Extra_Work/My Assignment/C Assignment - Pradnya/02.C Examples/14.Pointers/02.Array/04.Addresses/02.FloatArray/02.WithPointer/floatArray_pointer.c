#include<stdio.h>

int main(void)
{
    float fArray_D[10];
    float *ptr_fArray = NULL;
    int i_D;

    for ( i_D = 0; i_D < 10; i_D++)
        fArray_D[i_D] = (float)(i_D + 1) * 1.5;

    ptr_fArray = fArray_D;

    printf("\n\n");
    printf("Elements Of The float Array : \n\n");

    for ( i_D = 0; i_D < 10; i_D++)
        printf("fArray_D[%d] = %f\n", i_D, *(ptr_fArray + i_D));
    
    printf("\n\n");
    printf("Elements of the ietegers Array : \n\n");
    
    for ( i_D = 0; i_D < 10; i_D++)
        printf("fArray_D[%d] = %f    \t\t Address = %p\n", i_D, *(ptr_fArray + i_D), (ptr_fArray + i_D));
    
    printf("\n\n");
    return(0);
}


