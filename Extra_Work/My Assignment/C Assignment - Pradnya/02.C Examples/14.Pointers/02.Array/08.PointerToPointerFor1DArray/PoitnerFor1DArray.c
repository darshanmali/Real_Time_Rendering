#include<stdio.h>
#include<stdlib.h>

int main(void)
{
    void MyAlloc(int **ptr, unsigned int numberofElements);

    int *piArray = NULL;
    unsigned int num_elements;
    int i;

    printf("\n\n");
    printf("How Many Elements you want in Integer Array ?\n\n");
    scanf("%d", &num_elements);

    printf("\n\n");
    MyAlloc(&piArray, num_elements);

    printf("Enter %d Elements to fill up Your Integers Array : \n\n",num_elements);
    for (i = 0; i < num_elements; i++)
        scanf("%d", &piArray[i]);
    
    printf("\n\n");
    printf("The %d Elemets Entered By you In the Integers Array : \n\n",num_elements);
    for (i = 0; i < num_elements; i++)
        printf("%d\t",piArray[i]);

    printf("\n\n");
    if (piArray)
    {
        free(piArray);
        piArray = NULL;
        printf(" Memory Allocated By you For Array Is Freed !!!!\n");
    }

    return(0);    

}

void MyAlloc(int **ptr,unsigned int numberofElements)
{
    *ptr = (int *)malloc(numberofElements * sizeof(int));
    if(*ptr == NULL)
    {
        printf("could Not Allocate Memory !!! Exitting Now ...\n\n");
        exit(0);
    }
    printf("MyAlloc() Has SuccessFully Allocated %lu Bytes For Integers Array !!!\n\n", (numberofElements * sizeof(int)));
}

