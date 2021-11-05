//Last update time : 01.04.2010 12:44:46

#include <vcl.h>
#pragma hdrstop

#include "FRM_FieldSpaces.h"
#include "GB_ActionBrowser.h"
#include "FRMsub_actObjectWin.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmFieldSpaces *frmFieldSpaces;

//---------------------------------------------------------------------------
__fastcall TfrmFieldSpaces::TfrmFieldSpaces(TComponent* Owner)
	: TFrame(Owner)
{
 actObjects.set_length(512);
 actLastIndex=0;
 actLastClicked=NULL;
 //--------------------
 LinkObjS1 = NULL;
 LinkState = 0i16;
 //-------------------
 bActDrag = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmFieldSpaces::DeleteSelectedActObj()
{
 if(actLastClicked != NULL)
 {
	 actObjects[actLastClicked->MyIndex] = NULL;
	 actLastClicked->DestroyComponents();
	 delete actLastClicked;
	 actLastClicked = NULL;
 }
}
//---------------------------------------------------------------------------

void __fastcall TfrmFieldSpaces::SetLinkMode(bool isLinkedMode)
{
 if( LinkState == 0 )
 {
	  LinkState = 1;
	  for(short i=0i16; i<512i16; ++i)
		 if( actObjects[i] != NULL ) { actObjects[i]->Cursor = crDrag; }
 }
 else
 { LinkState = 0;
   	 for(short i=0i16; i<512i16; ++i)
		 if( actObjects[i] != NULL ) actObjects[i]->Cursor = crDefault;
 }
}
//---------------------------------------------------------------------------

void __fastcall TfrmFieldSpaces::MenuClassesPopup(TObject *Sender)
{
 TPopupMenu* tpm = dynamic_cast<TPopupMenu*>(Sender);
 TMenuItem* tmi = dynamic_cast<TMenuItem*>(Sender);
 if( tpm != NULL )
 {
  if(BrwAction->SelectedObject.IsEmpty() || (BrwAction->SelectedState != 0))
   tpm->Items[0][0]->Enabled = false;
  else
   tpm->Items[0][0]->Enabled = true;

   tpm->Items[0][0]->Caption = UnicodeString(L"Add ") + BrwAction->SelectedObject;
 }

 if( tmi != NULL )
 {
  switch(tmi->Tag)
  {
	  case 1879048193:
	  break;
	  case 1879048194:
		for(int fi = 0; fi<512; ++fi) if(actObjects[fi]==NULL) { actLastIndex = fi; break; }

	   actObjects[actLastIndex] = new TactObjectWin(this);
	   actObjects[actLastIndex]->InitState(this, L"ImpulseBody", actLastIndex);
	  break;
	  case 1879048195:
	  break;
	  case 1879048196:
	  break;
  }
 }
}
//---------------------------------------------------------------------------

void __fastcall TfrmFieldSpaces::PaintBox1Paint(TObject *Sender)
{
  PaintBox1->Canvas->FillRect( PaintBox1->Canvas->ClipRect );

  if( bActDrag ) return;

  PaintBox1->Canvas->Pen->Width = 3;
  for(short i=0i16; i<512i16; ++i)
  {
	 if((actObjects[i] != NULL) && (actObjects[i]->LinkOther != NULL))
	 {
		 PaintBox1->Canvas->MoveTo(actObjects[i]->Left, actObjects[i]->Top );
		 PaintBox1->Canvas->LineTo(actObjects[i]->LinkOther->Left, actObjects[i]->LinkOther->Top  );
     }
  }
}
//---------------------------------------------------------------------------