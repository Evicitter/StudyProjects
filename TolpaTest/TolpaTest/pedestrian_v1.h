//Last update time : 19.03.2012 08:19:11

#ifndef PEDESTRIAN_V1_H
#define PEDESTRIAN_V1_H

#include "vector2d.h"
#include <vector>

class Pedestrian_v1
{
public:

    vector2d location;  //позиция
    vector2d speed;     //скорость
    vector2d accel;     //ускорение
    double   mass;      //масса
    double   radius;    //радиус


    Pedestrian_v1();

    //Функция инициализации пешехода
    void Init(double locx, double locy, double rad, double m);
    //Проверка на столкновение с другим пешеходом
    void Check(Pedestrian_v1& other, double delta);
    //Обновление состояния пешехода
    void Update(double delta);
    //рисование пешехода
    void Draw();
};

extern vector2d* pixmap;
extern Pedestrian_v1* glbPed;
extern int glbPedSize;

extern void addPed(int count);
extern void delPed();


#endif // PEDESTRIAN_V1_H
