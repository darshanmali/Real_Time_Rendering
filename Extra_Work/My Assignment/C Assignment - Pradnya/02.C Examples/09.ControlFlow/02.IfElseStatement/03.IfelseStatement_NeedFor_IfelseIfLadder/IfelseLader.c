#include<stdio.h>

int main(void)
{
    int n;

    printf("Enter Value for N : ");
    scanf("%d", &n);

    if (n < 0)
    {
        printf("n = %d is Less 0 (Negative) !!!!\n\n", n);
    }
    else
    {
        if ((n > 0) && ( n<= 100))
        {
            printf("n = %d is Between 0 And 100 !!!!\n\n", n);
        }
        else
        {
            if ((n > 100) && ( n<= 200))
            {
                printf("n = %d is Between 100 And 200 !!!!\n\n", n);
            }
            else
            {
                if((n > 200) && ( n<= 300))
                {
                    printf("n = %d is Between 200 And 300 !!!!\n\n", n);
                }
                else
                {
                    if((n > 300) && ( n<= 400))
                    {
                        printf("n = %d is Between 300 And 400 !!!!\n\n", n);
                    }
                    else
                    {
                        if((n > 400) && ( n<= 500))
                        {
                            printf("n = %d is Between 400 And 500 !!!!\n\n", n);
                        }
                        else
                        {
                            printf("n = %d is Greater than 500 !!!!\n\n", n);
                        }


                    }

                }
            
            }
            
        }
        
    }
    
    return (0);
}
