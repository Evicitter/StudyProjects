//Last update time : 14.10.2015 13:57:16

#pragma hdrstop

#include "structTable.h"
#include "MainForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

structTable GTable;

TComponent *CopyComponent(TComponent *Component)
{
    TComponent *Result = NULL;
    Application->CreateForm(Component->ClassType(), &Result); //Создаем новый объект по ClassType исходника
    TMemoryStream *Stream = new TMemoryStream; //Создаем временный поток
    UnicodeString Name = Component->Name; //Запоминаем имя исходника
    Component->Name = ""; //Очизаем имя исходника во избежании конфликта имен
    Stream->WriteComponent(Component); //Выгружаем все измененные published в поток
    Component->Name = Name; //Восстанавливаем имя
    Stream->Position = 0; //Возвращаемся в начало потока
    Stream->ReadComponent(Result); //Загружаем в новый объект данные из потока
    delete Stream; //Удаляем временный поток
    Typinfo::TPropList pList; //Выгружаем все события
	//int nPropCount = GetPropList((Typinfo::PTypeInfo)Component->ClassInfo(), Typinfo::TTypeKinds() << tkMethod, &pList);
	//for(int i = 0; i < nPropCount; i++) //Копируем все события в новый объект
	//    SetMethodProp(Result, AnsiString(pList[i]->Name), GetMethodProp(Component, AnsiString(pList[i]->Name)));
    return Result;
}

sCell::sCell()
{
 index=-1;
 type=-1;
 uCell=dCell=lCell=rCell=NULL;
 MyRow=NULL;
 mEdit=NULL;
 NotifyValueChange = voidNotify;
 Dig << '0' << L'1' << L'2' << L'3' << L'4' << L'5' << L'6' << L'7' << L'8' << L'9' << L'\b';
}

void sCell::CreateEdit(TEdit* me, bool ReadOnly, bool bHeader)
 {
   wchar_t nname[8]={L'\0'};
   if(me==NULL) return;

   if(bHeader)
    mEdit = me;
   else
	mEdit = (TEdit*)CopyComponent(me);

   //mEdit = new TEdit(me->Owner);
   if(mEdit==NULL) return; //assert

   swprintf(nname, L"medit%i%i\0", rand(), rand());
   mEdit->Name = nname;
   mEdit->Parent = me->Parent;
   mEdit->TabStop = false;
   mEdit->TabOrder = me->TabOrder;
   mEdit->ReadOnly = ReadOnly;
   mEdit->MaxLength = 16; //over!
   mEdit->Visible = true;

   if(bHeader)
   {
	mEdit->ReadOnly = true;
	mEdit->Text = me->Text;
	mEdit->Width = me->Width;
	mEdit->Height = me->Height;
	mEdit->Left = me->Left;
	mEdit->Top = me->Top;
   }
   else
   {
    mEdit->Color = clWindow;
	mEdit->ReadOnly = ReadOnly;
	ClearValue();
    CalcPosition(me);
   }

   //Добавление делегатов
   mEdit->OnKeyPress 	= this->gKeyPress;
   mEdit->OnKeyDown 	= MyRow->MyTable->gKeyDown;
   mEdit->OnEnter 		= this->gEnter;
   mEdit->OnExit 		= this->gExit;
 }

 void sCell::CalcPosition(TEdit* me)
 {
 	mEdit->Width = me->Width;
	mEdit->Height = me->Height;
	mEdit->Left = me->Left;
	mEdit->Top = me->Top + me->Height;

	if(MyRow!=NULL)
	   mEdit->Top += MyRow->Interval;
 }

void	 sCell::DelCell()
{
  //if(uCell!=NULL) uCell->dCell = dCell;
  //if(dCell!=NULL) dCell->uCell = uCell;
  //if(lCell!=NULL) lCell->rCell = rCell;
  //if(rCell!=NULL) rCell->lCell = lCell;
  Dig.Clear();
  //MyRow=NULL;
  if(mEdit!=NULL)
  {
   mEdit->Parent = NULL;
   mEdit->Free();
  }
  //FreeAndNil(mEdit);
  //mEdit=NULL;
}

void __fastcall sCell::gEnter(TObject *Sender)
{
 if(MyRow!=NULL && MyRow->MyTable!=NULL)
 {
	MyRow->MyTable->indexSelRow = MyRow->index;
	MyRow->MyTable->indexSelCell = index;
 }
}

void __fastcall sCell::gExit(TObject *Sender)
{
 if(MyRow!=NULL && MyRow->MyTable!=NULL)
 {
	MyRow->MyTable->indexSelRow = -1;
	MyRow->MyTable->indexSelCell = -1;
 }
}

void __fastcall sCell::gKeyPress(TObject *Sender, System::WideChar &Key)
{
  Key=0;
}

void __fastcall structTable::gKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
 //UnicodeString rr = IntToStr(Key);
 //MessageBoxW(NULL, rr.c_str(), L"vk", 0);
 TEdit* e = dynamic_cast<TEdit*>(Sender);
 if(e==NULL) return;

 switch(Key)
 {
	 case VK_ESCAPE: break;
	 case VK_ADD:
	 case VK_RETURN:
	  AddRow();
	 break;
	 case VK_SUBTRACT: DelRow(); break;
	 case VK_DIVIDE: MForm->DelRow->Enabled=true; break; //DelRowFrom(indexSelRow); break;
	 default: break;
 }
}

void __fastcall sIntegerCell::gKeyPress(TObject *Sender, System::WideChar &Key)
{
  if (!Dig.Contains(Key))
  {
   Key = 0;
  }
  else
  {
    //HideCaret(mEdit->Handle);
   /*if(Key==L'-')
   {
	   register int s = mEdit->SelStart;
	   value = StrToIntDef(mEdit->Text,0) * -1;
	   mEdit->Text = IntToStr(value);
	   mEdit->SelStart = s;
	   Key=0;
   }*/
  }
}

void __fastcall sFloatCell::gKeyPress(TObject *Sender, System::WideChar &Key)
{
  if (!Dig.Contains(Key))
  {
   Key = 0;
  }
  else
  {
   /*if(Key==L'-')
   {
	   register int s = mEdit->SelStart;
	   value = StrToFloatDef(mEdit->Text,0.0) * -1.0;
	   mEdit->Text = FloatToStrF(value, ffFixed, prec, digits);
	   mEdit->SelStart = s;
	   Key=0;
   }*/
   if(Key==L'.')
   {
	   register int s = mEdit->SelStart+1;
	   register int p = mEdit->Text.Pos(L'.');
	   if(p!=-1)
	   {
		mEdit->Text = mEdit->Text.Delete(p,1);
		mEdit->Text = mEdit->Text.Insert(L'.',s);
	   }
   	   value = StrToFloatDef(mEdit->Text,0.0);
	   mEdit->Text = FloatToStrF(value, ffFixed, prec, digits);
	   mEdit->SelStart = s-1;
       Key=0;
   }
  }
}