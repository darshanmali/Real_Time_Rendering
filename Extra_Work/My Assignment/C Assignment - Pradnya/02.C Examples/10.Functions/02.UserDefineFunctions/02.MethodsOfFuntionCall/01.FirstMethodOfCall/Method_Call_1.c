#include<stdio.h>

int main(int argc, char *argv[], char *envp[])
{
    void MyAddition(void);
    int MySubtraction(void);
    void MyMultiplication(int, int);
    int MyDivision(int, int);

    int result_subtraction_D;
    int a_multiplication_D, b_multiplication_D;
    int a_division_D, b_division_D, result_division_D;

    MyAddition();
    
    result_subtraction_D = MySubtraction();
    printf("\n\n");
    printf("Subtraction Yields Result = %d\n",result_subtraction_D);

    printf("\n\n");
    printf("Enter Integer Value For 'A' For Multiplication : ");
    scanf("%d", &a_multiplication_D);

    printf("\n\n");
    printf("Enter Integer Value For 'B' For Multiplication : ");
    scanf("%d", &b_multiplication_D);

    MyMultiplication(a_multiplication_D,b_multiplication_D);

    
    printf("\n\n");
    printf("Enter Integer Value For 'A' For Devision : ");
    scanf("%d", &a_division_D);

    printf("\n\n");
    printf("Enter Integer Value For 'B' For Devision : ");
    scanf("%d", &b_division_D);

    result_division_D = MyDivision(a_division_D, b_division_D);
    printf("\n\n");

    printf("Division Of %d And %d = %d (Quontient)\n", a_division_D , b_division_D, result_division_D);

    printf("\n\n");
    
    return(0);

}


void MyAddition(void)
{
    int a_D, b_D, sum_D;

    printf("\n\n");
    printf("Enter Integer value for 'A' For Addition : ");
    scanf("%d",&a_D);
    
    printf("\n\n");
    printf("Enter Integer value for 'B' For Addition : ");
    scanf("%d",&b_D);
    
    sum_D = a_D + b_D;

    printf("\n\n");
    printf("Sum Of %d And %d = %d\n\n", a_D, b_D, sum_D);

}

int MySubtraction(void)
{
    int a_D, b_D, sub_D;

    printf("\n\n");
    printf("Enter Integer value for 'A' For Subtraction : ");
    scanf("%d",&a_D);
    
    printf("\n\n");
    printf("Enter Integer value for 'B' For Subtraction : ");
    scanf("%d",&b_D);
    
    sub_D = a_D - b_D;

    return (sub_D);
}

void MyMultiplication(int a,int b)
{
    int multiplication_D;

    multiplication_D = a * b;

    printf("\n\n");
    printf("Multiplication of %d And %d = %d\n\n", a, b,multiplication_D);

}

int MyDivision(int a, int b)
{
    int Division_quontient_D;
    if(a > b)
        Division_quontient_D = a / b;
    else
        Division_quontient_D = b / a;
    
    return (Division_quontient_D);
}


