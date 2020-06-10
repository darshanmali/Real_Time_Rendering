#include<stdio.h>
#include<conio.h>

#define CHAR_ALFABET_UPPER_CASE_BEGINNING 65
#define CHAR_ALFABET_UPPER_CASE_ENDING 90

#define CHAR_ALFABET_LOWER_CASE_BEGINNING 97
#define CHAR_ALFABET_LOWER_CASE_ENDING 122

#define CHAR_DIGIT_BEGINNING 65
#define CHAR_DIGIT_ENDING 90

int main(void)
{
    char c;
    int ch_value;

    printf("\n\n");

    printf("Enter Character : ");
    c = getch();

    printf("\n\n");
    
    switch (c)
    {
    case 'A':
    case 'a':

    case 'E':
    case 'e':
    
    case 'I':
    case 'i':
    
    case 'O':
    case 'o':
    
    case 'U':
    case 'u':
        printf("character is vowel !!\n\n");
        break;
    
    default:
        ch_value = (int)c;
        if((ch_value >= CHAR_ALFABET_UPPER_CASE_BEGINNING && ch_value <= CHAR_ALFABET_UPPER_CASE_ENDING) || (ch_value >= CHAR_ALFABET_LOWER_CASE_BEGINNING && ch_value <= CHAR_ALFABET_LOWER_CASE_ENDING))
        {
            printf("Character you entered \" %c \" is Consonant Charater.\n\n", c);
        }
        else if (ch_value >= CHAR_DIGIT_BEGINNING && ch_value <= CHAR_ALFABET_UPPER_CASE_ENDING)
        {
            printf("Character you entered \" %c \" is Digit Charater.\n\n", c);
        }
        
        else
        {
            printf("Character you entered \" %c \" is Special Charater.\n\n", c);
        }
        break;
    }

    return(0);
}
