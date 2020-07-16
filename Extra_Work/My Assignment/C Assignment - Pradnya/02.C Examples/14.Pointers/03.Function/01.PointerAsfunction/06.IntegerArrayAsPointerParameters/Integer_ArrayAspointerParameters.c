#include<stdio.h>
#include<stdlib.h>

int main(void)
{
    void MultiplyArrayElementsByNumber(int *,int, int);

    int *iArray = NULL;
    int num_elemets;
    int i_D, Num_D;

    printf("\n\n");
    printf("Enter How many elements you want in Integer Array : ");
    scanf("%d", &num_elemets);

    iArray = (int *)malloc(num_elemets * sizeof(int));
    if(iArray == NULL)
    {
        printf("Memory Alocation to 'iArray' has Failed Failed !!! Exitting Now....\n\n");
        exit(0);
    }
    printf("\n\n");
    printf("Enter %d Elements For The Integer Array : \n", num_elemets);
    for (i_D = 0; i_D < num_elemets; i_D++)
    {
        scanf("%d", &iArray[i_D]);
    }

    printf("\n\n");
    printf("Array Before passing To Function MultiplyArrayElemtsByNumber() :\n\n");
    for(i_D = 0; i_D < num_elemets; i_D++)
    {
        printf("iArray[%d] = %d\n",i_D,iArray[i_D]);
    }

    if (iArray)
    {
        free(iArray);
        iArray = NULL;
        printf("\n\n");
        printf("Memory Allocation to the  'iArray' Has Been successFully Freed..!!!\n\n ");
    }
    return(0);
    
}

void MultiplyArrayElementsByNumber (int *arr, int iNumElemets, int n)
{
    int i_D;

    for (i_D = 0; i_D < iNumElemets; i_D++)
    {
        arr[i_D] = arr[i_D] * n;
    }
    
}