//Last update time : 27.12.2009 05:24:56

#ifndef RandTestH
#define RandTestH

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include "Chart.hpp"
#include "TeEngine.hpp"
#include "TeeProcs.hpp"
#include <ExtCtrls.hpp>
#include "Series.hpp"

class TForm2 : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *GroupBox1;
	TCheckBox *CheckBox1;
	TEdit *Edit1;
	TGroupBox *GroupBox2;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TEdit *Edit2;
	TEdit *Edit3;
	TEdit *Edit4;
	TPageControl *PageControl1;
	TTabSheet *TabSheet1;
	TGroupBox *GroupBox3;
	TButton *Button1;
	TLabel *Label5;
	TTrackBar *TrackBar2;
	TEdit *Edit5;
	TLabel *Label6;
	TLabel *Label7;
	TEdit *Edit6;
	TLabel *Label8;
	TEdit *Edit7;
	TLabel *Label4;
	TComboBox *ComboBox1;
	TGroupBox *GroupBox4;
	TGroupBox *GroupBox5;
	TLabel *Label9;
	TLabel *Label10;
	TEdit *Edit8;
	TEdit *Edit9;
	TLabel *Label11;
	TEdit *Edit10;
	TLabel *Label12;
	TLabel *Label13;
	TEdit *Edit11;
	TLabel *Label14;
	TEdit *Edit12;
	TGroupBox *GroupBox6;
	TLabel *Label15;
	TEdit *Edit13;
	TLabel *Label16;
	TLabel *Label17;
	TEdit *Edit14;
	TEdit *Edit15;
	void __fastcall CheckBox1Click(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm2(TComponent* Owner);
};

extern PACKAGE TForm2 *Form2;
#endif
