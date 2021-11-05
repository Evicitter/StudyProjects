//Last update time : 28.03.2010 11:53:18

#include <vcl.h>
#pragma hdrstop

#include "GB_MaterialBrowser.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TBrwMaterial *BrwMaterial;
//---------------------------------------------------------------------------
__fastcall TBrwMaterial::TBrwMaterial(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
