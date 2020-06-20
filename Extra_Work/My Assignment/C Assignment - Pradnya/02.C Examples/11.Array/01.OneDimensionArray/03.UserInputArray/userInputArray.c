#include<stdio.h>

#define INT_AARAY_NUM_ELEMETS 5
#define FLOAT_AARAY_NUM_ELEMETS 3
#define CHAR_AARAY_NUM_ELEMETS 15

int main(int argc,char *argv[],char *envp[])
{
    int iArray_D[INT_AARAY_NUM_ELEMETS];
    float fArray_D[FLOAT_AARAY_NUM_ELEMETS];
    char cArray_D[CHAR_AARAY_NUM_ELEMETS];
    int i_D, num_D;

    printf("\n\n");
    printf("Enter Elements for 'Integer' Array : \n");
    for(i_D = 0; i_D < INT_AARAY_NUM_ELEMETS; i_D++)
        scanf("%d",&iArray_D[i_D]);

    
    printf("\n\n");
    printf("Enter Elements for 'Floating-Point' Array : \n");
    for(i_D = 0; i_D < FLOAT_AARAY_NUM_ELEMETS; i_D++)
        scanf("%f",&fArray_D[i_D]);


    printf("\n\n");
    printf("Enter Elements for 'Character' Array : \n");
    for(i_D = 0; i_D < CHAR_AARAY_NUM_ELEMETS; i_D++)
    {
        cArray_D[i_D] = getch();
        printf("%c\n", cArray_D[i_D]);
    }

    printf("\n\n");
    printf("Integer Array Entered By You : \n");
    for(i_D = 0; i_D < INT_AARAY_NUM_ELEMETS; i_D++)
        printf("%d\t",iArray_D[i_D]);

    printf("\n\n");
    printf("Floating-Point Array Entered By You : \n");
    for(i_D = 0; i_D < FLOAT_AARAY_NUM_ELEMETS; i_D++)
        printf("%f\t",fArray_D[i_D]);

    
    printf("\n\n");
    printf("Character Array Entered By You : \n");
    for(i_D = 0; i_D < CHAR_AARAY_NUM_ELEMETS; i_D++)
        printf("%c", cArray_D[i_D]);

    return(0);    


}
