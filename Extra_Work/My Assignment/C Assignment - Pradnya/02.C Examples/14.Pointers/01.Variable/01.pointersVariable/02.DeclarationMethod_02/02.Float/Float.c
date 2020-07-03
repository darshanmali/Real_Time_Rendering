#include<stdio.h>

int main(void)
{
    float num_D;
    float* ptr_D = NULL;

    num_D = 7.45454545f;

    printf("\n\n");
    
    printf("**********Before PTR_D = &num_D ************\n\n");
    printf("Value of 'num'            = %f\n\n",num_D);
    printf("Address of 'num'          = %p\n\n",&num_D);
    printf("Value At Address of 'num' = %f\n\n",*(&num_D));

    ptr_D = &num_D;

    printf("\n\n");

    printf("**********Afore PTR_D = &num_D ************\n\n");
    printf("Value of 'num'            = %f\n\n",num_D);
    printf("Address of 'num'          = %p\n\n",ptr_D);
    printf("Value At Address of 'num' = %f\n\n", *ptr_D);

    return(0);

}


