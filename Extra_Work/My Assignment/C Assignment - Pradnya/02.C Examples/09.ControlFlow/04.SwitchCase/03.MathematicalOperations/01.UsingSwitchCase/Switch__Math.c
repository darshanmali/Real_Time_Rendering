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

    switch (option)
    {
    case 'A':
    case 'a':
        result = a +b;
        printf("Addition of A = %d And B = %d Given result %d !!!\n\n", a, b, result);
        break;
    
    case 'S':
    case 's':
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
        break;

    case 'M':
    case 'm':
        result = a * b;
        printf("Multiplication of A = %d And B = %d Given Result is %d !!!\n\n", a, b, result);

        break;

    case 'D':
    case 'd':
        printf("Enter Option in Character : \n\n");
        printf("'Q' or 'q' or '/' For Quontient Upon Division : \n");
        printf("'R' or 'r' or '%' For Reminder Upon Division : \n");

        printf("\nEnter Option : ");
        option_devision = getch();
        switch (option_devision)
        {
            case 'Q':
            case 'q':
            case '/':
            if(a >= b)
            {
                result = a / b;
                printf("Division of A = %d From B = %d Given Reminder %d !!!\n\n", a, b, result);
            }
            else
            {
                result = b / a;
                printf("Division of of B = %d By A = %d Given Reminder %d !!!\n\n", b, a, result);
            }  
            break;
            
            case 'R':
            case 'r':
            case '%':
            if(a >= b)
            {
                result = a % b;
                printf("Division of A = %d From B = %d Given Reminder %d !!!\n\n", a, b, result);
            }
            else
            {
                result = b % a;
                printf("Division of of B = %d By A = %d Given Reminder %d !!!\n\n", b, a, result);
            }
            break;
        
            default:
            printf("Invalid Character %c Entered Please enter again...!!!\n\n", option_devision);
            break;
        }
    break;
    default:
        printf("Invalid Character %c Entered Please enter again...!!!\n\n", option_devision);
    break;
    }

    return (0);

}
