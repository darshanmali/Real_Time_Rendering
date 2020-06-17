#include<stdio.h>

extern int global_count_D;

void change_count_One(void)
{
    global_count_D = global_count_D + 1;
    printf("change_count_One() : Value of global_Count_D = %d\n", global_count_D);

}