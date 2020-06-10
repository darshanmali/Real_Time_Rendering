#include<stdio.h>
int main(void)
{
    int age;

    printf("\n\n");
    printf("Enter Age : ");
    scanf("%d",&age);
    printf("\n\n");
    if(age >= 18)
    {
        printf("Enetering if-block...\n\n");
        printf("You Are Eligible for Voting !!!\n\n");
    }
    else
    {
        printf("Enetering else-block...\n\n");
        printf("You Are Not Eligible for Voting !!!\n\n");
    }
    return(0);        
}
