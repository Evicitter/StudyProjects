//В Windows NT/2000/XP: для завершения работы системы необходимо получить привелегию SE_SHUTDOWN_NAME.

#include <Windows.h>

int MySystemShutdown()
{
   HANDLE hToken;
   TOKEN_PRIVILEGES tkp;

   // Получаем маркер текущего процесса.

   if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
      return 0;

   // Получаем LUID для привелегии завершения работы.

   LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
 
   tkp.PrivilegeCount = 1;  // будет установлена одна привелегия
   tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

   // Получаем привелегию завершения работы для этого процесса.

   AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);

   if (GetLastError() != ERROR_SUCCESS)
      return 0;

   // Шутдауним систему и заставляем все приложения закрыться.

   if (!ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE, 0))
      return 0;

   return 1;
}