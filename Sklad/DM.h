//Last update time : 06.09.2009 12:54:22
#ifndef DMH
#define DMH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ADODB.hpp>
#include <DB.hpp>
//---------------------------------------------------------------------------
class TDMMain : public TDataModule
{
__published:	// IDE-managed Components
        TADOConnection *ADOConnection1;
        TADOTable *ADO_Nakladnaya;
        TADOTable *ADO_Sklad;
        TADOTable *ADO_NakToTovar;
        TADOTable *ADO_Tovari;
        TIntegerField *ADO_NakToTovarDSDesigner3;
        TWideStringField *ADO_SkladDSDesigner2;
        TDateTimeField *ADO_NakladnayaDSDesigner3;
        TWideStringField *ADO_NakladnayaDSDesigner4;
        TWideStringField *ADO_NakladnayaDSDesigner5;
        TBlobField *ADO_TovariDSDesigner4;
        TMemoField *ADO_TovariDSDesigner5;
        TStringField *ADO_NakToTovarField;
        TIntegerField *ADO_Nakladnaya_;
        TIntegerField *ADO_NakToTovar_;
        TIntegerField *ADO_NakladnayaDSDesigner;
        TIntegerField *ADO_SkladDSDesigner;
        TIntegerField *ADO_NakToTovar_2;
        TWideStringField *ADO_TovariDSDesigner;
        TWideStringField *ADO_TovariDSDesigner2;
        TIntegerField *ADO_Tovari_2;
        TStringField *ADO_TovariINFO;
        void __fastcall ADO_TovariCalcFields(TDataSet *DataSet);
        void __fastcall ADO_TovariPostError(TDataSet *DataSet,
          EDatabaseError *E, TDataAction &Action);
    void __fastcall ADO_TovariDeleteError(TDataSet *DataSet, EDatabaseError *E, TDataAction &Action);

private:	// User declarations
public:		// User declarations
        __fastcall TDMMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TDMMain *DMMain;
//---------------------------------------------------------------------------
#endif