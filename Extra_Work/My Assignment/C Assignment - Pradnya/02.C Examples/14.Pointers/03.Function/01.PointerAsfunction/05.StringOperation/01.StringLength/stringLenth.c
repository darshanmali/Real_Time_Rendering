#include<stdio.h>
#include<stdlib.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    int Mystrlen(char *);
    char *chArray = NULL;

    int istringlenght = 0;

    printf("\n\n");
    chArray = (char *)malloc(MAX_STRING_LENGTH * sizeof(char));
    if (chArray == NULL)
    {
        printf("Memory Could Not Allocated . Exitting Now...\n\n");
        exit(0);
    }
    printf("Enter A String : \n\n");
    gets_s(chArray, MAX_STRING_LENGTH);

    printf("\n\n");
    printf("String Entered by You Is : \n\n");
    printf("%s\n", chArray);
    
    printf("\n\n");
    istringlenght = Mystrlen(chArray);
    printf("Length of String Is :%d \n\n", istringlenght);

    if (chArray)
    {
        free(chArray);
        chArray = NULL;
    }
    return(0);
    
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
