//Last update time : 03.06.2010 14:24:36
#ifndef MFormH
#define MFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <DB.hpp>
#include <DBClient.hpp>
#include <ADODB.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
#include <ValEdit.hpp>
//---------------------------------------------------------------------------
class TGMForm : public TForm
{
__published:	// IDE-managed Components
	TButton *BtnLoad;
	TDataSource *DSTovar;
	TADOConnection *ADOC1;
	TADOQuery *QT;
	TDBGrid *DBGrid1;
	TButton *BtnSave;
	TButton *BtnAdd;
	TButton *BtnDel;
	TListBox *lbTovars;
	TValueListEditor *vleTovar;
	TButton *BtnUpdate;
	TADOQuery *ADOUpd;
	TLabel *Label1;
	TEdit *Edit1;
	TButton *Button1;
	void __fastcall BtnLoadClick(TObject *Sender);
	void __fastcall lbTovarsClick(TObject *Sender);
	void __fastcall BtnUpdateClick(TObject *Sender);
	void __fastcall BtnDelClick(TObject *Sender);
	void __fastcall BtnAddClick(TObject *Sender);
	void __fastcall BtnSaveClick(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);

private:	// User declarations
public:		// User declarations
	__fastcall TGMForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TGMForm *GMForm;
//---------------------------------------------------------------------------
#endif