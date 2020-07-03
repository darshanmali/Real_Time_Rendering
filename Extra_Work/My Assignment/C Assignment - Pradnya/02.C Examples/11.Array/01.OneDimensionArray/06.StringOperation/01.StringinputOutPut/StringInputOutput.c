#include<stdio.h>

#define MAX_STRING_LENGTH 512


int main(int argc, char* argv[],char* envp[])
{
    char chArray_D[MAX_STRING_LENGTH];

    printf("\n\n");
    printf("Enter A string \n");
    gets_s(chArray_D,MAX_STRING_LENGTH);

    printf("\n\n");
    printf("String Entered By You : \n\n");
    printf("%s \n",chArray_D);
    return (0);    



}


