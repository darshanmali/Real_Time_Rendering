#include<stdio.h>
#include<ctype.h>



#define MAX_STRING_LENGTH 512


int main(int argc, char* argv[],char* envp[])
{

    int Mystringlen(char[]);


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
            chArray_Capitalize_letter_D[j_D] = toupper(chArray_D[i_D]);           
       }
       else if (chArray_D[i_D] == ' ')
       {
           chArray_Capitalize_letter_D[j_D] = chArray_D[i_D];
           chArray_Capitalize_letter_D[j_D +1] = toupper(chArray_D[i_D + 1]);
           j_D++;
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

