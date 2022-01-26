//Last update time : 03.06.2010 15:47:56
/***********************************************************************
 * Module:  Sklad.cpp
 * Author:  Admin
 * Modified: 3 июня 2010 г. 0:54:46
 * Purpose: Implementation of the class Sklad
 * Comment: Класс склада содержащего товары.
 ***********************************************************************/

#include "Tovar.h"

#include "Fruit.h"
#include "Meat.h"
#include "Concerv.h"

#include "Sklad.h"

//---------------------------------------------------------------------------
#include <Dialogs.hpp>
#include "MForm.h"
//---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////
// Name:       Sklad::Sklad()
// Purpose:    Implementation of Sklad::Sklad()
// Return:     
////////////////////////////////////////////////////////////////////////

Sklad::Sklad()
{
   tovar = (void*)0;
}

////////////////////////////////////////////////////////////////////////
// Name:       Sklad::~Sklad()
// Purpose:    Implementation of Sklad::~Sklad()
// Return:     
////////////////////////////////////////////////////////////////////////

Sklad::~Sklad()
{
   if(tovar != (void*)0)
   {
	for(int i=0; i<CountVals; ++i)
	{
		if( tovar[i] != (void*)0 ) delete tovar[i];
	}
	delete[] tovar;
   }
}

////////////////////////////////////////////////////////////////////////
// Name:       Sklad::SumBadDays(int _days)
// Purpose:    Implementation of Sklad::SumBadDays()
// Comment:    Функция осущствляющая подсчёт срока годности всех товаров.
// Parameters:
// - _days
// Return:     long
////////////////////////////////////////////////////////////////////////

long Sklad::SumBadDays(int _days)
{
	int sumlost=0;
   if(tovar != (void*)0)
   {
	for(int i=0; i<CountVals; ++i)
	{
		if( tovar[i] != (void*)0 )
		{
		 sumlost += tovar[i]->LostCoast( _days );
		}
	}
   }
   return sumlost;
}

////////////////////////////////////////////////////////////////////////
// Name:       Sklad::Load()
// Purpose:    Implementation of Sklad::Load()
// Comment:    Функция загружающая список товаров на склад из файла.
// Return:     bool
////////////////////////////////////////////////////////////////////////

bool Sklad::Load(void)
{
// TODO : implement
	int countV=0;
	int tVals=0;

   if(tovar != (void*)0)
   {
	for(int i=0; i<CountVals; ++i)
	{
		if( tovar[i] != (void*)0 ) delete tovar[i];
	}
	delete[] tovar;
   }

	GMForm->QT->Open();
	GMForm->QT->First();
	while(!GMForm->QT->Eof)
	{
		++countV;
		GMForm->QT->Next();
	}

	CountVals=countV;

	tovar = new Tovar*[countV];
	GMForm->QT->First();
	while(!GMForm->QT->Eof)
	{
		if( GMForm->QT->Fields[0][1]->AsString == UnicodeString(L"Fruit") )
			tovar[tVals] = new Fruit();
		else if( GMForm->QT->Fields[0][1]->AsString == UnicodeString(L"Meat") )
			tovar[tVals] = new Meat();
		else if( GMForm->QT->Fields[0][1]->AsString == UnicodeString(L"Concerv") )
			tovar[tVals] = new Concerv();

		if( tovar[tVals]!=(void*)0 )
		{
			tovar[tVals]->setName( GMForm->QT->Fields[0][2]->AsString.c_str() ); //set name
			tovar[tVals]->setCoast( GMForm->QT->Fields[0][3]->AsInteger ); //set coast
			tovar[tVals]->setCount( GMForm->QT->Fields[0][4]->AsInteger );//set count
			tovar[tVals]->set_BadDays( GMForm->QT->Fields[0][5]->AsInteger );//set days
		}

		GMForm->QT->Next();
		++tVals;
	}
	//GMForm->QT->Close();

	return true;
}

////////////////////////////////////////////////////////////////////////
// Name:       Sklad::Save()
// Purpose:    Implementation of Sklad::Save()
// Comment:    Функция сохраняющая список товаров из склад в файл.
// Return:     bool
////////////////////////////////////////////////////////////////////////

bool Sklad::Save(void)
{
   // TODO : implement
   Tovar* tmpt;
   for(int i=0; i<CountVals; ++i)
   {
	tmpt = GetTovar(i);
	if( tmpt != (void*)0 )
	{
	 GMForm->ADOUpd->SQL->Clear();
	 GMForm->ADOUpd->SQL->Append(L"UPDATE tovar SET");
	 GMForm->ADOUpd->SQL->Append(L" tovar.Coast = " + UnicodeString(tmpt->getCoast()) );
	 GMForm->ADOUpd->SQL->Append(L", tovar.Count = " + UnicodeString(tmpt->getCount()) );
	 GMForm->ADOUpd->SQL->Append(L", tovar.BadDays = " + UnicodeString(tmpt->get_BadDays()) );
	 GMForm->ADOUpd->SQL->Append(L" WHERE tovar.Name = \'" + UnicodeString(tmpt->getName()) + L"\';");
	 GMForm->ADOUpd->ExecSQL();
	}
   }
	GMForm->QT->Close();
	GMForm->QT->Open();
   return false;
}

////////////////////////////////////////////////////////////////////////
// Name:       Sklad::getCountVals()
// Purpose:    Implementation of Sklad::getCountVals()
// Comment:    Возвращение числа товаров
// Return:     int
////////////////////////////////////////////////////////////////////////

int Sklad::getCountVals(void)
{
   return CountVals;
}

////////////////////////////////////////////////////////////////////////
// Name:       Sklad::GetTovar(int ind)
// Purpose:    Implementation of Sklad::GetTovar()
// Comment:    Возвращает указатель на товар по индексу.
// Parameters:
// - ind
// Return:     Tovar*
////////////////////////////////////////////////////////////////////////

Tovar* Sklad::GetTovar(int ind)
{
 if((ind < 0) || (ind >= CountVals)) return (void*)0;
 if(tovar == (void*)0) return (void*)0;
 return tovar[ind];
}

////////////////////////////////////////////////////////////////////////
// Name:       Sklad::AddTovar(Tovar* tov)
// Purpose:    Implementation of Sklad::AddTovar()
// Comment:    Добавляет товар
// Parameters:
// - tov
// Return:     void
////////////////////////////////////////////////////////////////////////

void Sklad::AddTovar(Tovar* tov)
{
   // TODO : implement
   if( tov == (void*)0 ) return;

   GMForm->ADOUpd->SQL->Clear();
   GMForm->ADOUpd->SQL->Append(L"INSERT INTO tovar (t_Class,Name,Coast) VALUES (");
   GMForm->ADOUpd->SQL->Append(L" \'" + UnicodeString(tov->getClassName()) + L"\' ,");
   GMForm->ADOUpd->SQL->Append(L" \'" + UnicodeString(tov->getName()) + L"\' ,");
   GMForm->ADOUpd->SQL->Append(UnicodeString(tov->getCoast()) + L" )");
   GMForm->ADOUpd->ExecSQL();

	 GMForm->ADOUpd->SQL->Clear();
	 GMForm->ADOUpd->SQL->Append(L"UPDATE tovar SET");
	 GMForm->ADOUpd->SQL->Append(L" tovar.Count = " + UnicodeString(tov->getCount()) );
	 GMForm->ADOUpd->SQL->Append(L", tovar.BadDays = " + UnicodeString(tov->get_BadDays()) );
	 GMForm->ADOUpd->SQL->Append(L", tovar.ReservedParam1 = " + UnicodeString(0) );
	 GMForm->ADOUpd->SQL->Append(L", tovar.ReservedParam2 = " + UnicodeString(0) );
	 GMForm->ADOUpd->SQL->Append(L" WHERE tovar.Name = \'" + UnicodeString(tov->getName()) + L"\';");
	 GMForm->ADOUpd->ExecSQL();

   GMForm->QT->Close();
   GMForm->QT->Open();

   Load();
}

////////////////////////////////////////////////////////////////////////
// Name:       Sklad::DelTovar(int ind)
// Purpose:    Implementation of Sklad::DelTovar()
// Comment:    Удаление выбранного товара
// Parameters:
// - ind
// Return:     void
////////////////////////////////////////////////////////////////////////

void Sklad::DelTovar(int ind)
{
   // TODO : implement
   Tovar* t=GetTovar(ind);
   if( t == (void*)0 ) return;

   GMForm->ADOUpd->SQL->Clear();
   GMForm->ADOUpd->SQL->Append(L"DELETE FROM tovar WHERE tovar.Name = \'" + UnicodeString(t->getName()) + L"\';");
   GMForm->ADOUpd->ExecSQL();
   GMForm->QT->Close();
   GMForm->QT->Open();

   delete t;
   tovar[ind]=(void*)0;

   --CountVals;
   if(CountVals<0) return;

   Tovar** tmp=new Tovar*[CountVals];
   for(int i=0, j=0; i<CountVals+1; ++i)
   {
	if(tovar[i] != (void*)0) { tmp[j]=tovar[i]; ++j; }
   }
   delete[] tovar;
   tovar = tmp;
}