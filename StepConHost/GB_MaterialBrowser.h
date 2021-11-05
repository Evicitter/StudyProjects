//Last update time : 28.03.2010 12:01:10

#ifndef GB_MaterialBrowserH
#define GB_MaterialBrowserH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TBrwMaterial : public TForm
{
__published:	// IDE-managed Components
	TToolBar *ToolBar1;
	TImageList *IL_ToolBar1;
	TPanel *Panel1;
	TStatusBar *StatusBar1;
private:	// User declarations
public:		// User declarations
	__fastcall TBrwMaterial(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TBrwMaterial *BrwMaterial;
//---------------------------------------------------------------------------
#endif