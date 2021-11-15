#include <windows.h>
#include <stdio.h>

#define BUFSIZE 1024

void main()
{
   LPTSTR lpszSystemInfo;      // указатель на строку, в которой
                               // будет информация о системе.
   DWORD cchBuff = 256;        // длина имени компьютера или
                               // пользователя.
   TCHAR tchBuffer[BUFSIZE];   // буфер для строки.

   DWORD dwResult;             // возвращаемое значение функции.

   lpszSystemInfo = tchBuffer;

   // Получаем и отображаем имя компьютера.

   if( GetComputerName(lpszSystemInfo, &cchBuff) )
      printf("Computer name:  %s\n", lpszSystemInfo);

   // Получаем и отображаем имя пользователя.

   if( GetUserName(lpszSystemInfo, &cchBuff) )
      printf("User name:  %s\n\n", lpszSystemInfo);

   // Получаем и отображаем системную директорию.

   if( GetSystemDirectory(lpszSystemInfo, MAX_PATH+1) )
      printf("System directory:  %s\n", lpszSystemInfo);

   // Получаем и отображаем директорию Windows.

   if( GetWindowsDirectory(lpszSystemInfo, MAX_PATH+1) )
      printf("Windows directory:  %s\n\n", lpszSystemInfo);


   printf("Переменные окружения (partial list): \n");

   // Получаем переменную окружения OS.

   dwResult = ExpandEnvironmentStrings(
                 "OS=%OS%",
                 lpszSystemInfo,
                 BUFSIZE);
   if( dwResult <= BUFSIZE )
      printf("  %s\n", lpszSystemInfo);


   // Получаем переменную окружения PATH.

   dwResult = ExpandEnvironmentStrings(
                 "PATH=%PATH%",
                 lpszSystemInfo,
                 BUFSIZE);
   if( dwResult <= BUFSIZE )
      printf("  %s\n", lpszSystemInfo);

   // Получаем переменную окружения TMP.

   dwResult = ExpandEnvironmentStrings(
               "TEMP=%TEMP%",
               lpszSystemInfo,
               BUFSIZE);
   if( dwResult <= BUFSIZE )
      printf("  %s\n", lpszSystemInfo);
}