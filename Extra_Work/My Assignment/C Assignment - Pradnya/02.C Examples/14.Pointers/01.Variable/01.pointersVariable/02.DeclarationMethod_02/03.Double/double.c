#include<stdio.h>

int main(void)
{
    double num_D;
    double* ptr_D = NULL;

    num_D = 4.35434335f;

    printf("\n\n");
    
    printf("**********Before PTR_D = &num_D ************\n\n");
    printf("Value of 'num'            = %lf\n\n",num_D);
    printf("Address of 'num'          = %p\n\n",&num_D);
    printf("Value At Address of 'num' = %lf\n\n",*(&num_D));

    ptr_D = &num_D;

    printf("\n\n");

    printf("**********Afore PTR_D = &num_D ************\n\n");
    printf("Value of 'num'            = %lf\n\n",num_D);
    printf("Address of 'num'          = %p\n\n",ptr_D);
    printf("Value At Address of 'num' = %lf\n\n", *ptr_D);

    return(0);

}


