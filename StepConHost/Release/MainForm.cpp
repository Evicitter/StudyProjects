//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

//Need inc
#include <shlobj.h>

#include "MainForm.h"
#include "ProgressDlg.h"
#include "BoolTech.h"
#include "FileProc.h"
#include "FPU_Tech.h"
#include "CalcRender_Init.h"
#include "R8_RND.h"
#include "R8_Electro_RND.h"
#include "HashShowDialogR8.h"
#include "HashShowDialog.h"
#include "HashShowDialogStep.h"
#include "HashShowDialogStepMir.h"
#include "SProfileExporter.h"
#include "RAM_MGR_Dlg.h"
#include "OptStepFormuls.h"
#include "GraphicView.h"
#include "ProgressVideo.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TForm1 *Form1;
//----------------------------------------------------------------------------

//GLOBAL VARIABLES ----------------------------------------------------------
StructGlobalVariables GlbVarApp;
DynamicArray<FileProcessor*> arrRenderFiles;
unsigned __int64 LastStatRes[12]={0i64,0i64,0i64,0i64,0i64,0i64,0i64,0i64,0i64,0i64,0i64,0i64};

#define ShowDig(x) ShowMessage( IntToStr((int)x) )
#define ShowUDig(x) ShowMessage( UIntToStr((unsigned int)x) )

long glbSelectFileDataRow=0;
BROWSEINFO bi= {NULL,  // Родитель
						NULL,  // не помню, т.е. не надо
						NULL,  //Начальная директория
						L"Выберите, папку для обработки...",  // Заголовок
						BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_EDITBOX,  // Стили
						NULL,  // ф-ция обработки мессаг
						NULL, // Не нужно нам такого
						0}; // Это не для нас


//Calculation p function
#include "CalcRender_1_INT.h"
//----------------------------------------------------------------------------

StructGlobalVariables::StructGlobalVariables()
{
  FileFilter=0x43;
  StatChecks=0xFF;
  HashChecks=0xffffffff;
  CalcChecks=0x3l; bStateStartHashBTN=0; iTimeStack=0; nThreads=1;
   lStartTime=0l;
   lEndTime=0l;
   lElapsedTime=0l;

   lIndexofEND=0l;
   lvidStartTime=0l;
   lvidEndTime=0l;
   lvidElapsedTime=0l;

  bctrl_AutoExState=false;
  ZeroMemory(&mem,sizeof(MEMORYSTATUS));
  mem.dwLength=sizeof(mem);

  GetBitFRS=1l;
  FreqBitFRS=10l;
  ElementType=8i8;

  bFilesSUM=false;

  bRenderSelected=false;
  bAutoRunVideoScan=false;

  globalHP = malloc(sizeof(HashProc));
  globalHR8P = malloc(sizeof(HashProc));
  globalSP = malloc(sizeof(StatProc));
  global_vHP = malloc(sizeof(HashProc));
  global_vSP = malloc(sizeof(StatProc));
  globalElecHash0 = malloc(sizeof(HashProc));
  globalElecHash1 = malloc(sizeof(HashProc));

  bAggrAlloc=false;
  bAlignAllocation=true;
  IndexPercentAlloc=4l;
  IndexDepthAgrAlloc=0l;
}
StructGlobalVariables::~StructGlobalVariables()
{
 if(globalSP != NULL) free(globalSP);
 if(globalHP != NULL) free(globalHP);
 if(globalHR8P != NULL) free(globalHR8P);
 if(global_vSP != NULL) free(global_vSP);
 if(global_vHP != NULL) free(global_vHP);
 if(globalElecHash0 != NULL) free(globalElecHash0);
 if(globalElecHash1 != NULL) free(globalElecHash1);
}

//---------------------------------------------------------------------------
void ListInheritFiles(UnicodeString path,TStrings* of)
{
// Эта процедура выводит список файлов и вызывает
// саму себя для всех каталогов
 TSearchRec sr;
 if (FindFirst(path+L"*.*", faAnyFile, sr) == 0)
 {
  do
  {
   if( IsMaskEqual(sr.Attr, 4) ) //sr.Attr & faDirectory)
   {
	if ((sr.Name!=".") && (sr.Name!=".."))
	 ListInheritFiles(path+sr.Name+L"\\",of);
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
 if (FindFirst(path+L"*.*", faAnyFile, sr) == 0)
 {
  do
  {
   if( IsMaskNotEqual(sr.Attr, 4) ) //!(sr.Attr & faDirectory))
	of->Add(path + sr.Name);
  }
  while (FindNext(sr) == 0);
  FindClose(sr);
 }
 Application->ProcessMessages();
}

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}

void __fastcall TForm1::FormCreate(TObject *Sender)
{
 if( !GetSystemMetrics(SM_MOUSEPRESENT) )
	MessageBox(WindowHandle, TEXT("Мышь не обнаруженна!!! :-О"), TEXT("ERROR"),MB_ICONERROR);

  srand( (unsigned)time(NULL) );

  TrayWarningTimer->Enabled=true;

  GlbVarApp.bMMX = IsMMX();
  GlbVarApp.bSSE = IsSSE();
  GlbVarApp.bCMOV = IsCMOV();
  GlbVarApp.bFCMOV = IsFCMOV();
  GlbVarApp.bMultiThread = IsMultiThreading();
 if( GlbVarApp.bMultiThread )
 {
  GlbVarApp.nThreads = GetThreadsCount();
  GlbVarApp.nThreads = GetClamp(GlbVarApp.nThreads, 1l,4l);
 }
 else GlbVarApp.nThreads = 1l;

 /*wchar_t tmpdir[256];
 unsigned long sizeb=256;
 GetTempPath(sizeb,tmpdir);
 OutEdit->Text = tmpdir;
 Global_TFB.SetFileDir(tmpdir);*/
 OutEdit->Text = L"C:\\";
 Global_TFB.SetFileDir(L"C:\\");

 //Global_MRP.TC = Chart1;
 TotalizeRND[0].Init_MMX(0);
 TotalizeRND[1].Init_MMX(0);
 TotalizeRND[2].Init_MMX(0);
 TotalizeRND[3].Init_MMX(0);
 TotalElec[0].Init_MMX(0);
 TotalElec[1].Init_MMX(0);

 //SetRoundFPU(ERC_CHOP);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::TrayWarningTimerTimer(TObject *Sender)
{
 TrayWarningTimer->Interval=5000;
 if(GlbVarApp.iTimeStack==0)
 {
  if(!GlbVarApp.bMMX)
  {
   TrayWarningMsg->BalloonTitle = L"Нет MMX";
   TrayWarningMsg->BalloonHint = L"Ваш процессор не поддерживает инструкции MMX!!!";
   TrayWarningMsg->Visible=true;
   TrayWarningMsg->ShowBalloonHint();
  }
  else TrayWarningTimer->Interval=500;
 }
 else if(GlbVarApp.iTimeStack==1)
 {
  if(!GlbVarApp.bSSE)
  {
   TrayWarningMsg->BalloonTitle = L"Нет SSE";
   TrayWarningMsg->BalloonHint = L"Ваш процессор не поддерживает инструкции SSE!!!";
   TrayWarningMsg->Visible=true;
   TrayWarningMsg->ShowBalloonHint();
  }
  else TrayWarningTimer->Interval=500;
 }
 else if(GlbVarApp.iTimeStack==2)
 {
  if(!GlbVarApp.bCMOV)
  {
   TrayWarningMsg->BalloonTitle = L"Нет Conditional move";
   TrayWarningMsg->BalloonHint = L"Ваш процессор не поддерживает инструкции CMOV!!!";
   TrayWarningMsg->Visible=true;
   TrayWarningMsg->ShowBalloonHint();
  }
  else TrayWarningTimer->Interval=500;
 }
 else if(GlbVarApp.iTimeStack==3)
 {
  if(!GlbVarApp.bFCMOV)
  {
   TrayWarningMsg->BalloonTitle = L"Нет Floating point/Conditional move";
   TrayWarningMsg->BalloonHint = L"Ваш процессор не поддерживает инструкции FCMOV!!!";
   TrayWarningMsg->Visible=true;
   TrayWarningMsg->ShowBalloonHint();
  }
  else TrayWarningTimer->Interval=500;
 }
 else if(GlbVarApp.iTimeStack==4)
 {
  if(!GlbVarApp.bMultiThread)
  {
   TrayWarningMsg->BalloonTitle = L"Нет многопоточности";
   TrayWarningMsg->BalloonHint = L"Ваш процессор не поддерживает многопоточность!!!";
   TrayWarningMsg->Visible=true;
   TrayWarningMsg->ShowBalloonHint();
  }
  else TrayWarningTimer->Interval=500;
 }
 else if(GlbVarApp.iTimeStack==5)
 {
  TrayWarningTimer->Enabled=false;
  TrayWarningMsg->Visible=false;
 }

 ++GlbVarApp.iTimeStack;
}
//---------------------------------------------------------------------------

//#include <float.h>


 //ShowMessage( FloatToStr( GetClamp(70.0L, 40.0L, 50.0L) ) );
 //char * xxx = new char[10];

 //long double x=-0.0L;
 //memset(&x,127,10);
 //*reinterpret_cast<__int64*>(&x) = 0xFFFFFFFFFFFFFFFFi64;
 //ShowMessage( _fpclassl(x)  ); //FloatToStr( GetNaN(x) ) );
 //delete [] xxx;
// TCanvas
 /*for(long i=0; i<PaintBox1->Width; ++i)
  for(long j=0; j<PaintBox1->Height; ++j)
	PaintBox1->Canvas->Pixels[i][j] = ((rand() & 255) << 16) | (((rand() & 255) << 8) | (rand() & 255));*/
 /*for(long i=0; i<PaintBox1->Width; ++i)
 {
  PaintBox1->Canvas->Pen->Color = ((rand() & 255) << 16) | (((rand() & 255) << 8) | (rand() & 255));

  PaintBox1->Canvas->MoveTo(0,0);
  //PaintBox1->Canvas->MoveTo(rand() % PaintBox1->Width,rand() % PaintBox1->Height);
  PaintBox1->Canvas->LineTo(rand() % PaintBox1->Width,rand() % PaintBox1->Height);
 }*/
//---------------------------------------------------------------------------

void __fastcall TForm1::FileFilterChecked(TObject *Sender)
{
 //checkbox checked
 TCheckBox* cb = dynamic_cast<TCheckBox*>(Sender);
if(cb->Checked)
 BitSet(GlbVarApp.FileFilter, cb->Tag);
else
 BitReset(GlbVarApp.FileFilter, cb->Tag);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn1Click(TObject *Sender)
{
 if(OpenDialog1->Execute(WindowHandle))
 {
	for(long i=0l; i<OpenDialog1->Files->Count; ++i)
	{
	 unsigned long mask = GetFileAttributes( OpenDialog1->Files[0][i].w_str() );
	 if( (IsMaskEqual(mask,2ul) || IsMaskEqual(mask,8ul)) ) continue;
	 if( !CheckBox3->Checked ) if(IsMaskEqual(mask,0ul)) continue; //Read
	 if( !CheckBox2->Checked ) if(IsMaskEqual(mask,1ul)) continue; //Hidden

	 ListBox1->AddItem(OpenDialog1->Files[0][i],NULL);
	}
	for(long i=1; i<ListBox1->Count; ++i)
	 if( ListBox1->Items[0][i-1] == ListBox1->Items[0][i] )
		 ListBox1->Items->Delete(i);
	ListBoxFS->Items = ListBox1->Items;

	register int temp = ListBoxFS->Count;

//--------------------------------//Destroy old objects
	for(long i=0l; i<arrRenderFiles.get_length(); ++i)
	 if(arrRenderFiles[i] != NULL) { arrRenderFiles[i]->FreeHash(); arrRenderFiles[i]->FreeHashR8(); arrRenderFiles[i]->FreeStat(); delete arrRenderFiles[i]; }
	arrRenderFiles.set_length(0);
//-----------------------------------------------------
	arrRenderFiles.set_length(temp);
	for(int cnt=0; cnt < temp; ++cnt)
	{
	  arrRenderFiles[cnt] = new FileProcessor();
	  arrRenderFiles[cnt]->SetWorkingFile(ListBox1->Items[0][cnt].w_str());
	}
 }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn2Click(TObject *Sender)
{
   ListBox1->DeleteSelected();
   ListBoxFS->Items = ListBox1->Items;

//--------------------------------------------------------------------
	for(long i=0l; i<arrRenderFiles.get_length(); ++i)
	{
	  if(arrRenderFiles[i] != NULL)
	  {
	   arrRenderFiles[i]->FreeHash();
       arrRenderFiles[i]->FreeHashR8();
	   arrRenderFiles[i]->FreeStat();
	   delete arrRenderFiles[i];
	  }
	}
	arrRenderFiles.set_length(0);
	ListBoxFSClick(NULL);
//--------------------------------------------------------------------
   if(ListBox1->GetCount() == 0) return;

   arrRenderFiles.set_length( ListBox1->GetCount() );
   for(long i=0; i<ListBox1->GetCount(); ++i)
   {
	  arrRenderFiles[i] = new FileProcessor();
	  arrRenderFiles[i]->SetWorkingFile(ListBox1->Items[0][i].w_str());
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn3Click(TObject *Sender)
{
 bi.hwndOwner = WindowHandle;

 LPCITEMIDLIST lpItemDList;
 lpItemDList=SHBrowseForFolder(&bi);
 if(!lpItemDList)
 {
  //MessageBox(WindowHandle, L"Сорри не удалось открыть окошко выбора папки... (Переустановите Винду)", L"Мессага об ошибке", MB_ICONERROR);
  return;
 }
// Достаем полный путь через
 wchar_t szPath[512];
 SHGetPathFromIDList(lpItemDList, szPath);

 int retval = MessageBox(WindowHandle, L"Включить файлы во вложенных папках?", L"?", MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2);
 if(retval == IDYES)
 {
//--------------------------------------------------------------------
	for(long i=0l; i<arrRenderFiles.get_length(); ++i)
	{
	  if(arrRenderFiles[i] != NULL)
	  {
	   arrRenderFiles[i]->FreeHash();
       arrRenderFiles[i]->FreeHashR8();
	   arrRenderFiles[i]->FreeStat();
	   delete arrRenderFiles[i];
	  }
	}
	arrRenderFiles.set_length(0);
	ListBoxFSClick(NULL);
//--------------------------------------------------------------------
  ListInheritFiles(UnicodeString(szPath) + L"\\",ListBox1->Items);
  ListBoxFS->Items = ListBox1->Items;
//--------------------------------------------------------------------
   arrRenderFiles.set_length(ListBox1->GetCount());
   for(long i=0; i<ListBox1->GetCount(); ++i)
   {
	  arrRenderFiles[i] = new FileProcessor();
	  arrRenderFiles[i]->SetWorkingFile(ListBox1->Items[0][i].w_str());
   }
//--------------------------------------------------------------------
 }
 else if(retval == IDNO)
 {
//--------------------------------------------------------------------
	for(long i=0l; i<arrRenderFiles.get_length(); ++i)
	{
	  if(arrRenderFiles[i] != NULL)
	  {
	   arrRenderFiles[i]->FreeHash();
	   arrRenderFiles[i]->FreeHashR8();
	   arrRenderFiles[i]->FreeStat();
	   delete arrRenderFiles[i];
      }
	}
	arrRenderFiles.set_length(0);
	ListBoxFSClick(NULL);
//--------------------------------------------------------------------
  ListLocalFiles(UnicodeString(szPath) + L"\\", ListBox1->Items);
  ListBoxFS->Items = ListBox1->Items;
//--------------------------------------------------------------------
   arrRenderFiles.set_length(ListBox1->GetCount());
   for(long i=0; i<ListBox1->GetCount(); ++i)
   {
	  arrRenderFiles[i] = new FileProcessor();
	  arrRenderFiles[i]->SetWorkingFile(ListBox1->Items[0][i].w_str());
   }
//--------------------------------------------------------------------
 }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn4Click(TObject *Sender)
{
 ListBox1->SelectAll();
 ListBox1->DeleteSelected();
 ListBoxFS->Items = ListBox1->Items;

//--------------------------------------------------------------------
	for(long i=0l; i<arrRenderFiles.get_length(); ++i)
	{
	 if(arrRenderFiles[i] != NULL)
	 {
	  arrRenderFiles[i]->FreeHash();
      arrRenderFiles[i]->FreeHashR8();
	  arrRenderFiles[i]->FreeStat();
	  delete arrRenderFiles[i];
     }
	}
	arrRenderFiles.set_length(0);
	ListBoxFSClick(NULL);
//--------------------------------------------------------------------
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SelectOutDir_Click(TObject *Sender)
{
 //
 bi.hwndOwner = WindowHandle;

 LPCITEMIDLIST lpItemDList;
 lpItemDList=SHBrowseForFolder(&bi);
 if(!lpItemDList)
 {
  //MessageBox(WindowHandle, L"Сорри не удалось открыть окошко выбора папки... (Переустановите Винду)", L"Мессага об ошибке", MB_ICONERROR);
  return;
 }
// Достаем полный путь через
 wchar_t szPath[512];
 SHGetPathFromIDList(lpItemDList, szPath);

 OutEdit->Text = szPath;
 Global_TFB.SetFileDir(szPath);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BT_FullStatClick(TObject *Sender)
{
 //
 if(GV_Form->Visible) GV_Form->Hide();
 else GV_Form->Show();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ListBox1Click(TObject *Sender)
{
 UnicodeString finfo;
 glbSelectFileDataRow = ListBox1->ItemIndex;

 unsigned long gfa = arrRenderFiles[glbSelectFileDataRow]->GetAttributes(); //GetFileAttributes( ListBox1->Items[0][ListBox1->ItemIndex].w_str() );

 finfo += L"Аттрибут: ";

 if( IsMaskEqual(gfa, 0ul) )	finfo += L"Только чтение, ";  //faReadOnly
 if( IsMaskEqual(gfa, 1ul) )	finfo += L"Скрытый, ";			//faHidden
 if( IsMaskEqual(gfa, 2ul) )	finfo += L"Системный, ";			//faSysFile
 if( IsMaskEqual(gfa, 3ul) )	finfo += L"VolumeID, ";			//faVolumeID
 if( IsMaskEqual(gfa, 4ul) )	finfo += L"Директория, ";			//faDirectory
 if( IsMaskEqual(gfa, 5ul) )	finfo += L"Архивный, ";			//faArchive
 if( IsMaskEqual(gfa, 6ul) )	finfo += L"SymLink, ";			//faSymLink
 if( IsMaskEqual(gfa, 7ul) )	finfo += L"Нормальный, ";			//faNormal
 if( IsMaskEqual(gfa, 8ul) )	finfo += L"Временный";			//faTemporary
 finfo += L'\n';

	FILETIME crtime, accestime, writetime;

	long shiftsize=0;
	__int64 fls = arrRenderFiles[glbSelectFileDataRow]->GetFileSize();

	arrRenderFiles[glbSelectFileDataRow]->GetFileTime(crtime,accestime,writetime);

	finfo += L"Создан: "; finfo += UIntToStr((qint)(((__int64)crtime.dwHighDateTime << 32ul) | crtime.dwLowDateTime));
	finfo += L"\nОткрыт: "; finfo += UIntToStr((qint)(((__int64)accestime.dwHighDateTime << 32ul) | accestime.dwLowDateTime));
	finfo += L"\nИзменён: "; finfo += UIntToStr((qint)(((__int64)writetime.dwHighDateTime << 32ul) | writetime.dwLowDateTime));

	finfo += '\n';

	while( (fls >> shiftsize) > 1024 )
	 shiftsize += 10;

	finfo += L"Размер: ";
	finfo += (fls >> shiftsize);
	switch(shiftsize)
	{
		case 0: finfo+= L" Байт"; break;
		case 10: finfo+= L"КБ"; break;
		case 20: finfo+= L"МБ"; break;
		case 30: finfo+= L"ГБ"; break;
		case 40: finfo+= L"ТБ"; break;
    }
	  finfo+=L'\n';
   //}

 FileData_BHint->Title= ListBox1->Items[0][ListBox1->ItemIndex];
 FileData_BHint->Description = finfo;
 FileData_BHint->ShowHint();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ListBox1MouseLeave(TObject *Sender)
{
  FileData_BHint->HideHint();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ListBoxFSClick(TObject *Sender)
{
 if(Sender!=NULL)
 {
  long ind = ListBoxFS->ItemIndex;

  if(arrRenderFiles.get_length() != NULL)
  {
	if(ind >= arrRenderFiles.get_length()) return;

	StatProc* aSP = arrRenderFiles[ind]->StatProperties;
	if(aSP != NULL)
	{
	 if( IsChange(LastStatRes[0],aSP->fs_zB) ) { Edit1->Text = UIntToStr( aSP->fs_zB ); LastStatRes[0]=aSP->fs_zB; }
	 if( IsChange(LastStatRes[1],aSP->fs_mB) ) { Edit2->Text = UIntToStr( aSP->fs_mB ); LastStatRes[1]=aSP->fs_mB; }
	 if( IsChange(LastStatRes[2],aSP->fs_aB) ) { Edit3->Text = UIntToStr( aSP->fs_aB ); LastStatRes[2]=aSP->fs_aB; }
	 if( IsChange(LastStatRes[3],aSP->fs_zW) ) { Edit4->Text = UIntToStr( aSP->fs_zW ); LastStatRes[3]=aSP->fs_zW; }
	 if( IsChange(LastStatRes[4],aSP->fs_mW) ) { Edit5->Text = UIntToStr( aSP->fs_mW ); LastStatRes[4]=aSP->fs_mW; }
	 if( IsChange(LastStatRes[5],aSP->fs_aW) ) { Edit6->Text = UIntToStr( aSP->fs_aW ); LastStatRes[5]=aSP->fs_aW; }
	 if( IsChange(LastStatRes[6],aSP->fs_zDW) ) { Edit7->Text = UIntToStr( aSP->fs_zDW ); LastStatRes[6]=aSP->fs_zDW; }
	 if( IsChange(LastStatRes[7],aSP->fs_mDW) ) { Edit8->Text = UIntToStr( aSP->fs_mDW ); LastStatRes[7]=aSP->fs_mDW; }
	 if( IsChange(LastStatRes[8],aSP->fs_aDW) ) { Edit9->Text = UIntToStr( aSP->fs_aDW );  LastStatRes[8]=aSP->fs_aDW; }
	 if( IsChange(LastStatRes[9],aSP->fs_zQW) ) { Edit10->Text = UIntToStr( aSP->fs_zQW );  LastStatRes[9]=aSP->fs_zQW; }
	 if( IsChange(LastStatRes[10],aSP->fs_mQW) ) { Edit11->Text = UIntToStr( aSP->fs_mQW ); LastStatRes[10]=aSP->fs_mQW; }
	 if( IsChange(LastStatRes[11],aSP->fs_aQW) ) { Edit12->Text = UIntToStr( aSP->fs_aQW ); LastStatRes[11]=aSP->fs_aQW; }

     if(IsMaskEqual(GlbVarApp.StatChecks, 31l))  GV_Form->UpdateGraph(aSP->FullStat, arrRenderFiles[ind]->GetFileName());
	}
	else
	{
	 Edit1->Text = L'0'; Edit2->Text = L'0'; Edit3->Text = L'0';
	 Edit4->Text = L'0'; Edit5->Text = L'0'; Edit6->Text = L'0';
	 Edit7->Text = L'0'; Edit8->Text = L'0'; Edit9->Text = L'0';
	 Edit10->Text = L'0'; Edit11->Text = L'0'; Edit12->Text = L'0';
     GV_Form->ClearGraph();
	}

	HashProc* aHP = arrRenderFiles[ind]->HashProperties;
	if(aHP != NULL)
	{
	 FormHS_STD->UpdateParamsSum(aHP->h_Sum32, aHP->h_Sum16, aHP->h_Sum8);
	 FormHS_STD->UpdateParamsDiff( aHP->h_Diff32, aHP->h_Diff16, aHP->h_Diff8);
	 FormHS_STD->UpdateParamsAvg( aHP->h_Avg32, aHP->h_Avg16, aHP->h_Avg8);
	 FormHS_STD->UpdateParamsDRIB( aHP->h_AsoDRIB, aHP->h_DRIB, aHP->h_Blenar);
	 FormHS_STD->UpdateParamsMIX( aHP->h_XOR, aHP->h_MiddleW, aHP->h_DiffByte);

	 FormHS_STD->UpdateTimes( aHP->t_s32, aHP->t_s16, aHP->t_s8,
						 aHP->t_d32, aHP->t_d16, aHP->t_d8,
						 aHP->t_a32, aHP->t_a16, aHP->t_a8,
						 aHP->t_AsoDRIB, aHP->t_DRIB, aHP->t_MiddleW,
						 aHP->t_XOR, aHP->t_Blenar, aHP->t_DiffByte);
	}
	else FormHS_STD->ClearParams();

    HashProc* aHR8P = arrRenderFiles[ind]->HashR8Properties;
    if(aHR8P != NULL)
    {
     FormHS_R8->UpdateParamsSum(aHR8P->h_Sum32, aHR8P->h_Sum16, aHR8P->h_Sum8);
	 FormHS_R8->UpdateParamsDiff( aHR8P->h_Diff32, aHR8P->h_Diff16, aHR8P->h_Diff8);
	 FormHS_R8->UpdateParamsAvg( aHR8P->h_Avg32, aHR8P->h_Avg16, aHR8P->h_Avg8);
	 FormHS_R8->UpdateParamsDRIB( aHR8P->h_AsoDRIB, aHR8P->h_DRIB, aHR8P->h_Blenar);
	 FormHS_R8->UpdateParamsMIX( aHR8P->h_XOR, aHR8P->h_MiddleW, aHR8P->h_DiffByte);

	 FormHS_R8->UpdateTimes( aHR8P->t_s32, aHR8P->t_s16, aHR8P->t_s8,
						 aHR8P->t_d32, aHR8P->t_d16, aHR8P->t_d8,
						 aHR8P->t_a32, aHR8P->t_a16, aHR8P->t_a8,
						 aHR8P->t_AsoDRIB, aHR8P->t_DRIB, aHR8P->t_MiddleW,
						 aHR8P->t_XOR, aHR8P->t_Blenar, aHR8P->t_DiffByte);
    }
    else FormHS_R8->ClearParams();
  }
 }
 else
 {
	 Edit1->Text = L'0'; Edit2->Text = L'0'; Edit3->Text = L'0';
	 Edit4->Text = L'0'; Edit5->Text = L'0'; Edit6->Text = L'0';
	 Edit7->Text = L'0'; Edit8->Text = L'0'; Edit9->Text = L'0';
	 Edit10->Text = L'0'; Edit11->Text = L'0'; Edit12->Text = L'0';
	 FormHS_STD->ClearParams();
     FormHS_R8->ClearParams();
 }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::StatEditClicks(TObject *Sender)
{
 //
 TEdit* med = dynamic_cast<TEdit*>(Sender);
 if(med == NULL) return;
 int nTag = GetClamp((long)med->Tag, 0l, 12l);
 wchar_t wbin[72];
 wchar_t whex[24];
 wchar_t woct[24];
 _i64tow(LastStatRes[ nTag ], wbin, 2);
 _i64tow(LastStatRes[ nTag ], woct, 8);
 _i64tow(LastStatRes[ nTag ], whex, 16);
 HelpStatusEditBalloon->Description = L"BIN: ";
 HelpStatusEditBalloon->Description += wbin;
 HelpStatusEditBalloon->Description += L"\nOCT: ";
 HelpStatusEditBalloon->Description += woct;
 HelpStatusEditBalloon->Description += L"\nHEX: ";
 HelpStatusEditBalloon->Description += whex;
 HelpStatusEditBalloon->ShowHint();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::StatEdit_MLeave(TObject *Sender)
{
 HelpStatusEditBalloon->HideHint();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ShowHashResButClick(TObject *Sender)
{
 TButton* myb = dynamic_cast<TButton*>(Sender);
 if( myb->Tag == 0 )
 {
  if( IsTrue( FormHS_STD->Visible ) ) FormHS_STD->Hide();
  else FormHS_STD->Show();
 }
 else if( myb->Tag == 1 )
 {
  if( IsTrue( FormHS_R8->Visible ) ) FormHS_R8->Hide();
  else FormHS_R8->Show();
 }
 else if( myb->Tag == 2 )
 {
    FormHS_RStep->ClearParams();
    E_X1->Text = IntToHex( (__int64)TotalElec[0].x[0], 16 );
    E_X2->Text = IntToHex( (__int64)TotalElec[0].x[1], 16 );
    E_X3->Text = IntToHex( (__int64)TotalElec[0].x[2], 16 );
    E_X4->Text = IntToHex( (__int64)TotalElec[0].x[3], 16 );
    E_X5->Text = IntToHex( (__int64)TotalElec[0].x[4], 16 );
    E_X6->Text = IntToHex( (__int64)TotalElec[0].x[5], 16 );
    E_X7->Text = IntToHex( (__int64)TotalElec[0].x[6], 16 );
    E_X8->Text = IntToHex( (__int64)TotalElec[0].x[7], 16 );

    HashProc* aHR = (HashProc*)GlbVarApp.globalElecHash0;
    if(aHR != NULL)
    {
     FormHS_RStep->UpdateParamsSum(aHR->h_Sum32, aHR->h_Sum16, aHR->h_Sum8);
	 FormHS_RStep->UpdateParamsDiff( aHR->h_Diff32, aHR->h_Diff16, aHR->h_Diff8);
	 FormHS_RStep->UpdateParamsAvg( aHR->h_Avg32, aHR->h_Avg16, aHR->h_Avg8);
	 FormHS_RStep->UpdateParamsDRIB( aHR->h_AsoDRIB, aHR->h_DRIB, aHR->h_Blenar);
	 FormHS_RStep->UpdateParamsMIX( aHR->h_XOR, aHR->h_MiddleW, aHR->h_DiffByte);

	 FormHS_RStep->UpdateTimes( aHR->t_s32, aHR->t_s16, aHR->t_s8,
						 aHR->t_d32, aHR->t_d16, aHR->t_d8,
						 aHR->t_a32, aHR->t_a16, aHR->t_a8,
						 aHR->t_AsoDRIB, aHR->t_DRIB, aHR->t_MiddleW,
						 aHR->t_XOR, aHR->t_Blenar, aHR->t_DiffByte);
    }

  if( IsTrue( FormHS_RStep->Visible ) ) FormHS_RStep->Hide();
  else FormHS_RStep->Show();
 }
 else if( myb->Tag == 4 )
 {
   FormHS_RStepMir->ClearParams();
    E_Y1->Text = IntToHex( (__int64)TotalElec[1].x[0], 16 );
    E_Y2->Text = IntToHex( (__int64)TotalElec[1].x[1], 16 );
    E_Y3->Text = IntToHex( (__int64)TotalElec[1].x[2], 16 );
    E_Y4->Text = IntToHex( (__int64)TotalElec[1].x[3], 16 );
    E_Y5->Text = IntToHex( (__int64)TotalElec[1].x[4], 16 );
    E_Y6->Text = IntToHex( (__int64)TotalElec[1].x[5], 16 );
    E_Y7->Text = IntToHex( (__int64)TotalElec[1].x[6], 16 );
    E_Y8->Text = IntToHex( (__int64)TotalElec[1].x[7], 16 );

    HashProc* aHR = (HashProc*)GlbVarApp.globalElecHash1;
    if(aHR != NULL)
    {
     FormHS_RStepMir->UpdateParamsSum(aHR->h_Sum32, aHR->h_Sum16, aHR->h_Sum8);
	 FormHS_RStepMir->UpdateParamsDiff( aHR->h_Diff32, aHR->h_Diff16, aHR->h_Diff8);
	 FormHS_RStepMir->UpdateParamsAvg( aHR->h_Avg32, aHR->h_Avg16, aHR->h_Avg8);
	 FormHS_RStepMir->UpdateParamsDRIB( aHR->h_AsoDRIB, aHR->h_DRIB, aHR->h_Blenar);
	 FormHS_RStepMir->UpdateParamsMIX( aHR->h_XOR, aHR->h_MiddleW, aHR->h_DiffByte);

	 FormHS_RStepMir->UpdateTimes( aHR->t_s32, aHR->t_s16, aHR->t_s8,
						 aHR->t_d32, aHR->t_d16, aHR->t_d8,
						 aHR->t_a32, aHR->t_a16, aHR->t_a8,
						 aHR->t_AsoDRIB, aHR->t_DRIB, aHR->t_MiddleW,
						 aHR->t_XOR, aHR->t_Blenar, aHR->t_DiffByte);
    }

  if( IsTrue( FormHS_RStepMir->Visible ) ) FormHS_RStepMir->Hide();
  else FormHS_RStepMir->Show();
 }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ScrollBarsChange(TObject *Sender)
{
 long double xdf = (long double)dynamic_cast<TScrollBar*>(Sender)->Position / (long double)dynamic_cast<TScrollBar*>(Sender)->Max;
 ScrollsShow->Description = FloatToStr(xdf);
 ScrollsShow->ShowHint();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ScrollBarFreqC(TObject *Sender)
{
 GlbVarApp.FreqBitFRS = ScrollBar7->Position;

 ScrollsShow->Description = IntToStr((int)GlbVarApp.FreqBitFRS);
 ScrollsShow->ShowHint();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ScrollBarsKeyUp(TObject *Sender, WORD &Key, TShiftState Shift)
{
 if( InLimit(Key, 48l, 57l) )
 {
  long double tableparam[11] = { 1.0L, 0.1L, 0.2L, 0.3L, 0.4L, 0.5L, 0.6L, 0.7L, 0.8L, 0.9L };
  long portial = Key - 48;

  dynamic_cast<TScrollBar*>(Sender)->Position = dynamic_cast<TScrollBar*>(Sender)->Max * tableparam[portial];
 }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::StatChecksClick(TObject *Sender)
{
 //ff
 TCheckBox* mysend = dynamic_cast<TCheckBox*>(Sender);
 if( mysend == NULL ) return;

 if(mysend->Checked) BitSet(GlbVarApp.StatChecks, (long)mysend->Tag);
 else BitReset(GlbVarApp.StatChecks, (long)mysend->Tag);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::HashChecksClick(TObject *Sender)
{
 TCheckBox* mysend = dynamic_cast<TCheckBox*>(Sender);
 if( mysend == NULL ) return;

 if(mysend->Checked) BitSet(GlbVarApp.HashChecks, (long)mysend->Tag);
 else BitReset(GlbVarApp.HashChecks, (long)mysend->Tag);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CalcChecksClick(TObject *Sender)
{
  TCheckBox* mysend = dynamic_cast<TCheckBox*>(Sender);
 if( mysend == NULL ) return;

 if(mysend->Checked) BitSet(GlbVarApp.CalcChecks, (long)mysend->Tag);
 else BitReset(GlbVarApp.CalcChecks, (long)mysend->Tag);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ComboBox_GetBitFRS(TObject *Sender)
{
 GlbVarApp.GetBitFRS=0l;
 Global_MRP.BitFRS=0l;
 BitSet(Global_MRP.BitFRS, ComboBox1->ItemIndex);
 BitSet(GlbVarApp.GetBitFRS, ComboBox1->ItemIndex);
}

void __fastcall TForm1::CB_WriteElementItChange(TObject *Sender)
{
 switch(CB_WriteElementIt->ItemIndex)
 {
   case 0: GlbVarApp.ElementType = 4i8; break;
   case 1: GlbVarApp.ElementType = 8i8; break;
   case 2: GlbVarApp.ElementType = 10i8; break;
 }
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void __fastcall TForm1::CheckBox22Click(TObject *Sender) { GlbVarApp.bFilesSUM = CheckBox22->Checked; }
void __fastcall TForm1::RenderSelectedClick(TObject *Sender)
{
 GlbVarApp.bRenderSelected = IsTrue(RadioButton1->Checked) && IsNot(RadioButton2->Checked);
 if(GlbVarApp.bRenderSelected) { for(long i=0l; i<arrRenderFiles.get_length(); ++i)
								 if(arrRenderFiles[i]!=NULL) arrRenderFiles[i]->Set_BitID_1(31l);  }
 else						   { for(long i=0l; i<arrRenderFiles.get_length(); ++i)
								 if(arrRenderFiles[i]!=NULL) arrRenderFiles[i]->Set_BitID_0(31l);  }
}

//---------------------------------------------------------------------------

void __fastcall TForm1::StartClick(TObject *Sender)
{
 AProgressDlg->ShowModal();
// CalcAnimButton->PopupMenu->Items
 if(GlbVarApp.bAutoRunVideoScan)
 {
  ProgressVid->ShowModal();
 }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ButtonExportClick(TObject *Sender)
{
	/*long double xxxx=0.0L;
	long double yyyy=0.0L;
    long double zz=1.0L;

    long double inc;
	//for(long i=0; i<10000000;++i)
    //for(inc=-1.0L; inc <= 1.0L; inc+=+0.01L)
//SetRoundFPU(ERC_CHOP);

    for(long tt=-127; tt <= 127; ++tt)
    {
     yyyy = (long double)tt / 127.0L;
     //if( !asm_requal_0(yyyy) )
	   xxxx += M_STEP_FUNCTION_27(yyyy);
    }

    ShowMessage( FloatToStr( xxxx ) + L" | " + FloatToStr( yyyy ) );
    xxxx=0.0L;

    for(long tt=-32767; tt <= 32767; ++tt)
    {
     yyyy = (long double)tt / 32767.0L;
     //if( !asm_requal_0(yyyy) )
	 xxxx += M_STEP_FUNCTION_27(yyyy);   //85   21845   5592406
    }

    ShowMessage( FloatToStr( xxxx ) + L" | " + FloatToStr( yyyy ) );
    xxxx=0.0L;

    //for(long tt=-16777215; tt <= 16777215; ++tt)
    for(long tt=-8388608; tt <= 8388608; ++tt)
    {
     yyyy = (long double)tt / 8388608.0L;
	 xxxx += M_STEP_FUNCTION_27(yyyy);
    }
    ShowMessage( FloatToStr( xxxx ) + L" | " + FloatToStr( yyyy ) );

return; */

 Global_Exp.SetFileDir( OutEdit->Text.w_str() );
 Global_Exp.CreateDataName();

 Global_Exp.s_Source->TotalFiles = arrRenderFiles.get_length();
 Global_Exp.s_Source->bFileFilter = GlbVarApp.FileFilter;

 Global_Exp.s_Source->TotalSize=0ui64;
 for(long i=0; i < arrRenderFiles.get_length(); ++i)
 {
  if(arrRenderFiles[i] != NULL)
   Global_Exp.s_Source->TotalSize += arrRenderFiles[i]->GetFileSize();
 }

 Global_Exp.s_Source->lSTime = GlbVarApp.lStartTime;
 Global_Exp.s_Source->lETime = GlbVarApp.lEndTime;
 Global_Exp.s_Source->lElapsTime = GlbVarApp.lElapsedTime;

 Global_Exp.s_Source->lvidSTime = GlbVarApp.lvidStartTime;
 Global_Exp.s_Source->lvidETime = GlbVarApp.lvidEndTime;
 Global_Exp.s_Source->lvidElapsTime = GlbVarApp.lvidElapsedTime;

 Global_Exp.s_Source->HashOpt = GlbVarApp.HashChecks;
 Global_Exp.s_Source->StatOpt = GlbVarApp.StatChecks;

 Global_Exp.s_Source->IsExis = IsMaskEqual(GlbVarApp.CalcChecks,2l);
 Global_Exp.s_Source->IsFilesSUM = GlbVarApp.bFilesSUM;
 Global_Exp.s_Source->ElecHash0 = GlbVarApp.globalElecHash0;
 Global_Exp.s_Source->ElecHash1 = GlbVarApp.globalElecHash1;

 Global_Exp.expCreateHeader();
 Global_Exp.expCreateBody();
 Global_Exp.expCreateFooter();
 Global_Exp.CloseFile();

 if( IsMaskEqual(GlbVarApp.StatChecks, 31l ) )
 {
  if( GlbVarApp.bFilesSUM )
  {
   UnicodeString S = L"GeneralGraph ";
   S += Date().FormatString(L"yyyy_MM_dd(");
   S += Time().FormatString(L"HH_mm_ss)");

   GV_Form->UpdateGraph( arrRenderFiles[0]->StatProperties->FullStat, S.w_str());
   GV_Form->Chart1->SaveToBitmapFile( OutEdit->Text + L"\\" + S + L".bmp");
   GV_Form->ClearGraph();
  }
  else
  {
   UnicodeString Sf = OutEdit->Text + L"\\statGraph\\";
   UnicodeString Tmp;
   CreateDirectory(Sf.w_str(),NULL);

   for(long i=0; i < arrRenderFiles.get_length(); ++i)
   {
    Tmp = Sf + MiniFilter_A0(arrRenderFiles[i]->GetFileName()) + L".bmp";
    GV_Form->UpdateGraph( arrRenderFiles[i]->StatProperties->FullStat, Tmp.w_str());
    GV_Form->Chart1->SaveToBitmapFile( Tmp );
   }
  }
 }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::GB_B01Click(TObject *Sender)
{
 if( GB_CB01->Checked )
	srand( (unsigned)time(NULL) );
 else
	srand( StrToIntDef(GB_E01->Text,0) );
 register unsigned long __eax = StrToIntDef(GB_E02->Text,0);
 if( __eax > 0 )
  for(--__eax; __eax>0; --__eax) rand();

 GB_LOut01->Caption = IntToStr( rand() );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::GB_B02Click(TObject *Sender)
{
 if( GB_CB02->Checked )
	set_seed( (unsigned)time(NULL) );
 else
	set_seed( (unsigned)StrToIntDef(GB_E03->Text,0) );
 register unsigned long __eax = StrToIntDef(GB_E04->Text,0);
 if( __eax > 0 )
  for(--__eax; __eax>0; --__eax) f_rand();

 GB_LOut02->Caption = FloatToStr( f_rand() );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::GB_B03Click(TObject *Sender)
{
 if( GB_CB03->Checked )
	TotalElec[0].Init_MMX( (unsigned)time(NULL) );
 else
    TotalElec[0].Init_MMX( (unsigned)StrToIntDef(GB_E05->Text,0) );
 register unsigned long __eax = StrToIntDef(GB_E06->Text,0);
 if( __eax > 0 )
  for(--__eax; __eax>0; --__eax) { TotalElec[0].Sort_Mix1(); TotalElec[0].Sort_TurnL(); }

 memcpy(&TotalElec[1].x, &TotalElec[0].x, 64);
 GB_LOut03->Caption = IntToHex( (__int64)TotalElec[0].x[0], 16 );
}
//---------------------------------------------------------------------------


void __fastcall TForm1::GB_B04Click(TObject *Sender)
{
 unsigned long tmp;
 if( GB_CB04->Checked ) TotalizeRND[0].Init_MMX( (unsigned)time(NULL) );
 else 	TotalizeRND[0].Init_MMX( (unsigned)StrToIntDef(GB_E07->Text,0) );

 register unsigned long __eax = StrToIntDef(GB_E08->Text,0);
 /*if( __eax > 0 )
  for(--__eax; __eax>0; --__eax) TotalizeRND.SortManual();*/

 if( __eax > 0 )
 {
  TotalizeRND[0].SortManual(__eax);
  memcpy(&TotalizeRND[1].m, &TotalizeRND[0].m, 64);
  memcpy(&TotalizeRND[2].m, &TotalizeRND[0].m, 64);
  memcpy(&TotalizeRND[3].m, &TotalizeRND[0].m, 64);
 }

 GB_LOut04->Caption = IntToHex( (__int64)TotalizeRND[0].x[0], 16 );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RAMMGRButtonClick(TObject *Sender)
{
 if( RAMMGR->ShowModal() == mrOk )
 {
  GlbVarApp.bAggrAlloc = RAMMGR->CheckBox1->Checked;
  GlbVarApp.bAlignAllocation = RAMMGR->CheckBox2->Checked;
  GlbVarApp.IndexPercentAlloc = RAMMGR->ComboBox1->ItemIndex;
  GlbVarApp.IndexDepthAgrAlloc = RAMMGR->ComboBox2->ItemIndex;
 }
 else
 {
  RAMMGR->CheckBox1->Checked = GlbVarApp.bAggrAlloc ;
  RAMMGR->CheckBox2->Checked = GlbVarApp.bAlignAllocation ;
  RAMMGR->ComboBox1->ItemIndex = GlbVarApp.IndexPercentAlloc;
  RAMMGR->ComboBox2->ItemIndex = GlbVarApp.IndexDepthAgrAlloc;
 }
 unsigned long MaxAvailPhys=0;
    GlobalMemoryStatus( &GlbVarApp.mem );

    switch( GlbVarApp.IndexPercentAlloc )
    {
       case 0l: MaxAvailPhys = (unsigned long)((long double)GlbVarApp.mem.dwAvailPhys * 0.05L); break;
       case 1l: MaxAvailPhys = (unsigned long)((long double)GlbVarApp.mem.dwAvailPhys * 0.1L); break;
       case 2l: MaxAvailPhys = (unsigned long)((long double)GlbVarApp.mem.dwAvailPhys * 0.2L); break;
       case 3l: MaxAvailPhys = (unsigned long)((long double)GlbVarApp.mem.dwAvailPhys * 0.25L); break;
       case 4l: MaxAvailPhys = (unsigned long)((long double)GlbVarApp.mem.dwAvailPhys * 0.5L); break;
       case 5l: MaxAvailPhys = (unsigned long)((long double)GlbVarApp.mem.dwAvailPhys * 0.75L); break;
       case 6l: MaxAvailPhys = (unsigned long)((long double)GlbVarApp.mem.dwAvailPhys * 0.95L); break;
       default: MaxAvailPhys = (unsigned long)((long double)GlbVarApp.mem.dwAvailPhys * 0.5L); break;
    }
  void* TestMem=(char*)malloc(MaxAvailPhys);
  if(TestMem != NULL)
  {
   free(TestMem);
  }
  else
  {
   ShowMessage(L"Задайте меньше процент выделения памяти!!!");
    RAMMGRButtonClick(Sender);
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BOSF_OptionsClick(TObject *Sender)
{
 //
 FormOSF->ShowModal();
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

void __fastcall TForm1::CalcVideoClick(TObject *Sender)
{
 ProgressVid->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CButAnim_PopMenuClk(TObject *Sender)
{
 //
 GlbVarApp.bAutoRunVideoScan = APM_N1->Checked;
}
//---------------------------------------------------------------------------

