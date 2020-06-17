#include<stdio.h>

int main(void)
{
    int a_D = 5;
    void change_count(void);

    printf("\n");
    printf("A = %d \n\n", a_D);

    change_count();

    change_count();

    change_count();

    return(0);

}

void change_count(void)
{
    int local_count_D = 0;

    local_count_D += 1;
    printf("Local Count = %d\n", local_count_D);

}

