#include<stdio.h>
#include<conio.h>

int main(void)
{
    int a, b;
    int result;

    char option, option_devision;

    printf("\n\n");

    printf("Enter A's Value : ");
    scanf("%d", &a);


    printf("Enter B's Value : ");
    scanf("%d", &b);

    printf("Enter Optionb In character : \n\n");
    printf("'A' or 'a' For Addition: \n");
    printf("'S' or 's' For Subtraction: \n");
    printf("'M' or 'm' For Multiplication: \n");
    printf("'D' or 'd' For Division: \n");


    printf("Enter an Option : ");
    option = getch();

    printf("\n\n");
    if(option == 'A' || option == 'a')
    {
        result = a + b;
        printf("Addition of A = %d And B = %d Given result %d !!!\n\n", a, b, result);
    }
    else if(option == 'S' || option == 's')
    {
        if(a >= b)
        {
            result = a - b;
            printf("Subtraction of B = %d From a = %d Given result %d !!!\n\n", b, a, result);
        }
        else
        {
            result = b - a;
            printf("Subtraction of A = %d And B = %d Given result %d !!!\n\n", a, b, result);
        }
        
    }
    else if(option == 'M' || option == 'm')
    {
        result = a * b;
        printf("Multiplication of A = %d And B = %d Given Result is %d !!!\n\n", a, b, result);

    }
    else if(option == 'D' || option == 'd')
    {
        printf("Enter Option in Character : \n\n");
        printf("'Q' or 'q' or '/' For Quontient Upon Division : \n");
        printf("'R' or 'r' or '%' For Reminder Upon Division : \n");

        printf("\nEnter Option : ");
        option_devision = getch();
        if(option_devision == 'Q' || option_devision == 'q' || option_devision == '/')
        {
            if(a >= b)
            {
                result = a / b;
                printf("Division of A = %d By B = %d Given result %d !!!\n\n", a, b, result);
            }
            else
            {
                result = b / a;
                printf("Division of B = %d By A = %d Given result %d !!!\n\n", b, a, result);
            }  
        }
        else if(option_devision == 'R' || option_devision == 'r' || option_devision == '%')
        {
            if(a >= b)
            {
                result = a / b;
                printf("Division of A = %d From B = %d Given Reminder %d !!!\n\n", a, b, result);
            }
            else
            {
                result = b % a;
                printf("Division of of B = %d By A = %d Given Reminder %d !!!\n\n", b, a, result);
            }  
 
        }
        else
        {
            printf("Invalid Character %c Entered Please enter again...!!!\n\n", option_devision);
        }
    
    }
    else
    {
        printf("Invalid Character %c Entered Please enter again...!!!\n\n", option);
    }
    printf("If - Else If Else Ladder Completed !!\n");
 
    return (0);
}


