#-------------------------------------------------
#
# Project created by QtCreator 2012-02-20T01:05:01
#
#-------------------------------------------------

QT       += core gui \
           opengl

TARGET = TolpaTest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    Scene3D.cpp \
    pedestrian_v1.cpp \
    vector2d.cpp

HEADERS  += mainwindow.h \
    Scene3D.h \
    glext.h \
    pedestrian_v1.h \
    vector2d.h \
    structData.h

FORMS    += mainwindow.ui

RESOURCES += \
    pictures.qrc
