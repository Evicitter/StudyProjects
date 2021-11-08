//Last update time : 06.09.2009 12:51:52
#include <vcl.h>
#pragma hdrstop

#include "DM.h"
#include "Nakl.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TDMMain *DMMain;
//---------------------------------------------------------------------------
__fastcall TDMMain::TDMMain(TComponent* Owner)
        : TDataModule(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TDMMain::ADO_TovariCalcFields(TDataSet *DataSet)
{
#define FBN(x) ADO_Tovari->FieldByName(x)->AsString
        ADO_Tovari->FieldByName(L"ТоварINFO")->Value=

        FBN(L"№_Товара")+" "+FBN(L"Название")+" "+FBN(L"Тип");
#undef FBN
}
//---------------------------------------------------------------------------

void __fastcall TDMMain::ADO_TovariPostError(TDataSet *DataSet,
      EDatabaseError *E, TDataAction &Action)
{
 if((ADO_Tovari->FieldByName(L"Название")->IsNull) || (ADO_Tovari->FieldByName(L"Название")->AsString==""))
 {
  Application->MessageBox(L"Необходимо обязательно в товарах указать \"Название\"",L"Ошибка", MB_ICONERROR);
  Action=daAbort; // Запрещаем вывод ошибки СУБД
  return;
 }

 if((ADO_Tovari->FieldByName("Тип")->IsNull) || (ADO_Tovari->FieldByName("Тип")->AsString==""))
 {
  Application->MessageBox(L"Необходимо обязательно в товарах указать \"Тип товара\"",L"Ошибка", MB_ICONERROR);
  Action=daAbort; // Запрещаем вывод ошибки СУБД
 }
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

void __fastcall TDMMain::ADO_TovariDeleteError(TDataSet *DataSet, EDatabaseError *E,
          TDataAction &Action)
{
 Application->MessageBox(L"Нельзя удалить данный товар, так как он указан в накладной. Сначала удалите накладную данного товара", L"Ошибка удаления", MB_ICONERROR);
 Action=daAbort;
}
//---------------------------------------------------------------------------