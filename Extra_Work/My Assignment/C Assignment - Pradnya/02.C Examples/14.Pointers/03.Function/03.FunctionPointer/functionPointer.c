#include<stdio.h>

int main(void)
{

    int AddIntegers(int, int);
    int SubtractIntegers(int, int);
    float AddFloats(float, float);

    typedef int (*AddIntsFnPtr)(int, int);
    AddIntsFnPtr ptrAddtwoIntegers = NULL;
    AddIntsFnPtr ptrFunc = NULL;

    typedef float (*AddFloatsFnPtr)(float, float);
    AddFloatsFnPtr ptrAddTwoFloats = NULL;

    int iAnswer = 0;
    float fAnswer = 0;

    ptrAddtwoIntegers = AddIntegers;
    iAnswer = ptrAddtwoIntegers(9, 21);
    printf("\n\n");
    printf("Sum of Intergers = %d \n\n",iAnswer);

    ptrFunc = SubtractIntegers;
    iAnswer = ptrFunc(33, 3);
    printf("\n\n");
    printf("Subtraction of Integers = %d \n\n", iAnswer);

    ptrAddTwoFloats = AddFloats;
    fAnswer = ptrAddTwoFloats(33.34f, 3.65f);
    printf("\n\n");
    printf("Sum of two Floats = %f \n\n", fAnswer);
    
    return(0);

}

int AddIntegers(int a, int b)
{
    int c;
    c = a + b;
    return(c);
}

int SubtractIntegers(int a,int b)
{
    int c_D;
    if (a > b)
    {
        c_D = a - b;
    }
    else
    {
        c_D = b - a; 
    }
    return(c_D);
}

float AddFloats (float f_num1, float f_num2)
{
    float ans_D;
     
     ans_D = f_num1 + f_num2;
     return(ans_D);
}

