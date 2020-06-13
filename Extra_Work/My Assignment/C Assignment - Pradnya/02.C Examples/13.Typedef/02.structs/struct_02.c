#include<stdio.h>

#define MAX_NAME_LENGTH 100

struct Employee
{
    char name_D[MAX_NAME_LENGTH];
    unsigned int age_D;
    char gender_D;
    double salary_D;
};

struct MyData
{
    int i_D;
    float f_D;
    double d_D;
    char c_D;

};

int main(void)
{
    typedef struct Employee MY_EMPLOYEE_TYPE;
    typedef struct MyData MY_DATA_TYPE;

    struct Employee Emp = {"Funny", 25, 'M',500000.00};
    MY_EMPLOYEE_TYPE Emp_typedef = {"Bunny", 23, 'F',20000.00};

    struct MyData md = {30, 11.45f, 34.8907, 'X'};
    MY_DATA_TYPE md_typedef;

    md_typedef.i_D = 9;
    md_typedef.f_D = 1.5f;
    md_typedef.d_D = 8.5454564;
    md_typedef.c_D = 'P';

    printf("\n\n");
    printf("struct Employee : \n\n");
    printf("emp.name = %s\n", Emp.name_D);
    printf("emp.age = %d\n", Emp.age_D);
    printf("emp.gender = %c\n", Emp.gender_D);
    printf("emp.salary = %lf\n", Emp.salary_D);


    printf("\n\n");
    printf("struct MY_EMPLOYEE_TYPE : \n\n");
    printf("emp_typedef.name = %s\n", Emp_typedef.name_D);
    printf("emp_typedef.age = %d\n", Emp_typedef.age_D);
    printf("emp_typedef.gender = %c\n", Emp_typedef.gender_D);
    printf("emp_typedef.salary = %lf\n", Emp_typedef.salary_D);

    printf("\n\n");
    printf("struct MyData : \n\n");
    printf("md.i_D = %d\n", md.i_D);
    printf("md.f_D = %f\n", md.f_D);
    printf("md.d_D = %lf\n", md.d_D);
    printf("md.c_D = %c\n", md.c_D);

    printf("\n\n");
    printf("struct MY_DATA_TYPE : \n\n");
    printf("md_typedef.i_D = %d\n", md_typedef.i_D);
    printf("md_typedef.f_D = %f\n", md_typedef.f_D);
    printf("md_typedef.d_D = %lf\n", md_typedef.d_D);
    printf("md_typedef.c_D = %c\n", md_typedef.c_D);

    printf("\n\n");

    return(0);
    
}


