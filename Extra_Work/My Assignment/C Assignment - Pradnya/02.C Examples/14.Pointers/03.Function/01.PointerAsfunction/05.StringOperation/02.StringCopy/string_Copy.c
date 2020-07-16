#include<stdio.h>
#include<stdlib.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    int Mystrlen(char *);
    void MyStrCpy(char *, char *);
    
    char *chArray_Original = NULL;
    char *chArray_Copy = NULL;

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
    chArray_Copy = (char *)malloc(Original_stringlenght * sizeof(char));
    if (chArray_Copy == NULL)
    {
        printf("Memory Could Not Allocated . Exitting Now...\n\n");
        exit(0);
    }
    
    MyStrCpy( chArray_Copy ,chArray_Original);

    printf("\n\n");
    printf("The Oroginal String Entered by You Is : \n\n");
    printf("%s\n", chArray_Original);

    printf("\n\n");
    printf("The Copied String Is : \n\n");
    printf("%s\n", chArray_Copy);


    if (chArray_Copy)
    {
        free(chArray_Copy);
        chArray_Copy = NULL;
        printf("\n\n");
        printf("Memory Allocation Of Copied string Is Freed, Now SuccessFull exiting.....!!!\n\n");
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
