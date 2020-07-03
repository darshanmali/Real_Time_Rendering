#include<stdio.h>

#define INT_ARRAY_SIZE 10
#define FLOAT_ARRAY_SIZE 5
#define CHAR_ARRAY_SIZE 26

#define NUM_STRING 10
#define MAX_CHARACTERS_PER_STRING 20

#define ALPHABET_BEGINING 65

struct MyDataOne
{
    int iArray_D[INT_ARRAY_SIZE];
    float fArray_D[FLOAT_ARRAY_SIZE];
};

struct MyDataTwo
{
    char cArray_D[CHAR_ARRAY_SIZE];
    char strArray_D[NUM_STRING][MAX_CHARACTERS_PER_STRING];
};

int main(int argc,char *argv[], char *envp[])
{
    struct MyDataOne data_One_D;
    struct MyDataTwo data_Two_D;
    int i_D;

    data_One_D.fArray_D[0] = 0.1f;
    data_One_D.fArray_D[1] = 1.2f;
    data_One_D.fArray_D[2] = 2.3f;
    data_One_D.fArray_D[3] = 3.4f;
    data_One_D.fArray_D[4] = 4.5f;

    printf("\n\n");
    printf("Enter %d Integers : \n\n",INT_ARRAY_SIZE);
    for(i_D = 0; i_D < INT_ARRAY_SIZE ; i_D++)
    {
        scanf("%d", &data_One_D.iArray_D[i_D]);
    }
    
    for (i_D = 0; i_D < CHAR_ARRAY_SIZE; i_D++)
    {
        data_Two_D.cArray_D[i_D] = (char)(i_D + ALPHABET_BEGINING);
    }
    strcpy(data_Two_D.strArray_D[0],"Welcome !!!!");
    strcpy(data_Two_D.strArray_D[1],"This");
    strcpy(data_Two_D.strArray_D[2],"Is");
    strcpy(data_Two_D.strArray_D[3],"Astromedicomp's");
    strcpy(data_Two_D.strArray_D[4],"Real");
    strcpy(data_Two_D.strArray_D[5],"Time");
    strcpy(data_Two_D.strArray_D[6],"Rebdering");
    strcpy(data_Two_D.strArray_D[7],"Batch");
    strcpy(data_Two_D.strArray_D[8],"Of");
    strcpy(data_Two_D.strArray_D[9],"2020-2021 !!!");

    printf("\n\n");
    printf("Menbers Of 'Struct Data One' Alongwith Their  Assignment Value Are : \n\n");
    printf("\n\n");
    printf("Integer Array (data.one.iArray[]) : \n\n");
    for (i_D = 0; i_D < INT_ARRAY_SIZE; i_D++)
    {
        printf("data_One.iArray[%d] = %d \n", i_D, data_One_D.iArray_D[i_D]);
    }
    printf("\n\n");
    printf("Float Array (data.one.fArray[]) : \n\n");
    for (i_D = 0; i_D < FLOAT_ARRAY_SIZE; i_D++)
    {
        printf("data_One.fArray[%d] = %f \n", i_D, data_One_D.fArray_D[i_D]);
    }

    printf("\n\n");
    printf("Menbers Of 'Struct Data Float' Alongwith Their  Assignment Value Are : \n\n");
    printf("\n\n");
    printf("Character Array (data_Two.cArray[]) : \n\n");
    for (i_D = 0; i_D < CHAR_ARRAY_SIZE; i_D++)
    {
        printf("data_One.cArray[%d] = %c \n", i_D, data_Two_D.cArray_D[i_D]);
    }
     printf("\n\n");
    printf("String Array (data_Two.strArray[]) : \n\n");
    for (i_D = 0; i_D < NUM_STRING; i_D++)
    {
        printf("%s ", data_Two_D.strArray_D[i_D]);
    }
    printf("\n\n");

    return(0);

}



