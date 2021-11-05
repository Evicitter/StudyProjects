//Last update time : 01.04.2010 11:13:34

#ifndef FRM_FieldSpacesH
#define FRM_FieldSpacesH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <ImgList.hpp>
//---------------------------------------------------------------------------

#include "FRMsub_actObjectWin.h"
#include <AppEvnts.hpp>
#include <ExtCtrls.hpp>

class TfrmFieldSpaces : public TFrame
{
__published:	// IDE-managed Components
	TPopupMenu *MenuClasses;
	TMenuItem *AddObject1;
	TMenuItem *AddObject2;
	TMenuItem *AddObject3;
	TMenuItem *N1;
	TMenuItem *AddObject4;
	TImageList *IL_IconsEl;
	TPaintBox *PaintBox1;
	void __fastcall MenuClassesPopup(TObject *Sender);
	void __fastcall PaintBox1Paint(TObject *Sender);
private:	// User declarations
public:		// User declarations
    TactObjectWin*				 actLastClicked;
	DynamicArray<TactObjectWin*> actObjects;
	int							 actLastIndex;
	short 			LinkState; //0 - no; 1 - present; 2-select first = 0;
	TactObjectWin*  LinkObjS1;
	bool			bActDrag;
	void __fastcall DeleteSelectedActObj();
	void __fastcall SetLinkMode(bool isLinkedMode);
	__fastcall TfrmFieldSpaces(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmFieldSpaces *frmFieldSpaces;
//---------------------------------------------------------------------------
#endif