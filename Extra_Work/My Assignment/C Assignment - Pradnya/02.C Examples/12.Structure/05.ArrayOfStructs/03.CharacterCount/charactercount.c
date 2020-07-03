#include<stdio.h>
#include<ctype.h>
#include<string.h>

#define MAX_STRING_LENGTH 1024

struct CharacterCunt
{
    char ch;
    int ch_count;
} character_and_count[] = { {'A', 0 },
                            {'B', 0 },
                            {'C', 0 },
                            {'D', 0 },
                            {'E', 0 },
                            {'F', 0 }, 
                            {'G', 0 }, 
                            {'H', 0 }, 
                            {'I', 0 }, 
                            {'J', 0 }, 
                            {'K', 0 }, 
                            {'L', 0 }, 
                            {'M', 0 }, 
                            {'N', 0 }, 
                            {'O', 0 },
                            {'P', 0 },
                            {'Q', 0 }, 
                            {'R', 0 }, 
                            {'S', 0 },
                            {'T', 0 }, 
                            {'U', 0 }, 
                            {'V', 0 }, 
                            {'W', 0 }, 
                            {'X', 0 }, 
                            {'Y', 0 }, 
                            {'Z', 0 }};

#define SIZE_OF_ENTIRE_ARRAY_OF_STRUCTS sizeof(character_and_count)
#define SIZE_OF_ONE_STRUCT_FROM_ENTIRE_ARRAY_OF_STRUCTS sizeof(character_and_count[0])

#define NUM_ELEMEMTS_IN_ARRAY (SIZE_OF_ENTIRE_ARRAY_OF_STRUCTS / SIZE_OF_ONE_STRUCT_FROM_ENTIRE_ARRAY_OF_STRUCTS)

int main(void)
{
    char str[MAX_STRING_LENGTH];
    int i_D,j_D, actual_String_length = 0;
    printf("\n\n");
    printf("Enter A String : \n\n");
    gets_s(str,MAX_STRING_LENGTH);

    actual_String_length = strlen(str);

    printf("\n\n");
    printf("The String you have entered Is : \n\n");
    printf("%s\n\n", str);

    for (i_D = 0; i_D < actual_String_length; i_D++)
    {
        for (j_D = 0; j_D < NUM_ELEMEMTS_IN_ARRAY; j_D++)
        {
            str[i_D] = toupper(str[i_D]);

            if (str[i_D] == character_and_count[j_D].ch)
                character_and_count[j_D].ch_count++;
        }
    }
    printf("\n\n");
    printf("The Number of Occurence of all character From The alphabet Are as follows : \n\n");
    for (i_D = 0; i_D < NUM_ELEMEMTS_IN_ARRAY; i_D++)
    {
        printf("character %c = %d \n",character_and_count[i_D].ch,character_and_count[i_D].ch_count);
    }
    printf("\n\n");
    return(0);

}


