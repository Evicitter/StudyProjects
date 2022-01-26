//Last update time : 03.06.2010 15:49:10
#include <vcl.h>
#pragma hdrstop

#include "MForm.h"

#include "Concerv.h"
#include "Fruit.h"
#include "Meat.h"
#include "Sklad.h"
#include "Tovar.h"
#include "AddTovForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TGMForm *GMForm;

Sklad GSklad;

//---------------------------------------------------------------------------
__fastcall TGMForm::TGMForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TGMForm::BtnLoadClick(TObject *Sender)
{
 GSklad.Load();

 lbTovars->Items->Clear();
 for(int i=0; i<GSklad.getCountVals(); ++i)
 {
  lbTovars->Items->Add( GSklad.GetTovar(i)->getName() );
 }
 vleTovar->Strings->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TGMForm::BtnSaveClick(TObject *Sender)
{
 GSklad.Save();
// QT->ExecSQL();
}
//---------------------------------------------------------------------------

void __fastcall TGMForm::lbTovarsClick(TObject *Sender)
{
 Tovar* tmpt = GSklad.GetTovar( lbTovars->ItemIndex );
 vleTovar->Strings->Clear();
 vleTovar->Strings->Add( UnicodeString(L"Class=") + UnicodeString(tmpt->getClassName()) );
 vleTovar->Strings->Add( UnicodeString(L"Name=") + UnicodeString(tmpt->getName()) );
 vleTovar->Strings->Add( UnicodeString(L"Coast=") + UnicodeString(tmpt->getCoast()) );
 vleTovar->Strings->Add( UnicodeString(L"Count=") + UnicodeString(tmpt->getCount()) );
 vleTovar->Strings->Add( UnicodeString(L"BadDays=") + UnicodeString(tmpt->get_BadDays()) );
}
//---------------------------------------------------------------------------


void __fastcall TGMForm::BtnUpdateClick(TObject *Sender)
{
 Tovar* tmpt = GSklad.GetTovar( lbTovars->ItemIndex );
 tmpt->setName( vleTovar->Values[L"Name"].w_str() );
 tmpt->setCoast( StrToIntDef(vleTovar->Values[L"Coast"],0) );
 tmpt->setCount( StrToIntDef(vleTovar->Values[L"Count"],0) );
 tmpt->set_BadDays( StrToIntDef(vleTovar->Values[L"BadDays"],0) );
}
//---------------------------------------------------------------------------

void __fastcall TGMForm::BtnDelClick(TObject *Sender)
{
 GSklad.DelTovar( lbTovars->ItemIndex );
 lbTovars->Items->Clear();
 for(int i=0; i<GSklad.getCountVals(); ++i)
 {
  lbTovars->Items->Add( GSklad.GetTovar(i)->getName() );
 }
 vleTovar->Strings->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TGMForm::BtnAddClick(TObject *Sender)
{
 if( FAddTov->ShowModal() == mrOk )
 {
  Tovar* t;
  if( FAddTov->cbClass->ItemIndex == 0 )
   t = new Fruit();
  else if( FAddTov->cbClass->ItemIndex == 1 )
   t = new Meat();
  else if( FAddTov->cbClass->ItemIndex == 2 )
   t = new Concerv();

  if( t != (void*)0 )
  {
   t->setName( FAddTov->vleT->Values[L"Name"].w_str() );
   t->setCoast( StrToIntDef(FAddTov->vleT->Values[L"Coast"],1) );
   t->setCount( StrToIntDef(FAddTov->vleT->Values[L"Count"],1) );
   t->set_BadDays( StrToIntDef(FAddTov->vleT->Values[L"BadDays"],1) );
   GSklad.AddTovar(t);
   delete t;
   BtnLoadClick(Sender);
  }
 }
}
//---------------------------------------------------------------------------



void __fastcall TGMForm::Button1Click(TObject *Sender)
{
 UnicodeString omsg = L"Потеря цены от испорченных товаров составляет: ";
 omsg += GSklad.SumBadDays( Edit1->Text.ToIntDef(1) );
 omsg += L" р.";
 ShowMessage( omsg );
}
//---------------------------------------------------------------------------