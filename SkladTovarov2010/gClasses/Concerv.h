//Last update time : 03.06.2010 02:57:15
/***********************************************************************
 * Module:  Concerv.h
 * Author:  Admin
 * Modified: 3 июня 2010 г. 2:54:05
 * Purpose: Declaration of the class Concerv
 * Comment: Товар категории "Консервы"
 ***********************************************************************/

#if !defined(__Tovari_Concerv_h)
#define __Tovari_Concerv_h

#include <Tovar.h>

class Concerv : public Tovar
{
public:
   Concerv();
   ~Concerv();
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

};

#endif