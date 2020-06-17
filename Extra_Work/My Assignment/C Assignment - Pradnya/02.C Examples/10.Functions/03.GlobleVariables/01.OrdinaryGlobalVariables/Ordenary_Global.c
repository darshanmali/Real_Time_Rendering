#include<stdio.h>

int gloabal_count_D = 0;

int main(int argc,char *argv[], char *envp[])
{
    void change_count_one(void);
    void change_count_two(void);
    void change_count_three(void);

    printf("\n\n");

    printf("main() : value of global_count = %d", gloabal_count_D);

    change_count_one();
    change_count_two();
    change_count_three();

    printf("\n\n");

    return(0);

}


void change_count_one(void)
{
    gloabal_count_D  = 100;
    printf("change_count_one() : value of gloabal_count = %d\n", gloabal_count_D);

}

void change_count_two(void)
{
    gloabal_count_D  += 1;
    printf("change_count_two() : value of gloabal_count = %d\n", gloabal_count_D);

}

void change_count_three(void)
{
    gloabal_count_D  += 10;
    printf("change_count_three() : value of gloabal_count = %d\n", gloabal_count_D);

}
