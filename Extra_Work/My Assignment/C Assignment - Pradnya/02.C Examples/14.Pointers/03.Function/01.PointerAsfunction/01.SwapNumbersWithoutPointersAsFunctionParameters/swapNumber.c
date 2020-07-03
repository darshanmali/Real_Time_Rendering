#include<stdio.h>

int main(void)
{
    void Swapnumbers(int, int );

    int a_D;
    int b_D;

    printf("\n\n");
    printf("Enter The Value of 'A' : ");
    scanf("%d",&a_D);
    
    printf("\n\n");
    printf("Enter The Value of 'B' : ");
    scanf("%d",&b_D);
    
    
    printf("\n\n");
    printf("*********BEFORE SWAPING *********\n\n");
    printf("Value of 'A' = %d \n\n",a_D);
    printf("Value of 'B' = %d \n\n",b_D);

    Swapnumbers(a_D,b_D);
    
    printf("\n\n");
    printf("*********AFTER SWAPING *********\n\n");
    printf("Value of 'A' = %d \n\n",a_D);
    printf("Value of 'B' = %d \n\n",b_D);
    
    return(0);
}

void Swapnumbers(int x, int y)
{
    int temp;

    printf("\n\n");
    printf("*********BEFORE SWAPING *********\n\n");
    printf("Value of 'X' = %d \n\n",x);
    printf("Value of 'Y' = %d \n\n",y);

    temp = x;
    x = y;
    y = temp;

    printf("\n\n");
    printf("*********AFTER SWAPING *********\n\n");
    printf("Value of 'X' = %d \n\n",x);
    printf("Value of 'Y' = %d \n\n",y);


}