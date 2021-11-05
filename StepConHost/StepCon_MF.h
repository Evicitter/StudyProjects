//Last update time : 11.05.2010 03:16:10

#ifndef StepCon_MFH
#define StepCon_MFH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ActnMan.hpp>
#include <ImgList.hpp>
#include <Ribbon.hpp>
#include <RibbonLunaStyleActnCtrls.hpp>
#include <ActnCtrls.hpp>
#include <ToolWin.hpp>
#include <ActnMenus.hpp>
#include <RibbonActnMenus.hpp>
#include <StdActns.hpp>
#include <Dialogs.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <AppEvnts.hpp>
#include <Menus.hpp>
#include "FRM_FieldSpaces.h"
//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
	TImageList *ImageList1;
	TActionManager *ActionManager1;
	TRibbon *Ribbon1;
	TRibbonPage *RibbonPage1;
	TRibbonGroup *RibbonGroup1;
	TRibbonApplicationMenuBar *RibbonApplicationMenuBar1;
	TAction *OpenIT;
	TAction *OpenFile;
	TAction *OpenFolder;
	TAction *OpenProject;
	TAction *CloseSelect;
	TAction *CloseAll;
	TAction *CloseIT;
	TImageList *ImageList2;
	TAction *SaveResults;
	TAction *SaveIT;
	TAction *SaveProject;
	TRibbonGroup *RibbonGroup2;
	TAction *SetExpDir;
	TAction *SetConfig;
	TAction *ExitApp;
	TRibbonPage *RibbonPage2;
	TRibbonGroup *RibbonGroup3;
	TAction *ViewLog;
	TAction *ShowTime;
	TRibbonGroup *RibbonGroup4;
	TAction *GB_Action;
	TAction *GB_Groups;
	TAction *GB_Waves;
	TAction *GB_Materials;
	TAction *GB_Space;
	TRibbonGroup *RibbonGroup5;
	TAction *GB_ED_ActProp;
	TAction *GB_ED_SpaceProp;
	TRibbonPage *RibbonPage3;
	TRibbonGroup *RibbonGroup6;
	TAction *Build_S1;
	TAction *Build_S2;
	TAction *Build_S3;
	TAction *Build_S4;
	TAction *Build_S5;
	TRibbonGroup *RibbonGroup7;
	TRibbonGroup *RibbonGroup8;
	TOpenDialog *OpenDialog_Files;
	TOpenDialog *OpenDialog_Project;
	TToolBar *ToolBar1;
	TToolBar *ToolBar2;
	TPageControl *PageControl1;
	TTabSheet *TabSheet1;
	TListBox *ListBoxFiles;
	TToolBar *ToolBar3;
	TImageList *IL_FileToolBar;
	TToolButton *ToolButton1;
	TToolButton *ToolButton2;
	TToolButton *ToolButton3;
	TImageList *IL_Left1ToolBars;
	TImageList *IL_Left2ToolBars;
	TToolButton *ToolButton4;
	TToolButton *ToolButton5;
	TToolButton *ToolButton6;
	TToolButton *ToolButton7;
	TToolButton *ToolButton8;
	TToolButton *ToolButton9;
	TToolButton *ToolButton10;
	TToolButton *ToolButton11;
	TStatusBar *StatusBarFile;
	TSaveDialog *SaveDialog_Project;
	TTabSheet *TabSheet2;
	TToolBar *ToolBar4;
	TApplicationEvents *AppEvents;
	TfrmFieldSpaces *frmFieldSpaces1;
	TToolButton *ToolButton12;
	TToolButton *ToolButton13;
	void __fastcall OpenITExecute(TObject *Sender);
	void __fastcall CloseITExecute(TObject *Sender);
	void __fastcall SaveITExecute(TObject *Sender);
	void __fastcall ExitAppExecute(TObject *Sender);
	void __fastcall ShowLog_Time(TObject *Sender);
	void __fastcall FileToolBar_Click(TObject *Sender);
	void __fastcall LeftToolBars_Click(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall GBrw_Exec(TObject *Sender);
	void __fastcall ListBoxFilesDrawItem(TWinControl *Control, int Index, TRect &Rect,
		  TOwnerDrawState State);
	void __fastcall AppEventsMessage(tagMSG &Msg, bool &Handled);
	void __fastcall Build_Exec(TObject *Sender);
private:	// User declarations
public:		// User declarations
	bool __fastcall NextLaunch();
	__fastcall TMainForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------

extern int IteratorLApp;

#endif
