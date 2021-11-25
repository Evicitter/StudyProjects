//Last update time : 16.10.2015 17:03:22

#ifndef MainFormH
#define MainFormH
//---------------------------------------------------------------------------
#include <System.IniFiles.hpp>
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Buttons.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <Vcl.Dialogs.hpp>
//---------------------------------------------------------------------------

#include "Protocols.h"
#include <Vcl.Imaging.pngimage.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.Samples.Gauges.hpp>
#include <Vcl.ValEdit.hpp>

class TMForm : public TForm
{
__published:	// IDE-managed Components
	TMainMenu *MainMenu1;
	TMenuItem *File;
	TMenuItem *Open;
	TMenuItem *Save;
	TMenuItem *Close;
	TMenuItem *Exit;
	TMenuItem *N1;
	TMenuItem *ExportFreqtoXML;
	TMenuItem *N2;
	TMenuItem *About;
	TStatusBar *StatusBar;
	TMenuItem *Sensor;
	TMenuItem *Connect;
	TSaveDialog *XMLSave;
	TMenuItem *Options;
	TMenuItem *ExportDigittoXML;
	TTimer *GTimer;
	TTimer *RandSkipper;
	TTimer *AutoSave;
	TTimer *DelRow;
	TMenuItem *Language;
	TMenuItem *English;
	TMenuItem *Russian;
	TMenuItem *N3;
	TMenuItem *Disconnect;
	TPageControl *PageControl1;
	TTabSheet *TabSheet2;
	TGroupBox *Information;
	TGroupBox *GroupBox1;
	TLabel *LCalibrator;
	TLabel *LDate;
	TLabel *LVehicleOwner;
	TLabel *LMarkVehicle;
	TLabel *LIndexVehicle;
	TLabel *LTypeTank;
	TLabel *LLocationTank;
	TGroupBox *GroupBox2;
	TShape *ShapeIndVehCorrect;
	TEdit *eCalibrator;
	TEdit *eDate;
	TEdit *eVehicleOwner;
	TEdit *eMarkVehicle;
	TEdit *eIndexVehicle;
	TComboBox *cbTypeOfTank;
	TComboBox *cbLocationTank;
	TTabSheet *TabSheet4;
	TGroupBox *GroupBoxData;
	TImage *Image1;
	TShape *ShapeStab;
	TLabel *LabelStab;
	TScrollBox *ScrollBox1;
	TEdit *Column01;
	TEdit *Column02;
	TEdit *Column03;
	TEdit *Column04;
	TEdit *Column05;
	TProgressBar *FuelLevel;
	TGroupBox *GroupBox6;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label7;
	TLabel *Label8;
	TEdit *edN;
	TEdit *edF;
	TEdit *edT;
	TEdit *edP;
	TGroupBox *GroupBox3;
	TGroupBox *GroupBox4;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TGroupBox *GroupBox5;
	TEdit *Edit1;
	TEdit *Edit2;
	TEdit *Edit3;
	TEdit *eTypeSensor;
	void __fastcall ExportFreqtoXMLClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall OpenClick(TObject *Sender);
	void __fastcall SaveClick(TObject *Sender);
	void __fastcall GTimerTimer(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall EnglishLangLoad(TObject *Sender);
	void __fastcall RussianLangLoad(TObject *Sender);
	void __fastcall eIndexVehicleChange(TObject *Sender);
	void __fastcall MenuSensorClick(TObject *Sender);


private:	// User declarations
	void __fastcall SetupLang();
	void __fastcall SetupINI(bool bLoad);

	UnicodeString LangStr;
	int isRun;

	HANDLE hThread;
	unsigned threadID;
public:		// User declarations
	__fastcall TMForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMForm *MForm;
extern PACKAGE TIniFile* Settings;
extern PACKAGE TIniFile* Lang;
extern Protocol* SelProtocol;
//---------------------------------------------------------------------------
#endif