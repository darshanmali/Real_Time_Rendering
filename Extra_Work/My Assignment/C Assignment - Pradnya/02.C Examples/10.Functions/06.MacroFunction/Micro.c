#include<stdio.h>

#define MAX_NUMBER(a, b) ((a > b) ? a : b)
int main(int argc, char *argv[],char *envp[])
{
    int iNum_01_D;
    int iNum_02_D;
    int iResult_D;

    float fNum_01_D;
    float fNum_02_D;
    float fresult_D;

    printf("\n\n");
    printf("Enter Value for Integer Number : \n\n");
    scanf("%d",&iNum_01_D);

    printf("\n\n");
    printf("Enter Value for Integer Number : \n\n");
    scanf("%d",&iNum_02_D);

    iResult_D = MAX_NUMBER(iNum_01_D,iNum_02_D);
    printf("\n\n"); 
    printf("Result Of Micro Function Max_number() = %d\n", iResult_D);
    printf("\n\n"); 

    // Floating point
    printf("\n\n");
    printf("Enter Value for Float Number : \n\n");
    scanf("%f", &fNum_01_D);

    printf("\n\n");
    printf("Enter Value for Float Number : \n\n");
    scanf("%f", &fNum_02_D);

    fresult_D = MAX_NUMBER(fNum_01_D, fNum_02_D);
    printf("\n\n"); 
    printf("Result Of Micro Function Max_number() = %f\n", fresult_D);
    printf("\n\n"); 
    
    return(0);
    
}

