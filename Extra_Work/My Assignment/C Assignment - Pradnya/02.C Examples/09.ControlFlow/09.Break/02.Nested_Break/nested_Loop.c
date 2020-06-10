#include<stdio.h>
#include<conio.h>

int main(void)
{
    int i_D, j_D;
    

    printf("\n\n");

    for(i_D = 1; i_D <= 20; i_D++)
    {
        for(j_D = 1; j_D <= 20; j_D++)
        {
            if(j_D > i_D)
            {
                break;
            }
            else
            {
                printf("* ");
            }
        }
        printf("\n");
    }
    printf("\n\n");
    return(0);
}

