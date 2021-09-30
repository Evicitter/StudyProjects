//Last update time : 18.04.2013 20:45:02
#pragma once

#include <stdio.h>
/*struct statmat
{
	double avg_ar;
	double avg_ge;
	double avg_hr;
	double avg_sq;
	double qlin; //среднее линейное отклонение
	double q; //средне квадратическое отклонение
	double qdisp; //дисперсия
};*/

class binFile
{
	FILE* hFile;
	int isOpenFile;
	int crecords;
public:
	binFile(void);
	~binFile(void);

	int openFile(wchar_t* fname);
	int saveFile(wchar_t* fname);
	int closeFile();
	int setpos( __int64 pos );
__int64 getpos(  );

	int checkRecords();

	//read
	int getCurFmt( __int64& fmt, __int64& count, unsigned char& size, short& groups, wchar_t** name );
	int skipCurFmt( );
	int getCurElement( __int64& fmt_header, unsigned char& size);
	int skipCurElement( );

	//get
	int setArray( __int64  fmt, __int64  count, unsigned char  size, short  groups, wchar_t*  name, void* Buf );
  void* getArray( __int64& fmt, __int64& count, unsigned char& size, short& groups, wchar_t** name, void* Buf, bool bNextChunk=true );
	int FreeArray( void* Buf );

  void* getElement( __int64& fmt_header, unsigned char& size, void* Buf, bool bNextChunk=true);
	long long GetCollide( __int64 fmt_header, unsigned char size );
  //------------------------------------------------------------------------------------------------

	//write
	int beginHeaderFile( __int64 fmt, __int64 count, char size, short groups, wchar_t* name );
	int writeChunk( void* buf, __int64 count, char size );

	//find

	//---------------------------------------------------------------------------------------------

	//export
	int exportText(wchar_t* filename);
	//---------------------------------------------------------------------------------------------
};