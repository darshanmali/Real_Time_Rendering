#include<stdio.h>
//Global Variable declaration 
struct Node
{
    int data_DM;
    struct Node *Next_DM;
} *Top = NULL;



int main(int argc,char *argv[], char *envp[])
{
    //Local Variable Declaration
    int *ch_DM = 0, *data_DM_P = 0;

    //Local Function Declaration
    struct Node* CreateNode();
    void Push_DM(int* );
    void Pop_DM();
    void Peek_DM();
    int IsEmpty_DM();

    //code
    printf("\n\n");
    printf("This is Stack Implementation : \n\n");
    ch_DM = (int *) malloc(sizeof(int));
    while(1)
    {
        printf("\n\n");
        printf("1. Push.\n");
        printf("2. Pop\n");
        printf("3. Peek\n");
        printf("4. Exit\n");
        printf("Enter the choice : ");
        scanf("%d",ch_DM);

        switch (*ch_DM)
        {

        case 1:
            printf("\n");
            data_DM_P = (int *) malloc(sizeof(int));
            printf("Please Enter the Data : ");
            scanf("%d", data_DM_P);
            Push_DM(data_DM_P);
            break;

        case 2:
            printf("\n");
            Pop_DM();
            break;

        case 3:
            printf("\n");
            Peek_DM();
            break;

        case 4:
            exit(0);
            break;

        default:
            printf("Choice is invalid : \n");
            break;
        }
    }
}

struct Node * CreateNode()
{
    struct Node* Temp_DM;
    Temp_DM = (struct Node *)malloc(sizeof(struct Node));
    return (Temp_DM);
}

int IsEmpty_DM(){
    if(Top == NULL)
        return (1);
    else
        return (0);
}

void Push_DM(int *data_P)
{
    struct Node *temp_DM;
    temp_DM = CreateNode();
    temp_DM->data_DM = *data_P;
    temp_DM->Next_DM = NULL;

    if (Top == NULL)
    {
        Top = temp_DM;
    }
    else if (Top != NULL)
    {
        temp_DM->Next_DM = Top;
        Top = temp_DM;        
    }
}

void Pop_DM()
{
    printf("Pop Is called....!!!\n");    
    struct Node* Temp_DM = Top, *Prev_DM = NULL;
    int Data_DM;
    if(IsEmpty_DM()){
        printf("\nStack is Empty..!!!");
    }
    else{
        Prev_DM = Top->Next_DM;
        Data_DM = Top->data_DM;
        free(Temp_DM);
        Top = Prev_DM;
        printf("Pop of %d Done..!", Data_DM);
    }
}

void Peek_DM()
{
    printf("Peek Is called....!!!\n");    
    struct Node *Temp_DM = Top;
    if(IsEmpty_DM())
        printf("\nStack is Empty..!!!");
    else
    {
        while (Temp_DM != NULL)
        {
            printf("\t%d",Temp_DM->data_DM);
            Temp_DM = Temp_DM->Next_DM;
        }
        printf("\n");    
    }
}
