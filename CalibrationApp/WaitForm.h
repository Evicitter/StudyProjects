//Last update time : 03.10.2015 19:04:50

#ifndef WaitFormH
#define WaitFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
//---------------------------------------------------------------------------
class TWaitProgress : public TForm
{
__published:	// IDE-managed Components
	TAnimate *Animate1;
	TProgressBar *ProgressBar1;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormHide(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TWaitProgress(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TWaitProgress *WaitProgress;
//---------------------------------------------------------------------------
#endif