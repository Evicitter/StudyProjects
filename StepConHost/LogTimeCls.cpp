//Last update time : 22.03.2010 09:41:40

#pragma hdrstop

#include "LogTimeCls.h"
#include <tchar.h>

//---------------------------------------------------------------------------
#pragma package(smart_init)

MemLog Global_LOG;

#define UseTick64 1

void __fastcall MemLog::InitLog(TCHAR* Directory, TCHAR* filename, int sizeReserv)
{
 if( (filename == NULL) || (sizeReserv<0) ) return;
 UINT flen = _tcslen(Directory) + _tcslen(filename);
 if( (flen == 0) || (flen > MAX_PATH) ) return;

 _tcscpy(FileOutput, Directory);
 _tcscat(FileOutput, TEXT("\\"));
 _tcscat(FileOutput, filename);

 //ss->SetSize( sizeReserv );
 ss->Seek(0i32, soBeginning);
}

void __fastcall MemLog::ClearLog()
{ ss->Clear(); ss->Seek(0i32, soBeginning); }

void __fastcall MemLog::CloseLog()
{
 ss->SaveToFile(FileOutput);
}

void __fastcall MemLog::WriteLog(TCHAR* us)
{
 ss->WriteString(TEXT("LOG:: "));
 ss->WriteString(us);
 ss->WriteString(TEXT("\r\n"));
}

TCHAR* __fastcall MemLog::ReadLog()
{
 return ss->DataString.w_str();
}


//------------------------------------------------------------------------
void __fastcall TimeLog::StartTime( short id )
{
 id = (id < 0) ? 0 : id;
 id = (id > 255) ? 255 : id;
#if (defined(UseTick64) && (UseTick64 == 1))
 Times[id] = GetTickCount64();
#else
 Times[id] = GetTickCount();
#endif
}

void __fastcall TimeLog::StopTime( short id )
{
 id = (id < 0) ? 0 : id;
 id = (id > 255) ? 255 : id;
#if (defined(UseTick64) && (UseTick64 == 1))
 Times[id] = GetTickCount64() - Times[id];
#else
 Times[id] = GetTickCount() - Times[id];
#endif
}

#undef UseTick64