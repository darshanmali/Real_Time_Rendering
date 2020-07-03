#include<stdio.h>

struct  Employee
{
    char Name_D[100];
    int age_D;
    float salary_D;
    char sex_D;
    char marital_status_D;
};

int main(void)
{
    printf("\n\n");
    printf("size Of Data And there Size : \n\n");

    printf("size Of (int)               :%d \t\t\t size Of Pointer To int (int*)                : %d\n\n",sizeof(int), sizeof(int *));
    printf("size Of (Float)             :%d \t\t\t size Of Pointer To int (float*)              : %d\n\n",sizeof(float), sizeof(float *));
    printf("size Of (Double)            :%d \t\t\t size Of Pointer To int (double*)             : %d\n\n",sizeof(double), sizeof(double *));
    printf("size Of (char)              :%d \t\t\t size Of Pointer To int (char*)               : %d\n\n",sizeof(double), sizeof(double *));
    printf("size Of (Struct Employee)   :%d \t\t\t size Of Pointer To int (strut Employee*)     : %d\n\n",sizeof(struct Employee), sizeof(struct Employee *));

    return(0);


}

