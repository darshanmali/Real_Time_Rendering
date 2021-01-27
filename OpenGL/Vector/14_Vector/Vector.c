#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

#define SUCCESS		1

struct vector
{
	int* a;
	int n;
};

struct vector* create_vector();
int push_back(struct vector* p_vec,int new_element);
int destroy_vector(struct vector* p_vec);

int main(void)
{
	int data;
	int choice;
	int i;
	struct vector* p_kb_vec = NULL;

	p_kb_vec = create_vector();

	do
	{
		printf("Enter an integer :");
		scanf("%d",&data);
		push_back(p_kb_vec,data);
		printf("Wanna countinue?[1-> [Every else-No]:");
		scanf("%d", &choice);

	}while(choice ==1);

	for (int i = 0; i < p_kb_vec->n; ++i)
	{
		printf("KBD:%d:%d\n",i,p_kb_vec->a[i]);
	}

	destroy_vector(p_kb_vec);
	p_kb_vec = NULL;

	return(EXIT_SUCCESS);

}


struct vector* create_vector()
{
	struct vector* p_vec = NULL;

	p_vec = (struct vector*)malloc(sizeof(struct vector));
	assert(p_vec != NULL);

	p_vec->a = NULL;
	p_vec->n = 0;
	return(p_vec);

}

int push_back(struct vector* p_vec,int new_element)
{
	p_vec->a = (int *)realloc(p_vec->a,(p_vec->n + 1) * sizeof(int));
	p_vec->n = p_vec->n + 1;
	p_vec->a[p_vec->n - 1] = new_element;
	return(SUCCESS);
}



int destroy_vector(struct vector* p_vec)
{
	free(p_vec->a);
	free(p_vec);
}

