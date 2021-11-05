//Last update time : 01.04.2010 22:16:26

#ifndef GB_GroupBrowserH
#define GB_GroupBrowserH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <ExtCtrls.hpp>
#include "TeeDraw3D.hpp"
#include "TeeProcs.hpp"
//---------------------------------------------------------------------------
class TBrwGroup : public TForm
{
__published:	// IDE-managed Components
	TImageList *IL_ToolBar1;
	TToolBar *ToolBar1;
	TListBox *ListBox1;
	TFlowPanel *FlowPanel1;
	void __fastcall FlowPanel1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);

private:	// User declarations
public:		// User declarations
	__fastcall TBrwGroup(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TBrwGroup *BrwGroup;
//---------------------------------------------------------------------------
#endif