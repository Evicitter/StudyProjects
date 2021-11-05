//Last update time : 07.05.2010 03:24:44

#ifndef CfgPrgClsH
#define CfgPrgClsH
//---------------------------------------------------------------------------

#include <windows.h>

class ConfigParams
{
private:
//Technique Parameters
   TCHAR PathApp[MAX_PATH];

public:
//Configuration Parameters
   TCHAR ExportPath[MAX_PATH];

//transient parameters
	long ProgressCurrent;
	long ProgressMax;

   ConfigParams() { ProgressMax=ProgressCurrent=0l; }
   ~ConfigParams() {}

   //----------------------------------------
   void __fastcall Init( TCHAR* directory );
   void __fastcall LoadParameters();
   void __fastcall SaveParameters();
   //----------------------------------------

   bool __fastcall SetExportPath(TCHAR* Exp);
};

extern ConfigParams Global_CFG;
#endif