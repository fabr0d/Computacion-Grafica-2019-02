#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <GL/glut.h>
#include <GL/glext.h>

#include "TextureManager.h"

using namespace std;

#define RED 0
#define GREEN 0
#define BLUE 0
#define ALPHA 1

#define ECHAP 27

 //definicion de un punto del terreno
struct Punto_Terreno
{
    GLfloat s, t; //coordenadas de texturas
    GLfloat nx, ny, nz; //coordenadas de la normal
    GLfloat x, y, z; //posicion del vertice
};

//definicion de un triangulo
struct Triangle
{
    GLint p1, p2, p3; //indices de los puntos
    GLfloat nx, ny, nz; //coordenadas de la normal
};

//separacion por etiqueta
vector<string> separateValues(string input, char label) {
	stringstream inputstream(input);
	string word;
	vector<string> words;
	while (getline(inputstream, word, label)) {
		words.push_back(word);
    }
    return words;
}

vector<float> DotProduct(float u[3], float v[3]) {
    vector<float> dp;
    float c = 0.0;
    c = (u[1]*v[2]) - (v[1]*u[2]);
    dp.push_back(c);
    c = -((u[0]*v[2]) - (v[0]*u[2]));
    dp.push_back(c);
    c = (u[0]*v[1]) - (v[0]*u[1]);
    dp.push_back(c);
    return dp;
}

float Magnitude(float x, float y, float z) {
    return sqrt((x*x)+(y*y)+(z*z));
}

vector<float> Normalize(float x, float y, float z) {
    vector<float> nv;
    float m = Magnitude(x,y,z);
    nv.push_back(x/m);
    nv.push_back(y/m);
    nv.push_back(z/m);
    return nv;
}

class Terrain {
public:
    Terrain(){};
    ~Terrain(){};
    //calcula la lista de indices (SE USA EN LOAD)
    void generateIndex() {
        lista_indices = new GLuint[(((nb_pt_x-1)*(nb_pt_z-1)*2)*3)];
        int sl = (nb_pt_x*(nb_pt_z-1))-1;//cantidad de elementos
        int nl = (nb_pt_x*nb_pt_z)-1;
        int sx = nb_pt_x;
        int di = 0;
        for (int i=0; i<sl; i++) {
            if ( (((i+1)%sx)!=0) || (i==0) ) {
                lista_indices[di] = i+1;
                lista_indices[di+1] = i;
                lista_indices[di+2] = i+sx;
                di+=3;
            }
        }
        int ui = di;
        for (int i=nl; i>sx; i--) {
            if ((i%sx)!=0) {
                lista_indices[ui] = i-1;
                lista_indices[ui+1] = i;
                lista_indices[ui+2] = i-sx;
                ui+=3;
            }
        }
    }
    //carga un archivo de modelo digital de terreno
    void load(string filename) {
        lista_vertices = new Punto_Terreno[nb_pt_x*nb_pt_z];
        vector<string> vline; //valores de la linea
        int ti = 0;
        int li = 0;
        Punto_Terreno tp;
        string line;
        ifstream myfile (filename);
        if(myfile.is_open()) {
            getline(myfile,line);
            while(getline(myfile,line)) {
                vline = separateValues(line,' ');
                tp.z = ti*dist_z;
                tp.t = ti*(1.0/(nb_pt_z-1.0));
                for (int tj=0; tj<nb_pt_x; tj++) {
                    tp.x = tj*dist_x;
                    tp.y = stof(vline[tj]);//
                    tp.s = tj*(1.0/(nb_pt_x-1.0));
                    lista_vertices[li] = tp;
                    li+=1;
                }
                ti+=1;
            }
            myfile.close();
            generateIndex();
            cout << "the lists were generated correctly";
        }
        else cout << "unable to open file";
    }
    //visualizacion del terreno
    void display() {
        int number = (((nb_pt_x-1)*(nb_pt_z-1)*2)*3);
        glInterleavedArrays(GL_T2F_N3F_V3F, sizeof(Punto_Terreno), &lista_vertices[0].s);
        glDrawElements(GL_TRIANGLES, number, GL_UNSIGNED_INT, (void*)lista_indices);
    }
    //calcula la lista de triangulos
    void generateTriangles() {
        int si = (((nb_pt_x-1)*(nb_pt_z-1)*2)*3); //cantidad de indices
        int st = ((nb_pt_x-1)*(nb_pt_z-1)*2); //cantidad de triangulos
        list_triangles = new Triangle[st];
        Triangle t;
        float u[3]; //vector u
        float v[3]; //vector v
        vector<float> p; //dot product
        vector<float> n; //normal
        for (int i=0; i<si; i+=3) {
            t.p1 = lista_indices[i];
            t.p2 = lista_indices[i+1];
            t.p3 = lista_indices[i+2];
            u[0] = lista_vertices[t.p3].x - lista_vertices[t.p2].x;
            u[1] = lista_vertices[t.p3].y - lista_vertices[t.p2].y;
            u[2] = lista_vertices[t.p3].z - lista_vertices[t.p2].z;
            v[0] = lista_vertices[t.p1].x - lista_vertices[t.p2].x;
            v[1] = lista_vertices[t.p1].y - lista_vertices[t.p2].y;
            v[2] = lista_vertices[t.p1].z - lista_vertices[t.p2].z;
            p = DotProduct(u,v);
            n = Normalize(p[0],p[1],p[2]);
            t.nx = p[0];
            t.ny = p[1];
            t.nz = p[2];
            list_triangles[i/3] = t;
        }
    }
    //calcula las normales de cada vertice
    void computeNormals() {
        int ct = 0; //contador de triangulos
        float sumx = 0; //suma de nx
        float sumy = 0; //suma de ny
        float sumz = 0; //suma de nz
        int sp = nb_pt_x*nb_pt_z; //cantidad de puntos
        int st = ((nb_pt_x-1)*(nb_pt_z-1)*2); //cantidad de triangulos
        for (int pi=0; pi<sp; pi++) {
            for (int ti=0; ti<st; ti++) {
                if ( (pi == list_triangles[ti].p1) ||
                     (pi == list_triangles[ti].p2) ||
                     (pi == list_triangles[ti].p3) ) {
                        ct+=1;
                        sumx+=list_triangles[ti].nx;
                        sumy+=list_triangles[ti].ny;
                        sumz+=list_triangles[ti].nz;
                }
            }
            lista_vertices[pi].nx = sumx/ct;
            lista_vertices[pi].ny = sumy/ct;
            lista_vertices[pi].nz = sumz/ct;
            ct = 0;
            sumx = 0;
            sumy = 0;
            sumz = 0;
        }
    }
private:
    int nb_pt_x = 101, nb_pt_z = 101; //cantidad de puntos en X y Z
    float dist_x = 50.0, dist_z = 50.0;//distancia entre dos puntos segun X y Z

    Punto_Terreno *lista_vertices; //tabla que contiene los puntos del terreno
    GLuint *lista_indices; //tabla que contiene los indices del terreno
    Triangle *list_triangles; //tabla que contiene los triangulos del terreno
};


//Illimination
GLfloat roofDiffuse[] = {0.9f, 0.9f, 0.9f, 1.0f};
GLfloat roofSpecular[] = {0.3f, 0.3f, 0.3f, 1.0f};
GLfloat roofAmbient[] = {0.6f, 0.6f, 0.6f, 1.0f};
GLfloat roofShininess[] = {9.0f};

GLfloat light0Position[] = {-20.0,20.0,20.0,0.0};
GLfloat light0Diffuse [] = {0.3f, 0.3f, 0.3f, 1.0f};
GLfloat light0Specular [] = {0.3f, 0.3f, 0.3f, 1.0f};
GLfloat light0Ambient [] = {0.3f, 0.3f, 0.3f, 1.0f};

GLint texture;

Terrain* tr;

void init_scene();
GLvoid initGL();
GLvoid window_display();
GLvoid window_reshape(GLsizei width, GLsizei height);
GLvoid window_key(unsigned char key, int x, int y);


//variables para el gizmo
float delta_x = 0.0;
float delta_y = 0.0;
float mouse_x, mouse_y;
float var_x = 0.0;
float var_z = -30.0;
float step = 0; //0.0 Posicion inicial. 1.0 Traslacion. 2.0 Primera Rotacion(en y). 3.0 Segunda Rotacion (en x) 4.0 Ultima Rotacion (en z)

GLvoid callback_special(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		var_z += 0.5;
		glutPostRedisplay();
		break;

	case GLUT_KEY_DOWN:
		var_z -= 0.5;
		glutPostRedisplay();
		break;

	case GLUT_KEY_LEFT:
		var_x += 0.5;
		glutPostRedisplay();
		break;

	case GLUT_KEY_RIGHT:
		var_x -= 0.5;
		glutPostRedisplay();
		break;

	case GLUT_KEY_PAGE_UP:
		step++;
		glutPostRedisplay();
		break;

	case GLUT_KEY_PAGE_DOWN:
		step--;
		glutPostRedisplay();
		break;


	}
}

GLvoid callback_mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
	{
		mouse_x = x;
		mouse_y = y;
	}
}

GLvoid callback_motion(int x, int y)
{

	delta_x += x - mouse_x;
	delta_y += y - mouse_y;
	mouse_x = x;
	mouse_y = y;
	glutPostRedisplay();
}


//function called on each frame
GLvoid window_idle();

int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);


	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("TP 5 : Terrain Model");


	initGL();
	init_scene();


	texture = TextureManager::Inst()->LoadTexture("C:/Users/Fab/Desktop/XD/Mio/LaRicaMesh/fontvieille.tga", GL_RGB, GL_RGB);
	cout << texture << endl;
	tr = new Terrain;
	tr->load("C:/Users/Fab/Desktop/XD/Computacion Grafica/P5/fontvieille.txt");
	tr->generateTriangles();
	tr->computeNormals();


	glutDisplayFunc(&window_display);
	glutReshapeFunc(&window_reshape);


	glutMouseFunc(&callback_mouse);
	glutMotionFunc(&callback_motion);

	glutKeyboardFunc(&window_key);
//	glutKeyboardUpFunc(&window_key_up); //key release events
	glutSpecialFunc(&callback_special);
//	glutSpecialUpFunc(&callback_special_up); //key release events

	//function called on each frame
	glutIdleFunc(&window_idle);

	glutMainLoop();

	return 1;
}

GLvoid initGL()
{
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, roofDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, roofSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, roofAmbient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, roofShininess);

    glShadeModel(GL_SMOOTH); // modelo de shading try GL_FLAT
    glEnable(GL_CULL_FACE); //no trata las caras escondidas
    glEnable(GL_DEPTH_TEST); // Activa el Z-Buffer
    glDepthFunc(GL_LEQUAL); //Modo de funcionamiento del Z-Buffer
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Activa la corrección de perspectiva

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light0Position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0Ambient);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0Ambient);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0Ambient);

	glClearColor(RED, GREEN, BLUE, ALPHA);

	glEnable(GL_TEXTURE_2D);
	glMatrixMode(GL_TEXTURE);
}

GLvoid window_display()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();


	gluPerspective(45.0f, 1.0f, 0.01f, 500.0f);

	glTranslatef(var_x, 0.0, var_z);
	glRotatef(delta_x, 0.0, 1.0, 0.0);
	glRotatef(delta_y, 1.0, 0.0, 0.0);

	glTranslatef(-25,0.0,-25);
	glScalef(0.01,0.01,0.01);


    glBindTexture(GL_TEXTURE_2D, texture);
	tr->display();

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
