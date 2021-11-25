//Last update time : 03.10.2015 15:15:52

#include <vcl.h>
#pragma hdrstop

#include "ConnectionOpt.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TConnectOptions *ConnectOptions;
//---------------------------------------------------------------------------
__fastcall TConnectOptions::TConnectOptions(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TConnectOptions::FormShow(TObject *Sender)
{
 TRegistry * Reg;
 TStringList * List;
 Reg=new TRegistry;
 List=new TStringList;
 UnicodeString w;
 Reg->RootKey=HKEY_LOCAL_MACHINE;
 Reg->OpenKey("HARDWARE\\DEVICEMAP\\SERIALCOMM", false);
 Reg->GetValueNames(List);

 //if(List->Count==0)
 portCB->Items->Clear();
 for (int i=0;i<List->Count;i++)
 {
  w=Reg->ReadString(List->Strings[i]);
  if ( _wcsnicmp(w.c_str(), L"COM", 3) == 0)
   portCB->Items->Add(w);
 }
 List->Free();
 Reg->CloseKey();
 Reg->Free();
 portCB->ItemIndex=0;
}

bool 			__fastcall TConnectOptions::isAllowedPort() const
{
	return (portCB->ItemIndex != -1);
}

UnicodeString 	__fastcall TConnectOptions::GetSelectedPort() const
{
	if(portCB->ItemIndex == -1) return L"";
    return portCB->Items[0][portCB->ItemIndex];
}

int			 	__fastcall TConnectOptions::GetSelectedSpeed() const
{
 switch(speedCB->ItemIndex)
 {
	 case 0: return CBR_1200;
	 case 1: return CBR_2400;
	 case 2: return CBR_4800;
	 case 3: return CBR_9600;
	 case 4: return CBR_14400;
	 case 5: return CBR_19200;
	 case 6: return CBR_115200;
	 case 7: return CBR_128000;
	 case 8: return CBR_256000;
	 default: return CBR_19200;
 }
}

int			 	__fastcall TConnectOptions::GetSelectedStopBit() const
{
  return ONESTOPBIT;
}

int			 	__fastcall TConnectOptions::GetSelectedParity() const
{
  switch(parityCB->ItemIndex)
  {
	  case 0: return NOPARITY;
	  case 1: return ODDPARITY;
	  case 2: return EVENPARITY;
	  case 3: return MARKPARITY;
	  case 4: return SPACEPARITY;
	  default : return NOPARITY;
  }
}

int			 	__fastcall TConnectOptions::GetSelectedSensorType() const
{
    return cbTypeSensor->ItemIndex;
}

//---------------------------------------------------------------------------