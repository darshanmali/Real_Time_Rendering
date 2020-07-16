#include<stdio.h>
#include<stdlib.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    void MyStrRev(char *, char *);
    int Mystrlen(char *);
    
    char *chArray_Original = NULL;
    char *chArray_Reverse = NULL;

    int Original_stringlenght = 0;

    printf("\n\n");
    chArray_Original = (char *)malloc(MAX_STRING_LENGTH * sizeof(char));
    if (chArray_Original == NULL)
    {
        printf("Memory Could Not Allocated . Exitting Now...\n\n");
        exit(0);
    }
    printf("Enter A String : \n\n");
    gets_s(chArray_Original, MAX_STRING_LENGTH);

    printf("\n\n");
    Original_stringlenght = Mystrlen(chArray_Original);
    chArray_Reverse = (char *)malloc(Original_stringlenght * sizeof(char));
    if (chArray_Reverse == NULL)
    {
        printf("Memory Could Not Allocated . Exitting Now...\n\n");
        exit(0);
    }
    
    MyStrRev( chArray_Reverse ,chArray_Original);

    printf("\n\n");
    printf("The Oroginal String Entered by You Is : \n\n");
    printf("%s\n", chArray_Original);

    printf("\n\n");
    printf("The Copied String Is : \n\n");
    printf("%s\n", chArray_Reverse);


    if (chArray_Reverse)
    {
        free(chArray_Reverse);
        chArray_Reverse = NULL;
        printf("\n\n");
        printf("Memory Allocation Of Reverse string Is Freed, Now SuccessFull exiting.....!!!\n\n");
    }
    
    if (chArray_Original)
    {
        free(chArray_Original);
        chArray_Original = NULL;
        printf("\n\n");
        printf("Memory Allocation Of Original string Is Freed, Now SuccessFull exiting.....!!!\n\n");
    }
    return(0);
    
}

void MyStrRev(char *strRev, char  *strSource)
{
    int Mystrlen(char *);

    int iStrLength = 0;
    int i_D, j_D, len;

    iStrLength = Mystrlen(strSource);

    len = iStrLength - 1;

    for (i_D = 0, j_D = len; i_D < iStrLength, j_D >= 0; i_D++, j_D--)
    {
        *(strRev + i_D) = *(strSource + j_D);
    }
    *(strRev + i_D) = '\0';
    
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
