//Last update time : 06.09.2009 12:30:16
#ifndef ReportH
#define ReportH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//#include <QuickRpt.hpp>
//#include <QRCtrls.hpp>
//---------------------------------------------------------------------------
class TfmReport : public TForm
{
__published:	// IDE-managed Components
        /*TQuickRep *QuickRepSale;
        TQRBand *QRBand1;
        TQRLabel *QRLabel2;
        TQRBand *QRBand2;
        TQRBand *QRBand3;
        TQRSubDetail *QRSubDetail1;
        TQRDBText *QRDBText1;
        TQRDBText *QRDBText2;
        TQRDBText *QRDBText4;
        TQRDBText *QRDBText5;
        TQRDBText *QRDBText6;
        TQRLabel *QRLabel1;
        TQRLabel *QRLabel3;
        TQRDBText *QRDBText3;*/
private:	// User declarations
public:		// User declarations
        __fastcall TfmReport(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfmReport *fmReport;
//---------------------------------------------------------------------------
#endif