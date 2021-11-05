//Last update time : 30.04.2010 13:46:32
#pragma hdrstop

#include "CfgPrgCls.h"
#include <tchar.h>
//---------------------------------------------------------------------------
ConfigParams Global_CFG;

#pragma package(smart_init)

bool __fastcall ConfigParams::SetExportPath(TCHAR* Exp)
{
 if( Exp == NULL ) return false;
 UINT explen = _tcslen(Exp);
 if( (explen == 0) || (explen > MAX_PATH) ) return false;

 _tcscpy(ExportPath, Exp);

 return (_tcscmp(ExportPath, Exp) == 0);
}

void __fastcall ConfigParams::Init( TCHAR* directory )
{
 if( directory == NULL ) return;
 UINT dirlen = _tcslen(directory);
 if( (dirlen == 0) || (dirlen > MAX_PATH) ) return;

 _tcscpy(PathApp, directory);
 _tcscat(PathApp, TEXT("\\"));
 _tcscat(PathApp, TEXT("MainCfg.ini"));
}

void __fastcall ConfigParams::LoadParameters()
{
  //HANDLE* procc = GetCurrentProcess();
  //GetProcess
  GetPrivateProfileString(TEXT("Main"), TEXT("ExportPath"), TEXT("C:\\"), ExportPath, MAX_PATH, PathApp);
}

void __fastcall ConfigParams::SaveParameters()
{
  //HANDLE* procc = GetCurrentProcess();
  //GetProcess
  WritePrivateProfileString(TEXT("Main"), TEXT("ExportPath"), ExportPath, PathApp);
}