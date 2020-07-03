#include<stdio.h>


#define MAX_STRING_LENGTH 512


int main(int argc, char* argv[],char* envp[])
{

    void Mystringcopy(char[],char[]);
    int Mystringlen(char[]);


    char chArray_D[MAX_STRING_LENGTH];
    int istrLen;
    int i_D;
    int w_c_D = 0, s_c_D = 0;
    

    printf("\n\n");

    printf("Enter String : ");
    gets_s(chArray_D,MAX_STRING_LENGTH);

    istrLen = Mystringlen(chArray_D);
    
    for (i_D = 0; i_D < istrLen; i_D++)
    {
        switch (chArray_D[i_D])
        {
            case 32:
                s_c_D++;
            break;
    
            default:
                break;
            
        }
    }

    w_c_D = s_c_D + 1;

    printf("\n\n");
    printf("The String Enter By you : %s\n\n", chArray_D);

    printf("\n\n");
    printf("The Numeber of spaces In the Input String : %d\n\n", s_c_D);
    printf("The Numeber of world In the Input String : %d\n\n", w_c_D);

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

