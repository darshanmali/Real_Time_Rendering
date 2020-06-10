#include<stdio.h>
int main(void)
{
    char option_D, ch_D = '\0';

    printf("\n\n");

    printf(" Once The Infinite loop Begins, Enter 'Q' or 'q' to Quit the enfinite for loop : \n\n");
    printf("Enter 'Y' or 'y' to Infinity User Cotrolled Infinity Loop : ");
    printf("\n\n");

    option_D = getch();
    if(option_D == 'Y' || option_D == 'y')
    {
        for(;;)
        {
            printf("In Loop...\n");
            ch_D = getch();
            if(ch_D == 'Q' || ch_D == 'q')
                break;
        }
    }
    printf("\n\n");
    printf("Exiting User controlled Infinite Loop...");
    printf("\n\n");

    return(0);

}
