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

    strArray_D[0][0] = 'M';
    strArray_D[0][1] = 'y';
    strArray_D[0][2] = '\0';

    strArray_D[1][0] = 'N';
    strArray_D[1][1] = 'A';
    strArray_D[1][2] = 'M';
    strArray_D[1][3] = 'E';
    strArray_D[1][4] = '\0';

    strArray_D[2][0] = 'I';
    strArray_D[2][1] = 'S';
    strArray_D[2][2] = '\0';

    strArray_D[3][0] = 'D';
    strArray_D[3][1] = 'A';
    strArray_D[3][2] = 'R';
    strArray_D[3][3] = 'S';
    strArray_D[3][4] = 'H';
    strArray_D[3][5] = 'A';
    strArray_D[3][6] = 'N';
    strArray_D[3][7] = '\0';

    strArray_D[4][0] = 'M';
    strArray_D[4][1] = 'A';
    strArray_D[4][2] = 'L';
    strArray_D[4][3] = 'I';
    strArray_D[4][4] = '\0';

    printf("\n\n");
    printf("The String In The 20 Character Array Are : \n\n");

    for (i_D = 0; i_D < strArray_num_row_D; i_D++)
        printf("%s ",strArray_D[i_D]);

    printf("\n\n");

    return (0);    

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

