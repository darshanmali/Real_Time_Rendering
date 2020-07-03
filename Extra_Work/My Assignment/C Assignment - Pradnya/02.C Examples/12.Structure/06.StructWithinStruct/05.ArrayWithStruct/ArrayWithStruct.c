#include<stdio.h>

struct MyNumber
{
    int num_D;
    int num_Table[10];
};
struct NumTables
{
    struct MyNumber a_D;
    struct MyNumber b_D;
    struct MyNumber c_D;

};

int main(int argc, char *argv[],char *envp[])
{
    struct NumTables tables;
    int i_D;

    tables.a_D.num_D = 2;
    for (i_D = 0; i_D < 10; i_D++)
        tables.a_D.num_Table[i_D] = tables.a_D.num_D * (i_D + 1);
    printf("\n\n");
    printf("Table Of %d : \n", tables.a_D.num_D);
    for (i_D = 0; i_D < 10; i_D++)
        printf("%d * %d = %d \n", tables.a_D.num_D, (i_D + 1), tables.a_D.num_Table[i_D]);    
    
    tables.a_D.num_D = 3;
    for (i_D = 0; i_D < 10; i_D++)
        tables.a_D.num_Table[i_D] = tables.a_D.num_D * (i_D + 1);
    printf("\n\n");
    printf("Table Of %d : \n", tables.a_D.num_D);
    for (i_D = 0; i_D < 10; i_D++)
        printf("%d * %d = %d \n", tables.a_D.num_D, (i_D + 1), tables.a_D.num_Table[i_D]);    
    
    tables.a_D.num_D = 4;
    for (i_D = 0; i_D < 10; i_D++)
        tables.a_D.num_Table[i_D] = tables.a_D.num_D * (i_D + 1);
    printf("\n\n");
    printf("Table Of %d : \n", tables.a_D.num_D);
    for (i_D = 0; i_D < 10; i_D++)
        printf("%d * %d = %d \n", tables.a_D.num_D, (i_D + 1), tables.a_D.num_Table[i_D]);    

    return (0);

}




