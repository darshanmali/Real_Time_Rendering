#include<stdio.h>
#include<stdlib.h>
#include<string.h>


struct Point3d{
	float x;
	float y;
	float z;
};

struct Point3d* create_Point3d(float init_x, float init_y, float init_z);
float get_point3d_x(struct Point3d* p);
float get_point3d_y(struct Point3d* p);
float get_point3d_z(struct Point3d* p);

void set_point3d_x(struct Point3d* p, float new_x);
void set_point3d_y(struct Point3d* p, float new_y);
void set_point3d_z(struct Point3d* p, float new_z);
void destroy_point3d(struct Point3d *p);

double getDistance(struct Point3d* p_p1,struct Point3d* p_p2);

int main()
{
	struct Point3d * P_p1 = NULL;
	float p1_x, p1_y, p1_z;

	P_p1 = create_Point3d( 1.1f, 2.2f, 3.3f);

	p1_x = get_point3d_x(P_p1);
	p1_y = get_point3d_y(P_p1);
	p1_z = get_point3d_z(P_p1);

	printf("p1_x = %f \tp1_y = %f \tp1_z = %f \n", p1_x, p1_y, p1_z);

	set_point3d_x(P_p1, -1.1f);
	set_point3d_y(P_p1, -2.2f);
	set_point3d_z(P_p1, -3.3f);

	p1_x = get_point3d_x(P_p1);
	p1_y = get_point3d_y(P_p1);
	p1_z = get_point3d_z(P_p1);		

	printf("p1_x = %f\tp1_y = %f\tp1_z = %f\n", p1_x, p1_y, p1_z);

	destroy_point3d(P_p1);
	P_p1 = NULL;

}

struct Point3d* create_Point3d(float init_x,float init_y,float init_z)
{
	struct Point3d* p_new = NULL;

	p_new = (struct Point3d *)malloc(sizeof(struct Point3d));

	if(p_new == NULL)
	{
		printf("Insufficient Memory\n");
		exit(-1);
	}

	memset(p_new, 0, sizeof(struct Point3d));
	p_new->x = init_x;
	p_new->y = init_y;
	p_new->z = init_z;

	return(p_new);
}

float get_point3d_x(struct Point3d* p)
{
	return(p->x);	
}

float get_point3d_y(struct Point3d* p)
{
	return(p->y);	
}

float get_point3d_z(struct Point3d* p)
{
	return(p->z);
}

void set_point3d_x(struct Point3d* p, float new_x)
{
	p->x = new_x;
}

void set_point3d_y(struct Point3d* p, float new_y)
{
	p->y = new_y;	
}

void set_point3d_z(struct Point3d* p, float new_z)
{
	p->z = new_z;
}

double getDistance(struct Point3d * p_p1,struct Point3d * p_p2)
{
	float x, y, z;

	x = p_p1->x - p_p2->x;
	y = p_p1->y - p_p2->y;
	z = p_p1->z - p_p2->z;

	return sqrt(x * x + y * y + z * z);
}

void destroy_point3d(struct Point3d *p)
{
	free(p);
	p = NULL;
}

