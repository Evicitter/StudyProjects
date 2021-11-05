//Last update time : 25.03.2010 11:27:46

#include <vcl.h>
#pragma hdrstop

#include "TimeForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormTime *FormTime;
//---------------------------------------------------------------------------
__fastcall TFormTime::TFormTime(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

__fastcall TFormTime::AddTime(UnicodeString TimeName, unsigned __int64 time)
{
 VLEditor->Strings->Add(TimeName + L"=" + time);
}
void __fastcall TFormTime::FormShow(TObject *Sender)
{
 //Load Strings
}
//---------------------------------------------------------------------------

void __fastcall TFormTime::FormHide(TObject *Sender)
{
 VLEditor->Strings->Clear();
}