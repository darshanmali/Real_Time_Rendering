#include<stdio.h>
#include<stdlib.h>

int main()
{
	int data;
	int choice;

	do
	{
		printf("Enter an integer :");
		scanf("%d", &data);
		printf("Entered data = %d\n",data);
		printf("Do you wantto enter yet another integer : [1-> yes|No otherwise]:");

		scanf("%d",&choice);
	}while(choice == 1);

	return(EXIT_SUCCESS);
}

