//Last update time : 28.03.2010 11:49:14

#include <vcl.h>
#pragma hdrstop

#include "GB_WaveBrowser.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TBrwWave *BrwWave;
//---------------------------------------------------------------------------
__fastcall TBrwWave::TBrwWave(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------