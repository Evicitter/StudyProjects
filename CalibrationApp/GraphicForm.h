//Last update time : 16.09.2015 05:46:10

#ifndef GraphicFormH
#define GraphicFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
//---------------------------------------------------------------------------
class TGraphicsForm : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *GroupBox2;
	TChart *MChart;
	TFastLineSeries *Series2;
	TFastLineSeries *Series1;
	TChart *Chart1;
private:	// User declarations
public:		// User declarations
    void __fastcall UpdateGraphic();
	__fastcall TGraphicsForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TGraphicsForm *GraphicsForm;
//---------------------------------------------------------------------------
#endif