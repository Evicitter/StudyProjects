//Last update time : 03.06.2010 15:13:32
#ifndef AddTovFormH
#define AddTovFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <ValEdit.hpp>
//---------------------------------------------------------------------------
class TFAddTov : public TForm
{
__published:	// IDE-managed Components
	TValueListEditor *vleT;
	TComboBox *cbClass;
	TLabel *Класс;
	TButton *Button1;
	TButton *Button2;
private:	// User declarations
public:		// User declarations
	__fastcall TFAddTov(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFAddTov *FAddTov;
//---------------------------------------------------------------------------
#endif