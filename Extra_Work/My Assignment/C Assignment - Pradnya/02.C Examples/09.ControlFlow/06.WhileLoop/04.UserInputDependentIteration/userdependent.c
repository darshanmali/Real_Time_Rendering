#include<stdio.h>

int main(void)
{
    int i_Num_D,Num_D,i_D;

    printf("\n\n");

    printf("Enter An Integer value from which Iteration must Begin : ");
    scanf("%d", &i_Num_D);

    printf("How many DIgit Do you want to Print From %d Onwords ? : ", i_Num_D);
    scanf("%d", &Num_D);

    printf("printing Digit %d to %d : \n\n", i_Num_D, (i_Num_D + Num_D));

    i_D = i_Num_D;

    while (i_D <= (i_Num_D + Num_D))
    {
        printf("\t%d \n", i_D);
        i_D++;
    }
    return(0);
 
}


