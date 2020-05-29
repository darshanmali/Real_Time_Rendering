#include<stdio.h>

int main(void)
{
    void PrintBinaryFromNumber(unsigned int);

    unsigned int x;
    unsigned int y;
    unsigned int result;

    printf("\n");
    printf("Enter X Value : ");
    scanf("%u", &x);

    printf("\n");
    printf("Enter Y Value : ");
    scanf("%u", &y);
    
    result = x ^ y;
    printf("Bitwise XOR : \n\n\tX = %d (Decimal),\n\tY = %d (Decimal)\n   Result = %d\n\n",x, y, result);
   
    PrintBinaryFromNumber(x);
    PrintBinaryFromNumber(y);
    PrintBinaryFromNumber(result);

    return(0);   

}

void PrintBinaryFromNumber(unsigned int decimal_numeber)
{
    unsigned int quat,rem;
    unsigned int n;
    unsigned int binary_array[8];
    int i;

    for( i = 0 ; i < 8; i++)
    {
        binary_array[i] = 0;
    }
    printf("This Binary Form of The Decimal is %d Is \t=\t",decimal_numeber);
    n = decimal_numeber;

    i = 7;
    while (n!=0)
    {
        quat = n / 2;
        rem = n % 2;
        binary_array[i] = rem;
        n = quat;
        i--; 
    }
        
    for (i = 0; i< 8; i++)
    {
        printf("%u",binary_array[i]);
    }
    printf("\n\n");
}
