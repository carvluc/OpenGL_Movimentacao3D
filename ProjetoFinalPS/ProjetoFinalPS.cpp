#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <GL/glut.h>

#define RED		1
#define BLUE	3
#define BLACK	7

// Variáveis globais
GLfloat anguloCubo = 0.0f,
		anguloAux = 0.0f,
		moveCubo = 0.0f,
		moveAux = 0.0f;

char *titulo = (char*)"Projeto Final";

// Conteúdo para aplicação de textura
static float materialColor[8][4] =
{
	{ 0.8, 0.8, 0.8, 1.0 },
{ 0.8, 0.0, 0.0, 1.0 },
{ 0.0, 0.8, 0.0, 1.0 },
{ 0.0, 0.0, 0.8, 1.0 },
{ 0.0, 0.8, 0.8, 1.0 },
{ 0.8, 0.0, 0.8, 1.0 },
{ 0.8, 0.8, 0.0, 1.0 },
{ 0.0, 0.0, 0.0, 0.6 },
}; 

// Definições de padrão de luz
static float lightPos[4] =
{ 2.0, 4.0, 2.0, 1.0 };
#if 0
static float lightDir[4] =
{ -2.0, -4.0, -2.0, 1.0 };
#endif
static float lightAmb[4] =
{ 0.2, 0.2, 0.2, 1.0 };
static float lightDiff[4] =
{ 0.8, 0.8, 0.8, 1.0 };
static float lightSpec[4] =
{ 0.4, 0.4, 0.4, 1.0 };

// Representará a sombra
static float groundPlane[4] =
{ 0.0, 1.0, 0.0, 1.499 }; 

static float fogColor[4] =
{ 0.0, 0.0, 0.0, 0.0 };
static float fogIndex[1] =
{ 0.0 };

// Definições da sombreamento
static unsigned char shadowPattern[128] =
{
	0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,  /* 50% Grey */
	0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
	0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
	0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
	0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
	0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
	0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
	0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
	0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
	0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
	0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
	0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
	0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
	0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
	0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
	0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55
}; 

static float verticesCubo[6][4][4] =
{
	{
		{ -1.0, -1.0, -1.0, 1.0 },
{ -1.0, -1.0, 1.0, 1.0 },
{ -1.0, 1.0, 1.0, 1.0 },
{ -1.0, 1.0, -1.0, 1.0 } },

  {
	  { 1.0, 1.0, 1.0, 1.0 },
{ 1.0, -1.0, 1.0, 1.0 },
{ 1.0, -1.0, -1.0, 1.0 },
{ 1.0, 1.0, -1.0, 1.0 } },

  {
	  { -1.0, -1.0, -1.0, 1.0 },
{ 1.0, -1.0, -1.0, 1.0 },
{ 1.0, -1.0, 1.0, 1.0 },
{ -1.0, -1.0, 1.0, 1.0 } },

  {
	  { 1.0, 1.0, 1.0, 1.0 },
{ 1.0, 1.0, -1.0, 1.0 },
{ -1.0, 1.0, -1.0, 1.0 },
{ -1.0, 1.0, 1.0, 1.0 } },

  {
	  { -1.0, -1.0, -1.0, 1.0 },
{ -1.0, 1.0, -1.0, 1.0 },
{ 1.0, 1.0, -1.0, 1.0 },
{ 1.0, -1.0, -1.0, 1.0 } },

  {
	  { 1.0, 1.0, 1.0, 1.0 },
{ -1.0, 1.0, 1.0, 1.0 },
{ -1.0, -1.0, 1.0, 1.0 },
{ 1.0, -1.0, 1.0, 1.0 } }
};

static float normaisCubo[6][4] =
{
	{ -1.0, 0.0, 0.0, 0.0 },
{ 1.0, 0.0, 0.0, 0.0 },
{ 0.0, -1.0, 0.0, 0.0 },
{ 0.0, 1.0, 0.0, 0.0 },
{ 0.0, 0.0, -1.0, 0.0 },
{ 0.0, 0.0, 1.0, 0.0 }
};

static void setColor(int c)
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, &materialColor[c][0]);
}

static void DesenhaCubo(int color)
{
	setColor(color);

	for (int i = 0; i < 6; ++i) {
		glNormal3fv(&normaisCubo[i][0]);
		glBegin(GL_POLYGON);
		glVertex4fv(&verticesCubo[i][0][0]);
		glVertex4fv(&verticesCubo[i][1][0]);
		glVertex4fv(&verticesCubo[i][2][0]);
		glVertex4fv(&verticesCubo[i][3][0]);
		glEnd();
	}
}

static void DesenhaChao(int w, int h, int evenColor, int oddColor)
{
	static int initialized = 0;
	static int usedLighting = 0;
	static GLuint checklist = 0;

	if (!initialized) {
		static float square_normal[4] =
		{ 0.0, 0.0, 1.0, 0.0 };
		static float square[4][4];
		int i, j;

		if (!checklist) {
			checklist = glGenLists(1);
		}

		glNewList(checklist, GL_COMPILE_AND_EXECUTE);

		for (j = 0; j < h; ++j)
			for (i = 0; i < w; ++i) {
				square[0][0] = -1.0 + 2.0 / w * i;
				square[0][1] = -1.0 + 2.0 / h * (j + 1);
				square[0][2] = 0.0;
				square[0][3] = 1.0;

				square[1][0] = -1.0 + 2.0 / w * i;
				square[1][1] = -1.0 + 2.0 / h * j;
				square[1][2] = 0.0;
				square[1][3] = 1.0;

				square[2][0] = -1.0 + 2.0 / w * (i + 1);
				square[2][1] = -1.0 + 2.0 / h * j;
				square[2][2] = 0.0;
				square[2][3] = 1.0;

				square[3][0] = -1.0 + 2.0 / w * (i + 1);
				square[3][1] = -1.0 + 2.0 / h * (j + 1);
				square[3][2] = 0.0;
				square[3][3] = 1.0;

				if (i & 1 ^ j & 1) {
					setColor(oddColor);
				}
				else {
					setColor(evenColor);
				}

				glBegin(GL_POLYGON);

				glVertex4fv(&square[0][0]);
				glVertex4fv(&square[1][0]);
				glVertex4fv(&square[2][0]);
				glVertex4fv(&square[3][0]);

				glEnd();
			}

		glEndList();

		initialized = 1;
	}
	else {
		glCallList(checklist);
	}
}

static void ShadowMatrix(float ground[4], float light[4])
{
	float dot;
	float shadowMat[4][4];

	dot = ground[0] * light[0] +
		ground[1] * light[1] +
		ground[2] * light[2] +
		ground[3] * light[3];

	shadowMat[0][0] = dot - light[0] * ground[0];
	shadowMat[1][0] = 0.0 - light[0] * ground[1];
	shadowMat[2][0] = 0.0 - light[0] * ground[2];
	shadowMat[3][0] = 0.0 - light[0] * ground[3];

	shadowMat[0][1] = 0.0 - light[1] * ground[0];
	shadowMat[1][1] = dot - light[1] * ground[1];
	shadowMat[2][1] = 0.0 - light[1] * ground[2];
	shadowMat[3][1] = 0.0 - light[1] * ground[3];

	shadowMat[0][2] = 0.0 - light[2] * ground[0];
	shadowMat[1][2] = 0.0 - light[2] * ground[1];
	shadowMat[2][2] = dot - light[2] * ground[2];
	shadowMat[3][2] = 0.0 - light[2] * ground[3];

	shadowMat[0][3] = 0.0 - light[3] * ground[0];
	shadowMat[1][3] = 0.0 - light[3] * ground[1];
	shadowMat[2][3] = 0.0 - light[3] * ground[2];
	shadowMat[3][3] = dot - light[3] * ground[3];

	glMultMatrixf((const GLfloat *)shadowMat);
}

void Teclado(unsigned char ch, int x, int y)
{
	if (ch == 27) {
		exit(0);
	}
	else if (ch == 'A') {
		moveCubo -= 0.10;
	}
	else if (ch == 'a') {
		moveCubo += 0.10;
	}
	else if (ch == 'D') {
		moveAux -= 0.10;
	}
	else if (ch == 'd') {
		moveAux += 0.10;
	}
}

void Desenha(void)
{
	GLfloat cubeXform[4][4];

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	glTranslatef(0.0, -1.5, 0.0);
	glRotatef(-90.0, 1, 0, 0);
	glScalef(2.0, 2.0, 2.0);

	DesenhaChao(6, 6, BLUE, BLACK);  /* draw ground */
	glPopMatrix();

	glPushMatrix();

	glTranslatef(0.0f, 0.0f, 0.0f); // Garante rotações do cubo
	glRotatef(anguloAux, 0, 1, 0);
	glRotatef(anguloCubo, 1, 0, 0);
	glTranslatef(-0.8f, 0.0f, 0.0f);

	glScalef(0.15, 0.15, 0.15); // Diminuindo tamanho para se adequar ao campo de visão
	glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)cubeXform);

	DesenhaCubo(RED);        /* draw cube */
	glPopMatrix();

	glDepthMask(GL_FALSE);

	glEnable(GL_BLEND);

	glPushMatrix();
	ShadowMatrix(groundPlane, lightPos);
	glTranslatef(0.0, 0.0, 2.0);
	glMultMatrixf((const GLfloat *)cubeXform);

	DesenhaCubo(BLACK);      /* draw ground shadow */
	glPopMatrix();

	glDepthMask(GL_TRUE);

	glDisable(GL_BLEND);

	glutSwapBuffers();

	anguloCubo -= moveCubo;
	anguloAux -= moveAux;
}

void Tempo(int value) {
	glutPostRedisplay(); // Função de renderização infinita de ms em ms
	glutTimerFunc(1, Tempo, 0);
}

int main(int argc, char **argv)
{
	int width = 512, height = 512;
	
	glutInitWindowSize(width, height);
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutCreateWindow(titulo);

	glutKeyboardFunc(Teclado);
	glutDisplayFunc(Desenha);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1.0, 1.0, -1.0, 1.0, 0.8, 45.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -2.0);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_LIGHTING);

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpec);

#if 0
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightDir);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 80);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 25);
#endif

	glEnable(GL_NORMALIZE);
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogfv(GL_FOG_INDEX, fogIndex);
	glFogf(GL_FOG_MODE, GL_EXP);
	glFogf(GL_FOG_DENSITY, 0.5);
	glFogf(GL_FOG_START, 1.0);
	glFogf(GL_FOG_END, 3.0);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glShadeModel(GL_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPolygonStipple((const GLubyte *)shadowPattern);
	glClearColor(0.0, 0.0, 0.0, 1);
	glClearIndex(0);
	glClearDepth(1);
	glutTimerFunc(0, Tempo, 0);
	glutMainLoop();
	return 0; 
}