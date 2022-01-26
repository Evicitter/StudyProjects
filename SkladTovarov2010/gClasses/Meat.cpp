//Last update time : 03.06.2010 15:41:08
/***********************************************************************
 * Module:  Meat.cpp
 * Author:  Admin
 * Modified: 3 июня 2010 г. 2:53:40
 * Purpose: Implementation of the class Meat
 * Comment: Товар категории "мясо"
 ***********************************************************************/

#include "Meat.h"

////////////////////////////////////////////////////////////////////////
// Name:       Meat::Meat()
// Purpose:    Implementation of Meat::Meat()
// Return:     
////////////////////////////////////////////////////////////////////////

Meat::Meat()
{
   isSuperMeat = false;
}

////////////////////////////////////////////////////////////////////////
// Name:       Meat::~Meat()
// Purpose:    Implementation of Meat::~Meat()
// Return:     
////////////////////////////////////////////////////////////////////////

Meat::~Meat()
{
   // TODO : implement
}

////////////////////////////////////////////////////////////////////////
// Name:       Meat::GetBeBadDays()
// Purpose:    Implementation of Meat::GetBeBadDays()
// Comment:    Функция возвращающая число дней за которое данный вид товара испортится.
// Return:     int
////////////////////////////////////////////////////////////////////////

int Meat::GetBeBadDays(void)
{
   // TODO : implement
   return 0;
}

////////////////////////////////////////////////////////////////////////
// Name:       Meat::Output()
// Purpose:    Implementation of Meat::Output()
// Comment:    Функция выводит информацию об объекте.
// Return:     void
////////////////////////////////////////////////////////////////////////

void Meat::Output(void) const
{
   // TODO : implement
}

////////////////////////////////////////////////////////////////////////
// Name:       Meat::Serialize()
// Purpose:    Implementation of Meat::Serialize()
// Comment:    Функция сериализирует данные объекта.
// Return:     void
////////////////////////////////////////////////////////////////////////

void Meat::Serialize(void)
{
   // TODO : implement
}

////////////////////////////////////////////////////////////////////////
// Name:       Meat::UnSerialize()
// Purpose:    Implementation of Meat::UnSerialize()
// Comment:    Функция диселиаризирует данные объекта
// Return:     void
////////////////////////////////////////////////////////////////////////

void Meat::UnSerialize(void)
{
   // TODO : implement
}

////////////////////////////////////////////////////////////////////////
// Name:       Meat::getClassName()
// Purpose:    Implementation of Meat::getClassName()
// Comment:    Функция возвращает имя класса.
// Return:     wchar_t*
////////////////////////////////////////////////////////////////////////

wchar_t* Meat::getClassName(void) const
{
   return L"Meat";
}