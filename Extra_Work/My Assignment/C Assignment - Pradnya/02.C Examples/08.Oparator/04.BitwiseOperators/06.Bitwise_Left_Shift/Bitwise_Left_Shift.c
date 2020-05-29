#include<stdio.h>

int main(void)
{
    void PrintBinaryFromNumber(unsigned int);

    unsigned int x;
    unsigned int num_bits;
    unsigned int result;

    printf("\n");
    printf("Enter X Value : ");
    scanf("%u", &x);

    printf("\n");
    printf("Enter bits you want to shift to Right : ");
    scanf("%u", &num_bits);
    
    result = x << num_bits;
    printf("Bitwise LEFT SHIT : \n\n\tX = %d by \n     Bits = %d\n   Result = %d (Decimal)\n\n",x, num_bits, result);
   
    PrintBinaryFromNumber(x);
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
