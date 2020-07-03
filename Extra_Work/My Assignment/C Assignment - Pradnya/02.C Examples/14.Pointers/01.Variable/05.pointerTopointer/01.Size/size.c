#include<stdio.h>

struct Employee
{
    char name[100];
    int age;
    float salary;
    char sex;
    char marital_Stetus;
};

int main(void)
{
    printf("\n\n");
    printf("Size Of Data type Pointers To Those Respective data types Are : \n");
    printf("Sizeof(int )            : %d   \tsize of pointer  to int ( int*)                          : %d \tsize of pointer to pointer to int (int **)         \t%d \n\n", sizeof(int ), sizeof(int *), sizeof(int **));
    printf("Sizeof(float )          : %d   \tsize of pointer  to float ( int*)                        : %d \tsize of pointer to pointer to int (int **)         \t%d \n\n", sizeof(float ), sizeof(float *), sizeof(float **));
    printf("Sizeof(double )         : %d   \tsize of pointer  to double ( int*)                       : %d \tsize of pointer to pointer to int (int **)         \t%d \n\n", sizeof(double ), sizeof(double *), sizeof(double **));
    printf("Sizeof(char )           : %d   \tsize of pointer  to char ( int*)                         : %d \tsize of pointer to pointer to int (int **)         \t%d \n\n", sizeof(char ), sizeof(char *), sizeof(char **));
    rintf("Sizeof(Struct Employee ) : %d   \tsize of pointer  to struct Employee ( struct Employee*)  : %d \tsize of pointer to pointer to int (int **)         \t%d \n\n", sizeof(struct Employee), sizeof(struct Employee*), sizeof(struct Employee **));
    
    
    return(0);





}