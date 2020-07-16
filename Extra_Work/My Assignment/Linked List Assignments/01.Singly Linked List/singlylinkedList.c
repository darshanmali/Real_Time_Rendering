//Header Files
#include<stdio.h>

//Global declaration
struct MySingly
{
    int data;
    struct MySingly *Next;
} *start = NULL;


//main Start
int main(int argc, char *argv[], char *envp[])
{
    //Function Declaration

    //Create Node
    struct MySingly* CreateNode(void);

    //Insrtion Section
    void Insert(void);
   
    //Deletion Section
    void Delete(void);
    

    //Traverse Section
    void Traverse(void);

    //Exit
    void End(void);

    //Variable Declaration
    struct Mysingly *list;
    int i_D, ch_D;

    
    while(1)
    {
        printf("\n\n");
        printf("Choose the Following option : \n\n");
        printf("1.Insertion.\n");
        printf("2.Deletion. \n");
        printf("3.Traverse. \n");
        printf("0.Exit.\n\n");
        printf("Please Enter the Choice : ");
        scanf("%d",&ch_D);
        
        switch (ch_D)
        {
        case 0:
            End();
            break;
        case 1:
            Insert();
            break;
        case 2:
            Delete();
            break;
        case 3:
            Traverse();
            break;
        default:
        printf("You have given the wrong choise : \n\n");
            break;
        }

    }    

}

struct MySingly* CreateNode(void)
{
    struct MySingly *temp = NULL;

    temp = (struct MySingly *)malloc(sizeof(struct MySingly));
    return(temp);
}

void End()
{
    exit(0);
}
void Insert()
{
    //Local Function
    void InsertAtStart(void);
    void InsertAtMiddle(void);
    void InsertAtLast(void);

    //Local Declaration
    int ch_D = 0;
    printf("Please Enter the Choice for Insertion : \n");
    printf("1.Insert At Begining.\n");
    printf("2.Insert At Middle. \n");
    printf("3.Insert At End.\n");
    scanf("%d",&ch_D);
    printf("\n\n");

    switch (ch_D)
    {
    case 1:
        InsertAtStart();
        break;
    case 2:
        InsertAtMiddle();
        break;
    case 3:
        InsertAtLast();
        break;
    
    default:
        printf("Please Give the Right choice !!\n\n");
        break;
    }

}
void InsertAtStart()
{
    struct MySingly *node;

    node = CreateNode();

    printf("Enter the Node Data : \n");
    scanf("%d",&node->data);
    node->Next = NULL;

    if (start == NULL)
    {
        start = node;
        printf("Insertion At begining Is Done.\n\n");
    }
    else
    {
        node->Next = start;
        start = node;

    }

}

void InsertAtMiddle()
{
    struct MySingly *node, *temp, *prev;
    int pos, i = 0;

    if (start == NULL)
    {
        printf("Your List Is NULL.!!!\n");
        exit(0);
    }
    else
    {
        printf("Enter the Position where you want to the Node : \n");
        scanf("%d",&pos);
        printf("\n");
        
        node = CreateNode();
        printf("Enter the Node Data : ");
        scanf("%d",&node->data);
        node->Next = NULL;

        temp = start;
        while(i < (pos - 1) && temp->Next != NULL)
        {
            prev = temp;
            temp = temp->Next;

            i++;
        }
        if(temp->Next == NULL)
        {
            printf("Given Choice is does not Exist, that is why adding at last ...!!!!!");
            temp->Next = node;
        }
        else
        {
            prev->Next = node;
            node->Next = temp;
        }
        
    }
    
}

void InsertAtLast()
{
    struct MySingly *node, *temp;

    node = CreateNode();

    printf("Enter the Node Data : \n");
    scanf("%d",&node->data);
    node->Next = NULL;

    if (start == NULL)
    {
        start = node;
        printf("Insertion At begining Is Done.\n\n");
    }
    else
    {
        temp = start;
        while (temp->Next != NULL)
        {
            temp = temp->Next;
        }
        temp->Next = node;

        printf("Insertion is done At last. !!!\n\n");
        
    }
    
}

void Delete()
{
    // Local Function
    void DeleteFirstNode(void);
    void DeleteMiddleNode(void);
    void DeleteLastNode(void);

    int ch_D = 0;
    printf("Please Enter the Choice for Deletion : \n");
    printf("1.Delete At Begining.\n");
    printf("2.Delete At Middle. \n");
    printf("3.Delete At End.\n");
    scanf("%d",&ch_D);

    printf("\n\n");

    switch (ch_D)
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
        printf("Please Give the Right choice !!\n\n");
        break;
    }

}

void DeleteFirstNode()
{
    struct MySingly *temp;
    temp = start;

    start = temp->Next;
    free(temp);
    printf("First Node is Deleted ..!!!\n\n");

}
void DeleteMiddleNode()
{
    struct MySingly *temp, *prev;
    int pos = 0;
    int i = 0;

    temp = start;
    printf("Please Enter The Position You want to Delete : ");
    scanf("%d", &pos);

    while ((i < (pos - 1)) && (temp->Next != NULL))
    {
        prev = temp;
        temp = temp->Next;
        i++;
    }
    prev->Next = temp->Next;
    free(temp);
    printf("As Given \' %d \'Node is Deleted ..!!!\n\n", i + 1);
    
}

void DeleteLastNode()
{
    struct MySingly *temp;
    temp = start;
    while (temp->Next != NULL)
    {
        temp = temp->Next;
        if(temp->Next->Next == NULL)
            break;
    }
    free(temp->Next->Next);
    temp->Next =NULL;
    printf("Deletion of last Node Is Done. !!!!\n\n");

}

void Traverse()
{
    struct MySingly *temp;

    temp = start;
    while(temp != NULL)
    {
        printf("%d \t", temp->data);
        temp = temp->Next;
    }

}
