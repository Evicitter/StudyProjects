//Last update time : 16.09.2015 05:46:54
#include <vcl.h>
#pragma hdrstop

#include "GraphicForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TGraphicsForm *GraphicsForm;
//---------------------------------------------------------------------------
__fastcall TGraphicsForm::TGraphicsForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TGraphicsForm::UpdateGraphic()
{
 MChart->Series[0]->Clear();
 MChart->Series[1]->Clear();
 //MChart->Series[0]->Clear();
 //MChart->Series[0]->Clear();
 //for(int i=1; i<MTable->RowCount; ++i)
 //{
  //Extended Litres = StrToFloatDef(MTable->Cells[0][i],0.0);
  //Extended Freq   = StrToFloatDef(MTable->Cells[1][i],0.0);
  //Extended Digit  = StrToFloatDef(MTable->Cells[3][i],0.0);
	  //delta -= StrToInt(MTable->Cells[0][i-1]);
  //MChart->Series[0]->AddY((double)delta, L"", MChart->Series[0]->Color);
  //MChart->Series[0]->AddXY(Litres, Freq, L"", MChart->Series[0]->Color);
  //MChart->Series[1]->AddXY(Litres, Digit, L"", MChart->Series[1]->Color);
 //}
}