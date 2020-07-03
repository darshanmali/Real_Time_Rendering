#include<stdio.h>


int main(int argc, char *argv[],char *envp[])
{
    int length_D, Breath_D, Area_D;
    struct Rectangle
    {
        struct MyPoint
        {
            int x_D;
            int y_D;
            
        }point_1,point_2;

    }rect = {{2, 3},{5, 7}};

    length_D = rect.point_2.y_D - rect.point_1.y_D;
    if (length_D < 0)
        length_D = length_D * -1;

    Breath_D = rect.point_2.x_D - rect.point_1.x_D;
    if (Breath_D < 0)
        Breath_D = Breath_D * -1;

    Area_D = length_D * Breath_D;

    printf("Length of Rectangle = %d\n\n",length_D);
    printf("Breath of Rectangle = %d\n\n",Breath_D);
    printf("Area of Rectangle = %d\n\n",Area_D);

    return(0);

}


