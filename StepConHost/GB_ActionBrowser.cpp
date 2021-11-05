//Last update time : 28.03.2010 13:02:00

#include <vcl.h>
#pragma hdrstop

#include "GB_ActionBrowser.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TBrwAction *BrwAction;
//---------------------------------------------------------------------------
__fastcall TBrwAction::TBrwAction(TComponent* Owner)
	: TForm(Owner)
{
 SelectedObject = L"Core";
 SelectedState = -1;
}
//---------------------------------------------------------------------------

void __fastcall TBrwAction::TreeView1Change(TObject *Sender, TTreeNode *Node)
{
 SelectedObject = Node->Text;
 SelectedState  = Node->StateIndex;
}
//---------------------------------------------------------------------------