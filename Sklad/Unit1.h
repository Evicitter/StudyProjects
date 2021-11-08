//Last update time : 06.09.2009 21:05:48

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ImgList.hpp>
#include <Menus.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <ActnList.hpp>
#include <ADODB.hpp>
#include <DB.hpp>
#include <DBCtrls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfmMain : public TForm
{
__published:	// IDE-managed Components
        TImageList *ImageList1;
        TMainMenu *MainMenu1;
        TPopupMenu *PopupMenu1;
        TMenuItem *N5;
        TMenuItem *N6;
        TMenuItem *N7;
        TMenuItem *N8;
        TMenuItem *N9;
        TMenuItem *N10;
        TToolBar *ToolBar1;
        TActionList *ActionList1;
        TAction *actExit;
        TToolButton *ToolButton2;
        TAction *actInfo;
        TToolButton *ToolButton3;
        TToolButton *ToolButton4;
        TToolButton *ToolButton5;
        TToolButton *ToolButton6;
        TAction *actNakladnaya;
        TAction *actSklad;
        TAction *actTovaram;
        TMenuItem *N1;
        TMenuItem *N2;
        TMenuItem *N3;
        TMenuItem *N4;
        TMenuItem *N11;
        TMenuItem *N12;
        TMenuItem *N13;
        TMenuItem *N14;
        TMenuItem *N15;
        TMenuItem *N16;
        TMenuItem *N17;
        TMenuItem *N18;
        TStatusBar *StatusBar1;
        TAction *act_NaklTov;
        TMenuItem *N19;
        TToolButton *ToolButton1;
        TMenuItem *N20;
        TAction *actSQL;
        TToolButton *ToolButton7;
        void __fastcall N3Click(TObject *Sender);
        void __fastcall N6Click(TObject *Sender);
        void __fastcall ToolButton1Click(TObject *Sender);
        void __fastcall actExitExecute(TObject *Sender);
        void __fastcall actInfoExecute(TObject *Sender);
        void __fastcall actNakladnayaExecute(TObject *Sender);
        void __fastcall actSkladExecute(TObject *Sender);
        void __fastcall actTovaramExecute(TObject *Sender);
        void __fastcall act_NaklTovExecute(TObject *Sender);
        void __fastcall actSQLExecute(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TfmMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfmMain *fmMain;
//---------------------------------------------------------------------------
#endif