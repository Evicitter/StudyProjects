//Last update time : 25.03.2010 11:23:22

#ifndef TimeFormH
#define TimeFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <ValEdit.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
//---------------------------------------------------------------------------
class TFormTime : public TForm
{
__published:	// IDE-managed Components
	TValueListEditor *VLEditor;
	TToolBar *ToolBar1;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormHide(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall AddTime(UnicodeString TimeName, unsigned __int64 time);
	__fastcall TFormTime(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormTime *FormTime;
//---------------------------------------------------------------------------
#endif
