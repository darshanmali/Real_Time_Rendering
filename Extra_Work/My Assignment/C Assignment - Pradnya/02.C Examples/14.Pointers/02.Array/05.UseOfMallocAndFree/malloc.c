#include<stdio.h>
#include<stdlib.h>

int main(main)
{
    int *ptr_iArray_D = NULL;
    unsigned int intArrayLength_D = 0;
    int i_D;

    printf("\n\n");
    printf("Enter the number  of elements you want in your Array : ");
    scanf("%d", &intArrayLength_D);
 
    ptr_iArray_D = (int *)malloc(sizeof(int) * intArrayLength_D);

    if (ptr_iArray_D == NULL)
    {
        printf("\n\n");
        printf("Memory Couldn't Complete be completed!!!");
        exit(0);
    }
    else
    {
        printf("\n\n");
        printf("Memory Allocation For Integer Array Has Succeeded !!!!\n\n");
        printf("memory Address from %p to %p have been Allocated  To integer Array!!!\n\n", ptr_iArray_D,(ptr_iArray_D + (intArrayLength_D - 1)));

    }
    printf("\n\n");
    printf("Enter %d Elemets For Integer Array  : ", intArrayLength_D);
    for ( i_D = 0; i_D < intArrayLength_D; i_D++)
    {
        scanf("%d",(ptr_iArray_D + i_D));
    }
    printf("\n\n");
    printf("The Integer Array Entered By you, Consisting of %d Elemets : \n", intArrayLength_D);
    for (i_D = 0; i_D < intArrayLength_D; i_D++)
    {
        printf("ptr_iArray_D [%d] = %d \t\t Address &ptr_iArray_D[%d] : %p\n", i_D,ptr_iArray_D[i_D], i_D, &ptr_iArray_D[i_D]);
    }
    printf("\n\n");
    for (i_D = 0; i_D < intArrayLength_D; i_D++)
    {
        printf("(ptr_iArray + %d) = %d \t\t At Address (ptr_iArray + %d) : %p\n", i_D, *(ptr_iArray_D + 1), i_D, (ptr_iArray_D + 1));

    }
    if(ptr_iArray_D)
    {
        free(ptr_iArray_D);
        ptr_iArray_D = NULL;
        
        printf("\n\n");
        printf("memory Allocation for Integer Array Been Scceshuly Freed !!!!!!");

    }
    return(0);


}


