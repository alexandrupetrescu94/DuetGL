#include<windows.h>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
using namespace std;

static GLdouble i = 600.0;
static GLdouble duet_ball_angle = 0.0;
static GLdouble alpha = 1.0;

void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	gluOrtho2D(0.0, 800.0, 0.0, 600.0);
	glShadeModel(GL_FLAT);
}
void dreptunghi(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();
	glTranslated(350.0, 250.0, 0.0);
	glPushMatrix();
	glRotated(duet_ball_angle, 0.0, 0.0, 1.0);
	glColor3f(1.0, 0.0, 0.0);
	glRecti(20, 20, 30, 30);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslated(350.0, 250.0, 0.0);
	glPushMatrix();
	glRotated(duet_ball_angle, 0.0, 0.0, 1.0);
	glColor3f(0.0, 0.0, 1.0);
	glRecti(-30, -30, -20, -20);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslated(375.0, i, 0.0);
	glColor3f(1.0, 0.0, 1.0);
	glRecti(-15, -15, 15, 5);
	glPopMatrix();

	glutSwapBuffers();
	glFlush();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-20.0, 780.0, 0.0, 600.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void miscad()
{
	duet_ball_angle += 0.1;
}

void miscas()
{
	duet_ball_angle -= 0.1;
	cout << "aa";
}

void falling_objects(void)
{
	i -= 0.1;
	//if (i < 280 && i > 220)
	//	cout << i << " ";
	glutPostRedisplay();
}

void falling_objects_left(void)
{
	i -= 0.1;
	duet_ball_angle -= 0.1;
	//if (i < 280 && i > 220)
	//	cout << i << " ";
	glutPostRedisplay();
}

void falling_objects_right(void)
{
	i -= 0.1;
	duet_ball_angle += 0.1;
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
			glutIdleFunc(falling_objects_left);
		else
			if (state == GLUT_UP)
				glutIdleFunc(falling_objects);
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)
			glutIdleFunc(falling_objects_right);
		else
			if (state == GLUT_UP)
				glutIdleFunc(falling_objects);
		break;
	}
}


/*
*  Request double buffer display mode.
*  Register mouse input callback functions
*/
void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Dreptunghi cu satelit");
	init();
	glutDisplayFunc(dreptunghi);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMainLoop();
}