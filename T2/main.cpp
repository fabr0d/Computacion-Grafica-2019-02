#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "GL/glut.h"
using namespace std;

#define RED 0
#define GREEN 0
#define BLUE 0
#define ALPHA 1

#define ECHAP 27
void init_scene();
void render_scene();
GLvoid initGL();
GLvoid window_display();
GLvoid window_reshape(GLsizei width, GLsizei height);
GLvoid window_key(unsigned char key, int x, int y);

//function called on each frame
GLvoid window_idle();

int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);


	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("TP 2 : Transformaciones");


	initGL();
	init_scene();

	glutDisplayFunc(&window_display);

	glutReshapeFunc(&window_reshape);

	glutKeyboardFunc(&window_key);

	//function called on each frame
	glutIdleFunc(&window_idle);

	glutMainLoop();

	return 1;
}

void create_circle(double centerX, double centerY, double radius) {
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 100; i++)
	{
		double theta = 2.0 * 3.1415926 * double(i) / 100.0;
		double x = radius * cosf(theta);
		double y = radius * sinf(theta);
		glVertex2f(x + centerX, y + centerY);
	}
	glEnd();
}

GLvoid initGL()
{
	GLfloat position[] = { 0.0f, 5.0f, 10.0f, 0.0 };

	//enable light : try without it
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glEnable(GL_LIGHTING);
	//light 0 "on": try without it
	glEnable(GL_LIGHT0);

	//shading model : try GL_FLAT
	glShadeModel(GL_SMOOTH);

	glEnable(GL_DEPTH_TEST);

	//enable material : try without it
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	glClearColor(RED, GREEN, BLUE, ALPHA);
}

/*
float aglob = 0.0;
float bglob = -10.0;
float sum = 0.009;
*/
void rotacion_tetera_a(int a, int posX, double tam)
{
	glPushMatrix();
	glRotatef(a, 0, 0, 1);
	glTranslatef(posX, 0, 0);
	glutSolidTeapot(tam);
	glPopMatrix();
}

void rotacion_tetera_b1(int b, int a, int posX, double tam)
{
	glPushMatrix();
	create_circle(b, 0, 0.5);
	glTranslatef(b, 0, 0);
	glPushMatrix();

	glRotatef(a, 0, 0, 1);
	glTranslatef(posX, 0, 0);
	glutSolidTeapot(tam);
	glPopMatrix();

	glPopMatrix();
}

void rotacion_tetera_torus(int a, int posX, double tam)
{
	glPushMatrix();
	glRotatef(a, 0, 0, 1);
	glTranslatef(posX, 0, 0);
	glutSolidTeapot(tam);
	glPushMatrix();
	glRotatef(a, 0, 1, 0);
	glTranslatef(posX - 2, 0, 0);
	glutSolidTorus(0.5, 1.5, 10, 10);
	glPopMatrix();
	glPopMatrix();
}

double tierravel = 0.0;
double solvel = 0.0;
double velocidad_tierra_alrededor_sol = 0.05;
double velocidad_sol_sobre_sol = 0.07;

GLvoid window_display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);

	glBegin(GL_LINES);
	glVertex2f(-10, 0);
	glVertex2f(10, 0);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(0, -10);
	glVertex2f(0, 10);
	glEnd();

	glLoadIdentity();
	glOrtho(-25.0f, 25.0f, -25.0f, 25.0f, -25.0f, 25.0f);

	//tetera con eje rotando en x
	/*
	glPushMatrix();
	create_circle(bglob, 0, 0.5);
	glTranslatef(bglob, 0, 0);
	glPushMatrix();
	glRotatef(aglob, 0, 0, 1);
	glTranslatef(-10.0, 0, 0);
	glutSolidTeapot(2.0);
	glPopMatrix();
	glPopMatrix();
	*/
	///////////////////////////////

	//tetera con torus alrededor
	/*
	glPushMatrix();
	create_circle(bglob, 0, 0.5);
	glTranslatef(bglob, 0, 0);
	glPushMatrix();
	glRotatef(aglob, 0, 0, 1);
	glTranslatef(-10.0, 0, 0);
	glutSolidTeapot(2.0);
	glPushMatrix();
	glRotatef(aglob, 0, 1, 0);
	glTranslatef(-10.0 - 2, 0, 0);
	glutSolidTorus(0.5, 1.5, 10, 10);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	*/
	//////////////////////////////

	//tetera con torus y cubo
	/*
	glPushMatrix();
	create_circle(bglob, 0, 0.5);
	glTranslatef(bglob, 0, 0);

	glPushMatrix();
	glPushMatrix();
	glRotatef(aglob, 1, 0, 0);
	glTranslatef(0, -5, 0);
	glutSolidCube(2.0);
	glPopMatrix();
	glRotatef(aglob, 0, 0, 1);
	glTranslatef(-10, 0, 0);
	glutSolidTeapot(2.0);
	glPushMatrix();
	glRotatef(aglob, 0, 1, 0);
	glTranslatef(-8, 0, 0);
	glutSolidTorus(0.5, 1.5, 10, 10);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	*/
	//////////////////////////////
	/*
	aglob += 0.5;
	bglob = bglob + sum;

	if (bglob >= 10.0004)
	{
	sum = -0.01;
	}
	if (bglob <= -10.0)
	{
	sum = 0.01;
	}
	*/

	//////////////sistema solar /////////////////

	glPushMatrix();
		glRotatef(solvel, 0, 0, 1);
		glutSolidSphere(4, 8, 8);//sol
	glPopMatrix();

	glPushMatrix();
		glRotatef(tierravel, 0, 0, 1);
		glTranslatef(10, 0, 0);
		glPushMatrix();
			glRotatef(solvel * 3, 0, 0, 1);
			glutSolidSphere(2, 8, 8);//tierra
		glPopMatrix();
		glPushMatrix();
			glRotatef(tierravel * 2, 0, 0, 1);
			glTranslatef(4, 0, 0);
			glPushMatrix();
				glRotatef(solvel * 1.5, 0, 0, 1);
				glutSolidSphere(1, 8, 8);//luna
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();

	glPushMatrix();
		glRotatef(tierravel, 0, 0, 1);
		glTranslatef(18, 0, 0);
		glPushMatrix();
			glRotatef(solvel, 0, 0, 1);
			glutSolidSphere(3, 8, 8);//marte
		glPopMatrix();
	glPopMatrix();

	solvel = solvel + velocidad_sol_sobre_sol;
	tierravel = tierravel + velocidad_tierra_alrededor_sol;

	glutSwapBuffers();
	glFlush();
}

GLvoid window_reshape(GLsizei width, GLsizei height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-25.0f, 25.0f, -25.0f, 25.0f, -25.0f, 25.0f);

	glMatrixMode(GL_MODELVIEW);
}

void init_scene()
{

}

GLvoid window_key(unsigned char key, int x, int y)
{
	switch (key) {
	case ECHAP:
		exit(1);
		break;

	default:
		printf("La touche %d non active.\n", key);
		break;
	}
}

//function called on each frame
GLvoid window_idle()
{
	glutPostRedisplay();
}

//*/