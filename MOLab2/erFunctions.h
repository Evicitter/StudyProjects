//Date: 11/06/2011 03:40:34

class vector2D;
class vrect;

extern __int8 CurVariant;
extern wchar_t* VariantName[30];
extern vrect Origin[30];

typedef float (__fastcall *geFun)(float x,float y);

extern geFun XFunctions[30];
extern geFun TFunctions[30]; 

#include <math.h>
class vector2D
{
public:
	float x,y;
	vector2D() : x(0.0f), y(0.0f) {}
	vector2D(float X, float Y) : x(X), y(Y) {}

	vector2D& operator =(const vector2D& v) { this->x=v.x; this->y=v.y; return *this; }
	vector2D operator +(const vector2D& v) const { return (vector2D(x + v.x, y + v.y)); }
	vector2D operator -(const vector2D& v) const { return (vector2D(x - v.x, y - v.y)); }
	vector2D operator *(const vector2D& v) const { return (vector2D(x * v.x, y * v.y)); }
	vector2D operator /(const vector2D& v) const { return (vector2D(x / v.x, y / v.y)); }

	vector2D operator *(const float v) const { return (vector2D(x * v, y * v)); }
	vector2D operator /(const float v) const { return (vector2D(x / v, y / v)); }

	float vFunction(bool bCompare=false) const;
	float SizeSqrt() const;

	vector2D AntiGrad(float step=0.01f, bool bCompare=false)
	{
		vector2D xbase(x+step,y);
		vector2D ybase(x,		 y+step);
		return vector2D( -(xbase.vFunction(bCompare) - vFunction(bCompare)) / step,
										 -(ybase.vFunction(bCompare) - vFunction(bCompare)) / step);
	}

	vector2D Gradient(float step=0.01f) //Central Difference
	{
		vector2D xbaseP(x+step,y);
		vector2D ybaseP(x,		  y+step);
		vector2D xbaseM(x-step,y);
		vector2D ybaseM(x,		  y-step);
		return vector2D( (xbaseP.vFunction(false) - xbaseM.vFunction(false)) / 2.0f*step,
										 (ybaseP.vFunction(false) - xbaseM.vFunction(false)) / 2.0f*step);
	}


	vector2D AntiHessian(float step=0.01)
	{
		/*float hessian[2][2];

		vector2D fPlus;
		vector2D fMinus;

		float curFValue = vFunction(false); //значение функции в текущей точке 

		//d2f/d2x
		fPlus.x = vector2D(x + step,0.0).vFunction(false);
		fMinus.x = vector2D(x - step,0.0).vFunction(false);
		hessian[0][0] = (fPlus.x - 2.0 * curFValue + fMinus.x) / (step * step);
		//d2f/d2y
		fPlus.y = vector2D(0.0,y + step).vFunction(false);
		fMinus.y = vector2D(0.0,y - step).vFunction(false);
		hessian[1][1] = (fPlus.y - 2.0 * curFValue + fMinus.y) / (step * step);

		//d2f/d2x*d2y
		hessian[0][1] = 0.0;//(vector2D(x + step, y + step).vFunction(false) - fPlus.x - fPlus.y + curFValue) /(step * step);
		//d2f/d2y*d2x
		hessian[1][0] = hessian[0][1];

		float Idet = 1.0f / (hessian[0][0]*hessian[1][1] - hessian[0][1]*hessian[1][0]);
		float revMat[2][2];
		revMat[0][0] = hessian[1][1]*Idet;
		revMat[0][1] =-hessian[0][1]*Idet;
		revMat[1][0] =-hessian[1][0]*Idet; 
		revMat[1][1] = hessian[0][0]*Idet;

		vector2D grad = vector2D(0.0f,0.0f) - Gradient(step);
		fMinus = grad / grad.SizeSqrt();
		//grad.x = fMinus.x * revMat[0][0] + fMinus.y * revMat[1][0];
		//grad.y = fMinus.x * revMat[0][1] + fMinus.y * revMat[1][1];
		grad.x = fMinus.x * revMat[0][0] + fMinus.y * revMat[0][1];
		grad.y = fMinus.x * revMat[1][0] + fMinus.y * revMat[1][1];

		return grad;*/

		double hessian[2][2];

		//d2f/d2x
		//http://www.wolframalpha.com/input/?i=d%5E2%2Fdx%5E2+%5Bx%2Aexp%28-%28x%2Ax%2F2%29+-+%28y%2Ay%2F2%29%29%5D
		hessian[0][0] = x*(x*x-3.0)*exp(-(x*x/2.0) - (y*y/2.0));
		//d2f/d2y
		//http://www.wolframalpha.com/input/?i=d%5E2%2Fdy%5E2+%5Bx%2Aexp%28-%28x%2Ax%2F2%29+-+%28y%2Ay%2F2%29%29%5D
		hessian[1][1] = x*(y*y-1.0)*exp(-(x*x/2.0) - (y*y/2.0));

		//d2f/d2x*d2y
		//http://www.wolframalpha.com/input/?i=d%5E2%2Fdx%5E2+d%5E2%2Fdy%5E2+%5Bx%2Aexp%28-%28x%2Ax%2F2%29+-+%28y%2Ay%2F2%29%29%5D
		hessian[0][1] = x*(x*x-3.0)*(y*y-1.0)*exp(-(x*x/2.0) - (y*y/2.0));
		//d2f/d2y*d2x
		//http://www.wolframalpha.com/input/?i=d%5E2%2Fdy%5E2+d%5E2%2Fdx%5E2+%5Bx%2Aexp%28-%28x%2Ax%2F2%29+-+%28y%2Ay%2F2%29%29%5D
		hessian[1][0] = x*(x*x-3.0)*(y*y-1.0)*exp(-(x*x/2.0) - (y*y/2.0));

		double Idet = 1.0 / (hessian[0][0]*hessian[1][1] - hessian[0][1]*hessian[1][0]);
		double revMat[2][2];
		revMat[0][0] = hessian[1][1]*Idet;
		revMat[0][1] =-hessian[0][1]*Idet;
		revMat[1][0] =-hessian[1][0]*Idet; 
		revMat[1][1] = hessian[0][0]*Idet;

		vector2D grad = Gradient(step);
		//grad.x = fMinus.x * revMat[0][0] + fMinus.y * revMat[1][0];
		//grad.y = fMinus.x * revMat[0][1] + fMinus.y * revMat[1][1];
		grad = vector2D(grad.x * revMat[0][0] + grad.y * revMat[0][1],
										grad.x * revMat[1][0] + grad.y * revMat[1][1]);

		return grad;
	}

	vector2D LPerp() const { return vector2D(-y,x); }
	vector2D RPerp() const { return vector2D(y,-x); }
};

class vrect
{
public:
	float Left,Right,Bottom,Top;

	vrect() : Left(0.0f), Right(1.0f), Bottom(0.0f), Top(1.0f) {}
	vrect(float l, float r, float b, float t) : Left(l), Right(r), Bottom(b), Top(t) {}

	float TransformPointByX(int x, int maxx) { return Left + ((float)x/(float)maxx)*(Right - Left); }
	float TransformPointByY(int y, int maxy) { return Top + ((float)y/(float)maxy)*(Bottom - Top); }

	void GetXLine(const vector2D& C, vector2D& Start, vector2D& End, bool bLeft=true)
	{
		End.y=Start.y=C.y;
		if(bLeft)	{ Start.x=Left;		End.x=Right;	}
		else		{ Start.x=Right;	End.x=Left;		}
	}
	void GetYLine(const vector2D& C, vector2D& Start, vector2D& End, bool bBottom=true)
	{
		End.x=Start.x=C.x;
		if(bBottom)	{ Start.y=Bottom;	End.y=Top;		}
		else		{ Start.y=Top;		End.y=Bottom;	}
	}
};