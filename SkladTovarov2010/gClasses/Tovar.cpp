//Last update time : 03.06.2010 15:46:14
/***********************************************************************
 * Module:  Tovar.cpp
 * Author:  Admin
 * Modified: 3 июня 2010 г. 2:51:45
 * Purpose: Implementation of the class Tovar
 * Comment: Класс товар
 ***********************************************************************/

#include "Tovar.h"
#include <tchar.h>
////////////////////////////////////////////////////////////////////////
// Name:       Tovar::Tovar()
// Purpose:    Implementation of Tovar::Tovar()
// Return:     
////////////////////////////////////////////////////////////////////////

Tovar::Tovar()
{
   Name = new wchar_t[32];
   Coast = 0;
   Count = 0;
   _BadDays = 0;
}

////////////////////////////////////////////////////////////////////////
// Name:       Tovar::~Tovar()
// Purpose:    Implementation of Tovar::~Tovar()
// Return:     
////////////////////////////////////////////////////////////////////////

Tovar::~Tovar()
{
   // TODO : implement
   delete[] Name;
}

////////////////////////////////////////////////////////////////////////
// Name:       Tovar::GetBeBadDays()
// Purpose:    Implementation of Tovar::GetBeBadDays()
// Comment:    Функция возвращающая число дней за которое данный вид товара испортится.
// Return:     int
////////////////////////////////////////////////////////////////////////

int Tovar::GetBeBadDays(void)
{
   // TODO : implement
   return _BadDays;
}

////////////////////////////////////////////////////////////////////////
// Name:       Tovar::LostCoast(int days)
// Purpose:    Implementation of Tovar::LostCoast()
// Comment:    Функция возвращающая потерю стоимости в случае негодности товара.
// Parameters:
// - days
// Return:     int
////////////////////////////////////////////////////////////////////////

int Tovar::LostCoast(int days)
{
	if(days < _BadDays) return 0;
	if( _BadDays == 0 ) return 0;
	return Coast * Count; //(Coast * Count / d);
}

////////////////////////////////////////////////////////////////////////
// Name:       Tovar::Output()
// Purpose:    Implementation of Tovar::Output()
// Comment:    Функция выводит информацию об объекте.
// Return:     void
////////////////////////////////////////////////////////////////////////

void Tovar::Output(void) const
{
   // TODO : implement
}

////////////////////////////////////////////////////////////////////////
// Name:       Tovar::Serialize()
// Purpose:    Implementation of Tovar::Serialize()
// Comment:    Функция сериализирует данные объекта.
// Return:     void
////////////////////////////////////////////////////////////////////////

void Tovar::Serialize(void)
{
   // TODO : implement
}

////////////////////////////////////////////////////////////////////////
// Name:       Tovar::UnSerialize()
// Purpose:    Implementation of Tovar::UnSerialize()
// Comment:    Функция диселиаризирует данные объекта
// Return:     void
////////////////////////////////////////////////////////////////////////

void Tovar::UnSerialize(void)
{
   // TODO : implement
}

////////////////////////////////////////////////////////////////////////
// Name:       Tovar::getName()
// Purpose:    Implementation of Tovar::getName()
// Return:     wchar_t*
////////////////////////////////////////////////////////////////////////

wchar_t* Tovar::getName(void) const
{
   return Name;
}

////////////////////////////////////////////////////////////////////////
// Name:       Tovar::setName(wchar_t* newName)
// Purpose:    Implementation of Tovar::setName()
// Parameters:
// - newName
// Return:     void
////////////////////////////////////////////////////////////////////////

void Tovar::setName(wchar_t* newName)
{
 int mlen = wcslen(newName);
 mlen = (mlen > 32) ? 32 : mlen;
 memset(Name,0,32*sizeof(wchar_t));
 wcsncpy(Name,newName,mlen);   
}

////////////////////////////////////////////////////////////////////////
// Name:       Tovar::getCoast()
// Purpose:    Implementation of Tovar::getCoast()
// Return:     int
////////////////////////////////////////////////////////////////////////

int Tovar::getCoast(void) const
{
   return Coast;
}

////////////////////////////////////////////////////////////////////////
// Name:       Tovar::setCoast(int newCoast)
// Purpose:    Implementation of Tovar::setCoast()
// Parameters:
// - newCoast
// Return:     void
////////////////////////////////////////////////////////////////////////

void Tovar::setCoast(int newCoast)
{
   Coast = newCoast;
}

////////////////////////////////////////////////////////////////////////
// Name:       Tovar::getCount()
// Purpose:    Implementation of Tovar::getCount()
// Return:     int
////////////////////////////////////////////////////////////////////////

int Tovar::getCount(void) const
{
   return Count;
}

////////////////////////////////////////////////////////////////////////
// Name:       Tovar::setCount(int newCount)
// Purpose:    Implementation of Tovar::setCount()
// Parameters:
// - newCount
// Return:     void
////////////////////////////////////////////////////////////////////////

void Tovar::setCount(int newCount)
{
   Count = newCount;
}

////////////////////////////////////////////////////////////////////////
// Name:       Tovar::get_BadDays()
// Purpose:    Implementation of Tovar::get_BadDays()
// Return:     int
////////////////////////////////////////////////////////////////////////

int Tovar::get_BadDays(void) const
{
   return _BadDays;
}

////////////////////////////////////////////////////////////////////////
// Name:       Tovar::set_BadDays(int new_BadDays)
// Purpose:    Implementation of Tovar::set_BadDays()
// Parameters:
// - new_BadDays
// Return:     void
////////////////////////////////////////////////////////////////////////

void Tovar::set_BadDays(int new_BadDays)
{
   _BadDays = new_BadDays;
}

////////////////////////////////////////////////////////////////////////
// Name:       Tovar::getClassName()
// Purpose:    Implementation of Tovar::getClassName()
// Comment:    Функция возвращает имя класса.
// Return:     wchar_t*
////////////////////////////////////////////////////////////////////////

wchar_t* Tovar::getClassName(void) const
{
   return L"Tovar";
}