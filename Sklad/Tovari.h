//Last update time : 21.03.2009 14:41:36
#ifndef TovariH
#define TovariH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <DB.hpp>
#include <DBCtrls.hpp>
#include <Mask.hpp>
#include <ExtCtrls.hpp>
#include <ADODB.hpp>
//---------------------------------------------------------------------------
class TfmTovari : public TForm
{
__published:	// IDE-managed Components
        TDataSource *dsTovari;
        TLabel *Label1;
        TLabel *Label2;
        TDBEdit *DBEdit1;
        TLabel *Label3;
        TDBComboBox *DBComboBox1;
        TButton *Button1;
        TButton *Button2;
        TDBEdit *DBEdit2;
        TADOQuery *ADOQueryTIP;
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TfmTovari(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfmTovari *fmTovari;
//---------------------------------------------------------------------------
#endif