#include<stdio.h>

int global_count_D = 0;


int main(void)
{
    void change_count(void);
    void change_count_One(void);
    void change_count_Two(void);

    extern int global_count_D;

    printf("\n");

    change_count();
    change_count_One();
    change_count_Two();
    return(0);
}



void change_count(void)
{
    global_count_D += 1;
    printf("global Count = %d\n",global_count_D);
}

