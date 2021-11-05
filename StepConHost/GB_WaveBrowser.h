//Last update time : 28.03.2010 11:50:14

#ifndef GB_WaveBrowserH
#define GB_WaveBrowserH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
//---------------------------------------------------------------------------
class TBrwWave : public TForm
{
__published:	// IDE-managed Components
	TToolBar *ToolBar1;
	TImageList *IL_ToolBar1;
private:	// User declarations
public:		// User declarations
	__fastcall TBrwWave(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TBrwWave *BrwWave;
//---------------------------------------------------------------------------
#endif