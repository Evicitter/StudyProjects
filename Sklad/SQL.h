//Last update time : 22.03.2009 19:13:28
#ifndef SQLH
#define SQLH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ADODB.hpp>
#include <DB.hpp>
#include <ExtCtrls.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
#include <Chart.hpp>
#include <DbChart.hpp>
#include <TeEngine.hpp>
#include <TeeProcs.hpp>
#include <Series.hpp>
//---------------------------------------------------------------------------
class TfmSQL : public TForm
{
__published:	// IDE-managed Components
        TPageControl *PageControl1;
        TTabSheet *TabSheetPrimers;
        TADOQuery *ADOQueryPrimers;
        TDataSource *dsPrimers;
        TRadioGroup *RadioGroupPrimers;
        TDBGrid *DBGridPrimers;
        TTabSheet *TabSheetParams;
        TGroupBox *GroupBox1;
        TLabel *Label1;
        TDateTimePicker *DateTimePickerDate1;
        TLabel *Label2;
        TDateTimePicker *DateTimePickerDate2;
        TButton *ButtonResult;
        TADOQuery *ADOQueryResult;
        TDBGrid *DBGrid1;
        TDataSource *DataSource1;
        TWideStringField *ADOQueryResult_;
        TIntegerField *ADOQueryResult___;
        TIntegerField *ADOQueryResultDSDesigner;
        TDateTimeField *ADOQueryResultDSDesigner2;
        TTabSheet *TabSheet1;
        TDBGrid *DBGrid2;
        TDataSource *dsKontrol;
        TADOQuery *ADOQueryKontrol;
        TButton *Button1;
        TEdit *Edit1;
        TWideStringField *ADOQueryKontrolDSDesigner;
        TWideStringField *ADOQueryKontrolDSDesigner2;
        TIntegerField *ADOQueryKontrol_;
        TIntegerField *ADOQueryKontrol_2;
        TFloatField *ADOQueryKontrolDSDesigner3;
        TTabSheet *TabSheetModify;
        TADOQuery *ADOQueryModify;
        TDBGrid *DBGridModify;
        TLabel *LabelName;
        TEdit *EditName;
        TLabel *LabelType;
        TEdit *EditType;
        TButton *ButtonInsert;
        TButton *ButtonUpdate;
        TButton *ButtonDelete1;
        TButton *ButtonDelete2;
        TLabel *LabelNum;
        TEdit *EditNum;
        TTabSheet *TabSheetDiagram;
        TADOQuery *ADOQueryDiagram;
        TDBChart *DBChartDiagram;
        TBarSeries *Series1;
        TPieSeries *Series2;
        TRadioGroup *RadioGroupDiagram;
        TFloatField *ADOQueryDiagramDSDesigner;
        TStringField *ADOQueryDiagramField;
        TIntegerField *ADOQueryDiagram_;
        void __fastcall RadioGroupPrimersClick(TObject *Sender);
        void __fastcall ButtonResultClick(TObject *Sender);
        void __fastcall Edit1Change(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall ButtonInsertClick(TObject *Sender);
        void __fastcall ButtonUpdateClick(TObject *Sender);
        void __fastcall ButtonDelete1Click(TObject *Sender);
        void __fastcall ButtonDelete2Click(TObject *Sender);
        void __fastcall EditNumChange(TObject *Sender);
        void __fastcall RadioGroupDiagramClick(TObject *Sender);
        void __fastcall TabSheetDiagramShow(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall ADOQueryDiagramCalcFields(TDataSet *DataSet);
private:	// User declarations
public:		// User declarations
        __fastcall TfmSQL(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfmSQL *fmSQL;
//---------------------------------------------------------------------------
#endif