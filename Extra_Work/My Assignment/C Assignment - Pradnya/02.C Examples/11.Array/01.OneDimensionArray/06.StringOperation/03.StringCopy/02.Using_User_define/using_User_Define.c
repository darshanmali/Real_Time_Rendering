#include<stdio.h>

#define MAX_STRING_LENGTH 512


int main(int argc, char* argv[],char* envp[])
{

    void Mystringcopy(char[],char[]);

    char chArray_Original_D[MAX_STRING_LENGTH], chArray_Copy_D[MAX_STRING_LENGTH];
    
    printf("\n\n");
    printf("Enter A string \n");
    gets_s(chArray_Original_D,MAX_STRING_LENGTH);

    Mystringcopy(chArray_Copy_D, chArray_Original_D);

    printf("\n\n");
    printf("String Entered By You : \n\n");
    printf("%s \n",chArray_Original_D);
    
    printf("\n\n");
    
    printf("Copied String is = %s \n\n",chArray_Copy_D);
    
    return (0);    

}


void Mystringcopy(char str_dest[], char str_source[])
{
    int MyStrlen(char[]);

    int string_length = 0;
    int j_D;

    string_length = MyStrlen(str_source);
    for (j_D = 0; j_D < string_length; j_D++)
    {
        str_dest[j_D] = str_source[j_D];
    }
    str_source[j_D] = '\0';
    
            
}


int MyStrlen(char str[])
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
