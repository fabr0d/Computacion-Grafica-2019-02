#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <math.h>
#include "GL/glut.h"
#include <iostream>
#define KEY_ESC 27
using namespace std;

void create_Square(double centerX, double centerY, double longitude)
{
	glBegin(GL_LINE_LOOP);
	glColor3d(255, 0, 0);
	glVertex2f(centerX - (longitude / 2), centerY + (longitude / 2));
	glVertex2f(centerX + (longitude / 2), centerY + (longitude / 2));
	glVertex2f(centerX + (longitude / 2), centerY - (longitude / 2));
	glVertex2f(centerX - (longitude / 2), centerY - (longitude / 2));
	glVertex2f(centerX - (longitude / 2), centerY + (longitude / 2));
	glEnd();
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

void create_internal_circles(int circlenum, double porcent, int centerx, int centery, int radio)
{
	double recentx = centerx;
	double recenty = centery;
	double recentradio = radio;
	for (int i = 0; i < circlenum; i++)
	{
		create_circle(recentx, recenty, recentradio);
		recentx = recentx + (recentradio * (porcent / 100.0));
		recentradio = recentradio + (recentradio * (porcent / 100.0));
		//cout << "recentx: " << recentx << endl;
	}

}

void create_horizontal_circles(int circlenum, double porcent, int centerx, int centery, int radio)
{
	double recentx = centerx;
	double recenty = centery;
	double recentradio = radio;
	for (int i = 0; i < circlenum; i++)
	{
		create_circle(recentx, recenty, recentradio);
		recentx = recentx + (recentradio - (recentradio * (porcent / 100.0))) + recentradio;
		recentradio = recentradio - (recentradio * (porcent / 100.0));
	}
}

void create_angle_circles(int circlenum, double porcent, int centerx, int centery, int radio, float angle)
{
	double recentx = centerx;
	double recenty = centery;
	double recentradio = radio;
	for (int i = 0; i < circlenum; i++)
	{
		create_circle(recentx, recenty, recentradio);

		recentx = recentx + (recentradio - (recentradio * (porcent / 100.0))) + recentradio;
		double temporalX = recentx;
		double temporalY = recenty;
		recentx = temporalX * cosf(angle) - temporalY * sinf(angle);
		recenty = temporalX * sinf(angle) + temporalY * cosf(angle);
		recentradio = recentradio - (recentradio * (porcent / 100.0));
	}
}

//dibuja un simple gizmo
void displayGizmo()
{
	//create_Square(10, 10, 10);
	//create_circle(0, 0, 20);
	//create_internal_circles(7, 20.0, 0, 0, 10);
	//create_horizontal_circles(10, 20.0, 0, 0, 5);
	create_angle_circles(5, 15.0, 0, 0, 5, 45.0);
}

//
//funcion llamada a cada imagen
void glPaint(void) {

	//El fondo de la escena al color initial
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //(R, G, B, transparencia) en este caso un fondo negro
	glLoadIdentity();


	//dibuja el gizmo
	displayGizmo();

	//doble buffer, mantener esta instruccion al fin de la funcion
	glutSwapBuffers();
}

//
//inicializacion de OpenGL
//
void init_GL(void) {
	//Color del fondo de la escena
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //(R, G, B, transparencia) en este caso un fondo negro

	//modo projeccion
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

//en el caso que la ventana cambie de tamaño
GLvoid window_redraw(GLsizei width, GLsizei height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-50.0, 50.0, -50.0, 50.0, -1.0, 1.0);
	// todas la informaciones previas se aplican al la matrice del ModelView
	glMatrixMode(GL_MODELVIEW);
}

GLvoid window_key(unsigned char key, int x, int y) {
	switch (key) {
	case KEY_ESC:
		exit(0);
		break;

	default:
		break;
	}

}
//
//el programa principal
//
int main(int argc, char** argv) {

	//Inicializacion de la GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 800); //tamaño de la ventana
	glutInitWindowPosition(0, 0); //posicion de la ventana
	glutCreateWindow("TP1 OpenGL : hello_world_OpenGL"); //titulo de la ventana

	init_GL(); //funcion de inicializacion de OpenGL

	glutDisplayFunc(glPaint);
	glutReshapeFunc(&window_redraw);
	// Callback del teclado
	glutKeyboardFunc(&window_key);

	glutMainLoop(); //bucle de rendering

	return 0;
}