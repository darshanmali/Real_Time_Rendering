#include<stdio.h>

typedef int MY_INT_D;


int main(void)
{

    MY_INT_D Add(MY_INT_D,MY_INT_D);

    typedef int MY_INT_D;

    typedef float PVG_FLOAT_D;

    typedef char CHARACTER_D;

    typedef double MY_DOUBLE_D;

    typedef unsigned  int UINT;

    typedef UINT HANDLE;

    typedef HANDLE HWND;

    typedef HANDLE HINSTANCE;

    MY_INT_D a_D = 11, i_D;

    MY_INT_D iArray[] = { 9, 18, 27, 36, 45, 54, 63, 72, 81, 90};

    PVG_FLOAT_D f_pvg = 30.9f;
    const PVG_FLOAT_D f_pvg_pi = 3.14f;

    CHARACTER_D ch = '*';
    CHARACTER_D chArray_01[] = "Hello";
    CHARACTER_D chArray_02[][10] = {"RTR","Batch","2020-2021"};

    MY_DOUBLE_D d = 5.767768;

    UINT uint = 3454;
    HANDLE handle = 789;
    HWND hwnd = 3435;
    HINSTANCE hInstance = 144455;

    printf("\n\n");
    printf("Type MY_INT variable a = %d\n", a_D);
    printf("\n\n");

    for (i_D = 0; i_D < (sizeof(iArray) / sizeof(int)); i_D++)
    {
        printf("Type MY_INT array variable iArray [%d] = %d\n", i_D, iArray[i_D]);
    }
    printf("\n\n");
    printf("\n\n");
    printf("Type PVG_FLOAT_D variable f = %f \n", f_pvg);
    printf("Type PVG_FLOAT_D Constant f_pvg_pi = %f \n", f_pvg_pi);

    printf("\n\n");
    printf("Type MY_DOUBLE variable d = %lf \n", d);
    
    printf("\n\n");
    printf("Type MY_CHARACTER_D variable ch = %c \n", ch);
    
    printf("\n\n");
    printf("Type MY_CHARACTER_D array variable chArray_01 = %s \n", chArray_01);
    printf("\n\n");

    for (i_D = 0; i_D < (sizeof(chArray_02) / sizeof(chArray_02[0])); i_D++)
    {
        printf("%s\t", chArray_02[i_D]);
    }
    
    printf("\n\n");
    printf("Type UINT variable uint = %u\n\n", uint);
    printf("Type HANDLE variable handle = %u\n\n", handle);
    printf("Type HWND variable hwnd = %u\n\n", hwnd);
    printf("Type HISTANCE variable hIstance = %u\n\n", hInstance);

    MY_INT_D x = 80;
    MY_INT_D y = 20;
    MY_INT_D ret;

    ret = Add( x, y);

    printf("ret = %d\n\n", ret);

    return(0);
}

MY_INT_D Add(MY_INT_D a,MY_INT_D b)
{
    MY_INT_D c;
    c = a + b;
    return (c);
}

