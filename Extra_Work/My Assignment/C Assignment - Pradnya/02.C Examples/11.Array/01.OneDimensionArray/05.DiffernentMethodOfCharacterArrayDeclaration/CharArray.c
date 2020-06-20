#include<stdio.h>

int main(int argc, char *argv[], char *envp[])
{
    char chArray_01_D[] = {'A','1','H','O','M','E','S','E','V','A','\0'};
    char chArray_02_D[10] = {'W','E','L','C','O','M','E','\'','S','\0'};
    char chArray_03_D[] = {'Y','O','U','\0'};
    char chArray_04_D[] = "To";
    char chArray_05_D[] = "EXPERIENCE THE BEST SERVICE AT YOUR PLACE.";


    char chArray_WithoutNullTermiator[] = {'H','E','L','L','0'};
    
    printf("\n\n");

    printf("size of chArray_01 : %lu\n\n", sizeof(chArray_01_D));
    printf("size of chArray_02 : %lu\n\n", sizeof(chArray_02_D));
    printf("size of chArray_03 : %lu\n\n", sizeof(chArray_03_D));
    printf("size of chArray_04 : %lu\n\n", sizeof(chArray_04_D));
    printf("size of chArray_05 : %lu\n\n", sizeof(chArray_05_D));

    printf("\n\n");

    printf("The String Are : ");
    printf("size of chArray_01 : %lu\n\n", sizeof(chArray_01_D));
    printf("size of chArray_02 : %lu\n\n", sizeof(chArray_02_D));
    printf("size of chArray_03 : %lu\n\n", sizeof(chArray_03_D));
    printf("size of chArray_04 : %lu\n\n", sizeof(chArray_04_D));
    printf("size of chArray_05 : %lu\n\n", sizeof(chArray_05_D));

    printf("\n\n");

    printf("Size Of chArray WithoutNull Terminator : %lu\n\n", sizeof(chArray_WithoutNullTermiator));
    printf("chArray_withoutNullTernimal : %s\n\n", chArray_WithoutNullTermiator);

    return(0);
    
}

