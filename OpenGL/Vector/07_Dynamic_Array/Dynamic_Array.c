#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

#define SUCCESS						1
#define ARRAY_INDEX_OUT_OF_BOUND	2

#define ARR_SIZE	10

typedef int ret_t;

struct dynamic_array
{
	int *p_arr;
	int size;
};

struct dynamic_array* create_dynamic_array(int arr_size);

int get_element_at_index(struct dynamic_array* p_dynamic_array, int index, int* p_element);

int set_element_at_index(struct dynamic_array* p_dynamic_array, int index, int new_element);

int destroy_dynamic_array(struct dynamic_array* p_dynamic_array);

int main(void)
{
	struct dynamic_array* p_dynamic_array = NULL;

	int i;
	int data;
	ret_t return_value;

	p_dynamic_array = create_dynamic_array(ARR_SIZE);
	for (int i = 0; i < ARR_SIZE; ++i)
	{
		return_value = set_element_at_index(p_dynamic_array, i,(i+1)*10);
		assert(return_value == SUCCESS);
	}
	for (int i = 0; i < ARR_SIZE; ++i)
	{
		return_value = get_element_at_index(p_dynamic_array, i, &data);
		assert(return_value == SUCCESS);
		printf("Data at index %d : %d\n",i, data);
	}
	
	return_value = destroy_dynamic_array(p_dynamic_array);
	assert(return_value == SUCCESS);
	p_dynamic_array = NULL;

	return(EXIT_SUCCESS);

}

struct dynamic_array* create_dynamic_array(int arr_size)
{
	struct dynamic_array* p_dynamic_array = NULL;

	p_dynamic_array = (struct dynamic_array*)malloc(sizeof(struct dynamic_array));

	if(p_dynamic_array == NULL)
	{
		printf("Insufficient memory\n");
		exit(-1);
	}
	if(arr_size < 0)
	{
		printf("Bad size for array\n");
		exit(-1);
	}

	p_dynamic_array->size = arr_size;
	p_dynamic_array->p_arr = (int *)malloc(sizeof(int *) * arr_size);
	if(p_dynamic_array->p_arr == NULL)
	{
		puts("Insufficient memory\n");
		exit(-1);
	}
	return(p_dynamic_array);
}

int get_element_at_index(struct dynamic_array* p_dynamic_array,int index, int* p_element)
{
	if(index < 0 || index>= p_dynamic_array->size)
		return(ARRAY_INDEX_OUT_OF_BOUND);
	*p_element = p_dynamic_array->p_arr[index];
	return(SUCCESS);
}

int set_element_at_index(struct dynamic_array* p_dynamic_array,int index, int new_element)
{
	if(index < 0 || index >= p_dynamic_array->size)
		return(ARRAY_INDEX_OUT_OF_BOUND);
	p_dynamic_array->p_arr[index] = new_element;
	return(SUCCESS);
}



int destroy_dynamic_array(struct dynamic_array* p_dynamic_array)
{
	free(p_dynamic_array->p_arr);
	free(p_dynamic_array);
	return(SUCCESS);
}

