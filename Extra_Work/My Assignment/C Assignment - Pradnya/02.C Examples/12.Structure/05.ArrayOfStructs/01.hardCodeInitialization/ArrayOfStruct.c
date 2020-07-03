#include<stdio.h>

#define NAME_LENGTH 100
#define MARITAL_STATUS 10

struct Employee
{
    char name_D[NAME_LENGTH];
    int age_D;
    float salary_D;
    char sex_D;
    char mariage_status[MARITAL_STATUS];
};

int main(int argc, char *argv[],char *envp[])
{
    struct Employee Employee_Record_D[5];
    
    char employee_Vinay[] = "Vinay";
    char employee_Sagar[] = "Sagar";
    char employee_Aditya[] = "Aditya";
    char employee_Vivek[] = "Vivel";
    char employee_Hitesh[] = "Hitesh";

    int i_D;

    //Record 1
    strcpy(Employee_Record_D[0].name_D, employee_Vinay);
    Employee_Record_D[0].age_D = 30;
    Employee_Record_D[0].sex_D = 'M';
    Employee_Record_D[0].salary_D = 50000.0f;
    strcpy(Employee_Record_D[0].mariage_status,"Unmaried");

    //Record 2
    strcpy(Employee_Record_D[1].name_D, employee_Sagar);
    Employee_Record_D[1].age_D = 27;
    Employee_Record_D[1].sex_D = 'M';
    Employee_Record_D[1].salary_D = 25000.0f;
    strcpy(Employee_Record_D[1].mariage_status,"Unmaried");

    //Record 3
    strcpy(Employee_Record_D[2].name_D, employee_Aditya);
    Employee_Record_D[2].age_D = 34;
    Employee_Record_D[2].sex_D = 'M';
    Employee_Record_D[2].salary_D = 70000.0f;
    strcpy(Employee_Record_D[2].mariage_status,"Marriad");
    
    //Record 4
    strcpy(Employee_Record_D[3].name_D, employee_Vivek);
    Employee_Record_D[3].age_D = 34;
    Employee_Record_D[3].sex_D = 'M';
    Employee_Record_D[3].salary_D = 70000.0f;
    strcpy(Employee_Record_D[3].mariage_status,"Marriad");
    
    //Record 5
    strcpy(Employee_Record_D[4].name_D, employee_Hitesh);
    Employee_Record_D[4].age_D = 21;
    Employee_Record_D[4].sex_D = 'M';
    Employee_Record_D[4].salary_D = 22000.0f;
    strcpy(Employee_Record_D[4].mariage_status,"Marriad");

    printf("\n\n");
    printf("********** DISPLAYING EMPLOYEE RECORD ************************");
    for (i_D = 0; i_D < 5; i_D++)
    {
        printf("************ EMPLOYEE NUMBER %d ***********\n\n",(i_D + 1));
        printf("Name                : %s \n", Employee_Record_D[i_D].name_D);
        printf("Age                 : %d Years \n", Employee_Record_D[i_D].age_D);
        if (Employee_Record_D[i_D].sex_D == 'M' || Employee_Record_D[i_D].sex_D == 'm')
            printf("Sex                 : Male\n");
        else
            printf("Sex                 : Female\n");
        
        printf("Salary              : Rs. %f \n", Employee_Record_D[i_D].salary_D);
        printf("marrital status     : %s  \n", Employee_Record_D[i_D].mariage_status);

        printf("\n\n");
    }
    return(0);

}

