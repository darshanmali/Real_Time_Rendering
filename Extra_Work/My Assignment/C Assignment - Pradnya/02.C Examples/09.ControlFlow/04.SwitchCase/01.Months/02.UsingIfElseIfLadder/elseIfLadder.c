#include<stdio.h>
int main(void)
{
    int Month_num;
    
    printf("\n\n");

    printf("Enter Number of month (1 to 12) : ");
    scanf("%d", &Month_num);

        if(Month_num == 1)
            printf("Month Number %d Is January !!\n\n", Month_num);

        else if(Month_num == 2)
            printf("Month Number %d Is Frebruary !!\n\n", Month_num);


        else if(Month_num == 3)
            printf("Month Number %d Is March !!\n\n", Month_num);

        else if(Month_num == 4)
            printf("Month Number %d Is April !!\n\n", Month_num);

        else if(Month_num == 5)
            printf("Month Number %d Is May !!\n\n", Month_num);

        else if(Month_num == 6)
            printf("Month Number %d Is June !!\n\n", Month_num);

        else if(Month_num == 7)
            printf("Month Number %d Is July !!\n\n", Month_num);

        else if(Month_num == 8)
            printf("Month Number %d Is August !!\n\n", Month_num);

         else if(Month_num == 9)
            printf("Month Number %d Is September !!\n\n", Month_num);

        else if(Month_num == 10)
            printf("Month Number %d Is October !!\n\n", Month_num);

        else if(Month_num == 11)
            printf("Month Number %d Is November !!\n\n", Month_num);

        else if(Month_num == 12)
            printf("Month Number %d Is December !!\n\n", Month_num);
        
        else
        printf("Invalid Month Number %d Entered, Please Try Again. !\n\n", Month_num);

    printf("if Else If - Else Ladder Complete !!!\n\n");

    return(0);
}