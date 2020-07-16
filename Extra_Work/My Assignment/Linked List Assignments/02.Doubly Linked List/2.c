#include<stdio.h>
struct Node
{
    struct Node *prev;
    int Data;
    struct Node *Next;
};

struct Node *start = NULL, *last = NULL;
void InsertNode();
void TraverseNode();
void DeleteNode();

//Inseert Function :
void InsertAtBeggining();
void InsertAtMiddle();
void InsertAtLast();

int main(void)
{
    
    struct Node* CreateNode();
    struct Node *trav;
    int ch = 0;
    while (1)
    {
        printf("1.Insert Node\n");
        printf("2.Traverse Node\n");
        printf("3.Delete Node\n");
        printf("4.Exit Node\n\n");
        printf("Plese Enter the Choice : \n");    
        scanf("%d",&ch);

        switch (ch)
        {
        case 1:
            InsertNode();
            break;
        case 2:
            TraverseNode();
        break;
        case 3:
            DeleteNode();
        break;
        case 4:
            exit(0);
        break;

        default:
            printf("You have given wrong Choice :");
            break;
        }
    }
    return (0);

}

struct Node* CreateNode()
{
    struct Node* temp;
    temp = (struct Node *)malloc(sizeof(struct Node));
    return(temp);
} 

void InsertNode()
{
    struct Node *temp;
    int ch = 0;

    printf("1.Insert At Beggining.\n");
    printf("2.Insert At Middle.\n");
    printf("3.Insert At Last.\n");
    printf("Please give your Choice : \n");
    scanf("%d",&ch);
    switch (ch)
    {
    case 1:
        InsertAtBeggining();
    break;
    case 2:
        InsertAtMiddle();
    break;
    case 3:
        InsertAtLast();
    break;
    default:
        printf("You have given wrong value for Insert Option !\n");
        break;
    }
}
void InsertAtBeggining()
{
    struct Node *temp;

    temp = CreateNode();
    printf("\n");
    printf("Please Enter the Data : ");
    scanf("%d", &temp->Data);
    temp->Next = NULL;
    temp->prev = NULL;

    if(start == NULL)
    {
        start = temp;
        last = temp;
        printf("\n");
        printf(" Insertion Is Done at start Point!\n");
    }
    else
    {
        temp->Next = start;
        start->prev = temp;
        start = temp;
        printf("\n");
        printf(" Insertion Is Done at start Point !\n");
    }

}

void InsertAtMiddle()
{
    struct Node *temp;
    struct Node *trav;

    int Pos = 0, i = 1 ,check = 0, count = 0;

    temp = CreateNode();

    printf("\n\n");
    printf("Please Enter the Data : ");
    scanf("%d", &temp->Data);
    temp->Next = NULL;
    temp->prev = NULL;
    printf("Please Give the Position For Node To be Added : \t");
    scanf("%d", &Pos);

    trav = start;
    
    while (i < Pos - 1 && trav != NULL)
    {
        trav = trav->Next;
        i++;
    }

    if (trav != NULL)
    {

        temp->prev = trav;
        temp->Next = trav->Next;
        trav->Next->prev = temp;
        trav->Next = temp;
        printf("\n\n");
        printf("Insertion Is Done At given Position %d : \n", Pos);
    }
    else
    {

        printf("Length of Linked list : %d \n Please Enter the value between 1 to %d.\n\n", count, count);
    }
    
}

void InsertAtLast()
{
    struct Node *temp;
    temp = CreateNode();
    printf("Please Enter the Data : ");
    scanf("%d", &temp->Data);
    temp->Next = NULL;
    temp->prev = NULL;
    if(start == NULL)
    {
        start = temp;
        last = temp;

        printf(" Insertion Is Done at start Point!\n");
    }
    else
    {
        struct Node *trav;
        trav = start;
        while (trav->Next != NULL)
        {
            trav = trav->Next;
        }
        last = temp; 
        trav->Next = temp;
        temp->prev = trav;
       
        printf(" Insertion Is Done !\n");
    }


}

        
    
void TraverseNode()
{
    int ch; 
    struct Node *trav;

    printf("\n\n");
    printf("1.For Start.\n");
    printf("2.For Last.\n");
    scanf("%d",&ch);
    switch (ch)
    {
    case 1:

        trav = start;
        printf("Traversing list from front side !\n\n");
        while (trav != NULL)
        {
            printf("Data Is : %d And Address Of Prev IS : %p Address of next is : %p\n\n",trav->Data, trav->prev, trav->Next);
            trav = trav->Next;
        }
        break;
    case 2:
        trav = last;
        printf("\n\n");
        printf("Traversing list from Back side !\n\n");
        while (trav != NULL)
        {
            printf("Data Is : %d And Address Of Prev IS : %p Address of next is : %p\n\n",trav->Data, trav->prev, trav->Next);
            trav = trav->prev;
        }

        break;
    default:
        break;
    }
    
}


void DeleteNode()
{
    struct Node *trav, *prev;
    
    int ch; 

    printf("\n\n");
    printf("1.Delete Start.\n");
    printf("2.Delete Last.\n");
    scanf("%d",&ch);

    switch (ch)
    {
    case 1:
        trav = last;
        if (last == NULL)
        {
            printf("Linked List is Empty :\n");
        }
        else
        {
            while (trav != NULL)
            {
                prev = trav;
                if (trav->prev == NULL)
                trav = trav->prev;
                {
                    start = prev;    
                    prev->prev = NULL;
                    free(trav);       
                    printf("Deletion is Successful\n");
                    break;
                }
            }    
        }
        break;

    case 2:
        trav = start;
        if (start == NULL)
        {
            printf("Linked List is Empty :\n");
        }
        else
        {
            while (trav != NULL)
            {
                prev = trav;
                if (trav->Next == NULL)
                trav = trav->Next;
                {
                    last = prev;    
                    prev->Next = NULL;
                    free(trav);       
                    printf("Deletion is Successful\n");
                    break;
                }
            
            }
        }
        break;
    default:
        break;
    }
}

