#include<stdio.h>
int main(void)
{
    float f_D;
    float f_num_D = 1.7f;

    printf("\n\n");

    printf("printing Number %f to %f :\n\n",f_num_D,(f_num_D * 10.0f));
    
    for(f_D = f_num_D; f_D <= (f_num_D * 10.0f); f_D = f_D +f_num_D)
    {
        printf("\t %f \n", f_D);
    }

    printf("\n\n");

    return (0);
}
