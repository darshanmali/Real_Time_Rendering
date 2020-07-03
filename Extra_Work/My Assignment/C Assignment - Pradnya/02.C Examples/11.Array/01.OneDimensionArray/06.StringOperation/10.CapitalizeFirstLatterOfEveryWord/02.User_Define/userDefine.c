#include<stdio.h>
#include<ctype.h>



#define MAX_STRING_LENGTH 512

#define SPACE ' '
#define FULLSTOP '.'
#define COMA ','
#define EXCLAMATION '!'
#define QUESTION_MARK '?'

int main(int argc, char* argv[],char* envp[])
{

    int Mystringlen(char[]);
    char myToUpper(char);

    char chArray_D[MAX_STRING_LENGTH],chArray_Capitalize_letter_D[MAX_STRING_LENGTH];
    int istrLen;
    int i_D,j_D;
    

    printf("\n\n");
    printf("Enter String : ");
    gets_s(chArray_D,MAX_STRING_LENGTH);

    istrLen = Mystringlen(chArray_D);
    j_D = 0;   
    for (i_D = 0; i_D < istrLen; i_D++)
    {
       if (i_D == 0)
       {
            chArray_Capitalize_letter_D[j_D] = myToUpper(chArray_D[i_D]);           
       }
       else if (chArray_D[i_D] == SPACE)
       {
           chArray_Capitalize_letter_D[j_D] = chArray_D[i_D];
           chArray_Capitalize_letter_D[j_D +1] = myToUpper(chArray_D[i_D + 1]);
           j_D++;
           i_D++;

       }
       else if((chArray_D[i_D] == FULLSTOP || chArray_D[i_D] == COMA || chArray_D[i_D] == EXCLAMATION ||chArray_D[i_D] == QUESTION_MARK) && (chArray_D[i_D] != SPACE))
       {
            chArray_Capitalize_letter_D[j_D] = chArray_D[i_D];
            chArray_Capitalize_letter_D[j_D + 1] = SPACE;
            chArray_Capitalize_letter_D[j_D + 2] = myToUpper(chArray_D[i_D + 1]);

            j_D = j_D  + 2;
            i_D++;
       }

        else
        {
            chArray_Capitalize_letter_D[j_D] = chArray_D[i_D];
        }
       j_D++;

       
    }

    chArray_Capitalize_letter_D[j_D] = '\0';
    printf("\n\n");
    printf("The String Enter By you : %s\n\n", chArray_D);

    printf("\n\n");
    printf("The String After Capitalizing  First Latter of every word : %s\n\n", chArray_Capitalize_letter_D);

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

char myToUpper(char ch)
{
    int num_D;
    int c_D;

    num_D = 'a' - 'A';

    if((int)ch >= 97 && (int)ch <= 122)
    {
        c_D = (int)ch - num_D;
        return((char)c_D);
    }
    else
    {
        return(ch);
    }
    
}

