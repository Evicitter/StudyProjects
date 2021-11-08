//Last update time : 22.03.2009 19:04:26
#include <vcl.h>
#pragma hdrstop

#include "Report.h"
#include "DM.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfmReport *fmReport;
//---------------------------------------------------------------------------
__fastcall TfmReport::TfmReport(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------