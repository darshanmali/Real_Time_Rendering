#include<stdio.h>

int main(void)
{
    char option_D, ch_D = '\0';

    printf("\n\n");
    printf("Once The Infinite Loop Begink, Enter 'Q' or 'q' To Exit : \n\n");

        do
        {
            do
            {
                printf("\n");
                printf("In Loop...\n");
                ch_D = getch();
            } while (ch_D != 'Q' && ch_D !='q');

            printf("\n");

        printf("Exiting the Loop...!!!!!");

        printf("\n\n");
        printf("Do ypu want to begin User 'Y' or 'y' to Initiate User Controlled Loop Again? ....(Y/y yes, Any Other Key - No) : \n\n");
        option_D = getch();

       }while (option_D == 'Y' || option_D =='y');

    return(0);
    
}

