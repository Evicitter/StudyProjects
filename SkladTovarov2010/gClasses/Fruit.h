//Last update time : 03.06.2010 02:57:16
/***********************************************************************
 * Module:  Fruit.h
 * Author:  Admin
 * Modified: 3 июня 2010 г. 2:52:46
 * Purpose: Declaration of the class Fruit
 * Comment: Товара катеории "фрукт"
 ***********************************************************************/

#if !defined(__Tovari_Fruit_h)
#define __Tovari_Fruit_h

#include <Tovar.h>

class Fruit : public Tovar
{
public:
   Fruit();
   ~Fruit();
   /* Функция выводит информацию об объекте. */
   virtual void Output(void) const;
   /* Функция возвращающая число дней за которое данный вид товара испортится. */
   virtual int GetBeBadDays(void);
   /* Функция сериализирует данные объекта. */
   virtual void Serialize(void);
   /* Функция диселиаризирует данные объекта */
   virtual void UnSerialize(void);
   /* Функция возвращает имя класса. */
   virtual wchar_t* getClassName(void) const;

protected:
private:
   /* Переменная указывающая состояние текущего объекта, что он является овощем и имеет более скоротечную порчу. */
   bool isOvosh;


};

#endif