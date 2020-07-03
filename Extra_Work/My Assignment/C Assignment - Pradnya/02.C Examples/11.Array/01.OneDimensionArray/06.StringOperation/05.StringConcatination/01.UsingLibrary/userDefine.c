#include<stdio.h>

#define MAX_STRING_LENGTH 512


int main(int argc, char* argv[],char* envp[])
{
    char chArray_One_D[MAX_STRING_LENGTH],chArray_Two_D[MAX_STRING_LENGTH];
    int iStringLen_D = 0;

    printf("\n\n");
    printf("Enter First string \n");
    gets_s(chArray_One_D,MAX_STRING_LENGTH);


    printf("\n\n");
    printf("Enter Second string \n");
    gets_s(chArray_Two_D,MAX_STRING_LENGTH);


    printf("******************Before Concatination**************");

    printf("\n\n");
    printf("First String Entered By You : \n\n");
    printf("%s \n",chArray_One_D);
    

    
    printf("\n\n");
    printf("Second String Entered By You : \n\n");
    printf("%s \n",chArray_Two_D);
    
    strcat(chArray_One_D, chArray_Two_D);

    printf("******************After Concatination**************");
    
    
    printf("\n\n");
    
    printf("String One is = %s \n\n",chArray_One_D);
    
    printf("String Two is = %s \n\n",chArray_Two_D);
    return (0);    

}
