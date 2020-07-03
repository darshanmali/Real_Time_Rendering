#include<stdio.h>

#define MAX_STRING_LENGTH 512


int main(int argc, char* argv[],char* envp[])
{

    void Mystringrev(char[], char[]);

    char chArray_Original_D[MAX_STRING_LENGTH], chArray_Reversed_D[MAX_STRING_LENGTH];

    printf("\n\n");
    printf("Enter A string \n");
    gets_s(chArray_Original_D,MAX_STRING_LENGTH);

    Mystringrev(chArray_Reversed_D,chArray_Original_D);
    
    
    printf("\n\n");
    printf("The Original String is = %s  \n\n",chArray_Original_D);

    printf("\n\n");
    printf("The Reversed String is = %s  \n\n",chArray_Reversed_D);

    
    return (0);    

}
void Mystringrev(char str_Destinaton[],char str_source[])
{
    int Mystrlen(char[]);

    int iStringLength = 0;
    int i_D, j_D, len_D;    
    iStringLength = Mystrlen(str_source);
    
    len_D = iStringLength - 1;
    
    for (i_D = 0, j_D = len_D; i_D < iStringLength ,j_D >= 0; i_D++, j_D--)
    {
        str_Destinaton[i_D] = str_source[j_D]; 

    }
    str_Destinaton[i_D] = '\0'; 

}


int Mystrlen(char str[])
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

