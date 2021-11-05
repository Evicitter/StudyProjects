//Last update time : 01.04.2010 22:16:22

#include <vcl.h>
#pragma hdrstop

#include "GB_GroupBrowser.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TeeDraw3D"
#pragma link "TeeProcs"
#pragma resource "*.dfm"
TBrwGroup *BrwGroup;
//---------------------------------------------------------------------------

__fastcall TBrwGroup::TBrwGroup(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TBrwGroup::FlowPanel1MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
    if(Button == mbLeft)
      {
        ReleaseCapture();
		SendMessage(FlowPanel1->Handle, WM_SYSCOMMAND, 0xF012, 0);
      }
}
//---------------------------------------------------------------------------