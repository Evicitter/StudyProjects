//Last update time : 03.06.2010 02:57:16
/***********************************************************************
 * Module:  Meat.h
 * Author:  Admin
 * Modified: 3 июня 2010 г. 2:53:40
 * Purpose: Declaration of the class Meat
 * Comment: Товар категории "мясо"
 ***********************************************************************/

#if !defined(__Tovari_Meat_h)
#define __Tovari_Meat_h

#include <Tovar.h>

class Meat : public Tovar
{
public:
   Meat();
   ~Meat();
   /* Функция возвращающая число дней за которое данный вид товара испортится. */
   virtual int GetBeBadDays(void);
   /* Функция выводит информацию об объекте. */
   virtual void Output(void) const;
   /* Функция сериализирует данные объекта. */
   virtual void Serialize(void);
   /* Функция диселиаризирует данные объекта */
   virtual void UnSerialize(void);
   /* Функция возвращает имя класса. */
   virtual wchar_t* getClassName(void) const;

protected:
private:
   /* Переменная указывающая состояние текущего объекта, что он является не портящимся "супер мясом". ) */
   bool isSuperMeat;


};

#endif