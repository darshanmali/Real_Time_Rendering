#include<stdio.h>

struct MyNumber
{
    int num_D;
    int num_Table[10];
};
struct NumTables
{
    struct MyNumber n_D;
   
};


int main(int argc, char *argv[],char *envp[])
{
    struct NumTables tables[10];
    int i_D, j_D;
    
    for (i_D = 0; i_D < 10; i_D++)
    {
        tables[i_D].n_D.num_D = (i_D + 1);     
    }

    for (i_D = 0; i_D < 10; i_D++)
    {
        printf("\n\n");
        printf("Table of %d : \n\n",tables[i_D].n_D.num_D);
        for (j_D = 0; j_D < 10; j_D++)
        {
            tables[i_D].n_D.num_Table[j_D] = tables[i_D].n_D.num_D * (j_D + 1);
            printf("%d * %d = %d\n", tables[i_D].n_D.num_D, (j_D +1),tables[i_D].n_D.num_Table[j_D]);
        }
    }
    return (0);

}


