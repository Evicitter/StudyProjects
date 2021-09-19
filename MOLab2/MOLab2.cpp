//Last update : 11.06.2011 03:41:54

#ifndef _WIN32_WINNT            
#define _WIN32_WINNT 0x0600
#endif

#include "stdio.h"
#include "tchar.h"
#include <intrin.h>

#include <Windows.h>
//#include <stdlib.h>
#include <locale.h>
#include <GL\GL.h>
#include <GL\GLU.h>
#include "glut.h"
#include <math.h>
#include <float.h>
#include <time.h>

//#include <conio.h>

#include "erFunctions.h"

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"glut32.lib")

#define MAX 600 //задает начальные параметры размера окна и порта просмотра

static float xMin=FLT_MAX;
static float xMax=FLT_MIN;
static float xStep=0.01f;
static float Ligtness=1.0f;
static float Saturation=1.0f;
static int LastIters=0;

static int bFindMode=0;
static int bShowGradientVector=0; //Rotor vector
static int bSetNormalVector=0;
static int bShowBlendFunctions=0;
static bool bTrack=false;			//mouse control

void aftc0(float x)	{	glColor3f(1.0f, 0.0f, 0.0f);	}
void aftc1(float x)	{	glColor3f(1.0f, x * 6.0f, 0.0f);}
void aftc2(float x)	{	glColor3f(1.0f - (x - 1.0f/6) * 6.0f, 1.0f, 0.0f);	}
void aftc3(float x)	{	glColor3f(0.0f, 1.0f, (x - 2.0f/6.0f) * 6.0f);	}
void aftc4(float x)	{	glColor3f(0.0f, 1.0f - (x - 3.0f/6.0f) * 6.0f, 1.0f);	}
void aftc5(float x)	{	glColor3f((x - 4.0f/6.0f) * 6.0f, 0.0f, 1.0f);}
void aftc6(float x)	{	glColor3f(1.0f, 0.0f, 1.0f - (x - 5.0f/6.0f) * 6.0f);	}


typedef void (* fTc)(float x);
fTc afTC[7] = { aftc1, aftc2, aftc3, aftc4, aftc5, aftc6, aftc0 };

void floatToColor2(float x)	
{
	afTC[ (int)(x*6.0f) ](x);
}

float gFun(float x, float y)
{
	//return x*y*expf(-(x*x/3.0f) - (y*y/4.0f));
	return XFunctions[CurVariant](x,y);
}

static vector2D Start(0.0f,0.0f);
static vector2D CurrPos(0.0f,0.0f);
static vector2D PointMin(0.0f,0.0f);

static vector2D d1(0.0f,1.0f);
static vector2D d2(1.0f,0.0f);
static vector2D d3(0.0f,-1.0f);
static vector2D d4(-1.0f,0.0f);

#define rrand(a,b) (a) + ((float)rand()/32768.0f)*((b)-(a))

static int WindowWidth=MAX,WindowHight=MAX, //ширина и высота окна 
OldViewportWidth=MAX,OldViewportHight=MAX; //ширина и высота порта просмотра на момент изменения размера окна

void HSV_to_RGB( float H, float S, float V ) { 	// H is given on [0, 6] or UNDEFINED. S and V are given on [0, 1]. 
	// RGB are each returned on [0, 1]. 
	float m, n, f; 
	int i; 

	if (H == 333.0f) glColor3f(V, V, V); 
	i = floorf(H);
	f = H - i; 
	if ( !(i&1) ) f = 1.0f - f; // if i is even 
	m = V * (1.0f - S); 
	n = V * (1.0f - S * f); 
	switch (i) { 
		case 6: 
		case 0: glColor3f(V, n, m); return; 
		case 1: glColor3f(n, V, m); return; 
		case 2: glColor3f(m, V, n);  return;
		case 3: glColor3f(m, n, V);  return;
		case 4: glColor3f(n, m, V);  return;
		case 5: glColor3f(V, m, n);  return;
	} 
}

void init()
{
	xMax=FLT_MIN;
	xMin=FLT_MAX;
	for(float xx=Origin[CurVariant].Left;	xx<Origin[CurVariant].Right;	xx+=0.01f)
	for(float yy=Origin[CurVariant].Bottom;	yy<Origin[CurVariant].Top;		yy+=0.01f)
	{
		register float t = gFun(xx,yy);
		if(bShowBlendFunctions==2)
		 t = t + powf(max(-TFunctions[0](xx,yy), 0.000000001f),1.0f);
		if(t<xMin) { xMin = t; PointMin.x = xx; PointMin.y = yy; }
		if(t>xMax) xMax = t;
	}
	
	glNewList(1, GL_COMPILE); //GL_COMPILE_AND_EXECUTE);
	glBegin(GL_POINTS);
	for(float xx=Origin[CurVariant].Left;	xx<Origin[CurVariant].Right;	xx+=0.01f)
	for(float yy=Origin[CurVariant].Bottom; yy<Origin[CurVariant].Top;		yy+=0.01f)
	{
		register float t = gFun(xx,yy);
		if(bShowBlendFunctions==2)
			t = t + powf(max(-TFunctions[0](xx,yy), 0.000000001f),1.0f);
		t = (t-xMin)/(xMax-xMin);
		floatToColor2(t);		
		glVertex2f(xx,yy);
	}
	glEnd();
	glEndList();
}

void Reshape(int WindowWidth, int WindowHight) //функция перерисовки
{
	float fratio;

	if (WindowWidth<WindowHight) //в зависимости от того какой параметр окна меньше
		fratio=((float)WindowWidth/OldViewportWidth);
	else 
		fratio=((float)WindowHight/OldViewportHight); 

	OldViewportWidth*=fratio; //получаем новую ширину и высоту
	OldViewportHight*=fratio;
	glViewport(0,0,OldViewportWidth,OldViewportHight);

	glClear(GL_COLOR_BUFFER_BIT); 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); 
	gluOrtho2D(Origin[CurVariant].Left,Origin[CurVariant].Right,Origin[CurVariant].Bottom,Origin[CurVariant].Top);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0,0.0,0.0,0.0);

	glCallList(1);

	glLineWidth(1.5f);
	glBegin(GL_LINES);

	CurrPos = Start;

	vector2D LastVect;
	int tt=10000;

	if(bShowGradientVector)
	{
	 for(float sX=Origin[CurVariant].Left; sX<Origin[CurVariant].Right; sX+=0.1f)
	 for(float sY=Origin[CurVariant].Bottom; sY<Origin[CurVariant].Top; sY+=0.1f)
	 {
		 LastVect.x = sX;
		 LastVect.y = sY;
		 glColor3f(0.0f,0.0f,0.0f);
		 glVertex2f(sX,sY);
		 LastVect = LastVect.AntiGrad(0.001f, bShowBlendFunctions>=1);
		 //LastVect = LastVect.AntiHessian(0.001f);
		 if(bSetNormalVector) LastVect = (LastVect / LastVect.SizeSqrt()) * 0.1f;
		 if(bShowGradientVector == 2) LastVect = LastVect.LPerp();
		 else if(bShowGradientVector == 3) LastVect = LastVect.RPerp();
		 glColor3f(0.0f,0.75f,0.75f);
		 glVertex2f(sX + LastVect.x, sY + LastVect.y);
	 }
	}

	glColor3f(1.0f,1.0f,1.0f);
	if(bFindMode == 0)
	{
		float fcT=0.0f;
		vector2D ag;
		vector2D FindVect;
		vector2D MinPosition;
		float findMin=FLT_MAX;
		float findFun=0.0f;
		while(--tt>0)
		{
			float dx = (CurrPos-LastVect).SizeSqrt();

			if(dx < 0.1f) { wprintf_s(L"F0_MinPos={%f,%f}\n", CurrPos.x, CurrPos.y); break; }
			vector2D ag = CurrPos.AntiGrad(xStep,bShowBlendFunctions>=1);
			dx = ag.SizeSqrt();
			ag = (ag / dx) * 2.0f;
			//if(dx < xStep) break;

			glVertex2f(CurrPos.x,CurrPos.y);

			fcT=0.0f;
			findMin=FLT_MAX;
			while(fcT<1.0f)
			{
				fcT += 0.01f;
				FindVect = (CurrPos*(1.0f-fcT) + (CurrPos+ag)*fcT);
				findFun=FindVect.vFunction(bShowBlendFunctions>=1);
				if(findFun < findMin)
				{
					findMin=findFun;
					MinPosition = FindVect;
				}
			}
			LastVect = CurrPos;	
			CurrPos = MinPosition;
			//CurrPos = CurrPos + CurrPos.AntiGrad(xStep, bShowBlendFunctions>=1);

			glVertex2f(CurrPos.x,CurrPos.y);
		}
	}
	else if(bFindMode == 1)
	{
		float fmc=FLT_MAX,fm1=FLT_MAX,fm2=FLT_MAX,fm3=FLT_MAX,fm4=FLT_MAX;
		while(--tt>0)
		{
			float dx = (CurrPos-PointMin).SizeSqrt();

			if(dx < 0.01f) { wprintf_s(L"F1_MinPos={%f,%f}\n", CurrPos.x, CurrPos.y); break; }

			glVertex2f(CurrPos.x,CurrPos.y);
			//fmc = CurrPos.vFunction();

			fm1 = (CurrPos + (d1*xStep)).vFunction(bShowBlendFunctions>=1);
			fm2 = (CurrPos + (d2*xStep)).vFunction(bShowBlendFunctions>=1);
			fm3 = (CurrPos + (d3*xStep)).vFunction(bShowBlendFunctions>=1);
			fm4 = (CurrPos + (d4*xStep)).vFunction(bShowBlendFunctions>=1);
			register float cMin = min(fmc,min(fm1,min(fm2,min(fm3,fm4))));

			if(cMin == fm1) CurrPos = (CurrPos + (d1*xStep));
			else if(cMin == fm2) CurrPos = (CurrPos + (d2*xStep));
			else if(cMin == fm3) CurrPos = (CurrPos + (d3*xStep));
			else if(cMin == fm4) CurrPos = (CurrPos + (d4*xStep));
			else if(cMin == fmc) { break; }

			glVertex2f(CurrPos.x,CurrPos.y);
		}
	}
	else if( bFindMode == 2 )
	{
		int count=0;
		CurrPos = Start;
		vector2D BorderS;
		vector2D BorderE;
		vector2D cFindVector;
		float fcMin=FLT_MAX;
		float fr=FLT_MAX;
		float fcT=0.0f;
		while(--tt>0)
		{
			float dx = (CurrPos-LastVect).SizeSqrt();
			if(dx < 0.001f) { wprintf_s(L"F2_MinPos={%f,%f}\n", CurrPos.x, CurrPos.y); break; }

			if(count & 1) //Bottom - Top = Vertical  (Down -> Up)
			{
				Origin[CurVariant].GetYLine(CurrPos,BorderS,BorderE);
				glColor3f(1.0f,0.0f,0.0f);
			}
			else //Left - Right = Horizontal  (Left -> Right)
			{
				Origin[CurVariant].GetXLine(CurrPos,BorderS,BorderE);
				glColor3f(0.0f,0.0f,1.0f);
			}

			LastVect = CurrPos;
			fcT = 0.0f;
			while(fcT < 1.0f)
			{
				cFindVector = BorderS*(1.0f-fcT) + BorderE*fcT;
				fr = cFindVector.vFunction( bShowBlendFunctions>=1 );
				fcT += xStep;
				if(fr<fcMin)
				{
					fcMin=fr; CurrPos = cFindVector;
				}
				--tt;
			}

			glVertex2f(BorderS.x,BorderS.y);

			if(count & 1)	// vertical
				glVertex2f(BorderE.x,CurrPos.y);
			else			//horizontal
				glVertex2f(CurrPos.x,BorderE.y);

			++count;
		}
	}

	glEnd();

	if(LastIters != tt) { wprintf_s(L"Iterations = %i\n", 10000-tt ); LastIters=tt; }

	glFlush();
	//glFinish();
	glutSwapBuffers();
}

void Idle()
{
	Draw();
	Sleep(10ul);
}

void Key(unsigned char key, int x, int y)
{
 if(key == 'z')
 {
	xStep-=0.01f;
	if(xStep<0.01f) xStep=0.01f;
	wprintf_s(L"xStep = %f\n", xStep);
	glutPostRedisplay(); //Redraw
 }
 else if(key == 'x')
 {
	xStep+=0.01f;
	wprintf_s(L"xStep = %f\n", xStep);
	glutPostRedisplay(); //Redraw
 }
 else if(key == 'm')
 {
	bFindMode = (bFindMode+1) % 3;
	if(bFindMode == 0) wprintf_s(L"Gradient mode\n\n");
	else if(bFindMode == 1) wprintf_s(L"Stair-Step mode\n\n");
	else if(bFindMode == 2) wprintf_s(L"Coordinate descent mode\n\n");
	glutPostRedisplay(); //Redraw
 }
 else if(key == 't')
 {
	bShowGradientVector = (bShowGradientVector+1) & 3;
	if(bShowGradientVector == 0) wprintf_s(L"Hide Vectors\n\n");
	else if(bShowGradientVector == 1) wprintf_s(L"Show Gradient Vectors\n\n");
	else if(bShowGradientVector == 2) wprintf_s(L"Show Left Rotors\n\n");
	else if(bShowGradientVector == 3) wprintf_s(L"Show Right Rotors\n\n");
	glutPostRedisplay(); //Redraw
 }
 else if(key == 'y')
 {
	bSetNormalVector = ~bSetNormalVector;
	if(bSetNormalVector) wprintf_s(L"Set Normal Vectors\n\n");
	else wprintf_s(L"Set UnNormal Vectors\n\n");
	glutPostRedisplay(); //Redraw
 }
 else if(key == '3')
 {
	bShowBlendFunctions = ++bShowBlendFunctions % 3;
	init();
	glutPostRedisplay();
 }
 else if(key == '[')
 {
	CurVariant -= 1i8;
	if(CurVariant < 0i8) CurVariant=0i8;
	wprintf_s(L"Variant (%i) = %s\n", CurVariant, VariantName[CurVariant]);
	Start.x=Start.y=0.0f;
	glClear(GL_COLOR_BUFFER_BIT); 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); 
	gluOrtho2D(Origin[CurVariant].Left,Origin[CurVariant].Right,Origin[CurVariant].Bottom,Origin[CurVariant].Top);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	init();
	glutPostRedisplay(); //Redraw
 }
 else if(key == ']')
 {
	CurVariant += 1i8;
	if(CurVariant > 29i8) CurVariant=29i8;
	wprintf_s(L"Variant (%i) = %s\n", CurVariant, VariantName[CurVariant]);
	Start.x=Start.y=0.0f;
	glClear(GL_COLOR_BUFFER_BIT); 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); 
	gluOrtho2D(Origin[CurVariant].Left,Origin[CurVariant].Right,Origin[CurVariant].Bottom,Origin[CurVariant].Top);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	init();
	glutPostRedisplay(); //Redraw
 }
}

void Mouse(int button, int state, int x, int y)
{
	if((button==2) && (state==0))
	{
		Start.x = Origin[CurVariant].TransformPointByX(x,WindowWidth);
		Start.y = Origin[CurVariant].TransformPointByY(y,WindowHight);
		wprintf_s(L"Start Point = (%f,%f) \t (%ix%i)\n", Start.x, Start.y, x,y);
		if(bFindMode==0)
		{
			vector2D agStart = Start; agStart = agStart.AntiGrad(xStep);
			wprintf_s(L"AntiGradient(Start) = (%f,%f)\n", agStart.x, agStart.y);
		}
		wprintf_s(L"Min Point = (%f,%f)\n", PointMin.x, PointMin.y);

		glutPostRedisplay(); //Redraw
	}
	if(button==1)
	{
		bTrack = (state==0);
	}
}

void Motion(int x, int y)
{
	if(bTrack)
	{
		Start.x = Origin[CurVariant].TransformPointByX(x,WindowWidth);
		Start.y = Origin[CurVariant].TransformPointByY(y,WindowHight);
		glutPostRedisplay(); //Redraw
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	srand(time(0)); //randomize для цвета

	_wsetlocale(LC_ALL,L"rus");

	glutInit(&argc, (char**)argv);
	glutInitWindowSize(WindowWidth,WindowHight); 
	glutInitWindowPosition(0,0);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); 
	glutCreateWindow("MOLab2"); 
	glClearColor(0,0,0,0);
  
	glutReshapeFunc(Reshape);
	glutDisplayFunc(Draw);
	glutKeyboardFunc(Key);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	//glutIdleFunc(Idle);
	
	init();
	
	glEnable(GL_POINT_SMOOTH);
	glutMainLoop();

	glDeleteLists(1,1);

	return 0;
}