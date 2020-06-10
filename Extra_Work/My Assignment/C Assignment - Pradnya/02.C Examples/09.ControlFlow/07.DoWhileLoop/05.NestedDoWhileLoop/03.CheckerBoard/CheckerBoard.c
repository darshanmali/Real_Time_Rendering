#include<stdio.h>

int main(void)
{
    int i_D, j_D, c_D;

    printf("\n\n");
    i_D = 0;
    do
    {
        j_D = 0;
        do
        {
            c_D = ((i_D & 0x8) == 0) ^ ((j_D & 0x8) == 0);
            if(c_D == 0)
                printf("  ");
            
            if(c_D == 1)
                printf("* ");    

             j_D++;

        }while(j_D < 64);

        printf("\n\n");
        i_D++;

    }while(i_D < 64);
    return(0);
}