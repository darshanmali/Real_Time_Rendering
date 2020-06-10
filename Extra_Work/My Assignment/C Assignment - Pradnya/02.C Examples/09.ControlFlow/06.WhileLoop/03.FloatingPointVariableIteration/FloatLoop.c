#include<stdio.h>

int main(void)
{
    float f_D, f_Num_D = 1.7f;

    printf("\n\n");

    printf("\n\n");

    printf("Printing Number %f to %f : \n\n", f_Num_D, (f_Num_D * 10.0f));

    f_D = f_Num_D;
    while (f_D <= (f_Num_D * 10.0f))
    {
        printf("\t%f\n",f_D);
        f_D += f_Num_D;
    }
    printf("\n\n");

    return(0);
     
}