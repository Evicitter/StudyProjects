//Last update time : 19.03.2012 02:52:20

#include "pedestrian_v1.h"

#include <GL/gl.h>

#include <math.h>
#include <mm_malloc.h>

vector2d* pixmap=NULL;
Pedestrian_v1* glbPed=NULL;
int glbPedSize=0;

#define drand() ((double)rand() / (double)RAND_MAX)

void addPed(int count)
{
    if( glbPed != NULL ) { _mm_free(glbPed); glbPed=NULL; }
    glbPedSize = count;
    glbPed = (Pedestrian_v1*)_mm_malloc( count * sizeof(Pedestrian_v1), 16 );
    if( glbPed == NULL ) glbPedSize = 0;
}

void delPed()
{
    if( glbPed != NULL )
    {
        _mm_free( glbPed );
        glbPed=NULL;
        glbPedSize=0;
    }
}

Pedestrian_v1::Pedestrian_v1()
{
    location = vector2d(0.0);
    speed   = vector2d(0.0);
    accel   = vector2d(0.0);
    radius  = 1.0;
    mass    = 1.0;
}

void Pedestrian_v1::Init(double locx, double locy, double rad, double m)
{
    location = vector2d(locx,locy);
    radius = rad;
    mass = m; //0.5 + (rand() / 32768.0 * 2.0);
}

void Pedestrian_v1::Check(Pedestrian_v1& other, double delta)
{
    double d = location.dist(other.location);
    if( d < (0.12) ) //if( d < (radius + other.radius) )
    {
        double f = (0.12) - d; //double f = (radius + other.radius) - d;
        location += (( location - other.location ) * f) / d / mass * delta;
        other.location -= (( location - other.location ) * f) / d / other.mass * delta;

        //speed += (( location - other.location ) * f) / d / mass * delta;
        //other.speed -= (( location - other.location ) * f) / d / other.mass * delta;
    }
}

void Pedestrian_v1::Update(double delta)
{
    int ddx=(int)(((location.getx() - 0.0) / (4.0 - 0.0)) * 256.0);
    int ddy=(int)(((location.gety() - -4.0) / (0.0 - -4.0)) * 256.0);

    //vector2d grad((double)ddx,(double)ddy);
    //grad = grad.normal(); * 0.05;
    ddx = (ddx + 256) % 256;
    ddy = (ddy + 256) % 256;

    accel = (pixmap[ ddy*256 + ddx ].normal()) / mass;
    speed = accel * delta * delta;
    location += speed;

    location.clamp( vector2d(0.0,-4.0), vector2d(4.0,0.0) );
}

void Pedestrian_v1::Draw()
{
    glPointSize(radius);
    glBegin(GL_POINTS);
        glColor3d(0.0,0.0,0.5);
    //glColor3d( 0.0 , 1.0 - (mass / 2.0) ,0.0);
        glVertex2d( location.getx(), location.gety() );
    glEnd();
}
