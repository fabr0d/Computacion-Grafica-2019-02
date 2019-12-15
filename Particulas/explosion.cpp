#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <list>
#include <GL/glut.h>
#include <GL/glext.h>
#include "TextureManager.h"

using namespace std;

#define RED 0
#define GREEN 0
#define BLUE 0
#define ALPHA 1

//Illimination
GLfloat gameDiffuse[] = { 0.9f, 0.9f, 0.9f, 1.0f };
GLfloat gameSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat gameAmbient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
GLfloat gameShininess[] = { 9.0f };

GLfloat light0Position[] = { 0.0,375.0,325.0,0.0 };
GLfloat light0Diffuse[] = { 0.9f, 0.9f, 0.9f, 1.0f };
GLfloat light0Specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat light0Ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };

double gravity = -50.8;
int particleNumber;

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

double VRandom()
{
	int s = rand() % 2;
	double rp = (rand() % 20) + 10;
	double rc = 0.0;
	if (s == 0) {
		rc = rp;
	}
	else {
		rc = -rp;
	}
	return rc;
}

double TRandom()
{
	double rp = (rand() % 8) + 1;
	return rp;
}

class Particle
{
	double x, y, z;
	double vx, vy, vz;//Velocidad
	double t;//Tiempo de Vida
	double a = 15.0;//arista (QUADS)
	GLint texture;

    double init_time=0;
	double masa=1.0;
public:
	Particle(double _x, double _y, double _z)
	{
		x = _x;
		y = _y;
		z = _z;
		vx = VRandom();
		vy = VRandom();
		vz = VRandom();
		t = TRandom();
		texture = rand() % 5+1;//
	}
	void resetParticle()
	{
        x = y = z = vx = vy = vz = t = 0.0;
        //texture = 0;
	}
	void restartParticle(double _x, double _y, double _z)
	{
        x = _x;
		y = _y;
		z = _z;
		vx = VRandom();
		vy = VRandom();
		vz = VRandom();
		t = TRandom();
		//texture = rand() % 5+1;//
	}
	double getT()
	{
		return t;
	}
	void Draw()
	{
		glBindTexture(GL_TEXTURE_2D, texture);
		//cout<<"texture"<<texture<<endl;
		glBegin(GL_QUADS);
		glNormal3d(0.0, 0.0, 1.0);
		glTexCoord2d(0.0, 1.0);
		glVertex3d(x - (a / 2), y - (a / 2), 0.0);
		glTexCoord2d(1.0, 1.0);
		glVertex3d(x + (a / 2), y - (a / 2), 0.0);
		glTexCoord2d(1.0, 0.0);
		glVertex3d(x + (a / 2), y + (a / 2), 0.0);
		glTexCoord2d(0.0, 0.0);
		glVertex3d(x - (a / 2), y + (a / 2), 0.0);
		glEnd();
	}
	void TMove(double dt)
	{
		//Time
		t = t - 2.0 * dt;
		init_time = init_time + 2.0 * dt;
		//Move
		x = x + vx * dt;
		y = y + (vy+(gravity/masa)+init_time) * dt;
		z = z + vz * dt;
	}
};

//Textures (Particle)
GLint t_particle1;
GLint t_particle2;
GLint t_particle3;
GLint t_particle4;
GLint t_particle5;

//Particles
list<Particle*> particles;

//global time
int time = 0;
int timebase = 0;

#define ECHAP 27
void init_scene();
void render_scene();
GLvoid initGL();
GLvoid window_display();
GLvoid window_reshape(GLsizei width, GLsizei height);
GLvoid window_key(unsigned char key, int x, int y);

//function called on each frame
GLvoid window_idle();

GLvoid callback_special(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		glutPostRedisplay();			// et on demande le réaffichage.
		//p->MoveUp(true);
		break;

	case GLUT_KEY_DOWN:
		glutPostRedisplay();			// et on demande le réaffichage.
		//p->MoveDown(true);
		break;

	case GLUT_KEY_LEFT:
		glutPostRedisplay();			// et on demande le réaffichage.
		//p->MoveLeft(true);
		break;

	case GLUT_KEY_RIGHT:
		glutPostRedisplay();			// et on demande le réaffichage.
		//p->MoveRight(true);
		break;
	}
}

void temp(int particleNumber)
{
	for (int i = 0; i < particleNumber; i++) {
		particles.push_back(new Particle(0,0,0));
	}
}

void clickParticle(double laequis, double elye)
{
    double _z = (rand() % 20) + 1;
    list<Particle*>::iterator itP;
    particleNumber = (rand()%1000)+800;
	int i = 0;
	for (itP = particles.begin(); i<particleNumber; itP++) {
		(*itP)->restartParticle(laequis,elye,0);
        i++;
	}

}

void OnMouseClick(int button, int state, int x, int y)
{
    float x1 = x /(float) 750;
    float y1 = y /(float) 750;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		//cout << "x: " <<x << " , y: " << y << endl;
        clickParticle(x1+((rand() % 20) + 1),y1+((rand() % 20) + 1));
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		//r = true;
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
		//r = false;
	}
}

int main(int argc, char** argv)
{
    particleNumber = 10000;
    temp(particleNumber);
    cout<<"list size: "<<particles.size()<<endl;
    particleNumber=0;
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);


	glutInitWindowSize(750, 750);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Particle motor");


	initGL();
	init_scene();

	//Textures

	t_particle1 = TextureManager::Inst()->LoadTexture("C:/Users/Fab/Desktop/XD/Computacion Grafica/FINAL/Models/Explosion1.png", GL_BGRA_EXT, GL_RGBA);
	t_particle2 = TextureManager::Inst()->LoadTexture("C:/Users/Fab/Desktop/XD/Computacion Grafica/FINAL/Models/Explosion2.png", GL_BGRA_EXT, GL_RGBA);
	t_particle3 = TextureManager::Inst()->LoadTexture("C:/Users/Fab/Desktop/XD/Computacion Grafica/FINAL/Models/Explosion3.png", GL_BGRA_EXT, GL_RGBA);
	t_particle4 = TextureManager::Inst()->LoadTexture("C:/Users/Fab/Desktop/XD/Computacion Grafica/FINAL/Models/Explosion4.png", GL_BGRA_EXT, GL_RGBA);
	t_particle5 = TextureManager::Inst()->LoadTexture("C:/Users/Fab/Desktop/XD/Computacion Grafica/FINAL/Models/Explosion5.png", GL_BGRA_EXT, GL_RGBA);

	glutDisplayFunc(&window_display);

	glutReshapeFunc(&window_reshape);

	glutKeyboardFunc(&window_key);

	glutMouseFunc(&OnMouseClick);

	glutSpecialFunc(&callback_special);//

	//function called on each frame
	glutIdleFunc(&window_idle);

	glutMainLoop();

	//End Sound

	return 1;
}

GLvoid initGL()
{
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, gameDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, gameSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, gameAmbient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, gameShininess);

    glShadeModel(GL_SMOOTH); // modelo de shading try GL_FLAT
    glEnable(GL_CULL_FACE); //no trata las caras escondidas
    glEnable(GL_DEPTH_TEST); // Activa el Z-Buffer
    glDepthFunc(GL_LEQUAL); //Modo de funcionamiento del Z-Buffer
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Activa la corrección de perspectiva

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light0Position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0Diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0Specular);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0Ambient);

	glClearColor(RED, GREEN, BLUE, ALPHA);

	glEnable(GL_TEXTURE_2D);
	glMatrixMode(GL_TEXTURE);
}

void GenerateParticles(double dt,int particleNumber) {
	list<Particle*>::iterator itP;
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	int i = 0;
	//cout<<"particle number: "<<particleNumber<<endl;
	for (itP = particles.begin(); i<particleNumber; itP++) {
		if ((*itP)->getT() > 0.0) {
			glEnable(GL_LIGHTING);
			(*itP)->Draw();
			glDisable(GL_LIGHTING);
			(*itP)->TMove(dt);
			i++;
		}
		else {
            (*itP)->resetParticle();
			//delete(*itP);
			//itP = particles.erase(itP);
			i++;
		}
	}
	particleNumber=0;
	glDisable(GL_BLEND);
}

GLvoid window_display()
{
	//Time dt
	time = glutGet(GLUT_ELAPSED_TIME); // recupera el tiempo ,que paso desde el incio de programa
	float dt = float(time - timebase) / 1000.0;// delta time
	//cout<<"dt: "<<dt<<endl;
	timebase = time;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);

	GenerateParticles(dt,particleNumber);

    //displayGizmo();
	glutSwapBuffers();
	glFlush();
}

GLvoid window_reshape(GLsizei width, GLsizei height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(0.0f, 500, 500, 0.0f, 0.0f, 1.0f);
	glOrtho(-325.0f, 325.0f, -375.0f, 375.0f, -325.0f, 325.0f);

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