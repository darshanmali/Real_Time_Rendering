#include<stdio.h>
#include<stdlib.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    char* ReplaceVovelsWithHashSymbol(char *);

    char String_D[MAX_STRING_LENGTH];
    char *Replaced_String_D = NULL;

    printf("\n\n");
    printf("Enter String : ");
    gets_s(String_D, MAX_STRING_LENGTH);
    
    Replaced_String_D = ReplaceVovelsWithHashSymbol(String_D);
    if ( Replaced_String_D == NULL )
    {
        printf("ReplaceVovelsWithHashSymbol() Funtion Has Failed!!\n\n");
        exit(0);
    }

    printf("\n\n");
    printf("Replace String  IS : \n\n");
    printf("%s\n\n",Replaced_String_D);

    if (Replaced_String_D)
    {
        free(Replaced_String_D);
        Replaced_String_D = NULL;
    }

    return(0);
    
}

char* ReplaceVovelsWithHashSymbol(char *str)
{
    void MyStrCpy(char *, char *);
    int Mystrlen(char *);

    char *new_string = NULL;
    int i_D;

    new_string = (char *)malloc( Mystrlen(str) * sizeof(char));
    if (new_string  == NULL )
    {
        printf("Allocation of Memory hs Faild !!\n\n");
        return(NULL);
    }
    MyStrCpy(new_string, str);
    for (i_D = 0; i_D < Mystrlen(new_string); i_D++)
    {
        switch (new_string[i_D])
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
            new_string[i_D] = '#';
            break;
        default:
            break;
        }
    }
    return(new_string);    
}

void MyStrCpy(char *strDes, char  *strSource)
{
    int Mystrlen(char *);

    int iStrLength = 0;
    int j_D;

    iStrLength = Mystrlen(strSource);
    for (j_D = 0; j_D < iStrLength; j_D++)
    {
        *(strDes + j_D) = *(strSource + j_D);
    }
    *(strDes + j_D) = '\0';
    
}

int Mystrlen(char *str)
{
    int j_D;
    int string_length = 0;
    for (j_D = 0; j_D < MAX_STRING_LENGTH; j_D++)
    {
        if (*(str + j_D) == '\0')
            break;
        else
            string_length++;       
    }
    return(string_length);
    
}
