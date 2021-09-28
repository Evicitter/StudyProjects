//Last update time : 29.02.2012 22:36:34

#include <QtGui>
#include "mainwindow.h"
#include "scene3D.h"
#include "structData.h"

MainWindow::MainWindow()
{
   QTextCodec::setCodecForTr(QTextCodec::codecForName(""));

   scene1 = new Scene3D(this);

   setCentralWidget(scene1);

   this->setWindowTitle(tr("MassModeling"));

   createActions();
   createMenus();
   createToolbar();
   createStatus();

   // проверяем, поддерживается ли сглаживание
   if (scene1->format().sampleBuffers()) // мультивыборка поддерживается
       statusLog->setText(QString("Multisampling is supported"));
      // сообщение-информация:
      //QMessageBox::information(0, "Ok", "Multisampling is supported");
   else // сглаживание не поддерживается
       statusLog->setText(QString("Multisampling does not supported"));
      // сообщение-предупреждение:
      //QMessageBox::warning(0, "Problem", "Multisampling does not supported");
}

void MainWindow::createActions()
{
   filea_new = new QAction(QIcon(":/ico/icons/new.png"), tr("New"), this);
   filea_open = new QAction(QIcon(":/ico/icons/open.png"), tr("Open"), this);
   filea_save = new QAction(QIcon(":/ico/icons/save.png"), tr("Save"), this);
   filea_close = new QAction(QIcon(":/ico/icons/close.png"), tr("Close"), this);
   filea_new->setToolTip(trUtf8("Создание нового файла"));
   filea_open->setToolTip(trUtf8("Открытие существующего файла"));
   filea_save->setToolTip(trUtf8("Сохранкние текущего файла"));
   filea_close->setToolTip(trUtf8("Закрыть текущий файл"));

   connect(filea_new, SIGNAL(triggered()), scene1, SLOT(FnewFile()));
   connect(filea_open, SIGNAL(triggered()), scene1, SLOT(FopenFile()));
   connect(filea_save, SIGNAL(triggered()), scene1, SLOT(FsaveFile()));
   connect(filea_close, SIGNAL(triggered()), scene1, SLOT(FcloseFile()));
   //---------------------------------------------------------------------------
   paint_ag = new QActionGroup( this );

   paint_cursor = new QAction(QIcon(":/ico/icons/new.png"), tr("Cursor"), this);
   paint_select = new QAction(QIcon(":/ico/icons/new.png"), tr("Select"), this);
   paint_fill = new QAction(QIcon(":/ico/icons/new.png"), tr("Fill"), this);

   paint_cursor->setToolTip(trUtf8("Курсор мыши"));
   paint_cursor->setCheckable(true);
   paint_cursor->setChecked(true);
   paint_cursor->setActionGroup( paint_ag );
   paint_cursor->setUserData(0, new PaintDD(0) );

   paint_select->setToolTip(trUtf8("Режим выделения"));
   paint_select->setCheckable(true);
   paint_select->setActionGroup( paint_ag );
   paint_select->setUserData(0, new PaintDD(1) );

   paint_fill->setToolTip(trUtf8("Закрашивание"));
   paint_fill->setCheckable(true);
   paint_fill->setActionGroup( paint_ag );
   paint_fill->setUserData(0, new PaintDD(2) );

   paint_fill->setMenu( new QMenu() );
   paint_fillDir0 = paint_fill->menu()->addAction( "dir 0" ); paint_fillDir0->setUserData(0, new PaintDD(0, 0.0,1.0));
   paint_fillDir1 = paint_fill->menu()->addAction( "dir 1" ); paint_fillDir1->setUserData(0, new PaintDD(1, 1.0,1.0));
   paint_fillDir2 = paint_fill->menu()->addAction( "dir 2" ); paint_fillDir2->setUserData(0, new PaintDD(2, 1.0,0.0));
   paint_fillDir3 = paint_fill->menu()->addAction( "dir 3" ); paint_fillDir3->setUserData(0, new PaintDD(3, 1.0,-1.0));
   paint_fillDir4 = paint_fill->menu()->addAction( "dir 4" ); paint_fillDir4->setUserData(0, new PaintDD(4, 0.0,-1.0));
   paint_fillDir5 = paint_fill->menu()->addAction( "dir 5" ); paint_fillDir5->setUserData(0, new PaintDD(5, -1.0,-1.0));
   paint_fillDir6 = paint_fill->menu()->addAction( "dir 6" ); paint_fillDir6->setUserData(0, new PaintDD(6, -1.0,0.0));
   paint_fillDir7 = paint_fill->menu()->addAction( "dir 7" ); paint_fillDir7->setUserData(0, new PaintDD(7, -1.0,1.0));

   connect( paint_fillDir0, SIGNAL(triggered(bool)), scene1, SLOT(PfillDir(bool)) );
   connect( paint_fillDir1, SIGNAL(triggered(bool)), scene1, SLOT(PfillDir(bool)) );
   connect( paint_fillDir2, SIGNAL(triggered(bool)), scene1, SLOT(PfillDir(bool)) );
   connect( paint_fillDir3, SIGNAL(triggered(bool)), scene1, SLOT(PfillDir(bool)) );
   connect( paint_fillDir4, SIGNAL(triggered(bool)), scene1, SLOT(PfillDir(bool)) );
   connect( paint_fillDir5, SIGNAL(triggered(bool)), scene1, SLOT(PfillDir(bool)) );
   connect( paint_fillDir6, SIGNAL(triggered(bool)), scene1, SLOT(PfillDir(bool)) );
   connect( paint_fillDir7, SIGNAL(triggered(bool)), scene1, SLOT(PfillDir(bool)) );

   connect( paint_ag, SIGNAL( selected( QAction* ) ), scene1, SLOT( Pselecta( QAction* ) ) );
   //---------------------------------------------------------------------------

   changeTexAct = new QAction(tr("Change"), this);
   connect(changeTexAct, SIGNAL(triggered()), scene1, SLOT(changeTex()));

   stopTimAct = new QAction(tr("Stop"), this);
   connect(stopTimAct, SIGNAL(triggered()), scene1, SLOT(stopTmr()));

   startTimAct = new QAction(tr("Start"), this);
   connect(startTimAct, SIGNAL(triggered()), scene1, SLOT(startTmr()));
}

void MainWindow::createMenus()
{
   file_menu = menuBar()->addMenu(tr("File"));
   file_menu->addAction(filea_new);
   file_menu->addAction(filea_open);
   file_menu->addAction(filea_save);
   file_menu->addAction(filea_close);

   texture_menu = menuBar()->addMenu(tr("Texture"));
   texture_menu->addAction(changeTexAct);

   timer_menu = menuBar()->addMenu(tr("Animation"));
   timer_menu->addAction(stopTimAct);
   timer_menu->addAction(startTimAct);
}

void MainWindow::createStatus()
{
    statusLog = new QLabel(statusBar());
    statusProgress = new QProgressBar(statusBar());
    statusBar()->addWidget( statusLog );
    statusBar()->addWidget( statusProgress );
}

void MainWindow::createToolbar()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(filea_new);
    fileToolBar->addAction(filea_open);
    fileToolBar->addAction(filea_save);
    fileToolBar->addAction(filea_close);
    fileToolBar->addSeparator();
    fileToolBar->addAction(paint_cursor);
    fileToolBar->addAction(paint_select);
    fileToolBar->addAction(paint_fill);
}
