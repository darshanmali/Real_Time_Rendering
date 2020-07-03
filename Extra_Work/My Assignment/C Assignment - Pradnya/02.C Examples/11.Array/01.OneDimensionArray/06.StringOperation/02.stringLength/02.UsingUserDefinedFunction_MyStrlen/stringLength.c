#include<stdio.h>

#define MAX_STRING_LENGTH 512


int main(int argc, char* argv[],char* envp[])
{

    int Mystringlen(char[]);

    char chArray_D[MAX_STRING_LENGTH];
    int iStringLen_D = 0;

    printf("\n\n");
    printf("Enter A string \n");
    gets_s(chArray_D,MAX_STRING_LENGTH);

    printf("\n\n");
    printf("String Entered By You : \n\n");
    printf("%s \n",chArray_D);  
    
    printf("\n\n");
    iStringLen_D = Mystringlen(chArray_D);
    printf("Length of String is = %d character !!!\n\n",iStringLen_D);
    
    return (0);    

}

int Mystringlen(char str[])
{
    int j_D;
    int string_length = 0;
    for (j_D = 0; j_D < MAX_STRING_LENGTH; j_D++)
    {
        if(str[j_D] == '\0')
            break;
        
        else 
            string_length++;
            

    }
    return (string_length);
            
}

