//Last update time : 29.02.2012 22:16:59

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QProgressBar>

#include <QActionGroup>

#include "scene3D.h"

class MainWindow : public QMainWindow
{
   Q_OBJECT

   public:
      Scene3D* scene1;


      QMenu* texture_menu;
      QMenu* timer_menu;

   //files category
      QMenu* file_menu;
   //file actions
      QAction* filea_new;
      QAction* filea_open;
      QAction* filea_save;
      QAction* filea_close;
   //paint actions
      QActionGroup* paint_ag;
      QAction* paint_cursor;
      QAction* paint_select;
      QAction* paint_fill;
      QAction* paint_fillDir0;
      QAction* paint_fillDir1;
      QAction* paint_fillDir2;
      QAction* paint_fillDir3;
      QAction* paint_fillDir4;
      QAction* paint_fillDir5;
      QAction* paint_fillDir6;
      QAction* paint_fillDir7;
   //---------------------
      QToolBar* fileToolBar;
      //--------------------

      QAction* changeTexAct;
      QAction* stopTimAct;
      QAction* startTimAct;

      //status bar ---------
      QLabel* statusLog;
      QProgressBar* statusProgress;
      //---------------------


      QToolBar *editToolBar;


      void createActions();
      void createMenus();
      void createStatus();
      void createToolbar();

   public:
      MainWindow();
};

#endif
