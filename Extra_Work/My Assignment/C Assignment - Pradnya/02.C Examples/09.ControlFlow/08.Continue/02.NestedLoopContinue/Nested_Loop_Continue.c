#include<stdio.h>

int main(void)
{
    int i_D, j_D;

    printf("\n\n");

    printf("Outer Loop Print Odd number From 0 to 10 : \n\n");
    printf("Outer Loop Print Even number From 0 to 10 For every  odd number : \n\n");

    for(i_D = 1; i_D <= 10; i_D++)
    {
        if(i_D % 2 != 0)
        { 
            printf("i_D = %d\n",i_D);
            printf("---------\n");
            for (j_D = 1; j_D <= 10; j_D++)
            {
                if(j_D % 2 != 0)
                {
                    printf("\t%d\n",j_D);
                }
                else
                {
                    continue;   
                }
            }
            printf("\n\n");
            
        }
        else
        {
            continue;
        }
                   
    }
    printf("\n\n");
    return(0);
}
