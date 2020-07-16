#include<stdio.h>
#include<stdarg.h>

int main(int argc, char *argv[], char *envp[])
{
    int CalculateSum(int, ...);

    int answer_D = 0;

    printf("\n\n");
    answer_D = CalculateSum(10, 20, 30, 40, 50);
    printf("Answer Is : %d \n\n",answer_D);

    answer_D = CalculateSum(10, 1, 1, 1, 1, 1, 1, 1, 1);
    printf("Answer Is : %d \n\n",answer_D);

    answer_D = CalculateSum(0);
    printf("Answer Is : %d \n\n",answer_D);
    
    return(0);
    
}   

int CalculateSum(int num, ...)
{
    int va_Calculate_Sum(int, va_list);

    int sum_D = 0;
    va_list number_list;

    va_start(number_list, num);

    sum_D = 0;
    sum_D = va_CalculateSum(num, number_list);
    va_end(number_list);
    return(sum_D);
}

int va_CalculateSum(int num, va_list list)
{
    int n_D;
    int sum_Total_D = 0;

    while (num)
    {
        n_D = va_arg(list, int);
        sum_Total_D = sum_Total_D + n_D;
        num--; 
    }
    return (sum_Total_D);
}

