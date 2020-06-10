#include<stdio.h>

int main(void)
{

    int x, y;
    int m, n; 
    char ch_result_01, ch_result_02;
    int i_result_01, i_result_02;

    printf("\n\n");

    x = 7;
    y = 5;

    ch_result_01 = (x > y) ? 'A' : 'B';
    i_result_01 = (x > y) ? x : y;

    printf("Ternary Operator Answer 1 ----- %c and %d.\n\n", ch_result_01, i_result_01);

    m = 30;
    n = 30;

    ch_result_02 = ( m != n) ? 'M' : 'N';
    i_result_02 = (m != m) ? m : n;
    printf("Ternary  Operator Answer 2 ----- %c and %d.\n\n", ch_result_02, i_result_02);

    printf("\n\n");

    return (0);

}