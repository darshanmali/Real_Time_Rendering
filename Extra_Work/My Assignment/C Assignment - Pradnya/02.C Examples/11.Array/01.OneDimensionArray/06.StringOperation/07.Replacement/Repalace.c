#include<stdio.h>


#define MAX_STRING_LENGTH 512


int main(int argc, char* argv[],char* envp[])
{

    void Mystringcopy(char[],char[]);
    int Mystringlen(char[]);


     char chArray_Original_D[MAX_STRING_LENGTH], chArray_Replacement_D[MAX_STRING_LENGTH];

    int istrLen;
    int i_D;

    

    printf("\n\n");

    printf("Enter String : ");
    gets_s(chArray_Original_D,MAX_STRING_LENGTH);

    Mystringcopy(chArray_Replacement_D , chArray_Original_D);

    istrLen = Mystringlen(chArray_Replacement_D);


    for (i_D = 0; i_D < istrLen; i_D++)
    {
        switch (chArray_Replacement_D[i_D])
        {
            case 'A':
            case 'a':
            case 'E':
            case 'e':
            case 'I':
            case 'i':
            case 'O':
            case 'o':
            case 'U':
            case 'u':
                chArray_Replacement_D[i_D] = '*';
            break;

            default:
            break;
            
        }
    }
    
    printf("\n\n");
    printf("The String Enter By you : %s\n\n", chArray_Original_D);

    printf("\n\n");
    printf("The String Afer Replacement Of : %s\n\n", chArray_Replacement_D);

    
    return(0);
}

void Mystringcopy(char str_des[],char str_source[])
{
   int Mystringlen(char[]);

   int iStringLength = 0;
    int j_D;    
    iStringLength = Mystringlen(str_source);
    
    for (j_D = 0; j_D < iStringLength; j_D++)
    {
        str_des[j_D] = str_source[j_D]; 

    }
    str_des[j_D] = '\0'; 

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

