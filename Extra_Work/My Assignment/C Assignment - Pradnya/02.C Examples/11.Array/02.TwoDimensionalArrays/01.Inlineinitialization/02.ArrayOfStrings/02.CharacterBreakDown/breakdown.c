#include<stdio.h>

#define MAX_STRING_LENGHT 512


int main(int argc, char *argv[], char *envp[])
{
    int MyStrlen(char[]);

    char strArray_D[10][15] = {"Hello!", "Welcome", "To", "Real", "Time","Rendering","Batch", "(2020-21)","of","ASTROMEDICOMP."};
    int char_size_D;
    int istrLengths[10];
    int strArray_size_D;
    int strArray_num_elements_D, strArray_num_rows_D, strArray_num_columns_D;
    int strActual_num_chars_D = 0;
    int i_D;
    
    
    strArray_size_D = sizeof(strArray_D);
    strArray_num_rows_D = strArray_size_D / sizeof(strArray_D[0]);

    for (i_D = 0; i_D < strArray_num_rows_D; i_D++)
        istrLengths[i_D] = MyStrlen(strArray_D[i_D]);

    printf("\n\n");
    printf("The Entire String Array : \n\n");

    for (i_D = 0; i_D < strArray_num_rows_D; i_D++)
        printf("%s ", strArray_D[i_D]);

    printf("\n\n");
    printf("string In the 2D Array : \n\n");

    for (i_D = 0; i_D < strArray_num_rows_D; i_D++)
    {
        printf("String number %d => %s \n\n", (i_D + 1),strArray_D[i_D]);
        for (int j_D = 0; j_D < istrLengths[i_D]; j_D++)
        {
            printf("Chracter %d = %c\n", (j_D + 1), strArray_D[i_D][j_D]);
        }
        printf("\n\n");
        
    }
    return(0);
    
}

int MyStrlen(char str[])
{
    int j_D;
    int string_length = 0;

    for (j_D = 0; j_D < MAX_STRING_LENGHT; j_D++)
    {
        if(str[j_D] == '\0')
            break;
        else
            string_length++;        
    }
    return(string_length);
    
}

