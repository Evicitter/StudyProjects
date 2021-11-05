//Last update time : 22.03.2010 01:32:34

#include <vcl.h>
#pragma hdrstop

#include "LogForm.h"
#include "LogTimeCls.h"

//#include <Clipbrd.hpp>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormLog *FormLog;
//---------------------------------------------------------------------------
__fastcall TFormLog::TFormLog(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormLog::OTB_Click(TObject *Sender)
{
 TToolButton* ttb = dynamic_cast<TToolButton*>( Sender );
  if( ttb == NULL ) return;

  switch( ttb->Tag )
  {
	  case 100000000:
        Global_LOG.ClearLog();
        Memo1->Clear();
	  break;
	  case 200000000:
		 Memo1->CopyToClipboard();
	  break;
  }

}
//---------------------------------------------------------------------------
void __fastcall TFormLog::FormHide(TObject *Sender)
{
 Memo1->Clear();
 Timer1->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TFormLog::FormShow(TObject *Sender)
{
 Timer1->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TFormLog::Timer1Timer(TObject *Sender)
{
 Memo1->Lines->SetText( Global_LOG.ReadLog() );
}
//---------------------------------------------------------------------------