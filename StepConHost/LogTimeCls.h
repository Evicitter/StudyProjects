//Last update time : 30.04.2010 14:52:48

#ifndef LogTimeClsH
#define LogTimeClsH
//---------------------------------------------------------------------------

#include <Classes.hpp>

class MemLog
{
private:
	TStringStream* ss;
	TCHAR FileOutput[MAX_PATH];
public:
   MemLog() { ss = new TStringStream(); }
   ~MemLog() { delete ss; }

   void __fastcall InitLog(TCHAR* Directory, TCHAR* filename, int sizeReserv);
   void __fastcall ClearLog();
   void __fastcall CloseLog();

   void __fastcall WriteLog(TCHAR* us);
   TCHAR* __fastcall ReadLog();
};

extern MemLog Global_LOG;

class TimeLog
{
private:
  unsigned __int64 Times[256];
public:
   TimeLog() {}
  ~TimeLog() {}

  void __fastcall StartTime( short id );
  void __fastcall StopTime( short id );
};

#endif
