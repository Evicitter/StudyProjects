//Last update time : 16.10.2015 17:04:02
#include <vcl.h>
#pragma hdrstop

#include <boost/regex.hpp>
#include <process.h>
#include "MainForm.h"
#include "ComChannel.h"
#include "Protocols.h"
#include "ConnectionOpt.h"
#include "GraphicForm.h"
#include "structTable.h"
#include "WaitForm.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMForm *MForm;


/*
OnAddRow
 OnDelRow
  if(MTable->RowCount>2)
  {
   MTable->RowCount--;
   GraphicsForm->UpdateGraphic();
  }
  GraphicsForm->UpdateGraphic();
  */

TIniFile* Settings = new TIniFile(ExtractFilePath( Application->ExeName ) + L"Calibration.ini");
TIniFile* Lang = NULL;

Protocol* SelProtocol=NULL;

volatile bool bRun=true;
volatile bool bRead=false;
volatile bool bChange=false;

//vector<> SendCommand;

unsigned __stdcall ThreadReadCOM_Port( void* pArguments )
{
 while(bRun)
 {
  if(GCom.IsOpenPort())
  {
   if(SelProtocol == NULL) { Sleep(500); continue; }

   bRead = true;
   int packetsize = SelProtocol->Pack(0, 0xFF, 5);
   GCom.ReqResp((char*)SelProtocol->container, packetsize, 50);
   bChange=true;

   memcpy_s(SelProtocol->container, sizeof(SelProtocol->container), &GCom.ExtBuffer[0], GCom.ExtBuffer.size());
   SelProtocol->UnPack(0, 0xFF, 5);
   SelProtocol->print();
   MForm->StatusBar->SimpleText = UnicodeString((const char*)SelProtocol->StrBuffer);
  }
  else
	Sleep(500);
 }

 _endthreadex( 0 );
 return 0;
}

// ---------------------------------------------------------------------------
__fastcall TMForm::TMForm(TComponent* Owner) : TForm(Owner) {}

// ---------------------------------------------------------------------------
void __fastcall TMForm::ExportFreqtoXMLClick(TObject *Sender) {
	XMLSave->FileName = L"xml_daf_000.xml";
	if( XMLSave->Execute(WindowHandle) )
	{
	 FILE* hfile;
	 if (_wfopen_s(&hfile, XMLSave->FileName.c_str(), L"wt") == 0)
	 {
		fputws(L"<?xml version=\"1.0\" encoding=\"Cp1251\"?>\n", hfile);
		for (int i = 0; i < 1; ++i) // for(int i=0; i<sensors; ++i)
		{
			fwprintf_s(hfile, L"<sensor number=\"%i\">\n", i);
			fputws(L"<!--Коды-Децилитры-->\n", hfile);
			//for(int j=1; j<MTable->RowCount; ++j)
			{
			// double t1 = StrToFloat(MTable->Cells[0][j]);
			// double t2 = StrToFloat(MTable->Cells[1][j]);
			// fwprintf_s(hfile, L"<value code=\"%.2f\"> %f</value>\n", t2, t1);
			}
			fputws(L"</sensor>\n", hfile);
		}
		fputws(L"<!--ПАРАМЕТРЫ МАШИНЫ-->\n", hfile);
		fclose(hfile);
	 }else
	 {
		 MessageBox(WindowHandle,L"Не удалось экспортировать в файл!",L"Ошибка",MB_ICONERROR);
     }
	}
}
// ---------------------------------------------------------------------------

void __fastcall TMForm::FormShow(TObject *Sender)
{
 srand(time(NULL));
 FormatSettings.DecimalSeparator = L'.';

 SetupINI(true);

 Lang = new TIniFile( ExtractFilePath( Application->ExeName ) + LangStr );
 SetupLang();

 if(isRun)
 {
	if( GetFileAttributesW(L"AutoSave.prj") != -1)
	{
	   int r = MessageBoxW(WindowHandle, L"Вы хотите восстановить работу с последнего сеанса?", L"Вопрос", MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON1);
	   if( r == IDYES )
	   {
		   //Load File
		   GTable.LoadTable(L"AutoSave.prj");
	   }
	}
 }
 isRun=1;
 SetupINI(false);

 eDate->Text = DateTimeToStr(Date());

 TEdit* theader[] = { Column01, Column02, Column03, Column04, Column05 };
 int ttype[] = { 1,1,1,0,0 };
 genNotify NotifyChange[] = { voidNotify, voidNotify, voidNotify, voidNotify, voidNotify };

 GTable.FirstRow(theader, ttype, NotifyChange, 5);

 for(int i=0;i<3;++i)
 {
  GTable.AddRow();
 }
}
//---------------------------------------------------------------------------


void __fastcall TMForm::OpenClick(TObject *Sender)
{
 /*FILE* hfile;
 unsigned long header = 0;
 int 			nvar=0;
 wchar_t	    buf[32];

	 if (_wfopen_s(&hfile, L"project.x", L"rb") == 0)
	 {
		fread(&header,sizeof(header),1,hfile);
		if(header != 0x121212ff)
		{
			fclose(hfile);
			MessageBox(WindowHandle,L"Неверный формат файла!",L"Ошибка",MB_ICONERROR);
			return;
		}

		fread(&nvar,sizeof(nvar),1,hfile);
		MTable->RowCount = nvar;

		for(int j=1; j<MTable->RowCount; ++j)
		{
		 for(int ind=0; ind<MTable->ColCount; ++ind)
		 {
		  memset(buf,0,sizeof(buf));
		  fread(buf,2,32,hfile);
		  MTable->Cells[ind][j]=buf;
		 }
        }

		fclose(hfile);
		GraphicsForm->UpdateGraphic();
	 }*/
}
//---------------------------------------------------------------------------

void __fastcall TMForm::SaveClick(TObject *Sender)
{
/* FILE* hfile;
 unsigned long header = 0x121212ff;
 int nvar=0;
 wchar_t		   buf[32];

	 if (_wfopen_s(&hfile, L"autosave.x", L"wb") == 0)
	 {
		fwrite(&header,sizeof(header),1,hfile);

		nvar = MTable->RowCount;
		fwrite(&nvar,sizeof(nvar),1,hfile);

		for(int j=1; j<MTable->RowCount; ++j)
		{
		 for(int ind=0; ind<MTable->ColCount; ++ind)
		 {
		  memset(buf,0,sizeof(buf));
		  memcpy(buf,MTable->Cells[ind][j].c_str(),(unsigned int)MTable->Cells[ind][j].Length()*2);
		  fwrite(buf,2,32,hfile);
         }
        }

		fclose(hfile);
	 }*/
}
//---------------------------------------------------------------------------

void __fastcall TMForm::GTimerTimer(TObject *Sender)
{
 TTimer* T = dynamic_cast<TTimer*>(Sender);
 if(T==NULL) return;

 if(T->Tag==0)
 {
  pOmnicomm* po = dynamic_cast<pOmnicomm*>(SelProtocol);
  if(po!=NULL)
  {
   edT->Text = IntToStr(po->temperature);
   FuelLevel->Position = po->difflevel;
   edN->Text = IntToStr(po->difflevel);
   edF->Text = IntToStr(po->freq);
  }

  if(bChange)
  {
   if( bRead )
   {
	Image1->Picture->Bitmap->LoadFromResourceName((int)HInstance,L"BM_Read");
	bRead = false;
   }
   else
   {
	Image1->Picture->Bitmap->LoadFromResourceName((int)HInstance,L"BM_Idle");
	bChange=false;
   }
  }
 }
 else if(T->Tag==1)  //RandomSkiper
 {
	 T->Interval = 1000 + rand();
 }
 else if(T->Tag==2) //AutoSave
 {
 	SetupINI(false);
 }
 else if(T->Tag==3)
 {
	 GTable.DelRowFrom(GTable.indexSelRow);
	 T->Enabled=false;
 }
}
//---------------------------------------------------------------------------

void __fastcall TMForm::FormClose(TObject *Sender, TCloseAction &Action)
{
  isRun = 0;
  SetupINI(false);
}
//---------------------------------------------------------------------------

void __fastcall TMForm::EnglishLangLoad(TObject *Sender)
{
 TMenuItem* tmi = dynamic_cast<TMenuItem*>(Sender);
 tmi->Checked = true;
 //EnglishLangLoad
 if(Lang != NULL) Lang->Free();

 Lang = new TIniFile( ExtractFilePath( Application->ExeName ) + L"eng.ini" );
 SetupLang();
}
//---------------------------------------------------------------------------

void __fastcall TMForm::RussianLangLoad(TObject *Sender)
{
 TMenuItem* tmi = dynamic_cast<TMenuItem*>(Sender);
 tmi->Checked = true;
 if(Lang != NULL) Lang->Free();

 Lang = new TIniFile( ExtractFilePath( Application->ExeName ) + L"rus.ini" );
 SetupLang();
}
//---------------------------------------------------------------------------

void __fastcall TMForm::SetupLang()
{
 if(Lang!=NULL)
 {
  StatusBar->SimpleText = Lang->ReadString(L"Main", L"StatusBar", L"Ready!");
  MForm->Caption = Lang->ReadString(L"Main", L"MForm", L"Calibration");
  GroupBoxData->Caption = Lang->ReadString(L"Main", L"GroupBoxData", L"Data");
  Information->Caption = Lang->ReadString(L"Main", L"Information", L"Information");

  int sind = cbLocationTank->ItemIndex;
  cbLocationTank->Items->Clear();
  cbLocationTank->Items->Add( Lang->ReadString(L"Main", L"Left", L"Left") );
  cbLocationTank->Items->Add( Lang->ReadString(L"Main", L"Right", L"Right") );
  cbLocationTank->Items->Add( Lang->ReadString(L"Main", L"Central", L"Central") );
  cbLocationTank->ItemIndex = sind;

  TStringList* jek = new TStringList();
  //---------------MENU LOAD---------------------------------------------
  Lang->ReadSectionValues(L"menu",jek);
  for(int i=0; i<jek->Count; ++i)
  {
	 TMenuItem* t = dynamic_cast<TMenuItem*>(FindComponent(jek->Names[i]));
	 if(t!=NULL)
		t->Caption = jek->ValueFromIndex[i];
  }
  jek->Clear();
  //----------------LABELS-----------------------------------------------
  Lang->ReadSectionValues(L"label",jek);
  for(int i=0; i<jek->Count; ++i)
  {
	 TLabel* t = dynamic_cast<TLabel*>(FindComponent(jek->Names[i]));
	 if(t!=NULL)
		t->Caption = jek->ValueFromIndex[i];
  }
  jek->Clear();
  //----------------EDIT (COLUMNS from TABLE)----------------------------
  Lang->ReadSectionValues(L"table",jek);
  for(int i=0; i<jek->Count; ++i)
  {
	 TEdit* t = dynamic_cast<TEdit*>(FindComponent(jek->Names[i]));
	 if(t!=NULL)
		t->Text = jek->ValueFromIndex[i];
  }
  jek->Clear();
  jek->Free();
  jek = NULL;
 }
}
//---------------------------------------------------------------------------

void __fastcall TMForm::SetupINI(bool bLoad)
{
 if(Settings == NULL) return;

 if(bLoad)
 {
  LangStr = Settings->ReadString(L"Main",L"Lang",L"rus.ini");

  AutoSave->Interval = Settings->ReadInteger(L"Main",L"AutoSavePeriod_ms",300000);
  if(AutoSave->Interval < 1000) AutoSave->Interval = 1000;

  isRun = Settings->ReadInteger(L"Main",L"isRun",0);

  eCalibrator->Text = Settings->ReadString(L"Main",L"eCalibrator",L"");
  eVehicleOwner->Text = Settings->ReadString(L"Main",L"eVehicleOwner",L"");
  eMarkVehicle->Text = Settings->ReadString(L"Main",L"eMarkVehicle",L"");
  eIndexVehicle->Text = Settings->ReadString(L"Main",L"eIndexVehicle",L"");
  cbTypeOfTank->Text = Settings->ReadString(L"Main",L"cbTypeOfTank",L"");
  cbLocationTank->ItemIndex = Settings->ReadInteger(L"Main",L"cbLocationTank",0);
 }
 else
 {
  Settings->WriteString(L"Main",L"eCalibrator", eCalibrator->Text);
  Settings->WriteString(L"Main",L"eVehicleOwner", eVehicleOwner->Text);
  Settings->WriteString(L"Main",L"eMarkVehicle", eMarkVehicle->Text);
  Settings->WriteString(L"Main",L"eIndexVehicle", eIndexVehicle->Text);
  Settings->WriteString(L"Main",L"cbTypeOfTank", cbTypeOfTank->Text);
  Settings->WriteInteger(L"Main",L"cbLocationTank", cbLocationTank->ItemIndex);

  Settings->WriteInteger(L"Main",L"AutoSavePeriod_ms", AutoSave->Interval);
  Settings->WriteInteger(L"Main",L"isRun",isRun);
 }
}
//---------------------------------------------------------------------------

void __fastcall TMForm::eIndexVehicleChange(TObject *Sender)
{
 using namespace boost;

 int u = eIndexVehicle->SelStart;
 eIndexVehicle->Text = AnsiUpperCase(eIndexVehicle->Text);
 eIndexVehicle->SelStart = u;

 wsmatch m;
 wregex re(L"\\b[А,В,Е,К,М,Н,О,Р,С,Т,У,Х]\\d{3}[А,В,Е,К,М,Н,О,Р,С,Т,У,Х]{2}\\d{2,3}\\b");
 std::wstring num = eIndexVehicle->Text.w_str();

  if( regex_search(num,m,re) )
  {
	ShapeIndVehCorrect->Brush->Color = clLime;  //НОМЕР ВАЛИДЕН
  }else
  {
	ShapeIndVehCorrect->Brush->Color = clRed;   //ГОС. НОМЕР НЕВЕРНЫЙ
  }
}
//---------------------------------------------------------------------------

void __fastcall TMForm::MenuSensorClick(TObject *Sender)
{
 TMenuItem* tmi = dynamic_cast<TMenuItem*>(Sender);
 if(tmi==NULL) return;

 if(tmi->Tag == 0) //Connect
 {
  if(ConnectOptions->isAllowedPort())
  {
	GCom.OpenPort(ConnectOptions->GetSelectedPort().c_str());
	GCom.SetupPort(false, ConnectOptions->GetSelectedSpeed(), 8, ConnectOptions->GetSelectedStopBit(), ConnectOptions->GetSelectedParity());
	GCom.SetTimeout();

	if(GCom.IsOpenPort())
	{
	   tmi->Enabled = false;
	   Disconnect->Enabled = true;

	   if(SelProtocol!=NULL)
	   {
		wchar_t tname[64];
		SelProtocol->wCopyProtocolName(tname);
		eTypeSensor->Text = tname;
	   }
	}

	hThread = (HANDLE)_beginthreadex( NULL, 0, &ThreadReadCOM_Port, NULL, 0, &threadID );
   //CloseHandle( hThread );
  }
 }
 else if(tmi->Tag == 1) //Disconnect
 {
   WaitProgress->Show();
   if(GCom.ClosePort())
   {
	   eTypeSensor->Text = L"";
	   tmi->Enabled=false;
	   Connect->Enabled=true;
   }
   WaitProgress->Hide();
 }
 else if(tmi->Tag == 2) //Options
 {
  if(ConnectOptions->ShowModal()==mrOk)
  {
   if(ConnectOptions->isAllowedPort())
   {
	 if(SelProtocol != NULL) delete SelProtocol;

	 eProtocolType tt = (eProtocolType)ConnectOptions->GetSelectedSensorType();
	 SelProtocol = GetInstanceProtocol(tt);
   }
  }
 }
}
//---------------------------------------------------------------------------