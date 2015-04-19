#include <windows.h>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
using namespace std;

#define PI (3.141592653589793)
static GLdouble i = 600.0;
static GLdouble duet_ball_angle = 0.0;
static GLdouble alpha = 1.0;

typedef struct
{
	double x_center;
	double y_center;
	double x_clicked_center;
	double y_clicked_center;
	int radius;
} ball;

typedef struct
{
	//down left
	double xdl;
	double ydl;
	//up right
	double xur;
	double yur;
	//down right
	double xdr;
	double ydr;
	//up left
	double xul;
	double yul;
} falling_object;

ball red_ball; //20 20 30 30
ball blue_ball; //-30 -30 -20 -20
falling_object fo;

void create_ball(double x, double y, int radius, ball &b) {
	b.x_center = x;
	b.y_center = y;
	b.radius = radius;

	int triangleAmount = 20; //# of triangles used to draw circle
	GLfloat twicePi = 2.0f * PI;
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x, y); // center of circle
	for (int point = 0; point <= triangleAmount; point++) {
		glVertex2f(
			x + (radius * cos(point *  twicePi / triangleAmount)),
			y + (radius * sin(point * twicePi / triangleAmount))
			);
	}
	glEnd();
}

void create_falling_object(double x_dl, double y_dl, double x_ur, double y_ur, falling_object &fo) {
	fo.xdl = x_dl;
	fo.ydl = y_dl;

	fo.xur = x_ur;
	fo.yur = y_ur;

	fo.xdr = x_ur;
	fo.ydr = y_ur;

	fo.xul = x_dl;
	fo.yul = y_dl;

	glRecti(fo.xdl, fo.ydl, fo.xur, fo.yur);
}

void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	gluOrtho2D(0.0, 800.0, 0.0, 600.0);
	glShadeModel(GL_FLAT);
}

void scena(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	//Red_ball
	glPushMatrix();
	glTranslated(350.0, 250.0, 0.0);
	glPushMatrix();
	glRotated(duet_ball_angle, 0.0, 0.0, 1.0);
	glColor3f(1.0, 0.0, 0.0);
	//glRecti(20, 20, 30, 30); //Initial recti, but changed to ball 
	create_ball(25, 0, 5, red_ball);
	glPopMatrix();
	glPopMatrix();

	//Blue_ball
	glPushMatrix();
	glTranslated(350.0, 250.0, 0.0);
	glPushMatrix();
	glRotated(duet_ball_angle, 0.0, 0.0, 1.0);
	glColor3f(0.0, 0.0, 1.0);
	//glRecti(-30, -30, -20, -20); //Initial recti, but changed to ball 
	create_ball(-25, 0, 5, blue_ball);
	glPopMatrix();
	glPopMatrix();

	//Falling Object
	glPushMatrix();
	glTranslated(375.0, i, 0.0);
	glColor3f(1.0, 0.0, 1.0);
	//glRecti(-15, -15, 15, 5); //changed to void function
	create_falling_object(-15,-15,15,15,fo);
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

void falling_objects(void)
{
	i -= 0.1;
	if (i < 280 && i > 220)
	{
		double dl, ul, dr, ur;
		dl = sqrt(
			(fo.xdl - red_ball.x_clicked_center)*(fo.xdl - red_ball.x_clicked_center) +
			(fo.ydl - red_ball.y_clicked_center)*(fo.ydl - red_ball.y_clicked_center)
			);
		if (dl < 5)
		{
			cout << "Game Over";
			exit(0);
		}

		dl = sqrt(
			(fo.xdl - blue_ball.x_clicked_center)*(fo.xdl - blue_ball.x_clicked_center) +
			(fo.ydl - blue_ball.y_clicked_center)*(fo.ydl - blue_ball.y_clicked_center)
			);
		if (dl < 5)
		{
			cout << "Game Over";
			exit(0);
		}
	}
	glutPostRedisplay();
}

void falling_objects_click_left(void)
{
	duet_ball_angle -= 0.1;
	red_ball.x_clicked_center = cos(duet_ball_angle) * red_ball.x_center;
	red_ball.y_clicked_center = cos(duet_ball_angle) * red_ball.x_center;
	blue_ball.x_clicked_center = - red_ball.x_clicked_center;
	blue_ball.y_clicked_center = - red_ball.x_clicked_center;

	i -= 0.1;
	if (i < 280 && i > 220)
	{
		double dl, ul, dr, ur;
		dl = sqrt( 
				(fo.xdl - red_ball.x_clicked_center)*(fo.xdl - red_ball.x_clicked_center) +
				(fo.ydl - red_ball.y_clicked_center)*(fo.ydl - red_ball.y_clicked_center)
			);
		if (dl < 5)
		{
			cout << "Game Over";
			exit(0);
		}

		dl = sqrt(
			(fo.xdl - blue_ball.x_clicked_center)*(fo.xdl - blue_ball.x_clicked_center) +
			(fo.ydl - blue_ball.y_clicked_center)*(fo.ydl - blue_ball.y_clicked_center)
			);
		if (dl < 5)
		{
			cout << "Game Over";
			exit(0);
		}
	}
	glutPostRedisplay();
}

void falling_objects_click_right(void)
{
	duet_ball_angle += 0.1;
	red_ball.x_clicked_center = cos(duet_ball_angle) * red_ball.x_center;
	red_ball.y_clicked_center = cos(duet_ball_angle) * red_ball.x_center;
	blue_ball.x_clicked_center = -red_ball.x_clicked_center;
	blue_ball.y_clicked_center = -red_ball.x_clicked_center;

	i -= 0.1;
	if (i < 280 && i > 220)
	{
		double dl, ul, dr, ur;
		dl = sqrt(
			(fo.xdl - red_ball.x_clicked_center)*(fo.xdl - red_ball.x_clicked_center) +
			(fo.ydl - red_ball.y_clicked_center)*(fo.ydl - red_ball.y_clicked_center)
			);
		if (dl < 5)
		{
			cout << "Game Over";
			exit(0);
		}

		dl = sqrt(
			(fo.xdl - blue_ball.x_clicked_center)*(fo.xdl - blue_ball.x_clicked_center) +
			(fo.ydl - blue_ball.y_clicked_center)*(fo.ydl - blue_ball.y_clicked_center)
			);
		if (dl < 5)
		{
			cout << "Game Over";
			exit(0);
		}
	}
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
			glutIdleFunc(falling_objects_click_left);
		else
			if (state == GLUT_UP)
				glutIdleFunc(falling_objects);
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)
			glutIdleFunc(falling_objects_click_right);
		else
			if (state == GLUT_UP)
				glutIdleFunc(falling_objects);
		break;
	}
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("DuetGL");
	init();
	glutDisplayFunc(scena);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMainLoop();
}