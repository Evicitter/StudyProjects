//Last update time : 01.04.2010 12:38:34

#include <vcl.h>
#pragma hdrstop

#include "FRMsub_actObjectWin.h"
#include "FRM_FieldSpaces.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TactObjectWin *actObjectWin;
//---------------------------------------------------------------------------
__fastcall TactObjectWin::TactObjectWin(TComponent* Owner)
	: TFrame(Owner)
{
 dragColor = clFuchsia;
 selColor = clYellow;
 freeColor = clLime;
 //-----------------
 LinkOther = NULL;
}
//---------------------------------------------------------------------------

void __fastcall TactObjectWin::InitState(TWinControl* Owner, const UnicodeString& fromClass, int id)
{
 Name = fromClass + L"_" + ((rand()<<16)+rand());
 Parent = Owner;
 LabelName->Caption = fromClass;
 MyIndex = id;
 //ImgIcon->Picture->Bitmap->LoadFromResourceName(0, L"Bitmap_1");
}

void __fastcall TactObjectWin::SetClicked(bool bclicked)
{
 if(bclicked) Shape1->Pen->Color = selColor;
 else Shape1->Pen->Color = freeColor;
}

//---------------------------------------------------------------------------
void __fastcall TactObjectWin::MainMouseDown(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y)
{
 TfrmFieldSpaces* frmfs = dynamic_cast<TfrmFieldSpaces*>(Parent);
 if( frmfs != NULL )
 {
	 if( frmfs->LinkState == 1 )
	 {
		 frmfs->LinkState = 2;
		 frmfs->LinkObjS1 = this;
		 return;
	 }
	 else if(frmfs->LinkState == 2)
	 {
		 if(frmfs->LinkObjS1 != NULL)
		 {
			frmfs->LinkObjS1->LinkOther = this;
			//LinkOther = NULL;
		 }
		 frmfs->SetLinkMode(false);
		 frmfs->LinkState = 0;
		 frmfs->PaintBox1->OnPaint(this);
		 return;
	 }
 }

 if(Button == mbLeft)
 {
  TColor oldcolor = Shape1->Pen->Color;
  Shape1->Pen->Color = dragColor;

  frmfs->bActDrag = true;
  ReleaseCapture();
  SendMessage(Handle, WM_SYSCOMMAND, 0xF012, 0);
  frmfs->bActDrag = false;

  Shape1->Pen->Color = oldcolor;

  if(frmfs != NULL) frmfs->PaintBox1->OnPaint(this);
 }
}
//---------------------------------------------------------------------------
void __fastcall TactObjectWin::ImgIconClick(TObject *Sender)
{
 TfrmFieldSpaces* frmfs = dynamic_cast<TfrmFieldSpaces*>(Parent);
 if( frmfs != NULL )
 {
  if( frmfs->actLastClicked != NULL ) frmfs->actLastClicked->SetClicked(false);
  SetClicked(true);
  frmfs->actLastClicked = this;
 }
}
//---------------------------------------------------------------------------