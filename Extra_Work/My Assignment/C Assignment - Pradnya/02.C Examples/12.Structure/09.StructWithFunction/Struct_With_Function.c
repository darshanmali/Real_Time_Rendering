#include<stdio.h>

struct MyData
{
    int i_D;
    float f_D;
    double d_D;
    char c_D; 
};
int main(void)
{
    struct MyData AddStructmembers(struct MyData,struct MyData,struct MyData);
    struct MyData data_1,data_2, data_3, answer_data;

    printf("\n\n");
    printf("****** Data 1 *******\n\n");
    printf("Enter Integer value For 'I' of 'struct MyData data_1' : ");
    scanf("%d", &data_1.i_D);

    printf("\n\n");
    printf("Enter Floating-Point value For 'F' of 'struct MyData data_1' : ");
    scanf("%f", &data_1.f_D);

    printf("\n\n");
    printf("Enter Double value For 'D' of 'struct MyData data_1' : ");
    scanf("%lf", &data_1.d_D);

    printf("\n\n");
    printf("Enter Character value For 'C' of 'struct MyData data_1' : ");
    data_1.c_D = getch();
    printf("%c", data_1.c_D);

    printf("\n\n");
    printf("****** Data 2 *******\n\n");
    printf("Enter Integer value For 'I' of 'struct MyData data_2' : ");
    scanf("%d", &data_2.i_D);

    printf("\n\n");
    printf("Enter Floating-Point value For 'F' of 'struct MyData data_2' : ");
    scanf("%f", &data_2.f_D);

    printf("\n\n");
    printf("Enter Double value For 'D' of 'struct MyData data_2' : ");
    scanf("%lf", &data_2.d_D);

    printf("\n\n");
    printf("Enter Character value For 'C' of 'struct MyData data_2' : ");
    data_2.c_D = getch();
    printf("%c", data_2.c_D);

    printf("\n\n");
    printf("****** Data 3 *******\n\n");
    printf("Enter Integer value For 'I' of 'struct MyData data_3' : ");
    scanf("%d", &data_3.i_D);

    printf("\n\n");
    printf("Enter Floating-Point value For 'F' of 'struct MyData data_3' : ");
    scanf("%f", &data_3.f_D);

    printf("\n\n");
    printf("Enter Double value For 'D' of 'struct MyData data_3' : ");
    scanf("%lf", &data_3.d_D);

    printf("\n\n");
    printf("Enter Character value For 'C' of 'struct MyData data_3' : ");
    data_3.c_D = getch();
    printf("%c", data_3.c_D);

    answer_data = AddStructmembers(data_1, data_2, data_3);

    printf("\n\n");
    printf("********* ANSWER *********\n\n");
    printf("answer_data.i_D = %d \n",answer_data.i_D);
    printf("answer_data.f_D = %f \n",answer_data.f_D);
    printf("answer_data.d_D = %lf \n\n",answer_data.d_D);
    
    answer_data.c_D = data_1.c_D;
    printf("answer_data.c_D (From data_1) = %c \n",answer_data.c_D);

    answer_data.c_D = data_2.c_D;
    printf("answer_data.c_D (From data_2) = %c \n",answer_data.c_D);
    
    answer_data.c_D = data_3.c_D;
    printf("answer_data.c_D (From data_3) = %c \n",answer_data.c_D);

    return (0);

}

struct MyData AddStructmembers(struct MyData mb_one, struct MyData mb_two, struct MyData mb_three)
{
    struct MyData answer;

    answer.i_D = mb_one.i_D + mb_two.i_D + mb_three.i_D;
    answer.f_D = mb_one.f_D + mb_two.f_D + mb_three.f_D;
    answer.d_D = mb_one.d_D + mb_two.d_D + mb_three.d_D;

    return (answer);

}





