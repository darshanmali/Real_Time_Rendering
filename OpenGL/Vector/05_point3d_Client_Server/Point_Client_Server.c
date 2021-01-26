#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

struct Point3d{
	float x, y, z;
};

struct Point3d *create_point3d( float init_x, float init_y, float init_z);

float get_x(struct Point3d* p);
float get_y(struct Point3d* p);
float get_z(struct Point3d* p);

void set_x(struct Point3d* p, float new_x);
void set_y(struct Point3d* p, float new_y);
void set_z(struct Point3d* p, float new_z);

void destroy_point3d(struct Point3d *p);

int main()
{
	struct Point3d* p = NULL;
	float f1, f2, f3;

	p = create_point3d(1.1f, 2.2f, 3.3f);

	f1 = get_x(p);
	f2 = get_y(p);
	f3 = get_z(p);

	printf("vertex(%.2f, %.2f, %.2f);\n", f1, f2, f3);

	set_x(p, -1.1f);
	set_y(p, -2.2f);
	set_z(p, -3.3f);

	f1 = get_x(p);
	f2 = get_y(p);
	f3 = get_z(p);

	printf("vertex(%.2f, %.2f, %.2f);\n", f1, f2, f3);	

	destroy_point3d(p);
	p = NULL;

	return(0);

}

struct Point3d* create_point3d(float init_x, float init_y, float init_z)
{
	struct Point3d* p = malloc(sizeof(struct Point3d));
	assert(p);

	p->x = init_x;
	p->y = init_y;
	p->z = init_z;

	return(p);
}

float get_x(struct Point3d *p)
{
	return(p->x);
}

float get_y(struct Point3d *p)
{
	return(p->y);
}

float get_z(struct Point3d *p)
{
	return(p->z);
}

void set_x(struct Point3d* p, float new_x)
{
	p->x = new_x;
}

void set_y(struct Point3d* p, float new_y)
{
	p->y = new_y;
}

void set_z(struct Point3d* p, float new_z)
{
	p->z = new_z;
}

void destroy_point3d(struct Point3d* p)
{
	free(p);
	p = NULL;
}
