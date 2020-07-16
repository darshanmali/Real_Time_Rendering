#include<stdio.h>
#include<ctype.h>

#define NAME_LENGTH 100
#define MARITAL_STATUS 10

struct MyEmployee
{
    char name[NAME_LENGTH];
    int age;
    char sex;
    float salary;
    char marital_status;
};

int main(int Argc, char *argv[], char *envp[])
{
    void MyGetString(char[], int);

    struct MyEmployee *pEmployeRecord = NULL;
    int num_employee, i_D;

    printf("\n\n");
    printf("Enter Number Of Employees Whose Details You Want to Record :");
    scanf("%d", &num_employee);

    printf("\n\n");
    pEmployeRecord = (struct MyEmployee *)malloc(sizeof(struct MyEmployee) * num_employee);
    if (pEmployeRecord == NULL)
    {
        printf("Failed to Alllocate Memory for 'struct MyEmployee'  !!! Exitting Now...\n\n");
        exit(0);
    }
    else
        printf("succesfully Allocated memory to 'struct MyEmployee' !!!\n\n");    

    for (i_D = 0; i_D < num_employee; i_D++)
    {
        printf("\n\n");
        printf("Enter the Data to store %d Employee Record : \n",(i_D + 1));

        printf("\n\n");
        printf("Enter Employee Name : ");
        MyGetString(pEmployeRecord[i_D].name, NAME_LENGTH);

        printf("\n\n");
        printf("Enter Employee's Age (in years) : ");
        scanf("%d", &pEmployeRecord[i_D].age);    

        printf("\n\n");
        printf("Enter Employee's Sex (M/m for male, F/f for Female) : ");
        pEmployeRecord[i_D].sex = getch();
        printf("%c",pEmployeRecord[i_D].sex);
        pEmployeRecord[i_D].sex = toupper(pEmployeRecord[i_D].sex);
        
        printf("\n\n");
        printf("Enter the Employee's Salary (in india Rupees) : ");
        scanf("%f", &pEmployeRecord[i_D].salary);

        printf("\n\n");
        printf("Is the employee Maried? (Y/y for yes, N/n For No) : ");
        pEmployeRecord[i_D].marital_status = getch();
        printf("%c", pEmployeRecord[i_D].marital_status);
        pEmployeRecord[i_D].marital_status = toupper(pEmployeRecord[i_D].marital_status);

    }
    
    printf("\n\n");
    printf("Display the Record :\n\n");
    for (i_D = 0; i_D < num_employee; i_D++)
    {
        printf("***********************Employee Number %d *******************\n\n", (i_D + 1));
        printf("Name            : %s \n",pEmployeRecord[i_D].name);
        printf("Age             : %d \n",pEmployeRecord[i_D].age);
        if ( pEmployeRecord[i_D].sex == 'M' )
            printf("Sex             : Male\n");
        else if ( pEmployeRecord[i_D].sex == 'F' )
           printf("Sex             : Female\n");
        else
            printf("Sex             : Invalid Data Emtered\n");
        
        printf("Salary            : Rs.%f \n",pEmployeRecord[i_D].salary);

        if ( pEmployeRecord[i_D].marital_status == 'Y' )
            printf("Maried status             : Married\n");
        else if ( pEmployeRecord[i_D].marital_status == 'N' )
            printf("Maried status             : Unmarried\n");
        else
            printf("Maried status             : Invalid Data Emtered\n");
        
        printf("\n");

    }
    
    if (pEmployeRecord)
    {
        free(pEmployeRecord);
        pEmployeRecord = NULL;
        printf("Meemory Allocated To %d Employee By you is SuccessFully freed..!!!!\n\n", num_employee);
    }
    
    return(0);
    
}


void MyGetString(char str[], int str_size)
{
    int i_D;
    char ch_D = '\0';

    i_D = 0;

    do
    {
        ch_D = getch();
        str[i_D] = ch_D;
        printf("%c", str[i_D]);
        i_D++;
    } while ((ch_D != '\r') && (i_D < str_size));

    if (i_D == str_size)
        str[i_D - 1] = '\0';
    else
        str[i_D] = '\0';    
    
}
