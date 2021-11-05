//Last update time : 11.05.2010 03:15:42

#include <vcl.h>
#pragma hdrstop

#include "StepCon_PF.h"
#include "CfgPrgCls.h"
#include "StepCon_MF.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TProgressForm *ProgressForm;

//---------------------------------------------------------------------------
__fastcall TProgressForm::TProgressForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TProgressForm::MainUpdateTime(TObject *Sender)
{
 PBar->Max = Global_CFG.ProgressMax;
 PBar->Position = Global_CFG.ProgressCurrent;
}
//---------------------------------------------------------------------------

void __fastcall TProgressForm::FormShow(TObject *Sender)
{
 UpdateTimer->Enabled=true;
 PBar->Position=0;
 //MainForm->Hide();
}
//---------------------------------------------------------------------------

void __fastcall TProgressForm::FormHide(TObject *Sender)
{
 UpdateTimer->Enabled=false;
 PBar->Position=0;
 //MainForm->Show();
}
//---------------------------------------------------------------------------