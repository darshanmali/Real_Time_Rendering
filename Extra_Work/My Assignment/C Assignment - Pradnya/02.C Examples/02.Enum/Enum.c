#include<stdio.h>

enum months
{
    JAN = 1,
    FEB,
    MARCH,
    APRIL,
    MAY,
    JUN,
    JULY,
    AUG,
    SEPT,
    OCT,
    NOV,
    DES
};

int main( int argc, char *argv[])
{
    enum months all;
    all =JAN; 
    printf("JAN = %d\n", all);
    all =FEB; 
    printf("FEB = %d\n", all);
    all =MARCH; 
    printf("MARCH = %d\n", all);
    all =APRIL; 
    printf("APRIL = %d\n", all);
    all =MAY; 
    printf("MAY = %d\n", all);
    all =JUN; 
    printf("JUN = %d\n", all);
    all =JULY; 
    printf("JULY = %d\n", all);
    all =AUG; 
    printf("AUG = %d\n", all);
    all =SEPT; 
    printf("SEPT = %d\n", all);
    all =OCT; 
    printf("OCT = %d\n", all);
    all =NOV; 
    printf("NOV = %d\n", all);
    all =DES; 
    printf("DES = %d\n", all);
 
    return (0);
}