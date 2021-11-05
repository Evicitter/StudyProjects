//Last update time : 11.05.2010 03:18:40

#include <vcl.h>
#pragma hdrstop

#include "StepCon_MF.h"
#include <shlobj.h>
#include <stdio.h>
//#include <proccess.h>

//---------------------------------------------------------------------------

//Include MyClasses -----------
#include "CfgPrgCls.h"
#include "LogTimeCls.h"
//----------------------------
#include "LogForm.h"
#include "TimeForm.h"
#include "GB_ActionBrowser.h"
#include "GB_GroupBrowser.h"
#include "GB_MaterialBrowser.h"
#include "GB_WaveBrowser.h"
#include "StepCon_PF.h"

#include "TransAppMsg.h"

#pragma package(smart_init)
#pragma link "FRM_FieldSpaces"
#pragma resource "*.dfm"
TMainForm *MainForm;

//Global Variables ----------------------------------------------
//for GUI
TToolButton* LastDropedButton = NULL;
//----------------------------------------------------------------
UnicodeString ExportDirectory;
UnicodeString FileProject;
int IteratorLApp=0;
//----------------------------------------------------------------
TransAppMsg tamServer;
//----------------------------------------------------------------

void __fastcall Rout1( void* p, unsigned short nsize )
{
	__int64 dd;
	//ShowMessage( UnicodeString(nsize) + L"    " + UnicodeString( IntToHex( *(__int64*)p, 16 ) )  );
	if( nsize == 8 )
	{
		dd = *(__int64*)p;
		switch( dd>>32 )
		{
		 case 0x0fff1212:
		   Global_CFG.ProgressCurrent = (long)(dd & 0x00000000ffffffff);
		   //ShowMessage( IntToHex(dd,16) );
		 break;
		 case 0x0fff1313:
		   ProgressForm->Hide();
		   if( MainForm->NextLaunch() )
			MainForm->Show();
		   //ShowMessage( IntToHex(dd,16) );
		 break;
		 case 0x0fff1314:
		 	MainForm->Hide();
		   ProgressForm->Show();
		   //ShowMessage( IntToHex(dd,16) );
		 break;
		 case 0x11000011:
		   Global_CFG.ProgressMax = (long)(dd & 0x00000000ffffffff);
		   //ShowMessage( IntToHex(dd,16) );
		 break;
        }
	}
}

void __fastcall Rout2( void* p, unsigned short nsize ) //Caption Progress Update
{
 UnicodeString pp = (wchar_t*)p;
 if(ProgressForm != NULL)
  ProgressForm->Caption = pp;
}

void __fastcall Rout3( void* p, unsigned short nsize ) //LogWrite
{
 UnicodeString pp = (wchar_t*)p;
 if( !pp.IsEmpty() )
 	 Global_LOG.WriteLog( pp.w_str() );
}


void ListInheritFiles(UnicodeString path,TStrings* of)
{
// Эта процедура выводит список файлов и вызывает
// саму себя для всех каталогов
 TSearchRec sr;
 if (FindFirst(path+TEXT("*.*"), faAnyFile, sr) == 0)
 {
  do
  {
   if((sr.Attr & faDirectory) == faDirectory )
   {
	if ((sr.Name!=TEXT(".")) && (sr.Name!=TEXT("..")))
	 ListInheritFiles(path+sr.Name+TEXT("\\"),of);
   }
   else
   {
	of->Add(path + sr.Name);
   }
 }
 while (FindNext(sr) == 0);
 FindClose(sr);
}
 Application->ProcessMessages();
}

void ListLocalFiles(UnicodeString path, TStrings* of)
{
 TSearchRec sr;
 if (FindFirst(path+TEXT("*.*"), faAnyFile, sr) == 0)
 {
  do
  {
   if( (sr.Attr & faDirectory) != faDirectory )
	of->Add(path + sr.Name);
  }
  while (FindNext(sr) == 0);
  FindClose(sr);
 }
 Application->ProcessMessages();
}

//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormCreate(TObject *Sender)
{
 if(!Mouse->MousePresent) ShowMessage(TEXT("Мышь не обнаруженна!!!"));
 if(!Mouse->WheelPresent) ShowMessage(TEXT("Отсутсвует колесико у мыши!!!"));

 //Screen->MonitorCount;

 Global_LOG.InitLog( GetCurrentDir().w_str(), TEXT("MainLog.txt"), 512<<10 );
 Global_LOG.WriteLog(TEXT("Init Loging-;"));

 Global_CFG.Init( GetCurrentDir().w_str() );
 Global_CFG.LoadParameters();

// ExtractFileDir();

 //-----------------------------------------
 StatusBarFile->Panels[0][0]->Text = UnicodeString(L"Export Directory:  ") + Global_CFG.ExportPath;

 //Enable Drag Files
 DragAcceptFiles( ListBoxFiles->Handle, true);

 //-------------------------------------------------------------------------------
 tamServer.SetCallBack( &Rout1, 0 );
 tamServer.SetCallBack( &Rout2, 1 );
 tamServer.SetCallBack( &Rout3, 2 );
 tamServer.Host(L"StepGLBMS");
 //-------------------------------------------------------------------------------
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormClose(TObject *Sender, TCloseAction &Action)
{
 Global_CFG.SaveParameters();
 Global_LOG.WriteLog(TEXT("All Config Params is write."));
 Global_LOG.WriteLog(TEXT("DeInit Loging-;"));
 Global_LOG.CloseLog();

 tamServer.Disconnect();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::OpenITExecute(TObject *Sender)
{
 //
 TAction* ta = dynamic_cast<TAction*>(Sender);
 if( ta == NULL ) return;

 switch(ta->Tag)
 {
  case 1:
  case 2:
   if(OpenDialog_Files->Execute(WindowHandle))
   {
    ListBoxFiles->Clear();
	for(ULONG i = 0ul; i<OpenDialog_Files->Files->Count; ++i)
	 ListBoxFiles->Items->Add(OpenDialog_Files->Files[0][i]);
   }
  break;
  //--------------------------------------------------------------------------
  case 3:
   BROWSEINFO              bi;
   TCHAR                   szDisplayName[MAX_PATH];
   LPITEMIDLIST            pidl;
   ZeroMemory(&bi, sizeof(bi));
   bi.hwndOwner = WindowHandle;
   bi.pszDisplayName = szDisplayName;
   bi.lpszTitle = TEXT("Select folder");
   bi.ulFlags = BIF_RETURNONLYFSDIRS;
   pidl = SHBrowseForFolder(&bi);
   if(pidl)
   {
	SHGetPathFromIDList(pidl, szDisplayName);
	INT retcode = MessageBox(WindowHandle, TEXT("Включить вложенные папки?"), TEXT("?"), MB_ICONQUESTION | MB_YESNO);
	if( retcode == IDYES )
	{
	 ListBoxFiles->Clear();
	 ListInheritFiles(UnicodeString(szDisplayName) + TEXT("\\"), ListBoxFiles->Items);
	}
	else if( retcode == IDNO )
	{
     ListBoxFiles->Clear();
	 ListLocalFiles(UnicodeString(szDisplayName) + TEXT("\\"), ListBoxFiles->Items);
    }
   }
  break;
  //-----------------------------------------------------------------------
  case 4:
   if(OpenDialog_Project->Execute(WindowHandle))
   {
	ListBoxFiles->Items->LoadFromFile(OpenDialog_Project->FileName, TEncoding::Unicode);
	FileProject = OpenDialog_Project->FileName;
   }
  break;
  case 5:
   BROWSEINFO              browsei;
   TCHAR                   szPath[MAX_PATH];
   LPITEMIDLIST            ppidl;
   ZeroMemory(&browsei, sizeof(browsei));
   browsei.hwndOwner = WindowHandle;
   browsei.pszDisplayName = szPath;
   browsei.lpszTitle = TEXT("Select folder");
   browsei.ulFlags = BIF_RETURNONLYFSDIRS;
   ppidl = SHBrowseForFolder(&browsei);
   if(ppidl)
   {
	SHGetPathFromIDList(ppidl, szPath);
	if( szPath[ wcslen(szPath)-1 ] != L'\\' ) wcscat(szPath, L"\\");

	if( !Global_CFG.SetExportPath(szPath) )
	   MessageBox(WindowHandle, TEXT("Внутренняя ошибка установки директории"), TEXT("Error!!!"), MB_ICONERROR);
	else
	   StatusBarFile->Panels[0][0]->Text = UnicodeString(L"Export Directory:  ") + Global_CFG.ExportPath;
   }
  break;
  case 6:
  break;
 }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::CloseITExecute(TObject *Sender)
{
 TAction* ta = dynamic_cast<TAction*>(Sender);
 if( ta == NULL ) return;

 switch(ta->Tag)
 {
	 case 40976:
		ListBoxFiles->DeleteSelected();
	 break;
	 case 41216:
		ListBoxFiles->Clear();
	 break;
 }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SaveITExecute(TObject *Sender)
{
 TAction* ta = dynamic_cast<TAction*>(Sender);
 if( ta == NULL ) return;

 switch(ta->Tag)
 {
	 case 1073741824: //result
		//if
	 break;
	 case 1073741825: //project
		if( SaveDialog_Project->Execute(WindowHandle) )
		{
		 if(ListBoxFiles->Items->Count == 0) {ShowMessage(L"Нечего сохранять в файл!!!"); break; }

			ListBoxFiles->Items->SaveToFile(SaveDialog_Project->FileName, TEncoding::Unicode);
			FileProject=SaveDialog_Project->FileName;
		}
	 break;
 }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ExitAppExecute(TObject *Sender)
{
 Application->Terminate();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ShowLog_Time(TObject *Sender)
{
 //View Log - Show Time - statistic category
 TAction* ta = dynamic_cast<TAction*>(Sender);
 if( ta == NULL ) return;

 switch( ta->Tag )
 {
  case 100000000:
	if(FormLog->Showing)  FormLog->Hide();
	else FormLog->Show();
  break;
  case 200000000:
	if(FormTime->Showing)  FormTime->Hide();
	else FormTime->Show();
  break;
 }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FileToolBar_Click(TObject *Sender)
{
  TToolButton* ttb = dynamic_cast<TToolButton*>( Sender );
  if( ttb == NULL ) return;

  switch(ttb->Tag)
  {
   case 10:  ListBoxFiles->DeleteSelected(); break;
   case 20:
	DragAcceptFiles( ListBoxFiles->Handle, false);
	ListBoxFiles->Sorted = !ListBoxFiles->Sorted;  ttb->Down=ListBoxFiles->Sorted;
	DragAcceptFiles( ListBoxFiles->Handle, true);
   break;
   case 100: frmFieldSpaces1->DeleteSelectedActObj(); break;
   case 200: frmFieldSpaces1->SetLinkMode( ttb->Down ); break;
  }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::LeftToolBars_Click(TObject *Sender)
{
 TToolButton* ttb = dynamic_cast<TToolButton*>( Sender );
  if( ttb == NULL ) return;


/*	TPicture *Picture = new TPicture();
Picture->RegisterFileFormat(__classid(TJPEGImage), "jpg", "jpg", __classid(TJPEGImage));
delete Picture;
*/
  //left
  if( ttb->Tag == 11 );

  //right
  if( ttb->Tag == 101 );

  //Сделаем так что все кнопки Таг которых меньше 1000 будут в состоянии down
  if( ttb->Tag < 1000 )
  {
   ttb->Down = true;	//Текущую нажатую кнопку задауним.
   //Если предыдущая кнопка не равна текущей то вернём предыдущей нормальное состояние.
   if((LastDropedButton != NULL) && (LastDropedButton != ttb))  LastDropedButton->Down = false;
   LastDropedButton = ttb;
  }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::GBrw_Exec(TObject *Sender)
{
 TAction* ta = dynamic_cast<TAction*>(Sender);
 if( ta == NULL ) return;

 switch( ta->Tag )
 {
   case 1000: //Action
	if(BrwAction->Showing)  BrwAction->Hide();
	else BrwAction->Show();
   break;
   case 2000: //Group
	if(BrwGroup->Showing)  BrwGroup->Hide();
	else BrwGroup->Show();
   break;
   case 3000: //Wave
	if(BrwWave->Showing)  BrwWave->Hide();
	else BrwWave->Show();
   break;
   case 4000: //Material
	if(BrwMaterial->Showing)  BrwMaterial->Hide();
	else BrwMaterial->Show();
   break;
   case 5000: //Space
   break;
   case 100000000: //Action Properties
   break;
   case 200000000: //Space Properties
   break;
 }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ListBoxFilesDrawItem(TWinControl *Control, int Index, TRect &Rect,
          TOwnerDrawState State)
{
 if(Index == 0)
 {
   ListBoxFiles->Canvas->FillRect(Rect);
   ListBoxFiles->Canvas->Brush->Color = clRed;
   ListBoxFiles->Canvas->Font->Color = ~clRed;
   ListBoxFiles->Canvas->TextOutW(Rect.Left, Rect.Top, ListBoxFiles->Items->Strings[Index]);
 }
 else
 {
  ListBoxFiles->Canvas->FillRect(Rect);
  ListBoxFiles->Canvas->Font->Color = clBlack;
  ListBoxFiles->Canvas->TextOutW(Rect.Left, Rect.Top, ListBoxFiles->Items->Strings[Index]);
 }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::AppEventsMessage(tagMSG &Msg, bool &Handled)
{
 if(Msg.message == WM_DROPFILES)
 {
	 TCHAR path[256];
	 UnicodeString string;

	HDROP DROP = (HDROP)Msg.wParam;
	int nFiles = DragQueryFile(DROP, -1, NULL, NULL);
	//количество файлов,
	//ведь можно перетаскивать один файл, а можно сразу несколько.
	for (int i = 0; i < nFiles; i++)
	{
		DragQueryFile(DROP, i, path, sizeof(path));
		if( GetFileAttributes(path) & faDirectory )
		{
		  INT retcode = MessageBox(WindowHandle, TEXT("Включить вложенные папки?"), TEXT("?"), MB_ICONQUESTION | MB_YESNO);
		  if( retcode == IDYES )
			ListInheritFiles(UnicodeString(path) + TEXT("\\"), ListBoxFiles->Items);
		  else if( retcode == IDNO )
			ListLocalFiles(UnicodeString(path) + TEXT("\\"), ListBoxFiles->Items);
		}
		else
		{
		 string= path;// полное имя перетсакиваемого на форму файла (включая путь)
		 //чтоб определить только имя файла надо использовать ф-цию ExtractFileName(AnsiString)
		 //например ListBox1->Items->Add(ExtractFileName(string));
		 //Дальше делаем с ним чего угодно например добавляем в ЛистБокс
		 ListBoxFiles->Items->Add(string);
		}
	}
	DragFinish(DROP);
 }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Build_Exec(TObject *Sender)
{
 TAction* ta = dynamic_cast<TAction*>(Sender);
 if( (ta == NULL) ) return;

 UnicodeString params;

 //params = ReplaceChar(FileProject, L' ', L'?') + L' ';
 IteratorLApp=0;
 params = ReplaceChar( ListBoxFiles->Items[0][IteratorLApp] , L' ', L'?') + L' ';
 params += ReplaceChar(Global_CFG.ExportPath, L' ', L'?');

 switch( ta->Tag )
 {
   case 10000: //HASH && STAT
		ShellExecute(WindowHandle, NULL, L"csHashStat.exe", params.w_str(), GetCurrentDir().w_str(), SW_SHOW);
   break;
 }
}
//---------------------------------------------------------------------------

bool __fastcall TMainForm::NextLaunch()
{
 UnicodeString params;

 //params = ReplaceChar(FileProject, L' ', L'?') + L' ';
 ++IteratorLApp;
 if( IteratorLApp >= ListBoxFiles->Count) return true;
 params = ReplaceChar( ListBoxFiles->Items[0][ IteratorLApp ] , L' ', L'?') + L' ';
 params += ReplaceChar(Global_CFG.ExportPath, L' ', L'?');

 ShellExecute(WindowHandle, NULL, L"csHashStat.exe", params.w_str(), GetCurrentDir().w_str(), SW_HIDE);
 return false;
}

