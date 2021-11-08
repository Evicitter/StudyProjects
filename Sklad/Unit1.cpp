//Last update time : 06.09.2009 21:07:02
#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include "List.h"
#include "DM.h"
#include "Nakl.h"
#include "Tovari.h"
#include "SQL.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfmMain *fmMain;
//---------------------------------------------------------------------------
__fastcall TfmMain::TfmMain(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::N3Click(TObject *Sender)
{
 Close();
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::N6Click(TObject *Sender)
{
 Close();        
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::ToolButton1Click(TObject *Sender)
{
 Close();        
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::actExitExecute(TObject *Sender)
{
 if (Application->MessageBox(L"Вы хотите выйти из программы?", L"Внимание", MB_YESNO + MB_ICONQUESTION) == IDYES)
  Close();
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::actInfoExecute(TObject *Sender)
{
 Application->MessageBox(L"(C)ТУСУР, КИБЭВС, Брагин А.А. гр.577-СИ, 2008", L"О программе", MB_ICONINFORMATION);
 StatusBar1->Panels->Items[1]->Text=L"(C)ТУСУР, КИБЭВС, 2008";
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::actNakladnayaExecute(TObject *Sender)
{
 fmList->DataSource1->DataSet=DMMain->ADO_Nakladnaya;
 fmList->Caption=L"Накладная";
 fmList->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::actSkladExecute(TObject *Sender)
{
 fmList->DataSource1->DataSet=DMMain->ADO_Sklad;
 fmList->Caption=L"Склад";
 fmList->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::actTovaramExecute(TObject *Sender)
{
 fmList->DataSource1->DataSet=DMMain->ADO_Tovari;
 fmList->Caption=L"Товары";
 fmList->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::act_NaklTovExecute(TObject *Sender)
{
 fmNakl->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::actSQLExecute(TObject *Sender)
{
        fmSQL->Caption=L"SQL Запросы";
        fmSQL->ShowModal();
}
//---------------------------------------------------------------------------