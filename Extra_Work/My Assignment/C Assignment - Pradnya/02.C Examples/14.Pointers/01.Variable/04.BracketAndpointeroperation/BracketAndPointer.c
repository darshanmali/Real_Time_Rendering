#include<stdio.h>

int main(void)
{
    int num_D;
    int *ptr_D = NULL;
    int ans_D;

    num_D = 3;
    ptr_D = &num_D;

    printf("\n\n");
    printf("num         =%d\n",num_D);
    printf("&num        =%p\n", &num_D);
    printf("*(&num_D)   =%d\n", *(&num_D));
    printf("ptr_D       =%p\n", ptr_D);
    printf("*ptr        =%d\n", *ptr_D);

    printf("\n\n");
    printf("Answer of (ptr_D + 10 ) = %p\n",(ptr_D + 10));
    printf("Answer of *(ptr_D + 10 ) = %d\n",*(ptr_D+10));
    printf("Answer of (*ptr_D + 10 ) = %d\n",(*ptr_D+10));

    ++*ptr_D;
    printf("Answer of ++*ptr_D : %d\n",*ptr_D);


    *ptr_D++;
    printf("Answer of *ptr_D++ : %d\n",*ptr_D);

    ptr_D = &num_D;
    (*ptr_D)++;
    printf("Answer of (*ptr_D)++ : %d\n",*ptr_D);

    return(0);

}


