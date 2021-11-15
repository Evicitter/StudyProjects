#include <windows.h>
#include <stdio.h>

void main()
{
   SYSTEM_INFO siSysInfo;

   // Копируем информацию о железе в структуру SYSTEM_INFO.

   GetSystemInfo(&siSysInfo);

   // Отображаем содержимое структуры SYSTEM_INFO.

   printf("Hardware information: \n");
   printf("  OEM ID: %u\n", siSysInfo.dwOemId);
   printf("  Number of processors: %u\n",
      siSysInfo.dwNumberOfProcessors);
   printf("  Page size: %u\n", siSysInfo.dwPageSize);
   printf("  Processor type: %u\n", siSysInfo.dwProcessorType);
   printf("  Minimum application address: %lx\n",
      siSysInfo.lpMinimumApplicationAddress);
   printf("  Maximum application address: %lx\n",
      siSysInfo.lpMaximumApplicationAddress);
   printf("  Active processor mask: %u\n",
      siSysInfo.dwActiveProcessorMask);
}




#include <windows.h>
#include <stdio.h>

void main()
{
   BOOL fResult;
   int aMouseInfo[3];

   fResult = GetSystemMetrics(SM_MOUSEPRESENT);

   if (fResult == 0)
      printf("No mouse installed.\n");
   else
   {
      printf("Mouse installed.\n");

      // Определяем, не поменялись ли кнопки мышки.

      fResult = GetSystemMetrics(SM_SWAPBUTTON);

      if (fResult == 0)
         printf("Buttons not swapped.\n");
      else printf("Buttons swapped.\n");

      // Получаем чувствительность мышки и её границы.

      fResult = SystemParametersInfo(
         SPI_GETMOUSE,  // получить информацию о мышке
         NULL,          // не используется
         &aMouseInfo,   // сюда поместить информацию
         NULL);         // не используется

      if( fResult )
      {
         printf("Speed: %d\n", aMouseInfo[2]);
         printf("Threshold (x,y): %d,%d\n",
            aMouseInfo[0], aMouseInfo[1]);
      }
   }
}



#include <windows.h>
#include <stdio.h>

void main()
{
   BOOL fResult;
   int aMouseInfo[3];       // массив для хранения информации

   // Получаем текущую чувствительность мышки.

   fResult = SystemParametersInfo(
      SPI_GETMOUSE,   // получаем информацию о мышке
      NULL,           // не используется
      &aMouseInfo,    // сюда поместить информацию
      NULL);          // не используется

   // Умножаем на 2.
 
   if( fResult )
   {
      aMouseInfo[2] = 2 * aMouseInfo[2];

      // Меняем чувствительность мышки.

      SystemParametersInfo(
         SPI_SETMOUSE,      // изменяем информацию мышки
         NULL,              // не используется
         aMouseInfo,        // массив с информацией
         SPIF_SENDCHANGE);  // обновить win.ini
   }
}