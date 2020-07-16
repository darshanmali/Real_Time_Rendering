#include<stdio.h>
#include<stdlib.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    void MyStrCat(char *, char *);
    int Mystrlen(char *);
    
    char *chArray_First = NULL;
    char *chArray_Second = NULL;

    int Original_stringlenght = 0;

    printf("\n\n");
    chArray_First = (char *)malloc(MAX_STRING_LENGTH * sizeof(char));
    if (chArray_First == NULL)
    {
        printf("Memory Could Not Allocated . Exitting Now...\n\n");
        exit(0);
    }
    printf("Enter First String : \n\n");
    gets_s(chArray_First, MAX_STRING_LENGTH);

    printf("\n\n");
    chArray_Second = (char *)malloc(Original_stringlenght * sizeof(char));
    if (chArray_Second == NULL)
    {
        printf("Memory Could Not Allocated . Exitting Now...\n\n");
        exit(0);
    }
    printf("Enter Second String : \n\n");
    gets_s(chArray_Second, MAX_STRING_LENGTH);
    
    printf("Before Concatinating : ");

    printf("\n\n");
    printf("The First String Entered by You Is : \n\n");
    printf("%s\n", chArray_First);

    printf("\n\n");
    printf("The Second String Entered By you Is : \n\n");
    printf("%s\n", chArray_Second);


    MyStrCat( chArray_First ,chArray_Second);

    
    printf("\n\n");
    printf("The First String Entered by You Is : \n\n");
    printf("%s\n", chArray_First);

    printf("\n\n");
    printf("The Second String Entered By you Is : \n\n");
    printf("%s\n", chArray_Second);


    if (chArray_Second)
    {
        free(chArray_Second);
        chArray_Second = NULL;
        printf("\n\n");
        printf("Memory Allocation Of chArray Second string Is Freed, Now SuccessFull exiting.....!!!\n\n");
    }
    
    if (chArray_First)
    {
        free(chArray_First);
        chArray_First = NULL;
        printf("\n\n");
        printf("Memory Allocation Of chArray First string Is Freed, Now SuccessFull exiting.....!!!\n\n");
    }
    return(0);
    
}

void MyStrCat(char *strDes, char  *strSource)
{
    int Mystrlen(char *);

    int iStrLength_Source = 0, istringLength_Destination = 0;
    int i_D, j_D;

    iStrLength_Source = Mystrlen(strSource);
    istringLength_Destination = Mystrlen(strDes);

    for (i_D = istringLength_Destination, j_D = 0; j_D < iStrLength_Source; i_D++, j_D++)
    {
        *(strDes + i_D) = *(strSource + j_D);
    }
    *(strDes + i_D) = '\0';
    
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
