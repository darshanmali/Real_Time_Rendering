#include<stdio.h>

int main(void)
{
    char ch_D;
    char *ptr_D = NULL;

    ch_D = 'D';

    printf("\n\n");
    
    printf("**********Before PTR_D = &ch_D ************\n\n");
    printf("Value of 'ch'            = %c\n\n",ch_D);
    printf("Address of 'ch'          = %p\n\n",&ch_D);
    printf("Value At Address of 'ch' = %c\n\n",*(&ch_D));

    ptr_D = &ch_D;

    printf("\n\n");

    printf("**********Afore PTR_D = &ch_D ************\n\n");
    printf("Value of 'ch'            = %c\n\n",ch_D);
    printf("Address of 'ch'          = %p\n\n",ptr_D);
    printf("Value At Address of 'ch' = %c\n\n", *ptr_D);

    return(0);

}


