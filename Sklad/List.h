//Last update time : 22.03.2009 15:16:56
#ifndef ListH
#define ListH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <DB.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
#include <DBCtrls.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <Dialogs.hpp>
#include <ExtDlgs.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TfmList : public TForm
{
__published:	// IDE-managed Components
        TDataSource *DataSource1;
        TDBGrid *DBGrid1;
        TDBNavigator *DBNavigator1;
        TDataSource *dsTovari;
        TGroupBox *GroupBox1;
        TDBText *DBText1;
        TDBImage *DBImage1;
        TSplitter *Splitter1;
        TLabel *Label1;
        TDBMemo *DBMemo1;
        TSplitter *Splitter2;
        TToolBar *ToolBar1;
        TToolButton *ToolButton1;
        TToolButton *ToolButton2;
        TToolButton *ToolButton3;
        TToolButton *ToolButton4;
        TToolButton *ToolButton5;
        TToolButton *ToolButton6;
        TToolButton *ToolButton7;
        TToolButton *ToolButton8;
        TToolButton *ToolButton9;
        TToolButton *ToolButton10;
        TToolButton *ToolButton11;
        TToolBar *ToolBar2;
        TOpenDialog *OpenDialog1;
        TSaveDialog *SaveDialog1;
        TToolButton *ToolButtonCopyMemo;
        TToolButton *ToolButtonPasteMemo;
        TToolButton *ToolButtonOpenMemo;
        TToolButton *ToolButtonSaveMemo;
        TToolBar *ToolBar3;
        TToolButton *ToolButtonCopyImage;
        TToolButton *ToolButtonPasteImage;
        TToolButton *ToolButtonOpenImage;
        TOpenPictureDialog *OpenPictureDialog1;
        TSavePictureDialog *SavePictureDialog1;
        TToolButton *ToolButtonSaveImage;
        TToolBar *ToolBarFind;
        TCheckBox *CheckBoxFilter;
        TEdit *EditFilter;
        TBitBtn *btnFind;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall ToolButton1Click(TObject *Sender);
        void __fastcall ToolButton2Click(TObject *Sender);
        void __fastcall ToolButton3Click(TObject *Sender);
        void __fastcall ToolButton4Click(TObject *Sender);
        void __fastcall ToolButton6Click(TObject *Sender);
        void __fastcall ToolButton7Click(TObject *Sender);
        void __fastcall ToolButton8Click(TObject *Sender);
        void __fastcall ToolButton10Click(TObject *Sender);
        void __fastcall ToolButton11Click(TObject *Sender);
        void __fastcall ToolButtonCopyMemoClick(TObject *Sender);
        void __fastcall ToolButtonPasteMemoClick(TObject *Sender);
        void __fastcall ToolButtonOpenMemoClick(TObject *Sender);
        void __fastcall ToolButtonSaveMemoClick(TObject *Sender);
        void __fastcall ToolButtonCopyImageClick(TObject *Sender);
        void __fastcall ToolButtonPasteImageClick(TObject *Sender);
        void __fastcall ToolButtonOpenImageClick(TObject *Sender);
        void __fastcall ToolButtonSaveImageClick(TObject *Sender);
        void __fastcall CheckBoxFilterClick(TObject *Sender);
        void __fastcall btnFindClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TfmList(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfmList *fmList;
//---------------------------------------------------------------------------
#endif