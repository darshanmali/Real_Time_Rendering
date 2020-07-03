#include<stdio.h>

enum
{
    NEGATIVE = -1,
    ZERO,
    POSITIVE
};

int main(void)
{
    int Difference(int, int, int *);
    int a_D, b_D, answer, rect;

    printf("\n\n");
    printf("Enter value for 'A' : ");
    scanf("%d",&a_D);
    
    printf("\n\n");
    printf("Enter The Value of 'B' : ");
    scanf("%d",&b_D); 

    rect = Difference( a_D, b_D, &answer);
    printf("\n\n");
    printf("Differnnce of %d And %d = %d \n\n", a_D, b_D, answer);

    if (rect == POSITIVE)
    {
        printf("The difference of %d And %d Is Positive !!\n\n", a_D, b_D);
    }
    else if (rect == NEGATIVE)
    {
        printf("The difference of %d And %d Is Negative !!\n\n", a_D, b_D);
    }
    else
    {
        printf("The Difference Of %d And %d Is Zero !!!\n\n",a_D, b_D);
    }
    return (0);    

}

int Difference(int x, int y, int *diff)
{
    *diff = x-y;

    if(*diff > 0)
    {
        return (POSITIVE);
    }
    else if (*diff < 0)
    {
        return(NEGATIVE);
    }
    else
    {
        return(ZERO);
    }
    
}


