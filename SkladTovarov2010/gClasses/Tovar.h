//Last update time : 03.06.2010 02:57:16
/***********************************************************************
 * Module:  Tovar.h
 * Author:  Admin
 * Modified: 3 июня 2010 г. 2:51:45
 * Purpose: Declaration of the class Tovar
 * Comment: Класс товар
 ***********************************************************************/

#if !defined(__Tovari_Tovar_h)
#define __Tovari_Tovar_h

class Tovar
{
public:
   Tovar();
   ~Tovar();
   /* Функция возвращающая число дней за которое данный вид товара испортится. */
   virtual int GetBeBadDays(void);
   /* Функция возвращающая потерю стоимости в случае негодности товара. */
   virtual int LostCoast(int days);
   /* Функция выводит информацию об объекте. */
   virtual void Output(void) const;
   /* Функция сериализирует данные объекта. */
   virtual void Serialize(void);
   /* Функция диселиаризирует данные объекта */
   virtual void UnSerialize(void);
   virtual wchar_t* getName(void) const;
   virtual void setName(wchar_t* newName);
   virtual int getCoast(void) const;
   virtual void setCoast(int newCoast);
   virtual int getCount(void) const;
   virtual void setCount(int newCount);
   virtual int get_BadDays(void) const;
   virtual void set_BadDays(int new_BadDays);
   /* Функция возвращает имя класса. */
   virtual wchar_t* getClassName(void) const;

protected:
   /* Название товара */
   wchar_t* Name;
   /* Стоимость товара в размере 1 шт. */
   int Coast;
   /* Количество данного типа товаров. */
   int Count;
   /* Срок хранения товара в днях. */
   int _BadDays;

private:

};

#endif