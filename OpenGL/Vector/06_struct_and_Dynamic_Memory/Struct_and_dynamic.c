#include<stdio.h>
#include<stdlib.h>

struct Point3d{
	float x;
	float y;
	float z;
};

int main()
{
	struct Point3d* p1 = NULL;
	float f1, f2, f3;

	p1 = (struct Point3d *)malloc(sizeof(struct Point3d));
	if(p1 == NULL)
	{
		printf("Error\n");
		exit(-1);
	}

	p1->x = 1.1f;
	p1->y = 2.2f;
	p1->z = 3.3f;

	f1 = p1->x;
	f2 = p1->y;
	f3 = p1->z;

	printf("\nVertex(%.2f,%.2f,%.2f)\n", f1, f2, f3);

	free(p1);
	p1 = NULL;

	return(0);
}


