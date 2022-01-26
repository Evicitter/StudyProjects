//Last update time : 03.06.2010 15:41:02
/***********************************************************************
 * Module:  Fruit.cpp
 * Author:  Admin
 * Modified: 3 июня 2010 г. 2:52:46
 * Purpose: Implementation of the class Fruit
 * Comment: Товара катеории "фрукт"
 ***********************************************************************/

#include "Fruit.h"

////////////////////////////////////////////////////////////////////////
// Name:       Fruit::Fruit()
// Purpose:    Implementation of Fruit::Fruit()
// Return:     
////////////////////////////////////////////////////////////////////////

Fruit::Fruit()
{
   isOvosh = false;
}

////////////////////////////////////////////////////////////////////////
// Name:       Fruit::~Fruit()
// Purpose:    Implementation of Fruit::~Fruit()
// Return:     
////////////////////////////////////////////////////////////////////////

Fruit::~Fruit()
{
   // TODO : implement
}

////////////////////////////////////////////////////////////////////////
// Name:       Fruit::Output()
// Purpose:    Implementation of Fruit::Output()
// Comment:    Функция выводит информацию об объекте.
// Return:     void
////////////////////////////////////////////////////////////////////////

void Fruit::Output(void) const
{
   // TODO : implement
}

////////////////////////////////////////////////////////////////////////
// Name:       Fruit::GetBeBadDays()
// Purpose:    Implementation of Fruit::GetBeBadDays()
// Comment:    Функция возвращающая число дней за которое данный вид товара испортится.
// Return:     int
////////////////////////////////////////////////////////////////////////

int Fruit::GetBeBadDays(void)
{
   // TODO : implement
   return 0;
}

////////////////////////////////////////////////////////////////////////
// Name:       Fruit::Serialize()
// Purpose:    Implementation of Fruit::Serialize()
// Comment:    Функция сериализирует данные объекта.
// Return:     void
////////////////////////////////////////////////////////////////////////

void Fruit::Serialize(void)
{
   // TODO : implement
}

////////////////////////////////////////////////////////////////////////
// Name:       Fruit::UnSerialize()
// Purpose:    Implementation of Fruit::UnSerialize()
// Comment:    Функция диселиаризирует данные объекта
// Return:     void
////////////////////////////////////////////////////////////////////////

void Fruit::UnSerialize(void)
{
   // TODO : implement
}

////////////////////////////////////////////////////////////////////////
// Name:       Fruit::getClassName()
// Purpose:    Implementation of Fruit::getClassName()
// Comment:    Функция возвращает имя класса.
// Return:     wchar_t*
////////////////////////////////////////////////////////////////////////

wchar_t* Fruit::getClassName(void) const
{
   return L"Fruit";
}