#include<stdio.h>


#define MAX_STRING_LENGTH 512


int main(int argc, char* argv[],char* envp[])
{

    int Mystringlen(char[]);

    char chArray_D[MAX_STRING_LENGTH];
    int istrLen;
    int count_A = 0,count_E = 0, count_I = 0,count_O = 0,count_U = 0;
    int i_D;

    

    printf("\n\n");

    printf("Enter String : ");
    gets_s(chArray_D, MAX_STRING_LENGTH);

    printf("\n\n");

    printf("String Entered By You : \n\n");
    printf("%s \n",chArray_D);  

    istrLen = Mystringlen(chArray_D);

    for (i_D = 0; i_D < istrLen; i_D++)
    {
        switch (chArray_D[i_D])
        {
            case 'A':
            case 'a':
                count_A++;
            break;

            case 'E':
            case 'e':
                count_E++;
            break;
            case 'I':
            case 'i':
                count_I++;
            break;

            case 'O':
            case 'o':
                count_O++;
            break;

            case 'U':
            case 'u':
                count_U++;
            break;

            default:
            break;
            
        }
    }
    
    printf("\n\n");
    printf("The String Enter By you, The Vowel And the number of Ocurance Are: \n\n");
    printf("'A' has Occured = %d Times !!!\n\n", count_A);
    printf("'E' has Occured = %d Times !!!\n\n", count_E);
    printf("'I' has Occured = %d Times !!!\n\n", count_I);
    printf("'O' has Occured = %d Times !!!\n\n", count_O);
    printf("'U' has Occured = %d Times !!!\n\n", count_U);
    
    return(0);
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
            string_length++;
    }
    return (string_length);
}

