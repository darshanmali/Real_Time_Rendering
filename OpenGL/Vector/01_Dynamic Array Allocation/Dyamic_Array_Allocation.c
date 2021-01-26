#include<stdio.h>
#include<stdlib.h>

#define MAX_ARRAY_SIZE 10

int Array_DM[MAX_ARRAY_SIZE];

void read_write_onStatic_array();
void read_write_onDynamic_array();

int main()
{
	read_write_onStatic_array();
	read_write_onDynamic_array();
	return(0);
}

void read_write_onStatic_array()
{
	printf("This is Static Array:\n");
	for(int i = 0; i < MAX_ARRAY_SIZE; i++)
	{
		Array_DM[i] = (i + 1) * 100;
	}
	printf("\n");

	for(int i = 0; i < MAX_ARRAY_SIZE; i++)
	{
		printf("%d\t",Array_DM[i]);
	}
	printf("\n\n");
}

void read_write_onDynamic_array()
{
	printf("\n");

	printf("This is Dynamic Array:\n");

	int* Dynamic_Array_ptr = NULL;

	Dynamic_Array_ptr = (int *)malloc( MAX_ARRAY_SIZE * sizeof(int));

	for(int j = 0; j < MAX_ARRAY_SIZE; j++)
	{
		Dynamic_Array_ptr[j] = (j + 1) * 200;
	}

	printf("\n");

	for(int j = 0; j < MAX_ARRAY_SIZE; j++)
	{
		printf("Array[%d] : %d\n",(j + 1),Dynamic_Array_ptr[j]);
	}
	printf("\n");
	
	printf("Thank you!\n");
}

