#include<stdio.h>

int main(void)
{
    int i_D, j_D, c_D;

    printf("\n\n");

    for (i_D = 0; i_D < 64; i_D++)
    {
        for ( j_D = 0; j_D < 64; j_D++)
        {
            c_D = ((i_D & 0x8) == 0) ^ ((j_D & 0x8) == 0);
            if(c_D == 0)
                printf("  ");
            
            if(c_D == 1)
                printf("* ");    
        }
        printf("\n\n");
        
    }
    return(0);
}