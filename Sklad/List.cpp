//Last update time : 06.09.2009 12:32:22
#include <vcl.h>
#pragma hdrstop

#include "List.h"
#include "DM.h"
#include "Tovari.h"
#include "SQL.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfmList *fmList;
//---------------------------------------------------------------------------
__fastcall TfmList::TfmList(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfmList::FormShow(TObject *Sender)
{
if (DataSource1->DataSet == DMMain->ADO_Tovari)
        { //для списка Товаров особый вид
                GroupBox1->Visible=true;
                Splitter2->Left = GroupBox1->Left;
                Splitter2->Visible=true;
        }
        else
        {
                GroupBox1->Visible=False;
                Splitter2->Visible=False;
        }
  EditFilter->Text=L'\0';
}
//---------------------------------------------------------------------------

void __fastcall TfmList::ToolButton1Click(TObject *Sender)
{
  DataSource1->DataSet->First();
}
//---------------------------------------------------------------------------

void __fastcall TfmList::ToolButton2Click(TObject *Sender)
{
  DataSource1->DataSet->Prior();
}
//---------------------------------------------------------------------------

void __fastcall TfmList::ToolButton3Click(TObject *Sender)
{
  DataSource1->DataSet->Next();
}
//---------------------------------------------------------------------------

void __fastcall TfmList::ToolButton4Click(TObject *Sender)
{
  DataSource1->DataSet->Last();
}
//---------------------------------------------------------------------------

void __fastcall TfmList::ToolButton6Click(TObject *Sender)
{
 DataSource1->DataSet->Insert();
 if (DataSource1->DataSet==DMMain->ADO_Tovari)
 {
     fmTovari->Caption=L"Добавление сведений о товаре";
     fmTovari->ShowModal();
 }       
}
//---------------------------------------------------------------------------

void __fastcall TfmList::ToolButton7Click(TObject *Sender)
{
 if (Application->MessageBox(L"Удалить текущую запись?", L"Предупреждение", MB_YESNO) == IDYES)
        DataSource1->DataSet->Delete();
}
//---------------------------------------------------------------------------

void __fastcall TfmList::ToolButton8Click(TObject *Sender)
{
 DataSource1->DataSet->Edit();
 if (DataSource1->DataSet==DMMain->ADO_Tovari)
        {
           fmTovari->Caption=L"Редактирование сведений о сотруднике";
           fmTovari->ShowModal();
       	}        
}
//---------------------------------------------------------------------------

void __fastcall TfmList::ToolButton10Click(TObject *Sender)
{
 if ((DataSource1->DataSet->State ==dsEdit) || 
(DataSource1->DataSet->State ==dsInsert))
        DataSource1->DataSet->Post();
 else
      Application->MessageBox(L"Не было изменений", L"Внимание", MB_ICONINFORMATION);
}
//---------------------------------------------------------------------------

void __fastcall TfmList::ToolButton11Click(TObject *Sender)
{
 DataSource1->DataSet->Cancel();        
}
//---------------------------------------------------------------------------

void __fastcall TfmList::ToolButtonCopyMemoClick(TObject *Sender)
{
 DBMemo1->CopyToClipboard();
}
//---------------------------------------------------------------------------

void __fastcall TfmList::ToolButtonPasteMemoClick(TObject *Sender)
{
 DBMemo1->PasteFromClipboard();
}
//---------------------------------------------------------------------------

void __fastcall TfmList::ToolButtonOpenMemoClick(TObject *Sender)
{
        try
        {
                if (OpenDialog1->Execute())
                   DMMain->ADO_Tovari->Edit();
                 DBMemo1->Lines->LoadFromFile(OpenDialog1->FileName);
                
        }
        catch(...)
        {
                Application->MessageBox(L"Не тот тип файлов",L"Ошибка", MB_ICONERROR);
        }
        
}
//---------------------------------------------------------------------------

void __fastcall TfmList::ToolButtonSaveMemoClick(TObject *Sender)
{
 if (SaveDialog1->Execute())
        DBMemo1->Lines->SaveToFile(SaveDialog1->FileName);
}
//---------------------------------------------------------------------------

void __fastcall TfmList::ToolButtonCopyImageClick(TObject *Sender)
{
 DBImage1->CopyToClipboard(); //Копировать        
}
//---------------------------------------------------------------------------

void __fastcall TfmList::ToolButtonPasteImageClick(TObject *Sender)
{
 DBImage1->PasteFromClipboard(); //Вставить         
}
//---------------------------------------------------------------------------

void __fastcall TfmList::ToolButtonOpenImageClick(TObject *Sender)
{
 try
        {
                if (OpenPictureDialog1->Execute())
                 DMMain->ADO_Tovari->Edit();
                DBImage1->Picture->LoadFromFile(OpenPictureDialog1->FileName);
        }
        catch(...)
        {
                Application->MessageBox(L"Не поддерживаемый тип файлов",L"Ошибка", MB_ICONERROR);
        }
        
}
//---------------------------------------------------------------------------

void __fastcall TfmList::ToolButtonSaveImageClick(TObject *Sender)
{
 if (SavePictureDialog1->Execute())
        DBImage1->Picture->SaveToFile(SavePictureDialog1->FileName);        
}
//---------------------------------------------------------------------------

void __fastcall TfmList::CheckBoxFilterClick(TObject *Sender)
{
 if (CheckBoxFilter->Checked) // Нажата галочка
  {     //Если не чего не указанно выводим об этом сообщение.
        if (EditFilter->Text==L'\0')
        {
                Application->MessageBox(L"Вы ничего не задали", L"Внимание", MB_ICONINFORMATION);
                CheckBoxFilter->Checked=false;
         }
        else
        {
                try
                {
                 DataSource1->DataSet->Filter=DBGrid1->SelectedField->FieldName+L"='"+EditFilter->Text+L"'";
                 ShowMessage(DataSource1->DataSet->Filter);
                 DataSource1->DataSet->Filtered=true;
                }
                catch(...)
                {
                 Application->MessageBox(L"Ошибка фильтрации данных",L"Ошибка", MB_ICONERROR);
                 CheckBoxFilter->Checked=false;
                 EditFilter->Text=L'\0';
                }
        }
    }
     else //Мначе Фильрация отменяеться.
     {
      DataSource1->DataSet->Filtered=false;
      EditFilter->Text=L'\0';
     }

  //Если таких записей нет, но мы находимся в режиме фильтрации.
   if ((DataSource1->DataSet->RecordCount==0) && (DataSource1->DataSet->Filtered))
   {
        Application->MessageBox(L"Нет таких значений", L"Сообщение", MB_ICONINFORMATION);
        CheckBoxFilter->Checked=false;
        EditFilter->Text=L'\0';
   }
}
//---------------------------------------------------------------------------

void __fastcall TfmList::btnFindClick(TObject *Sender)
{
 if (EditFilter->Text==L'\0')
 {
  Application->MessageBox(L"Вы ничего не задали", L"Внимание", MB_ICONINFORMATION);
  exit;
 }
 else
 {
  try
  {
    if (DataSource1->DataSet->Locate(DBGrid1->SelectedField->FieldName, EditFilter->Text, TLocateOptions() << loCaseInsensitive << loPartialKey ))
       DBGrid1->SetFocus();
    else
    {
        ShowMessage(L"Значение "+EditFilter->Text+L" в поле "+DBGrid1->SelectedField->DisplayLabel+L" не найдено!");
        EditFilter->Text=L'\0';
        EditFilter->SetFocus();
    }
  }
  catch(...)
  {
    Application->MessageBox(L"Ошибка поиска",L"Ошибка", MB_ICONERROR);
    EditFilter->Text=L'\0';
  }
 }
}
//---------------------------------------------------------------------------