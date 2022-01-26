//Last update time : 03.06.2010 02:12:35
/***********************************************************************
 * Module:  Sklad.h
 * Author:  Admin
 * Modified: 3 июня 2010 г. 0:54:46
 * Purpose: Declaration of the class Sklad
 * Comment: Класс склада содержащего товары.
 ***********************************************************************/

#if !defined(__Tovari_Sklad_h)
#define __Tovari_Sklad_h

class Tovar;

#include <Tovar.h>

class Sklad
{
public:
   Sklad();
   ~Sklad();
   /* Функция осущствляющая подсчёт срока годности всех товаров. */
   long SumBadDays(int _days);
   /* Функция загружающая список товаров на склад из файла. */
   bool Load(void);
   /* Функция сохраняющая список товаров из склад в файл. */
   bool Save(void);
   /* Возвращение числа товаров */
   int getCountVals(void);
   /* Возвращает указатель на товар по индексу. */
   Tovar* GetTovar(int ind);
   /* Добавляет товар */
   void AddTovar(Tovar* tov);
   /* Удаление выбранного товара */
   void DelTovar(int ind);

   /* Индификация позиции склада. */
   int PositionID;
   
   Tovar** tovar;

protected:
private:
   /* Имя склада. И имя файла базы в которой склад хранит товары */
   char* skName;
   /* Число товаров */
   int CountVals;


};

#endif