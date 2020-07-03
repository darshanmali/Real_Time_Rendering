#include<stdio.h>

int main(void)
{
    int num_D;
    int *ptr = NULL;
    int *copy_ptr = NULL;

    num_D = 14;
    ptr = &num_D;

    printf("\n\n");
    printf("*********** BEFORE Copyptr = ptr *************\n\n");
    printf("num_D               =%d\n",num_D);
    printf("&num_D              =%p\n",&num_D);
    printf("+(&num_D)           =%d\n",*(&num_D));
    printf("ptr                 =%p\n",ptr);
    printf("*ptr                =%d\n",*ptr);


    copy_ptr = ptr;
    
    printf("\n\n");
    printf("*********** AFTER Copyptr = ptr *************\n\n");
    printf("num_D               =%d\n",num_D);
    printf("&num_D              =%p\n",&num_D);
    printf("+(&num_D)           =%d\n",*(&num_D));
    printf("ptr                 =%p\n",ptr);
    printf("*ptr                =%d\n",*ptr);
    printf("copy_ptr            =%p\n",copy_ptr);
    printf("*copy_ptr           =%d\n",*copy_ptr);

    return(0);
    
    
}
