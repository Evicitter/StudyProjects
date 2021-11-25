//Last update time : 03.10.2015 19:04:58

#include <vcl.h>
#pragma hdrstop

#include "WaitForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TWaitProgress *WaitProgress;
//---------------------------------------------------------------------------
__fastcall TWaitProgress::TWaitProgress(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TWaitProgress::FormShow(TObject *Sender)
{
 Animate1->ResHandle = (unsigned int)HInstance;
 Animate1->ResName = L"WAVI";
 Animate1->Active = true;
}
//---------------------------------------------------------------------------
void __fastcall TWaitProgress::FormHide(TObject *Sender)
{
 Animate1->Active = false;
}
//---------------------------------------------------------------------------