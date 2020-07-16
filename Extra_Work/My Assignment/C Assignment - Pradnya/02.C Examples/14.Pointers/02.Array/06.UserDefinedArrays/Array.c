#include<stdio.h>
#include<stdlib.h>

#define INT_SIZE sizeof(int)
#define FLOAT_SIZE sizeof(float)
#define DOUBLE_SIZE sizeof(double)
#define CHAR_SIZE sizeof(char)


int main(void)
{
    int *ptr_iArray_D = NULL;
    unsigned int intArraylength_D = 0;

    float *ptr_fArray_D = NULL;
    unsigned int floatArraylength_D = 0;
    
    double *ptr_dArray_D = NULL;
    unsigned int doubleArrayLength_D = 0;

    char *ptr_cArray_D = NULL; 
    unsigned int charArrayLength_D = 0;
    
    int i_D;

    printf("\n\n");
    printf("Enter The Number of Element you want int the Integer Array : ");
    scanf("%d", &intArraylength_D);

    ptr_iArray_D = (int *)malloc(INT_SIZE * intArraylength_D);
    if (ptr_iArray_D == NULL)
    {
        printf("Memory Allocation For Intger Array Failed !!! Existing Now \n\n");
        exit(0);
    }
    else
    {
        printf("\n\n");
        printf("Memory Allocation for the Integer Array Is Succeeded !!!\n\n");
    }
    printf("\n\n");
    printf("Enter The %d Integer Elements To Fil Up Integer Array : \n\n", intArraylength_D);
    for ( i_D = 0; i_D < intArraylength_D; i_D++)
    {
        scanf("%d", (ptr_iArray_D + i_D));
    }
    
    // Float Array    
    printf("\n\n");
    printf("Enter The Number of Element you want int the float Array : ");
    scanf("%u", &floatArraylength_D);

    ptr_fArray_D = (float *)malloc(FLOAT_SIZE * floatArraylength_D);
    if (ptr_fArray_D == NULL)
    {
        printf("Memory Allocation For float Array Failed !!! Existing Now \n\n");
        exit(0);
    }
    else
    {
        printf("\n\n");
        printf("Memory Allocation for the float Array Is Succeeded !!!\n\n");
    }
    printf("\n\n");
    printf("Enter The %d float Elements To Fil Up float Array : \n\n", intArraylength_D);
    for ( i_D = 0; i_D < floatArraylength_D; i_D++)
    {
        scanf("%f", (ptr_fArray_D + i_D));
    }
    
    // Double Array    
    printf("\n\n");
    printf("Enter The Number of Element you want int the Double Array : ");
    scanf("%d", &doubleArrayLength_D);

    ptr_dArray_D = (double *)malloc(DOUBLE_SIZE * doubleArrayLength_D);
    if (ptr_dArray_D == NULL)
    {
        printf("Memory Allocation For double Array Failed !!! Existing Now \n\n");
        exit(0);
    }
    else
    {
        printf("\n\n");
        printf("Memory Allocation for the double Array Is Succeeded !!!\n\n");
    }
    printf("\n\n");
    printf("Enter The %d double Elements To Fil Up double Array : \n\n", doubleArrayLength_D);
    for ( i_D = 0; i_D < doubleArrayLength_D; i_D++)
    {
        scanf("%lf", (ptr_dArray_D + i_D));
    }

    // Char Array    
    printf("\n\n");
    printf("Enter The Number of Element you want int the Char Array : ");
    scanf("%d", &charArrayLength_D);

    ptr_cArray_D = (char *)malloc(CHAR_SIZE * charArrayLength_D);
    if (ptr_cArray_D == NULL)
    {
        printf("Memory Allocation For Char Array Failed !!! Existing Now \n\n");
        exit(0);
    }
    else
    {
        printf("\n\n");
        printf("Memory Allocation for the Char Array Is Succeeded !!!\n\n");
    }
    printf("\n\n");
    printf("Enter The %d Char Elements To Fil Up Char Array : \n\n", charArrayLength_D);
    for ( i_D = 0; i_D < charArrayLength_D; i_D++)
    {
        *(ptr_cArray_D + i_D) = getch();
        printf("%c\n", *(ptr_cArray_D + i_D));
    }

    // ************* Dispalying Arrays ****************
    printf("\n\n");
    printf("The Integer Array Entered By You And Consisting of %d Elements is As Follows : \n\n", intArraylength_D);

    for ( i_D = 0; i_D < intArraylength_D; i_D++)
    {
        printf("%d       \t \t At Address : %p\n", *(ptr_iArray_D + i_D), (ptr_iArray_D + i_D));
    }    

    for ( i_D = 0; i_D < floatArraylength_D; i_D++)
    {
        printf("%f       \t \t At Address : %p\n", *(ptr_fArray_D + i_D), (ptr_fArray_D + i_D));
    }    
    
    for ( i_D = 0; i_D < doubleArrayLength_D; i_D++)
    {
        printf("%lf      \t \t At Address : %p\n", *(ptr_dArray_D + i_D), (ptr_dArray_D + i_D));
    }    
    
    for ( i_D = 0; i_D < charArrayLength_D; i_D++)
    {
        printf("%c       \t \t At Address : %p\n", *(ptr_cArray_D + i_D), (ptr_cArray_D + i_D));
    }    

    if (ptr_cArray_D)
    {
        free(ptr_cArray_D);
        ptr_cArray_D = NULL;

        printf("Memory occupied By character Array is freed !!!\n\n");
    }
    
    if (ptr_dArray_D)
    {
        free(ptr_dArray_D);
        ptr_dArray_D = NULL;

        printf("Memory occupied By character Array is freed !!!\n\n");
    }
    
    if (ptr_fArray_D)
    {
        free(ptr_fArray_D);
        ptr_fArray_D = NULL;

        printf("Memory occupied By character Array is freed !!!\n\n");
    }
    
    if (ptr_iArray_D)
    {
        free(ptr_iArray_D);
        ptr_iArray_D = NULL;

        printf("Memory occupied By character Array is freed !!!\n\n");
    }
    
}