/***************************************************
ISGI::Conduccion Nocturna
Genis Martinez Sanchis
Una carretra
Dependencias:
+freeglut
***************************************************/

#define PROYECTO "Carretera"
#define _CRT_SECURE_NO_WARNINGS

#define tasaFPS 60
//Camara
#define AlturaCam 1
//Carretra
#define AnchoCar 8
//Seno
#define Amplitud 5
#define Periodo 160

#include <iostream>
#include <sstream>
#include <ctime>
#include <Utilidades.h>

using namespace std;

static GLuint texturaCarretera, texturaFondoD, texturaFondoN, texturaPepsi;

float velocidad, angulo;
float posx, girox, posz, giroz;

bool noche = false;

//Luz de luna
GLfloat Al0[] = { 0.05f ,0.05f ,0.05f ,1.0 };
GLfloat Dl0[] = { 0.05f ,0.05f ,0.05f ,1.0 };
GLfloat Sl0[] = { 0.0f , 0.0f ,0.0f, 1.0 };
GLfloat directionalPosition[] = { 0.0f ,10.0f ,0.0f ,0.0f };
//Luz frontal-CascoMinero
GLfloat Al1[] = { 0.2f ,0.2f ,0.2f, 1.0 };
GLfloat Dl1[] = { 1.0f, 1.0f, 1.0f, 1.0 };
GLfloat Sl1[] = { 0.3f, 0.3f, 0.3f, 1.0 };
//Luz farolas
GLfloat Al2[] = { 0.0f, 0.0f, 0.0f, 1.0 };
GLfloat Dl2[] = { 0.5f, 0.5f, 0.2f, 1.0 };
GLfloat Sl2[] = { 0.0f, 0.0f, 0.0f, 1.0 };
GLfloat directionalFarola[] = { 0.0f, -1.0f, 0.0f, 0.0f };
//difuse y Specular de la carretera
GLfloat Dm[] = { 0.8f, 0.8f, 0.8f, 1.0 };
GLfloat Sm[] = { 0.3f, 0.3f, 0.3f, 1.0 };

GLfloat directionalView[] = { 0.0f, -0.5f, -0.7f };
GLfloat posicionFocal[] = { 0.0f, 0.7f, 0.0f, 1.0f };

float deriv(float u)
{
	return ((2 * PI*Amplitud) / Periodo)*cos(u*((2 * PI) / Periodo));
}

float funcion(float u)
{
	return Amplitud * sin(u * ((2 * PI) / Periodo));
}

void updateTitulo()
{
	stringstream titulo;
	titulo << PROYECTO << ". Velocidad: " << velocidad << " m/s";
	glutSetWindowTitle(titulo.str().c_str());
}

float *vectorN(float u)
{
	float vec[2] = { 0,0 };
	float base = 1 / (sqrt(1 + pow(deriv(u), 2)));
	vec[0] = base * -1 * deriv(u);
	vec[1] = base;
	return vec;
}

void crearPepsis() {
	//calculamos la posicion relativa
	glBindTexture(GL_TEXTURE_2D, texturaPepsi);
	if (noche) {
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	}
	else {
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	}
	float despFar = fmod(posx, 40.0f);
	float xFirst = posx - despFar;

	for (int j = 0; j < 4; j++) {//Creamos 4 pepsis
		float x = xFirst + 40 * j;
		float valorfunc = funcion(x-1);
		GLfloat posPepsi4[3] = { x, 5, valorfunc - (AnchoCar / 2) };
		GLfloat posPepsi3[3] = { x, 5, valorfunc + (AnchoCar / 2) };
		GLfloat posPepsi2[3] = { x, 2.5, valorfunc + (AnchoCar / 2) };
		GLfloat posPepsi1[3] = { x, 2.5, valorfunc  - (AnchoCar / 2) };
		switch (j)
		{
		case 0:
			quadtex(posPepsi1, posPepsi2, posPepsi3, posPepsi4);
			break;
		case 1:
			quadtex(posPepsi1, posPepsi2, posPepsi3, posPepsi4);
			break;
		case 2:
			quadtex(posPepsi1, posPepsi2, posPepsi3, posPepsi4);
			break;
		case 3:
			quadtex(posPepsi1, posPepsi2, posPepsi3, posPepsi4);
			break;
		}
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}

void crearfarolas() {

	float despFar = fmod(posx, 40.0f);
	float xFirst = posx - despFar;

	for (int j = 0; j < 4; j++) {
		float x = xFirst + 40 * j;
		GLfloat positionFarola[] = { x, 4, funcion(x),1 };
		switch (j)
		{
		case 0:
			glLightfv(GL_LIGHT2, GL_POSITION, positionFarola);
			break;
		case 1:
			glLightfv(GL_LIGHT3, GL_POSITION, positionFarola);
			break;
		case 2:
			glLightfv(GL_LIGHT4, GL_POSITION, positionFarola);
			break;
		case 3:
			glLightfv(GL_LIGHT5, GL_POSITION, positionFarola);
			break;
		}
	}
}

void crearCarretera() {

	glBindTexture(GL_TEXTURE_2D, texturaCarretera);
	if (noche) {
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	}else{ 
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); 
	}

	for (int i = posx / AlturaCam; i <= 80 + posx; i++) {//calculamos la carretera y la dibujamos crendo 75 quads
		float u = i * AlturaCam;
		float u2 = (i + 1)*AlturaCam;
		float fDe1 = funcion(u);
		float fDe2 = funcion(u2);
		float * vecN = vectorN(u);

		GLfloat v0[3] = { u, 0, AnchoCar / 2 + fDe1 };
		GLfloat v1[3] = { u2, 0, AnchoCar / 2 + fDe2 };
		GLfloat v2[3] = { u2, 0, (-1 * AnchoCar) / 2 + fDe2 };
		GLfloat v3[3] = { u, 0, (-1 * AnchoCar) / 2 + fDe1 };

		quadtex(v0, v1, v2, v3, 0.0, 1.0, 1.0, 0.0, 10, 10);

	}
	glBindTexture(GL_TEXTURE_2D, 0);
}

void crearFondo() {
	glPushMatrix();
	GLUquadric* qobj = gluNewQuadric();
	gluQuadricNormals(qobj, GLU_SMOOTH);
	glTranslatef(posx, -75, posz);
	glRotatef(-90, 1, 0, 0);
	if (!noche) {
		glBindTexture(GL_TEXTURE_2D, texturaFondoD);
	}
	else {
		glBindTexture(GL_TEXTURE_2D, texturaFondoN);
	}
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	gluQuadricTexture(qobj, true);
	gluCylinder(qobj, 150, 150, 150, 100, 1000);
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
}

void iniciarLuces() {
	glEnable(GL_LIGHTING);

	glLightfv(GL_LIGHT0, GL_AMBIENT, Al0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Dl0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, Sl0);
	glLightfv(GL_LIGHT0, GL_POSITION, directionalPosition);
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT1, GL_AMBIENT, Al1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, Dl1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, Sl1);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 25.0f);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 20.0f);
	glEnable(GL_LIGHT1);

	glLightfv(GL_LIGHT2, GL_AMBIENT, Al2);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, Dl2);
	glLightfv(GL_LIGHT2, GL_SPECULAR, Sl2);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 45.0f);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 10.0f);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, directionalFarola);
	glEnable(GL_LIGHT2);

	glLightfv(GL_LIGHT3, GL_AMBIENT, Al2);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, Dl2);
	glLightfv(GL_LIGHT3, GL_SPECULAR, Sl2);
	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 45.0f);
	glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 10.0f);
	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, directionalFarola);
	glEnable(GL_LIGHT3);

	glLightfv(GL_LIGHT4, GL_AMBIENT, Al2);
	glLightfv(GL_LIGHT4, GL_DIFFUSE, Dl2);
	glLightfv(GL_LIGHT4, GL_SPECULAR, Sl2);
	glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, 45.0f);
	glLightf(GL_LIGHT4, GL_SPOT_EXPONENT, 10.0f);
	glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, directionalFarola);
	glEnable(GL_LIGHT4);

	glLightfv(GL_LIGHT5, GL_AMBIENT, Al2);
	glLightfv(GL_LIGHT5, GL_DIFFUSE, Dl2);
	glLightfv(GL_LIGHT5, GL_SPECULAR, Sl2);
	glLightf(GL_LIGHT5, GL_SPOT_CUTOFF, 45.0f);
	glLightf(GL_LIGHT5, GL_SPOT_EXPONENT, 10.0f);
	glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION, directionalFarola);
	glEnable(GL_LIGHT5);
	//Definimos ls materiales de la carretera
	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, Dm);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Sm);
	glMaterialf(GL_FRONT, GL_SHININESS, 3.0);
}

void init()
{
	//Iniciamos el progrma
	cout << "Iniciando " << PROYECTO << endl;
	cout << "GL Version " << glGetString(GL_VERSION) << endl;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	iniciarLuces();

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glGenTextures(1, &texturaCarretera);
	glBindTexture(GL_TEXTURE_2D, texturaCarretera);
	loadImageFile((char*)"carretera.jpg");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glGenTextures(1, &texturaFondoD);
	glBindTexture(GL_TEXTURE_2D, texturaFondoD);
	loadImageFile((char*)"fondod.jpg");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glGenTextures(1, &texturaFondoN);
	glBindTexture(GL_TEXTURE_2D, texturaFondoN);
	loadImageFile((char*)"fondon.jpg");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glGenTextures(1, &texturaPepsi);
	glBindTexture(GL_TEXTURE_2D, texturaPepsi);
	loadImageFile((char*)"pepsi.jpg");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);

	//Inicializamos lavariables de la camara
	angulo = atan(funcion(AlturaCam) / AlturaCam);
	posx = 0;
	girox = 0;
	posz = 0;
	giroz = 0;
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if (noche) {
		glLightfv(GL_LIGHT1, GL_POSITION, posicionFocal);
		glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, directionalView);
	}
	//Colocamos la camara
	gluLookAt(posx, 1, posz, girox, 0, giroz, 0, 1, 0);

	glColor3f(0.0f, 0.0f, 0.0f);
	glPolygonMode(GL_FRONT, GL_FILL);
	if (noche){
		crearfarolas();
	}
	crearPepsis();

	crearFondo();

	crearCarretera();

	updateTitulo();

	glutSwapBuffers();
}

void onReshape(GLint w, GLint h)
// Funcion de atencion al redimensionado
{
	// Se usa toda el area de dibujo
	glViewport(0, 0, w, h);
	// Definimos la camara (matriz de proyección)
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float ratio = (float)w / h;
	// Se usa camara perspectiva
	gluPerspective(45, ratio, 0.1, 1000);
}

void update()
{
	// Inicialmente la hora de arranque
	static int antes = glutGet(GLUT_ELAPSED_TIME);
	//Hora actual
	int ahora = glutGet(GLUT_ELAPSED_TIME);
	// Tiempo transcurrido
	float tiempo_transcurrido = (ahora - antes) / 1000.0f;

	posx += velocidad * cos(angulo) * tiempo_transcurrido;
	posz += velocidad * sin(angulo) * tiempo_transcurrido;

	girox = 10000 * cos(angulo);
	giroz = 10000 * sin(angulo);

	// Encolar un evento de redibujo
	antes = ahora;
	glutPostRedisplay();
}

void onEspecialKey(int specialKey, int x, int y)
{
	switch (specialKey)
	{
	case GLUT_KEY_LEFT:
		angulo -= 0.25 * PI / 180;
		break;
	case GLUT_KEY_RIGHT:
		angulo += 0.25 * PI / 180;
		break;
	case GLUT_KEY_UP:
		velocidad += 0.1;
		break;
	case GLUT_KEY_DOWN:
		velocidad -= 0.1;
		if (velocidad < 0) { velocidad = 0.0; }
		break;
	default:
		break;
	}

	glutPostRedisplay();
}

void onKey(unsigned char tecla, int x, int y) {
	switch (tecla)
	{
	case 'l':
		noche = !noche;
		break;
	}
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(50, 200);
	glutCreateWindow(PROYECTO);

	init();

	glutDisplayFunc(display);
	glutReshapeFunc(onReshape);
	glutIdleFunc(update);
	glutSpecialFunc(onEspecialKey);
	glutKeyboardFunc(onKey);

	glutMainLoop();
}
