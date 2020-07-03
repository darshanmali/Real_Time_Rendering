#include<stdio.h>

#define MAX_STRING_LENGTH 512


int main(int argc, char* argv[],char* envp[])
{
    char chArray_copy_D[MAX_STRING_LENGTH], chArray_Original_D[MAX_STRING_LENGTH];
    int iStringLen_D = 0;

    printf("\n\n");
    printf("Enter A string \n");
    gets_s(chArray_Original_D,MAX_STRING_LENGTH);

    strcpy(chArray_copy_D,chArray_Original_D);


    printf("\n\n");
    printf("The Original String Entered By You : \n\n");
    printf("%s \n",chArray_Original_D);
    
    printf("\n\n");
    
    printf("The Copied String  : \n\n");
    printf("%s \n",chArray_copy_D);
    return (0);    

}


