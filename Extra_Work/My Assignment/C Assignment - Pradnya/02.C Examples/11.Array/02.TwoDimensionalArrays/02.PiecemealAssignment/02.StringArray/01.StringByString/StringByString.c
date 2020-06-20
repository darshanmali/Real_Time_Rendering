#include<stdio.h>

#define MAX_STRING_LENGTH 512

int main(int argc, char *argv[], char *envp[])
{
    void Mystrcpy(char[], char[]);

    char strArray_D[5][10];

    int char_size_D;
    int strArray_size_D;
    int strArray_num_elements_D, strArray_num_row_D, strArray_num_columns_D;
    int i_D;

    printf("\n\n");
    char_size_D = sizeof(char);

    strArray_size_D = sizeof(strArray_D);
    printf("size of Two Dimension ( 2D ) character Array (String Array) Is = %d\n\n",strArray_size_D);

    strArray_num_row_D = strArray_size_D / sizeof(strArray_D[0]);
    printf("number of Rows (strings) in Two Dimensional (2D) character Array (string Array) Is = %d \n\n", strArray_num_row_D);

    strArray_num_columns_D = sizeof(strArray_D[0]) / char_size_D;
    printf("number of Column in Two Dimensional (2D) character Array (string Array) Is = %d \n\n", strArray_num_columns_D);

    strArray_num_elements_D = strArray_num_row_D * strArray_num_columns_D;
    printf("maximum Number of elements ( characters ) In Two Dimensional (2D) character Array (string Array) Is = %d \n\n", strArray_num_elements_D);

    Mystrcpy(strArray_D[0], "My");
    Mystrcpy(strArray_D[1], "Name");
    Mystrcpy(strArray_D[2], "Is");
    Mystrcpy(strArray_D[3], "Darshan");
    Mystrcpy(strArray_D[4], "Mali");
    
    printf("\n\n");
    printf("The String In the 2D Character Array Are : \n\n");
    
    for (i_D = 0; i_D < strArray_num_row_D; i_D++)
    {
        printf("%s ", strArray_D[i_D]);
    }

    printf("\n\n");

    return (0);
    
}

void Mystrcpy(char str_destination[], char str_source[])
{
        int Mystrlen(char[]);

        int istringlength_D = 0;
        int j_D;

        istringlength_D = Mystrlen(str_source);
        for (j_D = 0; j_D < istringlength_D; j_D++)
            str_destination[j_D] = str_source[j_D];            

        str_destination[j_D] = '\0';        

}

int Mystrlen(char str[])
{
    int j_D;
    int String_length = 0;

    for (j_D = 0; j_D < MAX_STRING_LENGTH; j_D++)
    {
        if(str[j_D] == '\0')
            break;
        else
            String_length++;
    }
    return (String_length);
    
}

