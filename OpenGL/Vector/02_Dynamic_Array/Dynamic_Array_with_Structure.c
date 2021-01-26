#include<stdio.h>
#include<stdlib.h>

#define MAX_ARRAY_SIZE 10

struct Dynamic_Array{

	int* Dynamic_arr;
	int size;
};

struct Dynamic_Array* Create_Array(int );
void Set_Dynamic_Array(struct Dynamic_Array*);
void Get_Dynamic_Array(struct Dynamic_Array*);

int main(int argv, char* argc[])
{
	struct Dynamic_Array* p_array = NULL;

	printf("\n");
	p_array = Create_Array(MAX_ARRAY_SIZE);

	Set_Dynamic_Array(p_array);

	Get_Dynamic_Array(p_array);
	return(0);	
}

struct Dynamic_Array* Create_Array(int size)
{
	struct Dynamic_Array * ptr_Array = NULL;

	ptr_Array = (struct Dynamic_Array *)malloc(size * sizeof(struct Dynamic_Array));
	if(ptr_Array == NULL)
	{
		printf("insufficient Memory!\n");
		exit(1);
	}


	if(size < 0)
	{
		printf(" Size of Array is not currect.\n");
		exit(1);
	}

	ptr_Array->size = size;
	ptr_Array->Dynamic_arr = (int *)malloc(size * sizeof(int));
	if(ptr_Array->Dynamic_arr == NULL)
	{
		printf("Insufficient Memory!\n");
		exit(1);
	}
	return (ptr_Array);

}


void Set_Dynamic_Array(struct Dynamic_Array* ptr)
{
	for(int i = 0; i < ptr->size; i++)
	{
		ptr->Dynamic_arr[i] = (i + 1) * 100;
	}
}

void Get_Dynamic_Array(struct Dynamic_Array* ptr)
{
	for(int i = 0; i < ptr->size; i++)
	{
		printf("Array[%d] : %d\n", (i + 1), ptr->Dynamic_arr[i]);
	}
	printf("\nThank you!!!\n");
}

