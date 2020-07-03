#include<stdio.h>

#define NUM_EMPLOYEES 5
#define NAME_LENGTH 100
#define MARITAL_STATUS 10

struct Employee
{
    char name_D[NAME_LENGTH];
    int age_D;
    char sex_D;
    float salary_D;
    char mariage_status;
};

int main(int argc, char *argv[],char *envp[])
{
    void MyGetString(char[], int);

    struct Employee EmployeeRecord[NUM_EMPLOYEES];

    int i_D;

    for (i_D = 0; i_D < NUM_EMPLOYEES; i_D++)
    {
        printf("\n\n");
        printf("********** Data Entry for Employee numer %d *****\n",(i_D +1));

        printf("\n\n");
        printf("Enter Employee Name : ");
        MyGetString(EmployeeRecord[i_D].name_D, NAME_LENGTH);

        
        printf("\n\n");
        printf("Enter Employee Age (in years) : ");
        scanf("%d",&EmployeeRecord[i_D].age_D);

        printf("\n\n");
        printf("Enter Employee Sex (M/m For Male, F/f for Female) : ");
        EmployeeRecord[i_D].sex_D = getch();
        printf("%c",EmployeeRecord[i_D].sex_D);
        EmployeeRecord[i_D].sex_D = toupper(EmployeeRecord[i_D].sex_D);

        printf("\n\n");
        printf("Enter Employee salary (in Indian Rupees ) : ");
        scanf("%f", &EmployeeRecord[i_D].salary_D);

        printf("\n\n");
        printf("Is The Employee married? (Y/y For Yes, N/n For No) : ");
        EmployeeRecord[i_D].mariage_status = getch();
        printf("%c",EmployeeRecord[i_D].mariage_status);
        EmployeeRecord[i_D].mariage_status = toupper(EmployeeRecord[i_D].mariage_status);

    }

    printf("\n\n");
    printf("********** DISPLAYING EMPLOYEE RECORD ************************");
    for (i_D = 0; i_D < 5; i_D++)
    {
        printf("************ EMPLOYEE NUMBER %d ***********\n\n",(i_D + 1));
        printf("Name                : %s \n", EmployeeRecord[i_D].name_D);
        printf("Age                 : %d Years \n", EmployeeRecord[i_D].age_D);
        if (EmployeeRecord[i_D].sex_D == 'M')
            printf("Sex                 : Male\n");
        else if (EmployeeRecord[i_D].sex_D == 'm')
            printf("Sex                 : Female\n");
        else
        {
            printf("Invalid data Entered !\n");
        }
        
        printf("Salary              : Rs. %f \n", EmployeeRecord[i_D].salary_D);
        
        if (EmployeeRecord[i_D].mariage_status == 'Y')
            printf("marrital status     :Maried\n");    
        else if(EmployeeRecord[i_D].mariage_status == 'N')
            printf("marrital status     :Unmaried\n");    
        else
            printf("marrital status     :Invalid Input\n");    

        printf("\n\n");
    }
    return (0);   

}

void MyGetString(char str[], int str_size)
{
    int i_D;
    char ch_D ='\0';

    i_D = 0;
    do
    {
        ch_D = getch();
        str[i_D] = ch_D;
        printf("%c", str[i_D]);

    } while ((ch_D != '\r' )  && (i_D < str_size));
    
    if (i_D == str_size)
        str[i_D - 1] = '\0';
    else
        str[i_D] = '\0';
    

}


