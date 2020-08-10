#include<stdio.h>

struct Node
{
    struct Node *prev_DM;
    int data_DM;
    struct Node *Next_DM;
} *start_DM = NULL, *last_DM = NULL;

struct Node* createNode();

int main(int argc, char *argv[], char *envp[])
{
    //Variable Declaration
    int ch_DM, ch_DM_I, ch_DM_D, ch_DM_T;
    int L_data_DM;

    //Fuction Declaration
    void InsertAtLast(int);
    void InsertAtMiiddle(int);
    void InsertAtBeginning(int);

    void DeleteLastNode(void);
    void DeleteMiddleNode(void);
    void DeleteFirstNode(void);
    
    void TraverseFromLast(void);
    void TraverseFromBeginning(void);
    
     printf("Welcome to Doubly Circular Linked List Given options Are :\n");
    while (1)
    {
        printf("1.Insert Elements.\n");
        printf("2.Delete Elements.\n");
        printf("3.Traverse List.\n");    
        printf("4.Exit.\n");
        printf("\nChoose Your Choise : ");
        scanf("%d",&ch_DM);
        printf("\n");
        switch (ch_DM)
        {
        case 1:
                printf("1.Insert At Beginning.\n");
                printf("2.Insert At Middle.\n");
                printf("3.Insert At Last.\n");
                printf("Please Enter the choice : ");
                scanf("%d",&ch_DM_I);
                printf("\n\n");
                switch (ch_DM_I)
                {
                case 1:
                    printf("Please Enter Data : ");
                    scanf("%d", &L_data_DM);
                    printf("\n");
                    InsertAtBeginning(L_data_DM);
                    break;
                case 2:
                    printf("Please Enter Data : ");
                    scanf("%d", &L_data_DM);
                    printf("\n");
                    InsertAtMiiddle(L_data_DM);
                    break;
                case 3:
                    printf("Please Enter Data : ");
                    scanf("%d", &L_data_DM);
                    printf("\n");
                    InsertAtLast(L_data_DM);
                    break;
                default:
                    printf("you fave given the wrong choice!!!");
                    break;
                }
            break;
        case 2:
            printf("1.Delete First node : \n");
            printf("2.Delete Middle node : \n");
            printf("3.Delete Last node : \n");
            printf("Enter the choice : ");
            scanf("%d",&ch_DM_D);
            printf("\n");
            switch (ch_DM_D)
            {
            case 1:
                DeleteFirstNode();
                break;
            case 2:
                DeleteMiddleNode();
                break;
            case 3:
                DeleteLastNode();
                break;
            
            default:
                printf("you fave given the wrong choice!!!\n");
                break;
            }
            break;
        case 3:
            printf("1.Traverse From Beggining Of List :\n");
            printf("2.Traverse From Last Of List :\n");
            scanf("%d",&ch_DM_T);
            switch (ch_DM_T)
            {
                case 1:
                    TraverseFromBeginning();
                break;
                case 2:
                    TraverseFromLast();
                break;
                default:
                    printf("you fave given the wrong choice!!!\n");
                break;
            }
        break;
        case 4:
                return(0);
        
        default:
        printf("You have given Wrong Choice !!!!\n\n");
            break;
        }
    }
}

//Create Node
struct Node* createNode()
{
    struct Node *temp_DM = NULL;
    temp_DM = (struct Node *)malloc(sizeof(struct Node));
    return(temp_DM);
}

//Insertion of Node
void InsertAtBeginning(int field)
{
    struct Node *temp_DM = NULL,
                *trav = NULL;
    
    temp_DM = createNode();
    temp_DM->data_DM = field;
    temp_DM->prev_DM = NULL;
    temp_DM->Next_DM = NULL;

    if (start_DM == NULL)
    {
        start_DM = temp_DM;
        last_DM = temp_DM;
        temp_DM->Next_DM = temp_DM;
        temp_DM->prev_DM = temp_DM;
        printf("Insertion is done at First Node\n");
    }
    else 
    {
        temp_DM->Next_DM = start_DM;
        temp_DM->prev_DM = last_DM;
        start_DM->prev_DM = temp_DM;
        trav = start_DM;
        while (trav->Next_DM != start_DM)
        {
            trav = trav->Next_DM;
        }
        trav->Next_DM = temp_DM;
        start_DM = temp_DM;
        printf("Insertion is done at First Node\n");
    }
}

void InsertAtMiiddle(int field)
{
    int i = 0, pos = 0;
   struct Node *temp_DM = NULL,
                *trav = NULL,
                *prev_DM = NULL;
    
    temp_DM = createNode();
    temp_DM->data_DM = field;
    temp_DM->Next_DM = NULL;
    temp_DM->prev_DM = NULL;

    printf("\nPlease Enter the Position : ");
    scanf("%d", &pos);

    trav = start_DM;
    while (i != pos - 1 && trav->Next_DM != start_DM)
    {
        prev_DM = trav;
        trav = trav->Next_DM;
        i++;
    }

    if (trav->Next_DM == start_DM)
    {
        temp_DM->prev_DM = prev_DM;
        temp_DM->Next_DM = trav;
        
        prev_DM->Next_DM = temp_DM;
        trav->prev_DM = temp_DM;
        printf("Insertion is done !!!\n\n");
    }
    else if (trav->Next_DM != start_DM)
    {
        temp_DM->Next_DM = trav;
        temp_DM->prev_DM = prev_DM;

        prev_DM->Next_DM = temp_DM;
        trav->prev_DM = temp_DM;
        printf("Insertion is done\n\n");
    }
    
}

void InsertAtLast(int field)
{
    struct Node *temp_DM = NULL,
                *trav = NULL;
    
    temp_DM = createNode();
    temp_DM->data_DM = field;
    temp_DM->Next_DM = NULL;
    temp_DM->prev_DM = NULL;

    trav = start_DM;
    while (trav->Next_DM != start_DM)
    {
        trav = trav->Next_DM;
    }
    if (trav->Next_DM == start_DM)
    {
        temp_DM->prev_DM = trav;
        temp_DM->Next_DM = start_DM;
        trav->Next_DM = temp_DM;
        start_DM->prev_DM = temp_DM;
        last_DM = temp_DM;
        printf("Insertion is done at Last Node!!!\n\n");
    }
}

//Deletion Of Node 
void DeleteFirstNode(void)
{
    struct Node *trav = NULL;

    if (start_DM == NULL)
    {
        printf("Linked List Is Empty.\n\n");
    }
    else
    {
        trav = start_DM;
        start_DM = trav->Next_DM;
        start_DM->prev_DM = trav->prev_DM;
        last_DM->Next_DM = start_DM;
        free(trav);
        printf("Deletion is done.!!!\n\n");
    }

}

void DeleteMiddleNode(void)
{
    int pos = 0, i = 0;

    struct Node *prev_DM = NULL,
                *trav = NULL;
    
    if (start_DM == NULL)
    {
        printf("Linked list is Empty!!!\n\n");
    }
    else
    {
        printf("Please Enter the Position : ");
        scanf("%d", &pos);
        printf("\n");

        trav = start_DM;
        while (i != pos - 1 && trav->Next_DM != start_DM)
        {
            prev_DM = trav;
            trav = trav->Next_DM;
            i++;
        }

        if (trav->Next_DM == start_DM)
        {
            prev_DM->Next_DM = start_DM;
            start_DM->prev_DM = prev_DM;
            last_DM = prev_DM;
            free(trav);
            printf("Deletion is Done !!!\n\n");
        }

        prev_DM->Next_DM = trav->Next_DM;
        trav->Next_DM->prev_DM = prev_DM;
        free(trav);
        printf("Given Node is Deleted.!!\n");
    }
}

void DeleteLastNode(void)
{
    struct Node *prev_DM = NULL,
                *trav = NULL;

    trav = start_DM;
    while (trav->Next_DM != start_DM)
    {
        prev_DM = trav;
        trav = trav->Next_DM;
        if (trav->Next_DM == start_DM)
        {
            prev_DM->Next_DM = start_DM;
            start_DM->prev_DM = prev_DM;
            last_DM = prev_DM;
            free(trav);
            printf("Deletion is Done !!!\n\n");
            break;
        }
    }
}

//Traversing 
void TraverseFromBeginning(void)
{
    struct Node *temp_DM;
    printf("The Linked List is : \n\n");

    temp_DM = start_DM;
    printf("\t");
    if (start_DM == NULL)
    {
        printf("List is Empty!!!\n\n");
    }
    else
    {
        while(temp_DM->Next_DM != start_DM)
        {
           printf("%d\t",temp_DM->data_DM);
            temp_DM = temp_DM->Next_DM;
        }
        printf("%d",temp_DM->data_DM);   
        printf("\n");
    }
}

void TraverseFromLast(void)
{
    struct Node *temp_DM;
    
    printf("The Linked List is : \n\n");
    temp_DM = last_DM;
    printf("\t");
    if (last_DM == NULL)
    {
        printf("List os Empty!!!\n\n");
    }
    else
    {
        while(temp_DM->prev_DM != last_DM)
        {   
           printf("%d\t",temp_DM->data_DM);
            temp_DM = temp_DM->prev_DM;
        }    
        printf("%d",temp_DM->data_DM);
        printf("\n");
    }
}