#include<stdio.h>

int main(void)
{
    char option_D, ch_D = '\0';

    printf("\n\n");
    printf("Once The Infinite Loop Begink, Enter 'Q' or 'q' To Exit : \n\n");
    printf("Enter 'Y' or 'y' to Initiate User Controlled Loop : \n\n");
    option_D = getch();

    
    if(option_D == 'Y' || option_D =='y')
    {
        while (1)
        {
            printf("In Loop...\n");
            ch_D = getch();
            if(ch_D == 'Q' || ch_D =='q')
                break;
        }

        printf("\n\n");

        printf("Exiting the Loop...!!!!!");

        printf("\n\n");
    }

    else
    {
        printf("you must try y to initiate loop, Please Try Again!");

    }

    return(0);
    
}


