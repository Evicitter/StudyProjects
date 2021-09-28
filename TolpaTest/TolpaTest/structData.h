//Last update time : 29.02.2012 22:34:13

#ifndef STRUCTDATA_H
#define STRUCTDATA_H

#include <QObject>

class PaintDD : public QObjectUserData
{
  public:
    PaintDD( int nTag ) : Tag(nTag) {};
    PaintDD( int nTag, double X, double Y ) : Tag(nTag), dx(X), dy(Y) {};

    int Tag;
    double dx;
    double dy;
};

#endif // STRUCTDATA_H
