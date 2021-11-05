//Last update time : 28.03.2010 12:59:14

#ifndef GB_ActionBrowserH
#define GB_ActionBrowserH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <ImgList.hpp>
//---------------------------------------------------------------------------
class TBrwAction : public TForm
{
__published:	// IDE-managed Components
	TToolBar *ToolBar1;
	TTreeView *TreeView1;
	TImageList *IL_Tree;
	TImageList *IL_ToolBar;
	void __fastcall TreeView1Change(TObject *Sender, TTreeNode *Node);

private:	// User declarations
public:		// User declarations
	UnicodeString SelectedObject;
	int			  SelectedState;
	__fastcall TBrwAction(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TBrwAction *BrwAction;
//---------------------------------------------------------------------------
#endif