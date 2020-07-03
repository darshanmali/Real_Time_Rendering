#include<stdio.h>


int main(void)
{
    void MathaMathicalOperation(int, int, int *,int *,int *,int *,int *);
     int a_D, b_D;
     int answer_sum, answer_differnce, answer_product, answer_quotient,answer_reminder;

    printf("\n\n");
    printf("Enter value for 'A' : ");
    scanf("%d",&a_D);
    
    printf("\n\n");
    printf("Enter The Value of 'B' : ");
    scanf("%d",&b_D); 


    MathaMathicalOperation(a_D, b_D, &answer_sum, &answer_differnce, &answer_product, &answer_quotient,&answer_reminder);

    printf("\n\n");
    printf("******** Result ********\n\n");
    printf("sum = %d\n\n",answer_sum);
    printf("Difference = %d\n\n",answer_differnce);
    printf("Product = %d\n\n",answer_product);
    printf("Quotient = %d\n\n",answer_quotient);
    printf("Remainder = %d\n\n",answer_reminder);

    return(0);

}

void MathaMathicalOperation(int x, int y, int *sum,int *Difference,int *product,int *Quatient,int *remainder)
{
    *sum = x +y;
    *Difference = x - y;
    *product = x * y;
    *Quatient = x / y;
    *remainder = x % y;

}

