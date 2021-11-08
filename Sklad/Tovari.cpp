//Last update time : 06.09.2009 12:27:38
#include <vcl.h>
#pragma hdrstop

#include "Tovari.h"
#include "DM.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfmTovari *fmTovari;
//---------------------------------------------------------------------------
__fastcall TfmTovari::TfmTovari(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfmTovari::FormClose(TObject *Sender, TCloseAction &Action)
{
 if (ModalResult==mrOk)
        DMMain->ADO_Tovari->Post();
 else
        DMMain->ADO_Tovari->Cancel();
}
//---------------------------------------------------------------------------
void __fastcall TfmTovari::FormShow(TObject *Sender)
{
        DBComboBox1->Items->Clear();
        ADOQueryTIP->Open();
        ADOQueryTIP->First();
        while (!ADOQueryTIP->Eof)
        {
                DBComboBox1->Items->Add(ADOQueryTIP->FieldByName(L"θο")->AsString);
                ADOQueryTIP->Next();
        }
        ADOQueryTIP->Close();

}
//---------------------------------------------------------------------------