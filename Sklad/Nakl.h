//Last update time : 06.09.2009 13:06:08
#ifndef NaklH
#define NaklH
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
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
//---------------------------------------------------------------------------
class TfmNakl : public TForm
{
__published:	// IDE-managed Components
        TDataSource *dsNakl;
        TDataSource *dsNaklToTovar;
        TDBGrid *DBGridNakl;
        TGroupBox *GroupBox1;
        TDBNavigator *DBNavigatorNakl;
        TDBGrid *DBGrid1;
        TDBNavigator *DBNavigator1;
        TToolBar *ToolBar1;
        TBitBtn *BitAdd;
        TBitBtn *BitBtn1;
        TBitBtn *btnReportSale;
        void __fastcall btnReportSaleClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TfmNakl(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfmNakl *fmNakl;
//---------------------------------------------------------------------------
#endif