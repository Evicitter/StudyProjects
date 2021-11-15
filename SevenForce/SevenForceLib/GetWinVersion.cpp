//Для определения версии текущей запущенной операционной системы можно воспользоваться функцией GetVersionEx.
 

#include <windows.h>
#include <stdio.h>

#define BUFSIZE 80

BOOL DisplaySystemVersion()
{
   OSVERSIONINFOEX osvi;
   BOOL bOsVersionInfoEx;

 //Пытаемся вызвать GetVersionEx используя структуру OSVERSIONINFOEX.
 //В случае ошибки пытаемся проделать тоже самое со структурой OSVERSIONINFO.

   ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
   osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

   if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
   {
      // Если OSVERSIONINFOEX не работает, то используем OSVERSIONINFO.
      osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
      if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) )
         return FALSE;
   }

   switch (osvi.dwPlatformId)
   {
      // Проверяем семейство Windows NT.
      case VER_PLATFORM_WIN32_NT:

      // Проверяем версию продукта.

         if ( osvi.dwMajorVersion <= 4 )
            printf("Microsoft Windows NT ");

         if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 )
            printf ("Microsoft Windows 2000 ");


         if( bOsVersionInfoEx )  // Используем информацию из GetVersionEx.
         { 
         // Проверяем тип рабочей станции.
            if ( osvi.wProductType == VER_NT_WORKSTATION )
            {
               if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 )
                  printf ("Microsoft Windows XP ");

               if( osvi.wSuiteMask & VER_SUITE_PERSONAL )
                  printf ( "Home Edition " );
               else
                  printf ( "Professional " );
            }

         // Проверяем тип сервера.
            else if ( osvi.wProductType == VER_NT_SERVER )
            {
               if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2 )
                  printf ("Microsoft Windows .NET ");

               if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
                  printf ( "DataCenter Server " );
               else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
                  if( osvi.dwMajorVersion == 4 )
                     printf ("Advanced Server " );
                  else
                     printf ( "Enterprise Server " );
               else if ( osvi.wSuiteMask == VER_SUITE_BLADE )
                  printf ( "Web Server " );
               else
                  printf ( "Server " );
            }
         }
         else   // Для ранних версий Windows NT используем реестр.
         {
            HKEY hKey;
            char szProductType[BUFSIZE];
            DWORD dwBufLen=BUFSIZE;
            LONG lRet;

            lRet = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
               "SYSTEM\\CurrentControlSet\\Control\\ProductOptions",
               0, KEY_QUERY_VALUE, &hKey );
            if( lRet != ERROR_SUCCESS )
                return FALSE;

            lRet = RegQueryValueEx( hKey, "ProductType", NULL, NULL,
               (LPBYTE) szProductType, &dwBufLen);
            if( (lRet != ERROR_SUCCESS) || (dwBufLen > BUFSIZE) )
                return FALSE;

            RegCloseKey( hKey );

            if ( lstrcmpi( "WINNT", szProductType) == 0 )
               printf( "Professional " );
            if ( lstrcmpi( "LANMANNT", szProductType) == 0 )
               printf( "Server " );
            if ( lstrcmpi( "SERVERNT", szProductType) == 0 )
               printf( "Advanced Server " );
         }

      // Отображаем версию, service pack (если есть), и номер билда.

         if ( osvi.dwMajorVersion <= 4 )
         {
            printf ("version %d.%d %s (Build %d)\n",
               osvi.dwMajorVersion,
               osvi.dwMinorVersion,
               osvi.szCSDVersion,
               osvi.dwBuildNumber & 0xFFFF);
         }
         else
         { 
            printf ("%s (Build %d)\n",
               osvi.szCSDVersion,
               osvi.dwBuildNumber & 0xFFFF);
         }
         break;

      // Проверяем семейство Windows 95.
      case VER_PLATFORM_WIN32_WINDOWS:

         if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0)
         {
             printf ("Microsoft Windows 95 ");
             if ( osvi.szCSDVersion[1] == 'C' || osvi.szCSDVersion[1] == 'B' )
                printf("OSR2 " );
         } 

         if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10)
         {
             printf ("Microsoft Windows 98 ");
             if ( osvi.szCSDVersion[1] == 'A' )
                printf("SE " );
         } 

         if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90)
         {
             printf ("Microsoft Windows Millennium Edition ");
         } 
         break;
   }
   return TRUE; 
}