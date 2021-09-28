//Last update time : 17.03.2012 21:21:38

#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <tmmintrin.h>

#include <math.h>

//Implementation class for SSSE3 instructions
class vector2d
{
public:
/*
    union
    {
     struct {
                double x;
                double y;
            };
     double c[2];
     __m128d v;
    };
*/
    __m128d v;

           inline vector2d()                    { v = _mm_setzero_pd(); }
           inline vector2d(double a)            { v = _mm_set1_pd(a); }
           inline vector2d(double X, double Y)  { v = _mm_set_pd(Y,X); }
           inline vector2d(const vector2d &b)   { v = b.v; }
           inline vector2d(__m128d b)           { v = b; }

    vector2d&     operator=(const vector2d &b)  { v = b.v; return *this; }

    //double&       operator[](unsigned n)        { return v.m128d_f64[n]; }
    //const double& operator[](unsigned n) const	{ return v.m128d_f64[n]; }
    double        getx() const                  { return _mm_cvtsd_f64(v); }
    double        gety() const                  { return _mm_cvtsd_f64( _mm_castsi128_pd(_mm_srli_si128(_mm_castpd_si128(v),8)) ); }
    //double*       getv()						{ return (double*      )v; }
    //const double* getv() const				{ return (const double*)v; }

    void          set(double X, double Y)		{ v = _mm_set_pd(Y,X); }
    void          set(const vector2d &b)		{ v = b.v; }

    vector2d      operator-() const             { return _mm_mul_pd(v,_mm_set1_pd(-1.0)); }

    vector2d      operator-(const vector2d &b) const { return _mm_sub_pd(v,b.v); }
    vector2d      operator+(const vector2d &b) const { return _mm_add_pd(v,b.v); }
    vector2d      operator*(const vector2d &b) const { return _mm_mul_pd(v,b.v); }
    vector2d      operator/(const vector2d &b) const { return _mm_div_pd(v,b.v); }
    double        operator&(const vector2d &b) const { __m128d tmpd = _mm_add_pd(v,b.v); return _mm_cvtsd_f64(_mm_hadd_pd(tmpd,tmpd)); }

    vector2d      operator-(const double &f) const { return _mm_sub_pd(v,_mm_set1_pd(f)); }
    vector2d      operator+(const double &f) const { return _mm_add_pd(v,_mm_set1_pd(f)); }
    vector2d      operator*(const double &f) const { return _mm_mul_pd(v,_mm_set1_pd(f)); }
    vector2d      operator/(const double &f) const { return _mm_div_pd(v,_mm_set1_pd(f)); }

    vector2d&     operator-=(const vector2d &b)     { v = _mm_sub_pd(v,b.v); return *this; }
    vector2d&     operator+=(const vector2d &b)     { v = _mm_add_pd(v,b.v); return *this; }
    vector2d&     operator*=(const vector2d &b)     { v = _mm_mul_pd(v,b.v); return *this; }
    vector2d&     operator/=(const vector2d &b)     { v = _mm_div_pd(v,b.v); return *this; }

    vector2d&     operator-=(const double &f)       { v = _mm_sub_pd(v,_mm_set1_pd(f)); return *this; }
    vector2d&     operator+=(const double &f)       { v = _mm_add_pd(v,_mm_set1_pd(f)); return *this; }
    vector2d&     operator*=(const double &f)       { v = _mm_mul_pd(v,_mm_set1_pd(f)); return *this; }
    vector2d&     operator/=(const double &f)       { v = _mm_div_pd(v,_mm_set1_pd(f)); return *this; }

    //double        operator&(const vector2d &b) const { __m128d tmpd = _mm_add_pd(v,b); return _mm_cvtsd_f64(_mm_hadd_pd(tmpd,tmpd)); }

    inline double size() const
    {
        __m128d tmpd = _mm_mul_pd(v,v);
        tmpd = _mm_hadd_pd(tmpd,tmpd);
        tmpd = _mm_sqrt_sd(tmpd,tmpd);
        return _mm_cvtsd_f64(tmpd);
    }

    inline double sizeSQ() const
    {
        __m128d tmpd = _mm_mul_pd(v,v);
        tmpd = _mm_hadd_pd(tmpd,tmpd);
        return _mm_cvtsd_f64(tmpd);
    }

    inline double dist( const vector2d& b ) const
    {
        __m128d tmpd = _mm_sub_pd(v,b.v);
        tmpd = _mm_mul_pd(tmpd,tmpd);
        tmpd = _mm_hadd_pd(tmpd,tmpd);
        tmpd = _mm_sqrt_sd(tmpd,tmpd);
        return _mm_cvtsd_f64(tmpd);
    }

    inline vector2d normal() const
    {
      __m128d tmpd = _mm_mul_pd(v,v);
      tmpd = _mm_hadd_pd(tmpd,tmpd);
      tmpd = _mm_sqrt_pd(tmpd);
      if( abs(_mm_cvtsd_f64(tmpd) - 0.00000001) > 0.00000001 ) return _mm_setzero_pd(); //vect(0,0)
      tmpd = _mm_div_pd(v, tmpd);
      return tmpd;
    }

    inline vector2d left() const
    {
        return _mm_mul_pd(_mm_shuffle_pd(v,v,1), _mm_set_pd(1.0,-1.0));
        //double ddx = getx();
        //double ddy = gety();
        //return vector2d(-ddy, ddx);
    }

    inline void clamp( const vector2d &a, const vector2d &b )
    {
        v = _mm_max_pd(v, a.v);
        v = _mm_min_pd(v, b.v);
    }

    inline void mod( const vector2d& x, const vector2d& y )
    {
        vector2d dd = x - y;
        set(fmod(getx(),dd.getx()), fmod(gety(),dd.gety()));
    }

    //sA - начало A
    //eA - конец А
    //sB - начало B
    //eB - конец B
    inline void transformA( const vector2d& sA, const vector2d& eA, const vector2d& sB, const vector2d& eB )
    {
        __m128d tmpd = _mm_div_pd(_mm_sub_pd(v,sA.v),_mm_sub_pd(eA.v,sA.v));
        v = _mm_mul_pd(tmpd,_mm_sub_pd(eB.v,sB.v));
           // return X + ((intAB-A)/(B-A))*(Y-X);
    }
};

#endif // VECTOR2D_H
