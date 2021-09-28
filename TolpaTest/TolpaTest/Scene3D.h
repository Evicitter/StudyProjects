//Last update time : 17.03.2012 10:19:50

#ifndef SCENE3D_H
#define SCENE3D_H

//#include <QtOpenGL/QtOpenGL>
#include <QtOpenGL/QGLWidget>

class Scene3D : public QGLWidget
{
   Q_OBJECT

   private:
      int bRenderMode;
      int IndexMM; //Индекс вызываемой функции перемещения мыши (Или индекс режима рисования)
      QPoint mousePosition; //Позиция мыши
      double dirX;
      double dirY;

      GLfloat ratio;

      GLfloat xRot1;
      GLfloat yRot1;
      GLfloat zRot1;
      GLfloat zTra1;

      GLfloat xRot2;
      GLfloat yRot2;
      GLfloat zRot2;
      GLfloat zTra2;

      QTimer *timer;

      void getVerTexArrays();
      void getIndexArray();
      void genTextures();
      void drawBackground();
      void drawPoints();
      void draw_ui();

      void selectFigures(QPoint mp);

   protected:
      void initializeGL();
      void resizeGL(int nWidth, int nHeight);
      void paintGL();
      void mousePressEvent(QMouseEvent* pe);
      void mouseMoveEvent(QMouseEvent * me);

   private slots:
      void FnewFile();
      void FopenFile();
      void FsaveFile();
      void FcloseFile();
      //-----------------
      void Pselecta(QAction* at);
      void PfillDir(bool bc);

      //-----------------
      void change();
      void changeTex();
      void stopTmr();
      void startTmr();

   public:
      Scene3D(QWidget* parent = 0);
      ~Scene3D();
};

#endif // SCENE3D_H
