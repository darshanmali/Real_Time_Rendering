#include<stdio.h>

int n = 10;

void test(void);

int main()
{
	int m = 10;	
	test();
	return(0);
}

void test(void)
{
	int * p = NULL;

	free(p);
	p = NULL;
}