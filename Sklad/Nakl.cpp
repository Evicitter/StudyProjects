//Last update time : 06.09.2009 12:32:02
#include <vcl.h>
#pragma hdrstop

#include "Nakl.h"
#include "DM.h"
#include "Report.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfmNakl *fmNakl;
//---------------------------------------------------------------------------
__fastcall TfmNakl::TfmNakl(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfmNakl::btnReportSaleClick(TObject *Sender)
{
//DMMain->ADO_Nakladnaya->Filter=L"№_Накладной="+DMMain->ADO_Nakladnaya->FieldByName(L"№_Накладной")->AsString;
//DMMain->ADO_Nakladnaya->Filtered=true;
//fmReport->QuickRepSale->Preview();
//DMMain->ADO_Nakladnaya->Filtered=false;
}
//---------------------------------------------------------------------------