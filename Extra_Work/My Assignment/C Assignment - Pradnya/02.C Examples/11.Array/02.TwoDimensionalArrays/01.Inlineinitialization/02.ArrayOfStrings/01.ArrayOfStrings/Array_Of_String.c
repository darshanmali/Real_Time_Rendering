#include<stdio.h>

#define MAX_STRING_LENGHT 512


int main(int argc, char *argv[], char *envp[])
{
    int Mystrlen(char[]);

    char strArray_D[10][15] = {"Hello!", "Welcome", "To", "Real", "Time","Rendering","Batch", "(2020-21)","of","ASTROMEDICOMP."};
    int char_size_D;
    int strArray_size_D;
    int strArray_num_elements_D, strArray_num_rows_D, strArray_num_columns_D;
    int strActual_num_chars_D = 0;
    int i_D;
    
    
    printf("\n\n");

    char_size_D = sizeof(char);

    strArray_size_D = sizeof(strArray_D);
    printf("Size of two dimensional (2D) Character Array (Sting Array) Is = %d\n\n",strArray_size_D);

    strArray_num_rows_D = strArray_size_D / sizeof(strArray_D[0]);
    printf("Number of row(string) In Two dimensional (2D) Character Array (Sting Array) Is = %d\n\n",strArray_num_rows_D);
    
    strArray_num_columns_D = sizeof(strArray_D[0]) / char_size_D;
    printf("Number of row(string) In Two dimensional (2D) Character Array (Sting Array) Is = %d\n\n",strArray_num_columns_D);

    strArray_num_elements_D = strArray_num_rows_D * strArray_num_columns_D;
    printf("Maximum Number Of Elements( Characters ) In Two dimensional (2D) Character Array (Sting Array) Is = %d\n\n",strArray_num_elements_D);
    
    for (i_D = 0; i_D < strArray_num_rows_D; i_D++)
    {
        strActual_num_chars_D = strActual_num_chars_D + MyStrlen(strArray_D[i_D]);
    }

    printf("Actual Number Of elements (characters) In Two Dimenrsional ( 2D ) charaters Array (string Array ) Is = %d\n\n",strActual_num_chars_D);
    printf("\n\n");
    printf("String In The 2D Array: \n\n");

    printf("%s ", strArray_D[0]);
    printf("%s ", strArray_D[1]);
    printf("%s ", strArray_D[2]);
    printf("%s ", strArray_D[3]);
    printf("%s ", strArray_D[4]);
    printf("%s ", strArray_D[5]);
    printf("%s ", strArray_D[6]);
    printf("%s ", strArray_D[7]);
    printf("%s ", strArray_D[8]);
    printf("%s\n\n", strArray_D[9]);

    return(0);

}

int MyStrlen(char str[])
{
    int j_D;
    int string_length = 0;

    for (j_D = 0; j_D < MAX_STRING_LENGHT; j_D++)
    {
        if(str[j_D] == '/0')
            break;
        else
            string_length++;        
    }
    return(string_length);
    
}

