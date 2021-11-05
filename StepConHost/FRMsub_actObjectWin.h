//Last update time : 01.04.2010 10:53:30

#ifndef FRMsub_actObjectWinH
#define FRMsub_actObjectWinH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <Graphics.hpp>
//---------------------------------------------------------------------------
class TactObjectWin : public TFrame
{
__published:	// IDE-managed Components
	TImage *ImgIcon;
	TLabel *LabelName;
	TShape *Shape1;
	TProgressBar *PB_LoadImm;
	void __fastcall MainMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall ImgIconClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	TactObjectWin*		LinkOther;
	TColor	selColor;
	TColor	dragColor;
	TColor	freeColor;
	int		MyIndex;
	void __fastcall InitState(TWinControl* Owner, const UnicodeString& fromClass, int id);
	void __fastcall SetClicked(bool bclicked);
	__fastcall TactObjectWin(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TactObjectWin *actObjectWin;
//---------------------------------------------------------------------------
#endif