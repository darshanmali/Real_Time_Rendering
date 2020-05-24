#include<GL/freeglut.h>
#include<GL/glut.h>
#include<stdio.h>
#include<math.h>

#define PI 3.141592653589793238
bool bFullScreen = false;

int main(int argc, char** argv)
{
	//code
	//Our Function Declaration
	void initialize(void);
	void resize(int, int);
	void display(void);
	void keyboard(unsigned char, int, int);
	void mouse(int, int, int, int);
	void uninitialize(void);


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("GLUT:Darshan");


	//System Function Calling
	initialize();
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutCloseFunc(uninitialize);
	glutMainLoop();

	return(0);
}

void initialize(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void resize(int widght, int height)
{
	if (height <= 0)
	{
		height = 1;
	}
	glViewport(0, 0, (GLsizei)widght, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

void display(void)
{

	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glBegin(GL_POINTS);

	// CIRCLE's CENTER
	glVertex3f(0.0f, 0.0f, 0.0f);
	
    for(float i = 0.1; i < 2*PI*0.2f; i++)
    {

	    glColor3f(10.0f, 0.10f, 3.0f);
		glVertex3f(cos(i)*0.2f, sin(i)*0.2f, 0.0f);

    }

	glEnd();
	glFlush();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		glutLeaveMainLoop();
		break;

	case 'e':
	case 'E':
		glRotatef(45.0, 1.0, 0.0, 0.0);
		break;

	case 'F':
	case 'f':
		if (bFullScreen == false)
		{
			glutFullScreen();
			bFullScreen = true;

		}
		else
		{
			glutLeaveFullScreen();
			bFullScreen = false;
		}
		break;


	default:
		break;
	}
}

void mouse(int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		break;
	case GLUT_RIGHT_BUTTON:
		glutLeaveMainLoop();
		break;
	default:
		break;
	}
}

void uninitialize(void)
{

}

