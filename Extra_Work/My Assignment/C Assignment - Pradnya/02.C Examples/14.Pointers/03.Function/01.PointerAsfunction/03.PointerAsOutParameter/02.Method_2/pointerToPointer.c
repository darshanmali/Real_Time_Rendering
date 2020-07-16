#include<stdio.h>
#include<stdlib.h>

int main(void)
{
    void MathematicalOperation(int, int,int *, int *, int *,int *, int *);
    int a_D;
    int b_D;
    int *answer_sum_D = NULL;
    int *answer_difference_D = NULL;
    int *answer_product_D = NULL;
    int *answer_quotient_D = NULL;
    int *answer_remainder_D = NULL;


    printf("\n\n");
    printf("ENter Value of 'A' : ");
    scanf("%d", &a_D);


    printf("\n\n");
    printf("ENter Value of 'B' : ");
    scanf("%d", &b_D);

    answer_sum_D = (int *)malloc(1 * sizeof(int));
    if (answer_sum_D == NULL)
    {
        printf("Could Not Allocate Memory For 'Answer_Sum'. Existing Now ....!!!\n");
        exit(0);
    }
    
    answer_difference_D = (int *)malloc(1 * sizeof(int));
    if (answer_difference_D == NULL)
    {
        printf("Could Not Allocate Memory For 'answer_difference_D'. Existing Now ....!!!\n");
        exit(0);
    }
    
    answer_product_D = (int *)malloc(1 * sizeof(int));
    if (answer_product_D == NULL)
    {
        printf("Could Not Allocate Memory For 'answer_product_D'. Existing Now ....!!!\n");
        exit(0);
    }
    
    answer_quotient_D = (int *)malloc(1 * sizeof(int));
    if (answer_quotient_D == NULL)
    {
        printf("Could Not Allocate Memory For 'answer_quotient_D'. Existing Now ....!!!\n");
        exit(0);
    }
    
    answer_remainder_D = (int *)malloc(1 * sizeof(int));
    if (answer_remainder_D == NULL)
    {
        printf("Could Not Allocate Memory For 'answer_remainder_D'. Existing Now ....!!!\n");
        exit(0);
    }

    MathematicalOperation( a_D, b_D, answer_sum_D, answer_difference_D, answer_product_D, answer_quotient_D, answer_remainder_D);
    printf("\n\n");
    printf("*******RESULT **********\n\n");
    printf("sum = %d\n\n",*answer_sum_D);\
    printf("difference = %d\n\n",*answer_difference_D);
    printf("product = %d\n\n",*answer_product_D);
    printf("quotient = %d\n\n",*answer_quotient_D);
    printf("remaider = %d\n\n",*answer_remainder_D);

     if (answer_remainder_D )
    {
        free(answer_remainder_D);
        answer_remainder_D = NULL;
        printf("Memory Allocated For 'answer_remainder_D'. SuccessFully Freed ....!!!\n");
    }
     if (answer_quotient_D )
    {
        free(answer_quotient_D);
        answer_quotient_D = NULL;
        printf("Memory Allocated For 'answer_quotient_D'. SuccessFully Freed ....!!!\n");
    }
     if (answer_product_D )
    {
        free(answer_product_D);
        answer_product_D = NULL;
        printf("Memory Allocated For 'answer_product_D'. SuccessFully Freed ....!!!\n");
    }
    
     if (answer_difference_D )
    {
        free(answer_difference_D);
        answer_difference_D = NULL;
        printf("Memory Allocated For 'answer_difference_D'. SuccessFully Freed ....!!!\n");
    }

     if (answer_sum_D )
    {
        free(answer_sum_D);
        answer_sum_D = NULL;
        printf("Memory Allocated For 'answer_sum_D'. SuccessFully Freed ....!!!\n");
    }

    return(0);
}

void MathematicalOperation(int x,int y, int *sum, int *diffenrce, int *product, int *quotient, int *remainder)
{
    *sum = x + y;
    *diffenrce = x - y;
    *product = x * y;
    *quotient = x / y;
    *remainder = x % y;
}

