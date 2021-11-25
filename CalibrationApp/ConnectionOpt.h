//Last update time : 03.10.2015 15:15:02

#ifndef ConnectionOptH
#define ConnectionOptH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Registry.hpp>
//---------------------------------------------------------------------------
class TConnectOptions : public TForm
{
__published:	// IDE-managed Components
	TButton *butok;
	TButton *bitcancel;
	TGroupBox *GroupBox1;
	TLabel *LPortName;
	TLabel *LSpeed;
	TLabel *LParity;
	TLabel *LTypeSensor;
	TGroupBox *GroupBox2;
	TComboBox *portCB;
	TComboBox *speedCB;
	TComboBox *parityCB;
	TComboBox *cbTypeSensor;
	void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
	bool 			__fastcall isAllowedPort() const;
	UnicodeString 	__fastcall GetSelectedPort() const;
	int			 	__fastcall GetSelectedSpeed() const;
	int			 	__fastcall GetSelectedStopBit() const;
	int			 	__fastcall GetSelectedParity() const;
	int			 	__fastcall GetSelectedSensorType() const;
	__fastcall TConnectOptions(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TConnectOptions *ConnectOptions;
//---------------------------------------------------------------------------
#endif