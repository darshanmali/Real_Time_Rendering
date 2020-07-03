#include<stdio.h>
#include<string.h>


#define MAX_STRING_LENGTH 512


int main(int argc, char* argv[],char* envp[])
{

    char chArray_Original_D[MAX_STRING_LENGTH];

    printf("\n\n");
    printf("Enter A string \n");
    gets_s(chArray_Original_D,MAX_STRING_LENGTH);

    printf("\n\n");
    printf("String Entered By You : \n\n");
    printf("%s \n",chArray_Original_D);
    
    printf("\n\n");
    
    printf("the Reversed String is = %s \n\n",strrev(chArray_Original_D));
    
    return (0);    

}


