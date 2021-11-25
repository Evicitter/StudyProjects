//Last update time : 14.10.2015 15:04:42

#ifndef structTableH
#define structTableH
//---------------------------------------------------------------------------

#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>

#include <vector>
//#include <list>
//#include

typedef int(__fastcall *genEnter)(int, void*);
typedef int(__fastcall *genNotify)(int, void*);

int __fastcall voidNotify(int, void*)
{
    return 0xffffffff; //return result or mask
}

class sRow;
class structTable;

class sCell
{
 public:
	sCell();

 genNotify NotifyValueChange;

 int index;         			//Индекс ячейки
 int type;          			//Тип переменной 0-целое 1-вещественное
 int sizevalue;					//размер переменной
 unsigned char contvalue[8];	//Содержимое переменной

 sCell* uCell;
 sCell* dCell;
 sCell* lCell;
 sCell* rCell;
 sRow* MyRow;

 TEdit* mEdit;
 Set < System::WideChar, 0, 65535 > Dig;

 virtual bool 		   WriteValue(const UnicodeString w) = 0;
 virtual UnicodeString ReadValue () const 				 = 0;
 virtual bool		   ClearValue()						 = 0;
 virtual void		   DelCell();

 virtual void CreateEdit(TEdit* me, bool ReadOnly=false, bool bHeader=false);
 virtual void CalcPosition(TEdit* me);

 virtual void __fastcall gKeyPress(TObject *Sender, System::WideChar &Key);
 virtual void __fastcall gEnter(TObject *Sender);
 virtual void __fastcall gExit(TObject *Sender);
};

class sIntegerCell : public sCell
{
 public:
	sIntegerCell() : sCell() {type=0; value=0; sizevalue=4; }
 int value;

 virtual bool WriteValue(const UnicodeString w)
 {
  value = StrToInt(w);
  memcpy(contvalue,&value,sizevalue);
  if(mEdit!=NULL) mEdit->Text = IntToStr(value);
  return true;
 }
 virtual UnicodeString ReadValue() const { return IntToStr(value); }
 virtual bool		   ClearValue()	{ value=0; if(mEdit!=NULL) mEdit->Text = IntToStr(value); return true; }
 virtual void __fastcall gKeyPress(TObject *Sender, System::WideChar &Key);
};

class sFloatCell : public sCell
{
 public:
	sFloatCell() : sCell() {type=1; value=0.0; sizevalue=8; prec=10; digits=2; Dig << L'.';}
 double value;
 int prec;
 int digits;

 virtual bool WriteValue(const UnicodeString w)
 {
  value = (double)StrToFloat(w);
  memcpy(contvalue,&value,sizevalue);
  if(mEdit!=NULL) mEdit->Text = FloatToStrF(value, ffFixed, prec, digits);
  return true;
 }
 virtual UnicodeString ReadValue() const { return FloatToStrF(value,ffFixed, prec, digits); }
 virtual bool		   ClearValue()	{ value=0.0; if(mEdit!=NULL) mEdit->Text = FloatToStrF(value, ffFixed, prec, digits); return true; }
 virtual void __fastcall gKeyPress(TObject *Sender, System::WideChar &Key);
};

class sRow
{
 public:
	 sRow()
	 {
	  uRow=dRow=NULL;
	  Interval=2;
	  maxCells=0;
	  MyTable=NULL;
	  index=-1;
	 }
	 sRow* uRow;
	 sRow* dRow;
	 structTable* MyTable;
	 int Interval;
	 int maxCells;
	 int index;

	 std::vector<sCell*> Cells;

	 void SetIndex(int ind)
	 {
	  for(int i=0;i<Cells.size();++i) Cells[i]->index = i;
	  index = ind;
	 }

	 void AddCell(int type, TEdit* me, bool bHeader=false)
	 {
	  sCell* sc=NULL;

	  if     (type==0) sc = new sIntegerCell();
	  else if(type==1) sc = new sFloatCell();

	  if(sc==NULL) return;

	  sc->MyRow = this;
	  sc->CreateEdit(me,false,bHeader);
	  sc->index = Cells.size();

	  Cells.push_back( sc );
	 }

	 void DelRow()
	 {
	  for(int i=Cells.size()-1; i>=0; --i)
	  {
	   Cells[i]->ClearValue();
	   Cells[i]->DelCell();
	   delete Cells[i];
	   Cells[i] = NULL;
	  }
	  Cells.clear();
	  //if(uRow!=NULL) uRow->dRow = dRow;
	  //if(dRow!=NULL) dRow->uRow = uRow;
	 }
};

class structTable
{
public:
	structTable() { maxCells=0; indexSelCell=indexSelRow=-1; OnAddRow=OnDelRow=voidNotify; };

	sRow* headerRow;
	std::vector<sRow*> 		Rows;
	int maxCells;
	int indexSelRow;
	int indexSelCell;

	genNotify OnAddRow;
	genNotify OnDelRow;

	void FirstRow(TEdit* edits[], int* types, genNotify* notifyChange, int max)
	{
	 if(headerRow != NULL)
	 {
		headerRow->DelRow();
		delete headerRow;
		headerRow = NULL;
	 }
	 sRow* r = new sRow();
	 maxCells = max;
	 r->maxCells = max;
	 r->MyTable = this;
	 for(int i=0; i<max; ++i)
	 {
		 r->AddCell(types[i], edits[i], true);
		 r->Cells[i]->NotifyValueChange = notifyChange[i];
	 }
	 r->SetIndex(-1);
	 //r->Cells[0]->mEdit->Parent = NULL;
	 headerRow = r;
    }

	void AddRow()
	{
	 int res = 0xffffffff;
	 if(OnAddRow != NULL) res = OnAddRow(0,NULL);

	 if( res )
	 {
	  sRow* r = new sRow();
	  r->maxCells = maxCells;
	  r->MyTable = this;

	  int lind = Rows.size();

	  sRow* lastRow;
	  lastRow = (lind<=0) ? headerRow : Rows[Rows.size()-1];

	  for(int i=0; i<maxCells; ++i)
	  {
		   int t = lastRow->Cells[i]->type;
		   TEdit* tt = lastRow->Cells[i]->mEdit;
		   r->AddCell(t, tt, false);
	  }

	  r->SetIndex(lind);     						//Установка текущего индекса
	  --lind;               						//Предыдущий индекс

	  if((lind == indexSelRow) && (indexSelCell != -1))
	  {
	   //Установка значения в ячейке
	   r->Cells[indexSelCell]->NotifyValueChange(0,NULL);
	   r->Cells[indexSelCell]->mEdit->SetFocus();
	  }
	  Rows.push_back( r );

	  UpdateTable();
	 }
	}

	void DelRow()
	{
	 int res = 0xffffffff;
	 if(OnDelRow != NULL) res = OnDelRow(0,NULL);

	 if( res )
	 {
	  int lind = Rows.size()-1;
	  if(lind>=0)
	  {
		if((lind == indexSelRow) && (indexSelCell != -1))
		{
		 Rows[lind-1]->Cells[indexSelCell]->mEdit->SetFocus();
		}
		Rows[lind]->DelRow();
		delete Rows[lind];
		Rows[lind] = NULL;
		Rows.pop_back();
	  }
	 }
	}

	void DelRowFrom(int ind)
	{
	 int res = 0xffffffff;
	 if(OnDelRow != NULL) res = OnDelRow(0,NULL);

	 if( res )
	 {
	  if(ind>=0)
	  {
		Rows[ind]->DelRow();
		delete Rows[ind];
		Rows[ind] = NULL;

		//Удаление выделенной строки
		int delcnt=0;
		for(int i=0, of=0; i<Rows.size(); ++i, ++of)
		{
		 if(Rows[i] == NULL)
		 {
		  --of;
		  ++delcnt;
		  continue;
		 }
		 Rows[of]=Rows[i];
		}
		for(int i=0; i<delcnt; ++i) Rows.pop_back();
		//-------------------------------------------

		//for first row
		if(Rows.size()>=1)
		{
		 Rows[0]->SetIndex(0);
		 for(int j=0; j<Rows[0]->Cells.size(); ++j)
		  Rows[0]->Cells[j]->CalcPosition(headerRow->Cells[j]->mEdit);
		}

		//for other rows
        for(int i=1; i<Rows.size(); ++i)
		{
			Rows[i]->SetIndex(i);
			for(int j=0; j<Rows[i]->Cells.size(); ++j)
			 Rows[i]->Cells[j]->CalcPosition(Rows[i-1]->Cells[j]->mEdit);
		}

		//Выбор ячейки
		Rows[ind]->Cells[indexSelCell]->mEdit->SetFocus();

		UpdateTable();
      }
     }
    }

	void LoadTable(wchar_t* fromFile)
	{
	 while(Rows.size()>0) DelRow();
	 FILE* hf;
	 if(_wfopen_s(&hf,fromFile,L"rb") == 0)
	 {
		 //fread()
     }
	}
	void SaveTable(wchar_t* fromFile)
	{
	 while(Rows.size()>0) DelRow();
	 FILE* hf;
	 if(_wfopen_s(&hf,fromFile,L"wb") == 0)
	 {
		 //fread()
     }
    }

	void UpdateTable()
	{
	 for(int i=0; i<Rows.size(); ++i)
	 {
		register int last = i-1;
		last = (last<0) ? 0 : last;
		Extended d;

		d =  StrToFloatDef(Rows[i   ]->Cells[1]->ReadValue(), 0.0);
		d -= StrToFloatDef(Rows[last]->Cells[1]->ReadValue(), 0.0);
		sFloatCell* sfc = dynamic_cast<sFloatCell*>(Rows[i]->Cells[2]);
		if(sfc!=NULL)
		{
		 Rows[i]->Cells[2]->WriteValue(FloatToStrF(d,ffFixed,sfc->prec,sfc->digits));
        }
	 }
	}

	virtual void __fastcall gKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
};


extern structTable GTable;

#endif