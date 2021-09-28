//Last update time : 19.03.2012 08:19:17

#include <QtGui>
#include <time.h>
#include <math.h>
#include <GL/glu.h>

#include <omp.h>

#include "structData.h"
#include "scene3D.h"
#include "mainwindow.h"

#include "glext.h"

#include <mm_malloc.h>

//------------------------------------
#include "vector2d.h"
#include "pedestrian_v1.h"
//------------------------------------

#define transf(a,b,c) (b + ((c - b)*( (double)(a) / 256.0 )))
#define frand(a,b) ( a + (b-a)*( (double)rand() / (double)RAND_MAX) )
const GLfloat pi=3.141593; //, k=pi/180;

GLint viewport[4];

GLfloat fLeft=0.0f;
GLfloat fRight=4.0f;
GLfloat fBottom=-4.0f;
GLfloat fTop=0.0f;
GLfloat VertexArray[4][2];
GLfloat TextureArrayBG[4][2];
GLuint IndexArray[8][3];
GLuint textureID[2];

GLint signs[2]={1, 1};
bool motionParameters[2]={1, 1};
int textureParameters[2]={0, 1};

unsigned char* Plotn=NULL;

inline double FUNX(double x, double y) { return x*y*exp(-(x*x/3.0) - (y*y/4.0)); }

Scene3D::Scene3D(QWidget* parent) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    srand( time(NULL) );
    bRenderMode = 0;
    Plotn = (unsigned char*)_mm_malloc(64*64,16);
    memset(Plotn,0, 64*64);

    dirX=0.0;
    dirY=0.0;
    pixmap = (vector2d*)_mm_malloc( 256*256*sizeof(vector2d), 16 );
    const double xstep = 0.001;
    for(int i=0; i<256; ++i)
    for(int j=0; j<256; ++j)
    {
        double ddx = fLeft + (fRight - fLeft)*( (double)j / 256.0 );
        double ddy = fBottom + (fTop - fBottom)*( (double)i / 256.0 );
        double dy = (FUNX(ddx,ddy - xstep) - FUNX(ddx,ddy + xstep)) / 2.0*xstep;
        double dx = (FUNX(ddx - xstep,ddy) - FUNX(ddx + xstep,ddy)) / 2.0*xstep;
        pixmap[ i*256 + j ] = vector2d(dx, dy);
    }

   xRot1=-90.0f; yRot1=0.0f; zRot1=0.0f; zTra1=0.0f;
   xRot2=-90.0f; yRot2=0.0f; zRot2=0.0f; zTra2=0.0f;

   timer = new QTimer(this); // создаём объект таймера
   connect(timer, SIGNAL(timeout()), this, SLOT(change())); // связываем сигналы, генерируемые таймером, со слотом:
   timer->start(20); // запускаем таймер с интервалом 20 миллисекунд

   QGLFormat frmt;
   frmt.setSwapInterval(1);
   frmt.setSampleBuffers(true);
   setFormat(frmt);

   addPed( 1000 );
   if( glbPed != NULL ) for(int i=0; i<glbPedSize; ++i)
   { glbPed[i].Init(frand(fLeft,fRight), frand(fBottom,fTop), 8.0,0.45); }

   //------------------------
   setMouseTracking(true);
}

Scene3D::~Scene3D()
{
    // контекст рендеринга виджета станет текущим контекстом рендеринга OpenGL:
       //makeCurrent();
       //glDeleteTextures(2, textureID); // удаляем текстурные объекты
    if(Plotn != NULL) _mm_free(Plotn);
    if(pixmap != NULL) _mm_free(pixmap);
    delPed();
}

void Scene3D::initializeGL()
{
   qglClearColor(Qt::black);
   glEnable(GL_DEPTH_TEST);
   //glEnable(GL_CULL_FACE);
   //glDisable(GL_CULL_FACE);
   //glEnable(GL_TEXTURE_2D);
   glEnable(GL_MULTISAMPLE);

   getVerTexArrays();
   getIndexArray();
   genTextures();

   glEnableClientState(GL_VERTEX_ARRAY);
}

void Scene3D::resizeGL(int nWidth, int nHeight)
{
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   ratio=(GLfloat)nHeight/(GLfloat)nWidth;

   //if (nWidth>=nHeight)
   //   glOrtho(-2.0/ratio, 2.0/ratio, -2.0, 2.0, -10.0, 10.0);
   //else
   //   glOrtho(-2.0, 2.0, -2.0*ratio, 2.0*ratio, -10.0, 10.0);
   gluOrtho2D( fLeft, fRight, fBottom, fTop );

   glViewport(0, 0, (GLint)nWidth, (GLint)nHeight);
   glGetIntegerv(GL_VIEWPORT, viewport);
}

double xRot=0.0;

void Scene3D::paintGL()
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   //VERY BAD
   //TODO: use RenderToTexture
	   
   if(bRenderMode == 1)
   {
	   
       glBegin( GL_LINES );
       for( int i=0; i<256; i+=4 )
       for( int j=0; j<256; j+=4 )
       {
           double ddx = fLeft + (fRight - fLeft)*( (double)j / 256.0 );
           double ddy = fBottom + (fTop - fBottom)*( (double)i / 256.0 );
           glColor3ub(0,0,0);
           glVertex2d( ddx, ddy );
           ddx += pixmap[ i*256 + j ].normal().getx() * 0.1;
           ddy += pixmap[ i*256 + j ].normal().gety() * 0.1;
           glColor3ub(255,255,255);
           glVertex2d( ddx, ddy );
       }
       glEnd();
       return;
   }
   else if(bRenderMode == 2)
   {
       if(Plotn == NULL)
           return;
       memset(Plotn, 0, 64*64);

       for(int i=0; i<glbPedSize; ++i)
       {
           int xx = (int)(((glbPed[i].location.getx() - fLeft) / (fRight - fLeft)) * 255.0);
           int yy = (int)(((glbPed[i].location.gety() - fBottom) / (fTop - fBottom)) * 255.0);

           xx /= 4;
           yy /= 4;

           ++Plotn[yy*64 + xx];
       }

       //glPointSize(1.0);
       //glBegin( GL_POINTS );
       glBegin( GL_QUADS );
       for( int i=0; i<256; i+=4 )
       for( int j=0; j<256; j+=4 )
       {
           //double ddx = transf(j, fLeft,fRight);
           //double ddy = transf(i, fBottom,fTop);

           int c = (double)Plotn[ (i/4)*64 + (j/4)] / 16.0 * 255.0;
           glColor3ub(c,c,c);

           glVertex2d( transf(j, fLeft,fRight), transf(i, fBottom,fTop) );
           glVertex2d( transf(j+4, fLeft,fRight), transf(i, fBottom,fTop) );
           glVertex2d( transf(j+4, fLeft,fRight), transf(i+4, fBottom,fTop) );
           glVertex2d( transf(j, fLeft,fRight), transf(i+4, fBottom,fTop) );

           //double ddx = transf(j, fLeft,fRight);
           //double ddy = transf(i, fBottom,fTop);
           //glVertex2d( ddx, ddy );
       }
       glEnd();
       return;
   }
   //draw_ui();

   drawPoints();
   drawBackground();
}

void Scene3D::mousePressEvent(QMouseEvent* pe)
{
   mousePosition = pe->pos();
   //this->selectFigures(mousePosition);
}

void Scene3D::mouseMoveEvent(QMouseEvent *me)
{
   mousePosition = me->pos();
   /*static vector2d lastmpos;
   if( me->buttons() & Qt::MiddleButton )
   {
       double mvmatrix[16];
       double projmatrix[16];
       GLdouble XX,YY,ZZ;
       glGetIntegerv( GL_VIEWPORT, viewport );
       glGetDoublev(GL_MODELVIEW_MATRIX,mvmatrix);
       glGetDoublev(GL_PROJECTION_MATRIX,projmatrix);
       gluUnProject( (double)mousePosition.x(), (double)viewport[3] - (double)mousePosition.y(), 0.0, mvmatrix, projmatrix, viewport, &XX,&YY,&ZZ);

       for(int i=0; i<360; ++i)
       {
        vector2d angv( cos(i * 2.0 * M_PI / 360.0), sin(i * 2.0 * M_PI / 360.0) );
        double nX = XX + angv.getx()*0.025;
        double nY = YY + angv.gety()*0.025;
        //vector2d dddd = vector2d( XX-nX, YY-nY );
        nX = (nX - fLeft) / (fRight - fLeft) * 256.0;
        nY = (nY - fBottom) / (fTop - fBottom) * 256.0;

        nX = (nX < 0.0) ? 0.0 : nX;
        nX = (nX > 255.0) ? 255.0 : nX;
        nY = (nY < 0.0) ? 0.0 : nY;
        nY = (nY > 255.0) ? 255.0 : nY;
        pixmap[ (int)nY * 256 + (int)nX ] = (vector2d(XX,YY) - lastmpos).normal(); //dddd * 0.35;
       }
       lastmpos = vector2d(XX,YY);
   }*/
}

void Scene3D::genTextures()
{
    // создаём, связываем, загружаем, возвращаем уникальный номер:
    textureID[0]=bindTexture(QPixmap(QString(":/picture2.jpg")), GL_TEXTURE_2D);

    // далее параметры текстурного объекта
    // при фильтрации игнорируются тексели, выходящие за границу текстуры для s координаты
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // при фильтрации игнорируются тексели, выходящие за границу текстуры для t координаты
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // цвет текселя полностью замещает цвет фрагмента фигуры
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    // создаём, связываем, загружаем, возвращаем уникальный номер:
    textureID[1]=bindTexture(QPixmap(QString(":/picture1.jpg")), GL_TEXTURE_2D);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    return ;
    // загрузка изображений
       QImage image1, image2; // создаём объекты класса QImage (изображения)
       image1.load(":/picture2.jpg"); // загружаем изображение в переменную image1
       image2.load(":/picture1.jpg"); // загружаем изображение в переменную image2
       // конвертируем изображение в формат для работы с OpenGL:
       image1=QGLWidget::convertToGLFormat(image1);
       image2=QGLWidget::convertToGLFormat(image2);

       // создание имён для текстурных объектов
       glGenTextures(2, textureID); // создаём два имени и записываем их в массив

       // создаём и связываем текстурные объекты с состоянием текстуры
       // 1-ый текстурный объект
       glBindTexture(GL_TEXTURE_2D, textureID[0]);
       // связываем текстурный объект с изображением
       glTexImage2D(GL_TEXTURE_2D, 0, 3, (GLsizei)image1.width(), (GLsizei)image1.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image1.bits());

       // 2-ой текстурный объект
       glBindTexture(GL_TEXTURE_2D, textureID[1]);
       glTexImage2D(GL_TEXTURE_2D, 0, 3, (GLsizei)image2.width(), (GLsizei)image2.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image2.bits());

       // линейная фильтрация вблизи:
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
       // линейная фильтрация вдали:
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
       
	   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
       // цвет текселя полностью замещает цвет фрагмента фигуры
       glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

void Scene3D::getVerTexArrays()
{
   //GLfloat R=1.0f;
   //GLfloat alpha=pi/2;

   VertexArray[0][0]=fLeft;
   VertexArray[0][1]=fBottom;
   VertexArray[1][0]=fLeft;
   VertexArray[1][1]=fTop;
   VertexArray[2][0]=fRight;
   VertexArray[2][1]=fTop;
   VertexArray[3][0]=fRight;
   VertexArray[3][1]=fBottom;
   TextureArrayBG[0][0]=0.0f;
   TextureArrayBG[0][1]=0.0f;
   TextureArrayBG[1][0]=0.0f;
   TextureArrayBG[1][1]=1.0f;
   TextureArrayBG[2][0]=1.0f;
   TextureArrayBG[2][1]=1.0f;
   TextureArrayBG[3][0]=1.0f;
   TextureArrayBG[3][1]=0.0f;
}

void Scene3D::getIndexArray()
{

}

void Scene3D::drawBackground()
{
   //glEnable(GL_TEXTURE_2D);
   //glEnableClientState(GL_TEXTURE_COORD_ARRAY);
   //glBindTexture(GL_TEXTURE_2D, textureID[textureParameters[0]]); //glColor3d(0.5,0.6,0.55);
   glColor4d(1.0,1.0,1.0,1.0);
   glVertexPointer(2, GL_FLOAT, 0, VertexArray);
   //glTexCoordPointer(2, GL_FLOAT, 0, TextureArrayBG);
   glDrawArrays(GL_QUADS, 0, 4);
   //glDisableClientState( GL_TEXTURE_COORD_ARRAY );
   //glDisable(GL_TEXTURE_2D);
}

void Scene3D::drawPoints()
{
    /*glEnableClientState(GL_COLOR_ARRAY);
    //glColor3d(1.0,0.0,0.0);
    glVertexPointer(2, GL_FLOAT, 0, pPoints);
    glColorPointer( 4, GL_FLOAT, 0, pColors );
    glDrawArrays(GL_POINTS, 0, 4096);
    glDisableClientState(GL_COLOR_ARRAY);
*/

    if(glbPed != NULL)
    {
        glDisableClientState( GL_VERTEX_ARRAY );
        for(int i=0; i<glbPedSize; ++i) glbPed[i].Draw();
        glEnableClientState( GL_VERTEX_ARRAY );
    }
}

void Scene3D::draw_ui()
{
    if( IndexMM == 2 )
    {
      double mvmatrix[16];
      double projmatrix[16];
      GLdouble XX,YY,ZZ;
      glGetIntegerv  ( GL_VIEWPORT, viewport );
      glGetDoublev(GL_MODELVIEW_MATRIX,mvmatrix);
      glGetDoublev(GL_PROJECTION_MATRIX,projmatrix);
      gluUnProject( (double)mousePosition.x(), (double)viewport[3] - (double)mousePosition.y(), 0.0, mvmatrix, projmatrix, viewport, &XX,&YY,&ZZ);

      glBegin(GL_LINE_LOOP);
       glColor3d( 0.0, 0.0, 0.0 );
       glVertex2d( XX + 0.1, YY + 0.1 );
       glVertex2d( XX + 0.1, YY - 0.1 );
       glVertex2d( XX - 0.1, YY - 0.1 );
       glVertex2d( XX - 0.1, YY + 0.1 );
      glEnd();
    }
}

void Scene3D::selectFigures(QPoint mp)
{
}

void Scene3D::change()
{
    //glBindTexture( GL_TEXTURE_2D, textureID[textureParameters[0]] );
    //glTexImage2D(GL_TEXTURE_2D, 0, 3, (GLsizei)image1.width(), (GLsizei)image1.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image1.bits());
    //if(glbPed != NULL) for(int i=0; i<glbPedSize; ++i) glbPed[i].Check();
    static float lasttime=-0.1f;
    register float cctime = (float)clock();

    cctime = (cctime - lasttime) / 1000.0f;


    if(glbPed != NULL)
    {
#pragma omp parallel for schedule(static,256)
     for(int i=0; i<glbPedSize; ++i)
         for(int j=i+1; j<glbPedSize; ++j) glbPed[i].Check(glbPed[j], cctime);
    }

    if(glbPed != NULL)
    {
#pragma omp parallel for schedule(static,128)
        for(int i=0; i<glbPedSize; ++i) glbPed[i].Update(cctime);
    }

    MainWindow* mw = dynamic_cast<MainWindow*>(parentWidget());
    if(mw!=NULL)
    {
        mw->statusLog->setText( QString::number( 1000.0f / (cctime * 1000.0f) ) );
    }

    lasttime = (float)clock();

   updateGL();
}

void Scene3D::changeTex()
{
   if (textureParameters[0]==0)
   {
      textureParameters[0]=1;
      textureParameters[1]=0;
   }
   else
   {
      textureParameters[0]=0;
      textureParameters[1]=1;
   }

   updateGL();
}

void Scene3D::stopTmr() { timer->stop(); }
void Scene3D::startTmr() { timer->start(); }
//------------------------------------------------------------------------------------------------------------

void Scene3D::FnewFile()
{
 //clear all
     //alloc
}

void Scene3D::FopenFile()
{
 //load from file
}

void Scene3D::FsaveFile()
{
 //save file
}

void Scene3D::FcloseFile()
{
    //only clear
    ++bRenderMode;
    bRenderMode %= 3;
}

void Scene3D::Pselecta(QAction *at)
{
    PaintDD* pp = dynamic_cast<PaintDD*>(at->userData(0));
    if( pp != NULL )
    {
        Qt::CursorShape dd[4] = { Qt::ArrowCursor, Qt::CrossCursor, Qt::OpenHandCursor, Qt::ArrowCursor };
        IndexMM = pp->Tag;
        setCursor( dd[IndexMM] );
    }
}

void Scene3D::PfillDir(bool bc)
{
    MainWindow* mw = dynamic_cast<MainWindow*>(parentWidget());
    QAction* qa = dynamic_cast<QAction*>(sender());
    if(qa != NULL)
    {
      mw->statusLog->setText( qa->text() );
      PaintDD* pd = dynamic_cast<PaintDD*>(qa->userData(0));
      if(pd != NULL)
      {
       dirX = pd->dx;
       dirY = pd->dy;
      }
    }
}