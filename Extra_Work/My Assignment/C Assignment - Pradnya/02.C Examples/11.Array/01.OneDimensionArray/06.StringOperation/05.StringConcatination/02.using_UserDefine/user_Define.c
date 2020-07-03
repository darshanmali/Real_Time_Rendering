#include<stdio.h>

#define MAX_STRING_LENGTH 512


int main(int argc, char* argv[],char* envp[])
{

    void MyStrCat(char[], char[]);

    char chArray_One_D[MAX_STRING_LENGTH],chArray_Two_D[MAX_STRING_LENGTH];

    printf("\n\n");
    printf("Enter First string \n");
    gets_s(chArray_One_D,MAX_STRING_LENGTH);


    printf("\n\n");
    printf("Enter Second string \n");
    gets_s(chArray_Two_D,MAX_STRING_LENGTH);


    printf("******************Before Concatination**************");

    printf("\n\n");
    printf("First String Entered By You : \n\n");
    printf("%s \n",chArray_One_D);
    

    
    printf("\n\n");
    printf("Second String Entered By You : \n\n");
    printf("%s \n",chArray_Two_D);
    
    MyStrCat(chArray_One_D, chArray_Two_D);

    printf("******************After Concatination**************");
    
    
    printf("\n\n");
    
    printf("String One is = %s \n\n",chArray_One_D);
    
    printf("String Two is = %s \n\n",chArray_Two_D);
    return (0);    

}


void MyStrCat(char str_Des[],char str_sour[])
{
    int Mystringlen(char[]);

    int istring_Len_Des = 0, istring_Len_sour = 0;
    int i_D, j_D;

    istring_Len_sour = Mystringlen(str_sour);
    istring_Len_Des = Mystringlen(str_Des);

    for (i_D = istring_Len_Des, j_D = 0 ; j_D < istring_Len_sour; i_D++, j_D++)
    {
        str_Des[i_D] = str_sour[j_D]; 
    }
    str_Des[i_D] = '\0';
    

}




int Mystringlen(char str[])
{
    int j_D;
    int string_length = 0;
    for (j_D = 0; j_D < MAX_STRING_LENGTH; j_D++)
    {
        if(str[j_D] == '\0')
            break;
        
        else 
            string_length ++;
            

    }
    return (string_length);
            
}

