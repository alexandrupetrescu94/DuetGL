#include <windows.h>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
using namespace std;

#define PI (3.141592653589793)
static GLdouble i = 600.0;
static GLdouble duet_ball_angle = 0.0;
static bool collision_detected = false;


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

void create_falling_object(int x_dl, int y_dl, int x_ur, int y_ur, falling_object &fo) {
	fo.xdl = x_dl + 375;
	fo.ydl = y_dl;

	fo.xur = x_ur + 375;
	fo.yur = y_ur;

	fo.xdr = x_ur + 375;
	fo.ydr = y_ur;

	fo.xul = x_dl + 375;
	fo.yul = y_dl;

	glRecti(x_dl, y_dl, x_ur, y_ur);
}

void recalculate_ball_center() {
	double x, y, afterpoint;

	if (duet_ball_angle >= 0)
	{
		x = cos(duet_ball_angle*(PI/180)) * 25;
		y = sin(duet_ball_angle*(PI/180)) * 25;
	}
	else
	{
		afterpoint = 1 - (duet_ball_angle - floor(duet_ball_angle));
		x = cos(((360 + (int)floor(duet_ball_angle)) % 360 + afterpoint)*(PI / 180)) * 25;
		y = sin(((360 + (int)floor(duet_ball_angle)) % 360 + afterpoint)*(PI / 180)) * 25;
	}
	red_ball.x_clicked_center = x + 350;
	red_ball.y_clicked_center = y + 250;
	blue_ball.x_clicked_center = -x + 350;
	blue_ball.y_clicked_center = -y + 250;

}

void recalculate_falling_object() {
	fo.ydl += i;
	fo.yur += i;
	fo.ydr += i;
	fo.yul += i;
}

int collision_dl_checker(falling_object fo, ball b) {
	double radius;
	radius = sqrt(
			(fo.xdl - b.x_clicked_center)*(fo.xdl - b.x_clicked_center) +
			(fo.ydl - b.y_clicked_center)*(fo.ydl - b.y_clicked_center)
		);
	if (radius < 5) 
	{
		cout << "collision";
		return 0;
	}
	return 1;
}

int collision_ur_checker(falling_object fo, ball b) {
	double radius;
	radius = sqrt(
		(fo.xur - b.x_clicked_center)*(fo.xur - b.x_clicked_center) +
		(fo.yur - b.y_clicked_center)*(fo.yur - b.y_clicked_center)
		);

	if (radius < 5)
		return 0;

	return 1;
}

int collision_dr_checker(falling_object fo, ball b) {
	double radius;
	radius = sqrt(
			(fo.xdr - red_ball.x_clicked_center)*(fo.xdr - red_ball.x_clicked_center) +
			(fo.ydr - red_ball.y_clicked_center)*(fo.ydr - red_ball.y_clicked_center)
		);

	if (radius < 5)
		return 0;

	return 1;
}

int collision_ul_checker(falling_object fo, ball b) {
	double radius;
	radius = sqrt(
			(fo.xul - red_ball.x_clicked_center)*(fo.xul - red_ball.x_clicked_center) +
			(fo.yul - red_ball.y_clicked_center)*(fo.yul - red_ball.y_clicked_center)
		);
	if (radius < 5)
		return 0;

	return 1;
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
	if (!collision_detected)
	{
		recalculate_ball_center();
		recalculate_falling_object();

		i -= 0.1;
		if (i < 281 && i > 219)
		{
			//North
			if ( (fo.ydl <= red_ball.y_clicked_center + 5 && fo.xdl < red_ball.x_clicked_center && fo.xdr > red_ball.x_clicked_center) || 
				 (fo.ydl <= blue_ball.y_clicked_center + 5 && fo.xdl < blue_ball.x_clicked_center && fo.xdr > blue_ball.x_clicked_center) )
			{
				collision_detected = true;
				cout << "up coll";
				cout << "fo.ydl" << fo.ydl << endl;
				cout << "fo.xdl" << fo.xdl << endl;
				cout << "fo.xdr" << fo.xdr << endl;
				cout << "red_ball.x_clicked_center" << red_ball.x_clicked_center << endl;
				cout << "red_ball.y_clicked_center" << red_ball.y_clicked_center << endl;
				cout << "blue_ball.x_clicked_center" << blue_ball.x_clicked_center << endl;
				cout << "blue_ball.y_clicked_center" << blue_ball.y_clicked_center << endl;
			}

			//South
			if ((fo.yul >= red_ball.y_clicked_center - 5 && fo.xul < red_ball.x_clicked_center && fo.xur < red_ball.x_clicked_center) ||
				(fo.yul >= blue_ball.y_clicked_center - 5 && fo.xul < blue_ball.x_clicked_center && fo.xur < blue_ball.x_clicked_center))
			{
				collision_detected = true;
				cout << "down coll";
			}

			//WEST
			if ((fo.xur >= red_ball.x_clicked_center - 5 && fo.yur > red_ball.y_clicked_center && fo.ydr < red_ball.y_clicked_center) ||
				(fo.xur >= blue_ball.y_clicked_center - 5 && fo.yur > blue_ball.y_clicked_center && fo.ydr < blue_ball.y_clicked_center))
			{
				collision_detected = true;
				cout << "left coll";
			}

			//EAST
			if ( (fo.xdl <= red_ball.x_clicked_center + 5 && fo.yul > red_ball.y_clicked_center && fo.ydl < red_ball.y_clicked_center) || 
				 (fo.xdl <= blue_ball.y_clicked_center + 5 && fo.yul > red_ball.y_clicked_center && fo.ydl < red_ball.y_clicked_center) )
			{
				collision_detected = true;
				cout << "right coll";
			}
			//down left checker
			if (!(collision_dl_checker(fo, red_ball) && collision_dl_checker(fo, blue_ball)))
				collision_detected = true;
			else
				if (!(collision_ul_checker(fo, red_ball) && collision_ul_checker(fo, blue_ball)))
					collision_detected = true;
				else
					if (!(collision_dr_checker(fo, red_ball) && collision_dr_checker(fo, blue_ball)))
						collision_detected = true;
					else
						if (!(collision_dl_checker(fo, red_ball) && collision_dl_checker(fo, blue_ball)))
							collision_detected = true;
		}

		glutPostRedisplay();
	}
	else
	{
		i = 600;
		collision_detected = false;
	}
}

void falling_objects_click_left(void)
{
	if (!collision_detected)
	{
		duet_ball_angle += 0.1;
		recalculate_ball_center();
		recalculate_falling_object();

		i -= 0.1;
		if (i < 281 && i > 219)
		{

			if (fo.ydl <= red_ball.y_clicked_center + 5 || fo.ydl <= blue_ball.y_clicked_center + 5)
				collision_detected = true;
			if (fo.yul >= red_ball.y_clicked_center - 5 || fo.yul >= blue_ball.y_clicked_center - 5)
				collision_detected = true;
			if (fo.xur >= red_ball.x_clicked_center - 5 || fo.xur >= blue_ball.y_clicked_center - 5)
				collision_detected = true;
			if (fo.xdl <= red_ball.x_clicked_center + 5 || fo.xdl <= blue_ball.y_clicked_center + 5)
				collision_detected = true;

			//down left checker
			if (!(collision_dl_checker(fo, red_ball) && collision_dl_checker(fo, blue_ball)))
				collision_detected = true;
			else
				if (!(collision_ul_checker(fo, red_ball) && collision_ul_checker(fo, blue_ball)))
					collision_detected = true;
				else
					if (!(collision_dr_checker(fo, red_ball) && collision_dr_checker(fo, blue_ball)))
						collision_detected = true;
					else
						if (!(collision_dl_checker(fo, red_ball) && collision_dl_checker(fo, blue_ball)))
							collision_detected = true;
		}

		glutPostRedisplay();
	}
	else
	{
		i = 600;
		collision_detected = false;
	}
}

void falling_objects_click_right(void)
{
	if (!collision_detected)
	{
		duet_ball_angle -= 0.1;
		recalculate_ball_center();
		recalculate_falling_object();

		i -= 0.1;
		if (i < 281 && i > 219)
		{

			if (fo.ydl <= red_ball.y_clicked_center + 5 || fo.ydl <= blue_ball.y_clicked_center + 5)
				collision_detected = true;
			if (fo.yul >= red_ball.y_clicked_center - 5 || fo.yul >= blue_ball.y_clicked_center - 5)
				collision_detected = true;
			if (fo.xur >= red_ball.x_clicked_center - 5 || fo.xur >= blue_ball.y_clicked_center - 5)
				collision_detected = true;
			if (fo.xdl <= red_ball.x_clicked_center + 5 || fo.xdl <= blue_ball.y_clicked_center + 5)
				collision_detected = true;

			//down left checker
			if (!(collision_dl_checker(fo, red_ball) && collision_dl_checker(fo, blue_ball)))
				collision_detected = true;
			else
				if (!(collision_ul_checker(fo, red_ball) && collision_ul_checker(fo, blue_ball)))
					collision_detected = true;
				else
					if (!(collision_dr_checker(fo, red_ball) && collision_dr_checker(fo, blue_ball)))
						collision_detected = true;
					else
						if (!(collision_dl_checker(fo, red_ball) && collision_dl_checker(fo, blue_ball)))
							collision_detected = true;
		}

		glutPostRedisplay();
	}
	else
	{
		i = 600;
		collision_detected = false;
	}
}

void mouse(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
		{
			glutIdleFunc(falling_objects_click_left);
		}
		else
			if (state == GLUT_UP)
			{
				glutIdleFunc(falling_objects);
			}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)
		{
			glutIdleFunc(falling_objects_click_right);
		}
		else
			if (state == GLUT_UP)
			{
				glutIdleFunc(falling_objects);
			}
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