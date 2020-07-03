#include<stdio.h>


int main(void)
{
    int num;
    int *ptr = NULL;
    int **pptr = NULL;

    num = 10;

    printf("\n\n");

    printf("********* BEFORE PTR = &NUM **************\n\n");
    printf("Value  Of 'num '            = %d\n\n",num);   
    printf("Address of 'num'            = %p\n\n",&num);
    printf("Value at Address of 'num'   = %d\n\n",*(&num));


    ptr = &num;

    printf("\n\n");
    printf("********* AFTER PTR = &NUM **************\n\n");
    printf("Value  Of 'num '            = %d\n\n",num);   
    printf("Address of 'num'            = %p\n\n",ptr);
    printf("Value at Address of 'num'   = %d\n\n",*ptr);


    pptr = &ptr;

    printf("\n\n");
    printf("********* AFTER PPTR = &ptr **************\n\n");
    printf("Value  Of 'num '                            = %d\n\n",num);   
    printf("Address of 'num' (ptr)                      = %p\n\n",ptr);
    printf("Address of 'ptr' (pptr)                     = %p\n\n",pptr);
    printf("Value at Address of 'ptr' (*pptr)           = %p\n\n",*pptr);
    printf("Value at Address of 'num' (*ptr) (*pptr)    = %d\n\n",**pptr);
    
    return(0);

}