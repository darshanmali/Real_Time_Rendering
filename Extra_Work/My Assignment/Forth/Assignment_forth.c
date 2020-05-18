#include<stdio.h>

int main(int argc, char * argv[])
{
    int year = 0;
    
    printf("Pleas Enter Year:\n");
    scanf("%d",&year);

    if ((year % 4 == 0) && (year % 100 != 0) || (year % 400 == 0) )
    {
        printf("Year is Leap Year.");
    }
    else
    {
        printf("Year is not Leap Year.");
    }
    
    return (0);
}