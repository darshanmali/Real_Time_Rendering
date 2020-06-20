#include<stdio.h>

struct MyData
{
    int i_D;
    float f_D;
    double d_D;
    char c_D;
};



int main(void)
{

    struct MyData data_One_D = {3, 2.9f, 4.54764, 'D'};
    
    struct MyData data_Two_D = {'D', 66.345f, 22.54764, 69};

    struct MyData data_Tree_D = {65, 'D'};

    struct MyData data_Four_D = {76};
    
    printf("\n\n");
    printf("Data Members of 'struct MyData' are : \n\n");
    printf("I = %d\n", data_One_D.i_D);
    printf("F = %f\n", data_One_D.f_D);
    printf("D = %lf\n", data_One_D.d_D);
    printf("C = %c\n\n", data_One_D.c_D);

    printf("\n\n");
    printf("Data Members of 'struct MyData' are : \n\n");
    printf("I = %d\n", data_Two_D.i_D);
    printf("F = %f\n", data_Two_D.f_D);
    printf("D = %lf\n", data_Two_D.d_D);
    printf("C = %c\n\n", data_Two_D.c_D);

    printf("\n\n");
    printf("Data Members of 'struct MyData' are : \n\n");
    printf("I = %d\n", data_Tree_D.i_D);
    printf("F = %f\n", data_Tree_D.f_D);
    printf("D = %lf\n", data_Tree_D.d_D);
    printf("C = %c\n\n", data_Tree_D.c_D);
    
    printf("\n\n");
    printf("Data Members of 'struct MyData' are : \n\n");
    printf("I = %d\n", data_Four_D.i_D);
    printf("F = %f\n", data_Four_D.f_D);
    printf("D = %lf\n", data_Four_D.d_D);
    printf("C = %c\n\n", data_Four_D.c_D);

    return(0);

}

