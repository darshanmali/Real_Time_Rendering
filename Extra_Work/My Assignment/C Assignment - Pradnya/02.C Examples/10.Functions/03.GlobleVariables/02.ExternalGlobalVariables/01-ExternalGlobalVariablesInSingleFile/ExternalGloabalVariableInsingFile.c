#include<stdio.h>

int main(void)
{
    void change_count(void);

    extern int global_count_D;

    printf("\n");
    printf("Value of global Count Before Change)Count() = %d\n",global_count_D);

    change_count();

    printf("Value of global Count After Change)Count() = %d\n",global_count_D);
    return(0);
}

int global_count_D = 0;

void change_count(void)
{
    global_count_D = 5;
    printf("Value of global Count in Change_Count() = %d\n",global_count_D);
}

