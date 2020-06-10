#include<stdio.h>
int main(void)
{
    int i_num_D, num_D, i_D;

    printf("\n\n");

    printf("Enter an Integer value From Which Interaction Must Begin : ");
    scanf("%d", &i_num_D);

    printf("How many digit do you want to print from %d onword ? : ",i_num_D);
    scanf("%d", &num_D);

    printf("Printing Digits %d to %d : \n\n", i_num_D , (i_num_D + num_D));

    for(i_D = i_num_D; i_D <= (i_num_D + num_D); i_D++)
    {
        printf("\t%d \n",i_D);
    }
    printf("\n\n");
    
    return(0);

}

