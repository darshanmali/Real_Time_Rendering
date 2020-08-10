//Header Files
#include<stdio.h>

//Globle Variable Declaration
struct QNode
{
    int Data_DM;
    struct QNode* Next_DM;
} *Front_DM = NULL, *Rear_DM = NULL;

int main(int argc, char *argv[], char *envp[])
{
    //Variable Declaration
    int *ch_DM = NULL, *Data_DM_EQ = NULL;
    ch_DM = (int *)malloc(sizeof(int));
    Data_DM_EQ = (int *)malloc(sizeof(int));

    //Function Declaration
    void EnQueue_DM(int *);
    void DeQueue_DM(void);
    void PrintQueue_DM(void);
    struct QNode* CreateNode(void);

    //code
    printf("\n\n");
    printf("*Welcome to Queue Data structure*");

    while(1)
    {
        printf("\n\n");
        printf("1. EnQueue.\n");
        printf("2. DeQueue.\n");
        printf("3. Print Queue\n");
        printf("4. Exit.\n");
        printf("\n");
        printf("Please Enter The Choice : ");
        scanf("%d",ch_DM);

        switch (*ch_DM)
        {
        case 1:
            printf("Please Enter the data : ");
            scanf("%d", Data_DM_EQ);
            EnQueue_DM(Data_DM_EQ);
            break;
        case 2:
            DeQueue_DM();
            break;
        case 3:
            PrintQueue_DM();
            break;
        case 4:
            exit(0);
            break;
        default:
            break;
        }

    }
}

struct QNode* CreateNode(void)
{
    struct QNode *Temp_DM = NULL;
    Temp_DM = (struct QNode *)malloc(sizeof(struct QNode));
    return (Temp_DM);
}

void EnQueue_DM(int *data)
{
    struct QNode *Temp_DM = CreateNode();
    Temp_DM->Data_DM = *data;
    Temp_DM->Next_DM = NULL;

    if (Front_DM == NULL && Rear_DM == NULL)
    {
        Front_DM = Temp_DM;
        Rear_DM = Temp_DM;
    }
    else
    {
        Temp_DM->Next_DM = Front_DM;
        Front_DM = Temp_DM;
    }
    printf("Entered Data is Entered...!!!\n");   
}

void DeQueue_DM(void)
{
    struct QNode *Temp_DM = NULL, *Prev_DM = NULL;
    int *elm = 0;
    elm = (int *)malloc(sizeof(int));
    Temp_DM = Front_DM;
    
    while (Temp_DM->Next_DM != NULL)
    {
        Prev_DM = Temp_DM;
        Temp_DM = Temp_DM->Next_DM;    
    }
    Rear_DM = Prev_DM;
    *elm = Temp_DM->Data_DM;
    Prev_DM->Next_DM = NULL;
    free(Temp_DM);
    printf("%d Element is Removed..!!!\n",*elm);
}

void PrintQueue_DM(void)
{
    struct QNode *Temp_DM = NULL;
    Temp_DM = Front_DM;
    while (Temp_DM != NULL)
    {
        printf("\t%d", Temp_DM->Data_DM);
        Temp_DM = Temp_DM->Next_DM;
    }
}
