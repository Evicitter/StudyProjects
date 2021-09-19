//Date: 09/06/2011 11:02:48

//#include "stdafx.h"

#include "erFunctions.h"
#include <math.h>

__int8 CurVariant=0i8;

float vector2D::SizeSqrt() const { return sqrtf(x*x+y*y); }
float vector2D::vFunction(bool bCompare) const
{
#define tmax(a,b)            (((a) > (b)) ? (a) : (b))
	if(bCompare)
		return XFunctions[CurVariant](x,y) + powf(tmax(-TFunctions[0](x,y), 0.000000001f), 1.0f);
	return XFunctions[CurVariant](x,y);
#undef tmax
}

float __fastcall F3_01(float x, float y) { return (-(y*y/3.0f)-x-2.0f); }//{ return (y*(2.0f-y))-(5.0f*x)+8.0f; }			//а
float __fastcall F3_02(float x, float y) { return x*x*y*expf(-(x*x/3.0f) - (y*y/4.0f)); }		//б
float __fastcall F3_03(float x, float y) { return x*y*y*expf(-(x*x/3.0f) - (y*y/4.0f)); }		//в
float __fastcall F3_04(float x, float y) { return -(x*x)*y*y*expf(-(x*x/3.0f) - (y*y/4.0f)); } //г
float __fastcall F3_05(float x, float y) { return -(x*x)*y*y*expf(-(x*x/3.0f) - (y*y/2.0f)); } //д
float __fastcall F3_06(float x, float y) { return -(x*x)*y*y*expf(-(x*x/2.0f) - (y*y/2.0f)); } //её
float __fastcall F3_07(float x, float y) { return -(x*x)*y*y*expf(-(x*x/4.0f) - (y*y/2.0f)); } //ж
float __fastcall F3_08(float x, float y) { return x*x*y*expf(-(x*x/4.0f) - (y*y/2.0f)); } //з
float __fastcall F3_09(float x, float y) { return x*y*y*expf(-(x*x/4.0f) - (y*y/2.0f)); } //и,й
float __fastcall F3_10(float x, float y) { return x*y*expf(-(x*x/3.0f) - (y*y/9.0f)); } //к
float __fastcall F3_11(float x, float y) { return x*y*y*y*y*expf(-(x*x/6.0f) - (y*y/8.0f)); } //л
float __fastcall F3_12(float x, float y) { return x*y*y*y*expf(-(x*x/6.0f) - (y*y/7.0f)); } //м
float __fastcall F3_13(float x, float y) { return x*x*y*y*y*expf(-(x*x/5.0f) - (y*y/6.0f)); } //н
float __fastcall F3_14(float x, float y) { return x*x*x*y*y*y*expf(-(x*x/2.0f) - (y*y/3.0f)); } //о
float __fastcall F3_15(float x, float y) { return x*x*x*y*y*y*y*expf(-(x*x/2.0f) - (y*y/3.0f)); } //п
float __fastcall F3_16(float x, float y) { return x*expf(-(x*x/2.0f) - (y*y/4.0f)); } //р
float __fastcall F3_17(float x, float y) { return -(x*x)*expf(-(x*x/2.0f) - (y*y/4.0f)); } //с
float __fastcall F3_18(float x, float y) { return x*x*x*expf(-(x*x/2.0f) - (y*y/4.0f)); } //т
float __fastcall F3_19(float x, float y) { return x*expf(-(x*x/2.0f) - (y*y/2.0f)); } //у
float __fastcall F3_20(float x, float y) { return x*expf(-(x*x/4.0f) - (y*y/2.0f)); } //ф
float __fastcall F3_21(float x, float y) { return x*expf(-(x*x/2.0f) - (y*y/4.0f)); } //х
float __fastcall F3_22(float x, float y) { return -(x*x)*expf(-(x*x/2.0f) - (y*y/4.0f)); } //ц,ч
float __fastcall F3_23(float x, float y) { return -(x*x)*expf(-(x*x/4.0f) - (y*y/4.0f)); } //ш,щ
float __fastcall F3_24(float x, float y) { return -(x*x)*expf(-(x*x/2.0f) - (y*y/4.0f)); } //ъ,ь
float __fastcall F3_25(float x, float y) { return x*x*x*expf(-(x*x/3.0f) - (y*y/4.0f)); } //ы
float __fastcall F3_26(float x, float y) { return x*x*x*x*x*expf(-(x*x/2.0f) - (y*y/2.0f)); } //э
float __fastcall F3_27(float x, float y) { return x*x*x*expf(-(x*x/3.0f) - (y*y/3.0f)); } //ю
float __fastcall F3_28(float x, float y) { return -(x*x*x*x)*expf(-(x*x/4.0f) - (y*y/4.0f)); } //я
float __fastcall F3_29(float x, float y) { return -(x*x*x*x)*expf(-(x*x/5.0f) - (y*y/7.0f)); } //1
float __fastcall F3_30(float x, float y) { return (x*x*x*x*x)*expf(-(x*x/7.0f) - (y*y/7.0f)); } //2

float __fastcall Fun01(float x, float y) { return x*y*expf(-(x*x/3.0f) - (y*y/4.0f)); }			//а
float __fastcall Fun02(float x, float y) { return x*x*y*expf(-(x*x/3.0f) - (y*y/4.0f)); }		//б
float __fastcall Fun03(float x, float y) { return x*y*y*expf(-(x*x/3.0f) - (y*y/4.0f)); }		//в
float __fastcall Fun04(float x, float y) { return -(x*x)*y*y*expf(-(x*x/3.0f) - (y*y/4.0f)); } //г
float __fastcall Fun05(float x, float y) { return -(x*x)*y*y*expf(-(x*x/3.0f) - (y*y/2.0f)); } //д
float __fastcall Fun06(float x, float y) { return -(x*x)*y*y*expf(-(x*x/2.0f) - (y*y/2.0f)); } //её
float __fastcall Fun07(float x, float y) { return -(x*x)*y*y*expf(-(x*x/4.0f) - (y*y/2.0f)); } //ж
float __fastcall Fun08(float x, float y) { return x*x*y*expf(-(x*x/4.0f) - (y*y/2.0f)); } //з
float __fastcall Fun09(float x, float y) { return x*y*y*expf(-(x*x/4.0f) - (y*y/2.0f)); } //и,й
float __fastcall Fun10(float x, float y) { return x*y*expf(-(x*x/3.0f) - (y*y/9.0f)); } //к
float __fastcall Fun11(float x, float y) { return x*y*y*y*y*expf(-(x*x/6.0f) - (y*y/8.0f)); } //л
float __fastcall Fun12(float x, float y) { return x*y*y*y*expf(-(x*x/6.0f) - (y*y/7.0f)); } //м
float __fastcall Fun13(float x, float y) { return x*x*y*y*y*expf(-(x*x/5.0f) - (y*y/6.0f)); } //н
float __fastcall Fun14(float x, float y) { return x*x*x*y*y*y*expf(-(x*x/2.0f) - (y*y/3.0f)); } //о
float __fastcall Fun15(float x, float y) { return x*x*x*y*y*y*y*expf(-(x*x/2.0f) - (y*y/3.0f)); } //п
float __fastcall Fun16(float x, float y) { return x*expf(-(x*x/2.0f) - (y*y/4.0f)); } //р
float __fastcall Fun17(float x, float y) { return -(x*x)*expf(-(x*x/2.0f) - (y*y/4.0f)); } //с
float __fastcall Fun18(float x, float y) { return x*x*x*expf(-(x*x/2.0f) - (y*y/4.0f)); } //т
float __fastcall Fun19(float x, float y) { return x*expf(-(x*x/2.0f) - (y*y/2.0f)); } //у
float __fastcall Fun20(float x, float y) { return x*expf(-(x*x/4.0f) - (y*y/2.0f)); } //ф
float __fastcall Fun21(float x, float y) { return x*expf(-(x*x/2.0f) - (y*y/4.0f)); } //х
float __fastcall Fun22(float x, float y) { return -(x*x)*expf(-(x*x/2.0f) - (y*y/4.0f)); } //ц,ч
float __fastcall Fun23(float x, float y) { return -(x*x)*expf(-(x*x/4.0f) - (y*y/4.0f)); } //ш,щ
float __fastcall Fun24(float x, float y) { return -(x*x)*expf(-(x*x/2.0f) - (y*y/4.0f)); } //ъ,ь
float __fastcall Fun25(float x, float y) { return x*x*x*expf(-(x*x/3.0f) - (y*y/4.0f)); } //ы
float __fastcall Fun26(float x, float y) { return x*x*x*x*x*expf(-(x*x/2.0f) - (y*y/2.0f)); } //э
float __fastcall Fun27(float x, float y) { return x*x*x*expf(-(x*x/3.0f) - (y*y/3.0f)); } //ю
float __fastcall Fun28(float x, float y) { return -(x*x*x*x)*expf(-(x*x/4.0f) - (y*y/4.0f)); } //я
float __fastcall Fun29(float x, float y) { return -(x*x*x*x)*expf(-(x*x/5.0f) - (y*y/7.0f)); } //1
float __fastcall Fun30(float x, float y) { return (x*x*x*x*x)*expf(-(x*x/7.0f) - (y*y/7.0f)); } //2


vrect Origin[30] = {
	vrect( 0.0f,  4.0f, -4.0f,  0.0f), //а
	vrect(-4.0f,  0.0f, -4.0f,  0.0f),  //б
	vrect(-4.0f,  0.0f, -4.0f,  0.0f),  //в
	vrect( 0.0f,  4.0f,  0.0f,  4.0f),  //г
	vrect(-4.0f,  0.0f,  0.0f,  4.0f),  //д
	vrect(-4.0f,  0.0f, -4.0f,  0.0f),  //е,ё
	vrect(-4.0f,  0.0f,  0.0f,  4.0f),  //ж
	vrect( 0.0f,  4.0f, -4.0f,  0.0f),  //з
	vrect(-4.0f,  0.0f,  0.0f,  4.0f),  //и,й
	vrect( 0.0f,  4.0f, -4.0f,  0.0f),  //к
	vrect(-4.0f,  0.0f, -7.0f,  0.0f),  //л
	vrect( 0.0f,  4.0f, -7.0f,  0.0f),  //м
	vrect(-5.0f,  0.0f, -6.0f,  0.0f),  //н
	vrect(-4.0f,  0.0f,  0.0f,  6.0f),  //о
	vrect(-4.0f,  0.0f,  0.0f,  6.0f),  //п
	vrect(-3.0f,  0.0f, -4.0f,  3.0f),  //р
	vrect( 0.0f,  4.0f, -4.0f,  4.0f),  //с
	vrect(-4.0f,  0.0f, -4.0f,  4.0f),  //т
	vrect(-3.0f,  0.0f, -4.0f,  4.0f),  //у
	vrect(-4.0f,  0.0f, -3.0f,  3.0f),  //ф
	vrect(-3.0f,  0.0f, -4.0f,  3.0f),  //х
	vrect( 0.0f,  4.0f, -4.0f,  4.0f),  //ц,ч
	vrect(-5.0f,  0.0f, -4.0f,  4.0f),  //ш,щ
	vrect(-4.0f,  0.0f, -4.0f,  4.0f),  //ъ,ь
	vrect(-4.0f,  0.0f, -3.0f,  3.0f),  //ы
	vrect(-4.0f,  0.0f, -3.0f,  3.0f),  //э
	vrect(-4.0f,  0.0f, -3.0f,  3.0f),  //ю
	vrect(-5.0f,  0.0f, -3.0f,  3.0f),  //я
	vrect(-6.0f,  0.0f, -3.0f,  3.0f),  //1
	vrect(-7.0f,  0.0f, -4.0f,  4.0f)  //2
};

geFun XFunctions[30] = { Fun01, Fun02, Fun03, Fun04, Fun05, Fun06, Fun07, Fun08, Fun09, Fun10,
												 Fun11, Fun12, Fun13, Fun14, Fun15, Fun16, Fun17, Fun18, Fun19, Fun20,
											   Fun21, Fun22, Fun23, Fun24, Fun25, Fun26, Fun27, Fun28, Fun29, Fun30};

geFun TFunctions[30] = { F3_01, F3_02, F3_03, F3_04, F3_05, F3_06, F3_07, F3_08, F3_09, F3_10,
												 F3_11, F3_12, F3_13, F3_14, F3_15, F3_16, F3_17, F3_18, F3_19, F3_20,
												 F3_21, F3_22, F3_23, F3_24, F3_25, F3_26, F3_27, F3_28, F3_29, F3_30};

wchar_t* VariantName[30] = { L"а", L"б", L"в", L"г", L"д", L"е,ё", L"ж", L"з", L"и,й", L"к", L"л", L"м", L"н", L"о",
													   L"п", L"р", L"с", L"т", L"у", L"ф", L"х", L"ц,ч", L"ш,щ", L"ъ,ь", L"э", L"ю", L"я", L"1", L"2" };