//Last update time : 06.09.2009 12:26:32
#include <vcl.h>
#pragma hdrstop

#include "SQL.h"
#include "DM.h"
#include "List.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfmSQL *fmSQL;
//---------------------------------------------------------------------------
__fastcall TfmSQL::TfmSQL(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfmSQL::RadioGroupPrimersClick(TObject *Sender)
{
 ADOQueryPrimers->Close();
switch (RadioGroupPrimers->ItemIndex)
{
     case 0:
            ADOQueryPrimers->SQL->Text=L"SELECT * FROM Склад";
            break;
     case 1:
            //ADOQueryPrimers->SQL->Text=L"SELECT Название, Тип, Выход FROM Блюда";
            ADOQueryPrimers->SQL->Text=L"SELECT Название, Тип, №_Товара FROM Товар";
          break;
     case 2:
      ADOQueryPrimers->SQL->Clear();
      ADOQueryPrimers->SQL->Add(L"SELECT №_Товара, Подпись, Инициалы");
      ADOQueryPrimers->SQL->Add(L"FROM [Накладная к товару], Накладная");
      ADOQueryPrimers->SQL->Add(L"WHERE [Накладная к товару].№_Накладной=Накладная.№_Накладной");
     break;
  }
 ADOQueryPrimers->Open();
}
//---------------------------------------------------------------------------
void __fastcall TfmSQL::ButtonResultClick(TObject *Sender)
{
 ADOQueryResult->Close();
 ADOQueryResult->Parameters->ParamByName(L"d1")->Value=DateTimePickerDate1->Date;
 ADOQueryResult->Parameters->ParamByName(L"d2")->Value=DateTimePickerDate2->Date;
 ADOQueryResult->Open();
}
//---------------------------------------------------------------------------

void __fastcall TfmSQL::Edit1Change(TObject *Sender)
{
 int x;
 if(Edit1->Text.IsEmpty())
  return;
 try
 {
   x = Edit1->Text.ToInt();
   if(x==0)
    throw(NULL);
 }
 catch(...)
 {
   Application->MessageBox(L"Вводите целочисленное больше нуля значение",L"Ошибка",MB_ICONERROR);
   Edit1->Text=L'\0';
 }
}
//---------------------------------------------------------------------------

void __fastcall TfmSQL::Button1Click(TObject *Sender)
{
 ADOQueryKontrol->Close();
 ADOQueryKontrol->Parameters->ParamByName(L"d1")->Value=Edit1->Text.ToInt();
 ADOQueryKontrol->Open();
}
//---------------------------------------------------------------------------

void __fastcall TfmSQL::ButtonInsertClick(TObject *Sender)
{
 if(EditName->Text.IsEmpty())
 {
  Application->MessageBox(L"Укажите название товара", L"Внимание", MB_ICONINFORMATION);
  return;
 }
 else if(EditType->Text.IsEmpty())
 {
  Application->MessageBox(L"Укажите тип товара", L"Внимание", MB_ICONINFORMATION);
  return;
 }
 else if(EditNum->Text.IsEmpty())
 {
  Application->MessageBox(L"Обязательно укажите № товара", L"Внимание", MB_ICONINFORMATION);
  return;
 }

LabelName->Caption=L"Новое название товара";
LabelType->Caption=L"Новый тип товара";
LabelNum->Caption=L"Новый № Товара";
 ADOQueryModify->Close();
 ADOQueryModify->SQL->Clear();

 ADOQueryModify->SQL->Add(L"INSERT INTO Товар (Название, Тип, №_Товара)");
 ADOQueryModify->SQL->Add(L"VALUES (:n, :t, :num)"); //(SELECT MAX(t.№_Товара) FROM Товар t)+1
 ADOQueryModify->Parameters->ParamByName(L"n")->Value=EditName->Text;
 ADOQueryModify->Parameters->ParamByName(L"t")->Value=EditType->Text;
 ADOQueryModify->Parameters->ParamByName(L"num")->Value=EditNum->Text;
 ADOQueryModify->ExecSQL();
 Application->MessageBox(L"Будет добавлена запись", L"Внимание", MB_ICONINFORMATION);
// Чтобы обновить данные в НД для Товаров после добавления
DMMain->ADO_Tovari->Close();
DMMain->ADO_Tovari->Open();
}
//---------------------------------------------------------------------------

void __fastcall TfmSQL::ButtonUpdateClick(TObject *Sender)
{
 if(EditName->Text.IsEmpty())
 {
  Application->MessageBox(L"Укажите название товара", L"Внимание", MB_ICONINFORMATION);
  return;
 }
 else if(EditType->Text.IsEmpty())
 {
  Application->MessageBox(L"Укажите тип товара", L"Внимание", MB_ICONINFORMATION);
  return;
 }
 else if(EditNum->Text.IsEmpty())
 {
  Application->MessageBox(L"Обязательно укажите № товара", L"Внимание", MB_ICONINFORMATION);
  return;
 }
 
LabelName->Caption=L"Смена названия товара";
LabelType->Caption=L"Смена типа товара";
LabelNum->Caption=L"Для № Товара";
ADOQueryModify->Close();
ADOQueryModify->SQL->Clear();
 ADOQueryModify->SQL->Add(L"UPDATE Товар SET Название=:n, Тип =:t");
 ADOQueryModify->SQL->Add(L"WHERE (№_Товара = :num)");
ADOQueryModify->Parameters->ParamByName(L"n")->Value=EditName->Text;
ADOQueryModify->Parameters->ParamByName(L"t")->Value=EditType->Text;
ADOQueryModify->Parameters->ParamByName(L"num")->Value=EditNum->Text;
 ADOQueryModify->ExecSQL();
Application->MessageBox(L"Будет изменена запись", L"Внимание", MB_ICONINFORMATION);
DMMain->ADO_Tovari->Close();
DMMain->ADO_Tovari->Open();
        
}
//---------------------------------------------------------------------------

void __fastcall TfmSQL::ButtonDelete1Click(TObject *Sender)
{
 if(EditName->Text.IsEmpty())
 {
  Application->MessageBox(L"Укажите название товара", L"Внимание", MB_ICONINFORMATION);
  return;
 }
 else if(EditType->Text.IsEmpty())
 {
  Application->MessageBox(L"Укажите тип товара", L"Внимание", MB_ICONINFORMATION);
  return;
 }
 else if(EditNum->Text.IsEmpty())
 {
  Application->MessageBox(L"Обязательно укажите № товара", L"Внимание", MB_ICONINFORMATION);
  return;
 }

 LabelName->Caption=L"Удаление товара с названием";
 LabelType->Caption=L"Удаление товара с типом";
 LabelNum->Caption=L"Удаление товара с Номером";
 ADOQueryModify->Close();
 ADOQueryModify->SQL->Clear();
 ADOQueryModify->SQL->Add(L"DELETE FROM Товар");
 ADOQueryModify->SQL->Add(L"WHERE (Название =:n) OR (Тип=:t) OR (№_Товара =:num)");
 ADOQueryModify->Parameters->ParamByName(L"n")->Value=EditName->Text;
 ADOQueryModify->Parameters->ParamByName(L"t")->Value=EditType->Text;
 ADOQueryModify->Parameters->ParamByName(L"num")->Value=EditNum->Text;
 ADOQueryModify->ExecSQL();
 DMMain->ADO_Tovari->Close();
 DMMain->ADO_Tovari->Open();
}
//---------------------------------------------------------------------------

void __fastcall TfmSQL::ButtonDelete2Click(TObject *Sender)
{
 if(EditName->Text.IsEmpty())
 {
  Application->MessageBox(L"Укажите название товара", L"Внимание", MB_ICONINFORMATION);
  return;
 }
 else if(EditType->Text.IsEmpty())
 {
  Application->MessageBox(L"Укажите тип товара", L"Внимание", MB_ICONINFORMATION);
  return;
 }
 else if(EditNum->Text.IsEmpty())
 {
  Application->MessageBox(L"Обязательно укажите № товара", L"Внимание", MB_ICONINFORMATION);
  return;
 }

 LabelName->Caption=L"Удаление товара с названием";
 LabelType->Caption=L"Удаление товара с типом";
 LabelNum->Caption=L"Удаление товара с номером";
 ADOQueryModify->Close();
 ADOQueryModify->SQL->Clear();
 ADOQueryModify->SQL->Add(L"DELETE FROM Товар");
 ADOQueryModify->SQL->Add(L"WHERE (Название =:n) AND (Тип=:t) AND (№_Товара=:num)");
 ADOQueryModify->Parameters->ParamByName(L"n")->Value=EditName->Text;
 ADOQueryModify->Parameters->ParamByName(L"t")->Value=EditType->Text;
 ADOQueryModify->Parameters->ParamByName(L"num")->Value=EditNum->Text;
 ADOQueryModify->ExecSQL();
 DMMain->ADO_Tovari->Close();
 DMMain->ADO_Tovari->Open();
}
//---------------------------------------------------------------------------

void __fastcall TfmSQL::EditNumChange(TObject *Sender)
{
 //int x;
 bool iex;
 if(EditNum->Text.IsEmpty())
  return;
 try
 {
   EditNum->Text.ToInt();
 }
 catch(...)
 {
   Application->MessageBox(L"Вводите целочисленное положительное значение",L"Ошибка",MB_ICONERROR);
   EditNum->Text=L'\0';
 }
}
//---------------------------------------------------------------------------

void __fastcall TfmSQL::RadioGroupDiagramClick(TObject *Sender)
{
 switch (RadioGroupDiagram->ItemIndex)
{

    	case 0:
                DBChartDiagram->Series[0]->Active=true;
                DBChartDiagram->Series[0]->XLabelsSource=L"Накладная";
                DBChartDiagram->Series[1]->Active=false;

                break;
        case 1:
                DBChartDiagram->Series[1]->Active=true;
                DBChartDiagram->Series[1]->XLabelsSource=L"Накладная";
                DBChartDiagram->Series[0]->Active=false;
                break;

        }
        
}
//---------------------------------------------------------------------------

void __fastcall TfmSQL::TabSheetDiagramShow(TObject *Sender)
{
 RadioGroupDiagram->ItemIndex=0;
 RadioGroupDiagramClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TfmSQL::FormShow(TObject *Sender)
{
 ADOQueryDiagram->Close();
 ADOQueryDiagram->Open();
}
//---------------------------------------------------------------------------

void __fastcall TfmSQL::ADOQueryDiagramCalcFields(TDataSet *DataSet)
{
 ADOQueryDiagram->FieldByName(L"Накладная")->AsString = L"Накладная № =" + ADOQueryDiagram->FieldByName(L"№_Накладной")->AsString;
}
//---------------------------------------------------------------------------