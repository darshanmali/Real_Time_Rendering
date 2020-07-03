#include<stdio.h>

int main(void)
{
    int iArray_D[] = {12, 24, 36, 48, 60, 72,  84, 96, 108, 120};
    float fArray_D[] = {9.8f, 8.7f, 5.6f, 3.9f};
    double dArray_D[] = {1.22222, 2.33333, 4.676666};
    char cArray_D[] = {'D','A','R','S','H','A','N','\0'};

    printf("\n\n");
    printf("Integer Array Elements And The Address They Occupy Are As Follows : \n\n");
    printf("iArray_D[0] = %d \t At Address : %p\n",*(iArray_D + 0), (iArray_D + 0));
    printf("iArray_D[1] = %d \t At Address : %p\n",*(iArray_D + 1), (iArray_D + 1));
    printf("iArray_D[2] = %d \t At Address : %p\n",*(iArray_D + 2), (iArray_D + 2));
    printf("iArray_D[3] = %d \t At Address : %p\n",*(iArray_D + 3), (iArray_D + 3));
    printf("iArray_D[4] = %d \t At Address : %p\n",*(iArray_D + 4), (iArray_D + 4));
    printf("iArray_D[5] = %d \t At Address : %p\n",*(iArray_D + 5), (iArray_D + 5));
    printf("iArray_D[6] = %d \t At Address : %p\n",*(iArray_D + 6), (iArray_D + 6));
    printf("iArray_D[7] = %d \t At Address : %p\n",*(iArray_D + 7), (iArray_D + 7));
    printf("iArray_D[8] = %d \t At Address : %p\n",*(iArray_D + 8), (iArray_D + 8));
    printf("iArray_D[9] = %d \t At Address : %p\n",*(iArray_D + 9), (iArray_D + 9));


    printf("Floating Array Elements And The Address They Occupy Are As Follows : \n\n");
    printf("fArray_D[0] = %f \t At Address : %p\n",*(fArray_D + 0), (fArray_D + 0));
    printf("fArray_D[1] = %f \t At Address : %p\n",*(fArray_D + 1), (fArray_D + 1));
    printf("fArray_D[2] = %f \t At Address : %p\n",*(fArray_D + 2), (fArray_D + 2));
    printf("fArray_D[3] = %f \t At Address : %p\n",*(fArray_D + 3), (fArray_D + 3));
    printf("fArray_D[4] = %f \t At Address : %p\n",*(fArray_D + 4), (fArray_D + 4));


    printf("Double Array Elements And The Address They Occupy Are As Follows : \n\n");
    printf("dArray_D[0] = %lf \t At Address : %p\n",*(dArray_D + 0), (dArray_D + 0));
    printf("dArray_D[1] = %lf \t At Address : %p\n",*(dArray_D + 1), (dArray_D + 1));
    printf("dArray_D[2] = %lf \t At Address : %p\n",*(dArray_D + 2), (dArray_D + 2));


     printf("\n\n");
    printf("char Array Elements And The Address They Occupy Are As Follows : \n\n");
    printf("cArray_D[0] = %c \t At Address : %p\n",*(cArray_D + 0), (cArray_D + 0));
    printf("cArray_D[1] = %c \t At Address : %p\n",*(cArray_D + 1), (cArray_D + 1));
    printf("cArray_D[2] = %c \t At Address : %p\n",*(cArray_D + 2), (cArray_D + 2));
    printf("cArray_D[3] = %c \t At Address : %p\n",*(cArray_D + 3), (cArray_D + 3));
    printf("cArray_D[4] = %c \t At Address : %p\n",*(cArray_D + 4), (cArray_D + 4));
    printf("cArray_D[5] = %c \t At Address : %p\n",*(cArray_D + 5), (cArray_D + 5));
    printf("cArray_D[6] = %c \t At Address : %p\n",*(cArray_D + 6), (cArray_D + 6));
    printf("cArray_D[7] = %c \t At Address : %p\n",*(cArray_D + 7), (cArray_D + 7));

    return(0);
}