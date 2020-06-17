#include<stdio.h>

void change_count_Two(void)
{
    extern int global_count_D;
    global_count_D = global_count_D + 1;
    printf("change_count_tWO() : Value of global_Count_D = %d\n", global_count_D);

}