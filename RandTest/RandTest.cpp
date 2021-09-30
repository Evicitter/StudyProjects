//Last update time : 27.12.2009 05:28:34
#include <vcl.h>
#pragma hdrstop

#include "RandTest.h"
#include <math.h>

static double Mas[65536];

static unsigned long _mod=0ul;
static unsigned long _mul=0ul;
static unsigned long _add=0ul;

//---------------------------1 method
static unsigned long x_curr1[65536];
//---------------------------2 method
static unsigned long x_curr2[65536];
//---------------------------3 method
static unsigned long x_curr3[65536];
//---------------------------4 method
static unsigned long x_curr4[65536];
//---------------------------5 method
static unsigned long x_curr5[65536];
static unsigned short i1=0ui16;
static unsigned short i2=0ui16;
static unsigned short i3=0ui16;
static unsigned short i4=1ui16;
static unsigned short i5=0ui16;

unsigned short __fastcall indexat(unsigned short ff,short offset)
{
 return (unsigned short)(ff+offset);
}
//---------------------------

unsigned long __fastcall SetRandom(unsigned long seed)
{
//------------------------------------------------
 /*memset(x_curr1,0,sizeof(x_curr1));
 memset(x_curr2,0,sizeof(x_curr2));
 memset(x_curr3,0,sizeof(x_curr3));
 memset(x_curr4,0,sizeof(x_curr4));
 memset(x_curr5,0,sizeof(x_curr5));*/

 x_curr1[0] = seed;
//------------------------------------------------
 x_curr2[0] = seed;
 x_curr2[1] = seed;
//------------------------------------------------
 x_curr3[0] = seed;
 //-----------------------------------------------
 x_curr4[0] = seed;
 x_curr4[1] = seed;
 //-----------------------------------------------
 x_curr5[0] = seed;
//------------------------------------------------
}

unsigned long __fastcall Rand1( unsigned long xmul, unsigned long xadd, unsigned long xmod )
{
 x_curr1[indexat(i1,1)] = (x_curr1[i1] * xmul + xadd) % xmod;
 ++i1;
 return x_curr1[i1];
}

unsigned long __fastcall Rand2( unsigned long xmul, unsigned long xadd, unsigned long xmod )
{
 x_curr2[indexat(i2,1)] = ((x_curr2[indexat(i2,1)]*x_curr2[indexat(i2,1)])+(xmul*x_curr2[i2])+xadd) % xmod;
 ++i2;
 return x_curr2[i2];
}

unsigned long __fastcall Rand3( unsigned long xmod )
{
 x_curr3[indexat(i3,1)] = (x_curr3[i3]*(x_curr3[i3]+1)) % xmod;
 ++i3;
 return x_curr3[i3];
}

unsigned long __fastcall Rand4( unsigned long xmod )
{
 x_curr4[indexat(i4,1)] = (x_curr4[i4] + x_curr4[indexat(i4,-1)]) % xmod;
 ++i4;
 return x_curr4[i4];
}

unsigned long __fastcall Rand5( short k, unsigned long xmod )
{
 x_curr5[indexat(i5,1)] = (x_curr5[i5] + x_curr5[indexat(i5,-k)]) % xmod;
 ++i5;
 return x_curr5[i5];
}

#pragma package(smart_init)
#pragma link "Chart"
#pragma link "TeEngine"
#pragma link "TeeProcs"
#pragma link "Series"
#pragma resource "*.dfm"
TForm2 *Form2;
//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm2::CheckBox1Click(TObject *Sender)
{
 Edit1->Enabled = !CheckBox1->Checked;
}

//function selector;
unsigned long __fastcall GetRand(long method)
{
	switch(method)
	{
	 case 0: return Rand1(_mul, _add, _mod);
	 case 1: return Rand2(_mul, _add, _mod);
	 case 2: return Rand3(_mod);
	 case 3: return Rand4(_mod);
	 case 4: return Rand5(23,_mod);
    }
}

//---------------------------------------------------------------------------
void __fastcall TForm2::Button1Click(TObject *Sender)
{
 //memset(Mas, 0, sizeof(Mas));

 if(CheckBox1->Checked) SetRandom( (unsigned)time(NULL) );
 else SetRandom( StrToIntDef(Edit1->Text,0) );

 _mod = StrToInt64( Edit2->Text ) & 0x00000000ffffffffi64;
 _mul = StrToInt64( Edit4->Text ) & 0x00000000ffffffffi64;
 _add = StrToInt64( Edit3->Text ) & 0x00000000ffffffffi64;

 //Chart1->Series[0]->Clear();
  for(long i=0; i<TrackBar2->Position; ++i)
   Mas[i] = (double)(GetRand(ComboBox1->ItemIndex) % TrackBar2->Position) / (double)TrackBar2->Position;

 /*for(long i=0; i<TrackBar2->Position; ++i)
 {
  long xx = 0;
  for(long j=0; j<TrackBar2->Position; ++j)
   if(Mas[j] == i) ++xx;
  Chart1->Series[0]->AddY( xx );
 }*/

 double matidle=0.0;
 for(long i=0; i<TrackBar2->Position; ++i) matidle += Mas[i];
 matidle /= TrackBar2->Position;

 Edit5->Text = FloatToStr( matidle );

 double dispersion=0.0;
 for(long i=0; i<TrackBar2->Position; ++i) dispersion += (Mas[i]-matidle)*(Mas[i]-matidle);
 dispersion /= TrackBar2->Position;

 Edit6->Text = FloatToStr( dispersion );
 Edit7->Text = FloatToStr( sqrt(dispersion) );

 double _1mror=matidle - sqrt(dispersion);
 double _2mror=matidle + sqrt(dispersion);
 Edit8->Text = FloatToStr( _1mror );
 Edit9->Text = FloatToStr( _2mror );
 Edit10->Text = FloatToStr( (_2mror - _1mror) * 100.0 );
 
 //hi square ------------------------------------------
 long freqs[10]={0,0,0,0,0,0,0,0,0,0};

 for(long i=0; i<10; ++i)
  for(long j=0; j<TrackBar2->Position; ++j)
   if(  i == (long)(Mas[j] * 10.0) )
	 ++freqs[i];

  double ds=0.0;
  for(int i=0;i<10;++i)
   ds+=freqs[i]*freqs[i]/0.1;

  ds/=TrackBar2->Position;
  ds-=TrackBar2->Position;

  Edit11->Text = FloatToStr( ds );

  //static calculatuion
  //1
  int b[50];
  {
	int a[10];
	int t;
	for(int i=0;i<10;++i)
	{
		t=100000;
		a[i]=Mas[i+1]*t;
		t/=10;

		for(int j=0;j<5;j++)
		{
			b[i*5+j]=a[i]/t;
			a[i]-=b[i*5+j]*t;
			t/=10;
		}
		a[i]=0;
	}
	double mr[10];
	for(int i=0;i<50;i++) a[b[i]]++;
	
	//for(int i=0;i<10;i++){
	//	mr[i]=a[i]/10.;
	//	cout << mr[i];
	//}
	
	int N=50;
	double ds=0;
	for(int i=0;i<10;i++)
	{
			ds+=(a[i]*a[i])/0.1;
	}
	ds/=(double)N;
	ds-=N;
	ds*=10;
	Edit13->Text = FloatToStr( ds );
	//cout << "x^2 expermintalnoe = "<<ds<<endl;
	//cout << "p=50%..75%;\tx^2 kr =49,33..56,33"<<endl;
	//cout << "=> posledovatelnost sluchaina"<<endl;
 }

 //одинаковых цифр
 //2
 {
  int n2=0,n3=0;
  for(int i=0;i<48;i++)
  {
	if(b[i]==b[i+1] && b[i]==b[i+2]) n3++;
	else if(b[i]==b[i+1]) n2++;
  }
   Edit14->Text = IntToStr( n2 );
   Edit15->Text = IntToStr( n3 );
 }
}