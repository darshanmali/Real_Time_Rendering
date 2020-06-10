#include<stdio.h>

int main(void)
{
    int age;

    printf(" \n\n");

    printf("Enter Age : ");
    scanf("%d", &age);
    if(age >= 18)
    {
           printf("You are eligible for VOting.!!!\n\n");
    }

    printf("you are not eligible for Voting.!!!");
    return (0);
}
