#include<stdio.h>
#include<conio.h>

int main(void)
{
    int i_D;
    char ch_D;

    printf("\n\n");

    printf("printing Even Number from 1 to 100 For Every User Input. Existing Loop When Entered 'Q' or 'q' : \n\n");
    printf("Enter Character 'Q' or 'q' to Exit Loop : \n\n");


    for(i_D = 1; i_D <= 100; i_D++)
    {
        printf("\t %d \n", i_D);
        ch_D = getch();
        if(ch_D == 'Q' || ch_D == 'q')
        {
            break;
        }

    }
    printf("\n\n");
    return(0);
}

