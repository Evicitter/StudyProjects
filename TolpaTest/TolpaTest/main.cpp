//Last update time : 29.02.2012 19:34:13

#include <QtGui>
#include "mainwindow.h"

int main(int argc, char** argv)
{
   QApplication app(argc, argv);

   QTextCodec* codec = QTextCodec::codecForName("UTF-8");
   //QTextCodec::setCodecForCStrings(codec);
   //QTextCodec::setCodecForLocale(codec);
   QTextCodec::setCodecForTr(codec);

   // проверяем совместимость с версией OpenGL 2.0
   if (!(QGLFormat::openGLVersionFlags() & QGLFormat::OpenGL_Version_2_0))
   {
      // если версия OpenGL 2.0 не поддерживается платформой,
      // то выводим критическое сообщение и завершаем работу приложения
      QMessageBox::critical(0, "Message", "Your platform does not support OpenGL 2.0");
      return -1; // завершение приложения
   }

   MainWindow mainwindow1;
   mainwindow1.resize(640, 480);
   mainwindow1.show(); //Maximized();

   return app.exec();
}
