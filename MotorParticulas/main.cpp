#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <vector>
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
int time_ = 0;
int timebase = 0;

void init_scene();
void render_scene();
GLvoid initGL();

GLvoid window_display();
GLvoid window_reshape(GLsizei width, GLsizei height);
GLvoid window_key(unsigned char key, int x, int y);

float zmove = -30;
float xmove = 0;
float ymove = 0;

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

double double_rand(double fMin, double fMax)
{
	double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}

//Particle Motor

GLuint glInitTexture(char* filename)
{
	GLuint t = 0;

	glGenTextures(1, &t);
	glBindTexture(GL_TEXTURE_2D, t);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	unsigned char data[] = { 255, 0, 0, 255 };
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	return t;
}

void drawImage(GLuint file, float x, float y, float w, float h, float angle)
{
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glPushMatrix();
	glTranslatef(x, y, 0.0);
	glRotatef(angle, 0.0, 0.0, 1.0);

	glBindTexture(GL_TEXTURE_2D, file);
	glEnable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(x, y, 0.0f);
	glTexCoord2f(0.0, 2.0); glVertex3f(x, y + h, 0.0f);
	glTexCoord2f(2.0, 2.0); glVertex3f(x + w, y + h, 0.0f);
	glTexCoord2f(2.0, 0.0); glVertex3f(x + w, y, 0.0f);
	glEnd();

	glPopMatrix();
}

GLuint texture;

class Spark
{
public:
	double velX = 0.0;
	double velY = 0.0;
	double velZ= 0.0;

	double posX;
	double posY;
	double posZ;

	double oldposX;
	double oldposY;
	double oldposZ;

	double oldvelX;
	double oldvelY;
	double oldvelZ;
	
	int lifetime;
	double lifetimetotal;

	double masa = 0.5;
	
	double forceX = 0;
	double forceY = -0.98;
	
	Spark(float _posX, float _posY, float _posZ);

	void drawParticle()
	{
		if (lifetime > 0)
		{
			glPushMatrix();
			glTranslated(posX, posY, posZ);
				glColor3d(255, 255, 0);
				glutSolidSphere(1, 15, 15);
			glPopMatrix();

			lifetime--; //tiempo de vida que se reduce
			lifetimetotal = lifetimetotal + 1; //tiempo transcurrido

			oldposX = posX;
			oldposY = posY;
			oldposZ = posZ;

			oldvelX = velX;
			oldvelY = velY;
			oldvelZ = velZ;

			double acelerationX = forceX / masa;
			double acelerationY = forceY / masa;

			//velX = oldvelX + lifetimetotal + acelerationX;
			velY = oldvelY + lifetimetotal + acelerationY;

			posX = oldposX + lifetimetotal *velX;
			posY = oldposY + lifetimetotal *velY;
			posZ = oldposZ + lifetimetotal *velZ;

			cout << "velX: " << velX << " velY: " << velY << " velZ: " << velZ << endl;
		}
	}
};

Spark::Spark(float _posX, float _posY, float _posZ)
{
	posX = _posX;
	posY = _posY;
	posZ = _posZ;

	velX = (rand() % 5 + 1);
	velY = (rand() % 5 + 1);
	velZ = (rand() % 5 + 1);

	if (rand() % 10 == 0){ velX = velX * -1; }
	if (rand() % 10 == 0){ velY = velY * -1; }
	if (rand() % 10 == 0){ velZ = velZ * -1; }

	lifetime = rand() % 25;
	lifetimetotal = 0;
}

vector<vector<Spark>> world;

bool particlesChecker(vector<Spark> temp)
{
	for (int i = 0; i < temp.size(); i++)
	{
		if (temp[i].lifetime > 0)
		{
			return false; //sigue algo con vida
		}
	}
	return true; //la explosion acabo
}

void genparticles(float initx, float inity, float initz)
{
	vector<Spark> particles;
	int x = 20; //NUMERO DE PARTICULAS
	for (int i = 0; i < x; i++)
	{
		Spark temporal(initx, inity, initz);
		particles.push_back(temporal);
	}
	world.push_back(particles);
}

void drawparticles()
{
	
	for (int i = 0; i < world.size(); i++)
	{
		for (int j = 0; j < world[i].size(); j++)
		{
			//cout << world[i].size() << endl;
			if (particlesChecker(world[i]) == true)
			{
				world.erase(world.begin() + i);
				//cout << "murio una explosion" << endl;
				int tempx = rand() % 10 + 1;
				if (rand() % 2 == 0)
				{
					tempx = tempx * -1;
				}
				int tempy = rand() % 10 + 1;
				if (rand() % 2 == 0)
				{
					tempy = tempy * -1;
				}
				int tempz = rand() % 10 + 1;
				if (rand() % 2 == 0)
				{
					tempz = tempz * -1;
				}
				genparticles(tempx, tempy, tempz);
			}
			else
			{
				world[i][j].drawParticle();
			}
			
		}
	}

}

//


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
	//cout << "x: " << x << " , y: " << y << endl;
	int dx = mx - x;
	int dy = my - y;
	mx = x;
	my = y;
	ax += dx * speed;
	ay += dy * speed;
}

void OnMouseMotionPassive(int x, int y)
{
	int dx = mx - x;
	int dy = my - y;
	mx = x;
	my = y;
	ax += dx * speed * 2;
	ay += dy * speed * 2;
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

int timetemp = 0;

GLvoid window_display()
{
	time_ = glutGet(GLUT_ELAPSED_TIME); // recupera el tiempo ,que paso desde el incio de programa
	float dt = float(time_ - timebase) / 1000.0;// delta time
	timebase = time_;

	//cout << "timetemp: " << timetemp << endl;
	//timetemp++;

	const double w = glutGet(GLUT_WINDOW_WIDTH);
	const double h = glutGet(GLUT_WINDOW_HEIGHT);

	drawImage(texture, 0.0f, 0.0f, 4.0f, 4.0f, 0.0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluPerspective(45.0, 1.0, 1.0, 100.0);
	glTranslatef(0, 0, zmove);
	glTranslatef(0, ymove, 0);
	glTranslatef(xmove, 0, 0);
	glRotatef(ax, 0, 1, 0);
	glRotatef(ay, 1, 0, 0);
	displayGizmo();
	


	drawparticles();


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
		//#define KEY_UP 87		//W
		//#define KEY_DOWN 83	//S
		//#define KEY_LEFT 65	//A
		//#define KEY_RIGHT 68	//D
	case KEY_UP:
		ymove = ymove + 1.5;
		break;
	case KEY_DOWN:
		ymove = ymove - 1.5;
		break;
	case KEY_LEFT:
		xmove = xmove + 1.5;
		break;
	case KEY_RIGHT:
		xmove = xmove - 1.5;
	default:
		printf(" %d non active.\n", key);
		break;
	}
}

int main(int argc, char** argv)
{

	genparticles(0, 0, 0);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowSize(wi, hei);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("ParticlesMotor");

	initGL();
	init_scene();

	glutDisplayFunc(&window_display);	// Dentro de esa funcion se dibujan las cosas

	glutReshapeFunc(&window_reshape);

	glutKeyboardFunc(&window_key);
	glutMouseFunc(&OnMouseClick);
	//glutPassiveMotionFunc(&OnMouseMotionPassive); //4
	glutMotionFunc(&OnMouseMotion);
	//function called on each frame
	glutIdleFunc(&window_idle);

	glutMainLoop();

	return 1;
}

