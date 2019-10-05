#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "GL/glut.h"
using namespace std;

#define KEY_ESC 27
#define KEY_UP 87		//W
#define KEY_DOWN 83		//S
#define KEY_LEFT 65		//A
#define KEY_RIGHT 68	//D

#define KEY_ZPLUS 75	//K
#define KEY_ZLESS 76	//L

#define RED 0
#define GREEN 0
#define BLUE 0
#define ALPHA 1

#define ECHAP 27
#define PI 3.14159265

//global
int time = 0;
int timebase = 0;

void init_scene();
void render_scene();
GLvoid initGL();

GLvoid window_display();
GLvoid window_reshape(GLsizei width, GLsizei height);
GLvoid window_key(unsigned char key, int x, int y);

float zmove = -30;

///////////////////////
int wi = 800;
int hei = 800;
bool r = false;
int mx = 0;
int my = 0;
float ax = 0.0;
float ay = 0.0;
float speed = 0.1;
//////////////////////

//Sistema Solar Clasico - BEGIN
float aglob = 0.0;
float bglob = -10.0;
float sum = 0.009;

double tierravel = 0.0;
double solvel = 0.0;
double velocidad_tierra_alrededor_sol = 0.05;
double velocidad_sol_sobre_sol = 0.07;

void sistemasolar()
{
	//////////////sistema solar /////////////////

	glPushMatrix();
	glRotatef(solvel, 0, 0, 1);//velocidad de rotacion
	glColor3f(0.815, 0.905, 0.113);
	glutSolidSphere(4, 8, 8);//sol
	glPopMatrix();

	glPushMatrix();
	glRotatef(tierravel, 0, 0, 1);//velocidad de la tierra alrededor del sol
	glTranslatef(10, 0, 0);
	glPushMatrix();
	glRotatef(solvel * 3, 0, 0, 1);//velocidad de rotacion de la tierra
	glColor3f(0.047, 0.192, 0.027);
	glutSolidSphere(2, 8, 8);//tierra
	glPopMatrix();
	glPushMatrix();
	glRotatef(tierravel * 2, 0, 0, 1);//velocidad alrededor de la tierra
	glTranslatef(4, 0, 0);
	glPushMatrix();
	glRotatef(solvel * 1.5, 0, 0, 1);//velocidad de rotacion
	glColor3f(1, 1, 1);
	glutSolidSphere(1, 8, 8);//luna
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glRotatef(tierravel - 0.01, 0, 0, 1);//velocidad alrededor del sol
	glTranslatef(18, 0, 0);
	glPushMatrix();
	glRotatef(solvel, 0, 0, 1);//velocidad de rotacion
	glColor3f(0.862, 0.454, 0.035);
	glutSolidSphere(3, 8, 8);//marte
	glPopMatrix();
	glPopMatrix();

	solvel = solvel + velocidad_sol_sobre_sol;
	tierravel = tierravel + velocidad_tierra_alrededor_sol;
}
//Sistema Solar Clasico - END

void OnMouseClick(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		mx = x;
		my = y;
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		r = true;
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
		r = false;
	}
}

void OnMouseMotion(int x, int y)
{
	cout << "x: " << x << " , y: " << y << endl;
	int dx = mx - x;
	int dy = my - y;
	mx = x;
	my = y;
	ax += dx * speed;
	ay += dy * speed;
}

//function called on each frame
GLvoid window_idle()
{
	glutPostRedisplay();
}

void displayGizmo()
{
	glBegin(GL_LINES);
	glColor3d(255, 0, 0);//x
	glVertex3d(-30, 0, 0);
	glVertex3d(30, 0, 0);
	glColor3d(0, 255, 0);//y
	glVertex3d(0, -30, 0);
	glVertex3d(0, 30, 0);
	glColor3d(0, 0, 255);//z
	glVertex3d(0, 0, -30);
	glVertex3d(0, 0, 30);
	glEnd();
}

//-----------------------1------------------------
double angle1 = 0.0;
double xo1 = 0.0;
double yo1 = 0.0;
double max1 = 8.0;
double min1 = -8.0;
bool s1 = true;

void Rotate_with_translate(float dt) {
	angle1 = angle1 + (PI / 30) * dt;
	xo1 = 10 * cos(angle1);
	yo1 = 10 * sin(angle1);
	glTranslatef(xo1, yo1, 0);
}

void Rotate_Teapot(float dt) {
	glColor3d(0, 0, 255);
	glutSolidSphere(0.5, 360, 360); //center
	Rotate_with_translate(dt);
	glColor3d(100, 100, 100);
	glutSolidTeapot(1);//Teapot
}
//--------------------------------------------------

//------------------------2-------------------------
double ars = 0.0;
double arl = 0.0;
double atl = 0.0;
double atm = 0.0;
double arm = 0.0;
double vs = 0.07;
double angle = 0.0;
double xo = 0.0;
double zo = 0.0;


void Fun3T(float dt) {
	angle1 = angle1 + 6.0 * 3.0 * dt;
	glRotatef(angle1, 0.0, 1.0, 0.0);
	glTranslatef(3.0, 0.0, 0.0);
}
void Fun3C(float dt) {
	angle1 = angle1 + 6.0 * 2.0 * dt;
	glRotatef(angle1, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 2.0, 0.0);
}
void sistemasolar2()
{
	glPushMatrix();
	ars = ars + vs;
	glRotatef(ars, 0.0, 1.0, 0.0);
	glColor3d(255, 255, 0);
	glutSolidSphere(4, 8, 8);//sol
	glPopMatrix();
	glPushMatrix();
	angle = angle + (PI / 30) * 0.01;
	xo = 10 * cos(angle);
	zo = -10 * sin(angle);
	glTranslatef(xo, 0, zo);
	glColor3f(0.047, 0.192, 0.027);
	glutSolidSphere(2, 8, 8);//tierra

	atl = atl + (vs * 4.0);
	glRotatef(atl, 0.0, 1.0, 0.0);
	glTranslatef(2.5, 0.0, 0.0);
	arl = arl + (vs * 1.5);
	glRotatef(arl, 0.0, 1.0, 0.0);
	glColor3f(1, 1, 1);
	glutSolidSphere(0.5, 8, 8);//luna

	glPopMatrix();
	glPushMatrix();
	atm = atm + (vs * 1.5);
	glRotatef(atm, 0.0, 1.0, 0.0);
	glTranslatef(18.0, 0.0, 0.0);
	arm = arm + (vs);
	glRotatef(arm, 0.0, 1.0, 0.0);
	glColor3f(0.862, 0.454, 0.035);
	glutSolidSphere(1, 8, 8);//marte
	glPopMatrix();
}
//--------------------------------------------------

GLfloat position[] = { 0.0f, 0.0f, 0.0f, 1.0 };

GLvoid initGL()
{

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
	glLoadIdentity();
}


GLvoid window_display()
{
	time = glutGet(GLUT_ELAPSED_TIME); // recupera el tiempo ,que paso desde el incio de programa
	float dt = float(time - timebase) / 1000.0;// delta time
	timebase = time;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	//1 - BEGIN
	/*
	glOrtho(-25.0f, 25.0f, -25.0f, 25.0f, -25.0f, 25.0f);
	displayGizmo();
	Rotate_Teapot(dt);
	*/
	//1 - END

	//2 - BEGIN

	gluPerspective(45.0, 1.0, 1.0, 100.0);
	glTranslatef(0, 0, zmove);

	glRotatef(ax, 0, 1, 0);
	glRotatef(ay, 1, 0, 0);
	sistemasolar();
	displayGizmo();

	//2 - END

	//3-BEGIN
	/*
	gluPerspective(45.0, 1.0, 1.0, 100.0);
	gluLookAt(25, 25, 25, xo, 0, zo, 0, 1, 0);
	sistemasolar2();
	displayGizmo();
	*/
	//3-END
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glutSwapBuffers();
	glFlush();
}

GLvoid window_reshape(GLsizei width, GLsizei height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
}

void init_scene()
{
	glEnable(GL_DEPTH_TEST);
}

GLvoid window_key(unsigned char key, int x, int y)
{

	switch (key) {
	case ECHAP:
		exit(1);
		break;
	case KEY_ZPLUS:
		zmove = zmove + 1.5;
		break;
	case KEY_ZLESS:
		zmove = zmove - 1.5;
		break;
	default:
		printf(" %d non active.\n", key);
		break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);


	glutInitWindowSize(wi, hei);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("TP 3 : Camera");


	initGL();
	init_scene();

	glutDisplayFunc(&window_display);	// Dentro de esa funcion se dibujan las cosas

	glutReshapeFunc(&window_reshape);

	glutKeyboardFunc(&window_key);
	glutMouseFunc(&OnMouseClick);
	glutMotionFunc(&OnMouseMotion);
	//function called on each frame
	glutIdleFunc(&window_idle);

	glutMainLoop();

	return 1;
}

