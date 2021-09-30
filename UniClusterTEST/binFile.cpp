﻿//Last update time : 19.04.2013 22:53:47

#include "binFile.h"
//#include "ColidPool.h"
#include <intrin.h>

binFile::binFile(void)
{
	hFile=NULL;
	isOpenFile=0;
	crecords=0;
}

binFile::~binFile(void)
{
	if( hFile!=NULL ) { fclose(hFile); hFile=NULL; isOpenFile=0; }
}

int binFile::openFile(wchar_t* fname)
{
	if( fname == NULL ) return 1;
	if( hFile!=NULL ) { fclose(hFile); hFile=NULL; isOpenFile=0; }
	if( _wfopen_s(&hFile, fname, L"rbS") != 0 ) return 2;
	setvbuf(hFile,NULL,_IOFBF,65536);
	//setvbuf(hFile,NULL,_IOFBF,4194304);
	//setvbuf(hFile,NULL,_IONBF,0);
	isOpenFile=1;
	return 0;
}

int binFile::saveFile(wchar_t* fname)
{
	if( fname == NULL ) return 1;
	if( hFile!=NULL ) { fclose(hFile); hFile=NULL; isOpenFile=0; }
	if( _wfopen_s(&hFile, fname, L"wb") != 0 ) return 2;
	isOpenFile=1;
	return 0;
}

int binFile::closeFile()
{
	if( hFile!=NULL ) { fclose(hFile); hFile=NULL; isOpenFile=0; return 0; }
	else return -1;
}

int binFile::setpos( __int64 pos )
{
	if( hFile == NULL ) return -1;
	_fseeki64_nolock(hFile,pos,SEEK_SET);
	return 0;
}

__int64 binFile::getpos(  )
{
	if( hFile == NULL ) return -1i64;
	return _ftelli64_nolock(hFile);
}


int binFile::checkRecords()
{
	if( hFile == NULL ) return -1;
	__int64 savepos = _ftelli64_nolock(hFile);
	_fseeki64_nolock(hFile,0,SEEK_SET);
	crecords=0;
	while( true )
	{
		__int64 fmt=0;
		__int64 count=0;
		unsigned char size=0;
		__int64 index=0;
		short groups=0;
		size_t bytereads;
		
		bytereads = _fread_nolock_s(&fmt, 8, 8, 1, hFile);
		bytereads += _fread_nolock_s(&count, 8, 8, 1, hFile);
		bytereads += _fread_nolock_s(&size, 1, 1, 1, hFile);
		bytereads += _fread_nolock_s(&index, 8, 8, 1, hFile);
		bytereads += _fread_nolock_s(&groups, 2, 2, 1, hFile);

		if( bytereads != 5 ) break;
		++crecords;
		groups = ((groups)<0i16) ? -(groups) : (groups);
		_fseeki64_nolock(hFile, count * size * groups, SEEK_CUR);
	}
	_fseeki64_nolock(hFile,savepos,SEEK_SET);
	return crecords;
}

int binFile::getCurFmt( __int64& fmt, __int64& count, unsigned char& size, short& groups, wchar_t** name  )
{
	if( hFile == NULL ) return -1;

	__int64 index=0;
	size_t bytereads;

	__int64 savepos = _ftelli64_nolock(hFile);

	bytereads = _fread_nolock_s(&fmt, 8, 8, 1, hFile);
	bytereads += _fread_nolock_s(&count, 8, 8, 1, hFile);
	bytereads += _fread_nolock_s(&size, 1, 1, 1, hFile);
	bytereads += _fread_nolock_s(&index, 8, 8, 1, hFile);
	bytereads += _fread_nolock_s(&groups, 2, 2, 1, hFile);

	_fseeki64_nolock(hFile, savepos, SEEK_SET);

	if( bytereads != 5 ) return 1;

	return 0;
}

int binFile::skipCurFmt( )
{
	if( hFile == NULL ) return -1;

	__int64 fmt=0;
	__int64 count=0;
	unsigned char size=0;
	__int64 index=0;
	short groups=0;
	size_t bytereads;
		
	bytereads = _fread_nolock_s(&fmt, 8, 8, 1, hFile);
	bytereads += _fread_nolock_s(&count, 8, 8, 1, hFile);
	bytereads += _fread_nolock_s(&size, 1, 1, 1, hFile);
	bytereads += _fread_nolock_s(&index, 8, 8, 1, hFile);
	bytereads += _fread_nolock_s(&groups, 2, 2, 1, hFile);

	if( bytereads != 5 ) return 1;

	groups = (groups<0i16) ? -groups : groups;
	_fseeki64_nolock(hFile, count * size * groups, SEEK_CUR);

	return 0;
}

int binFile::getCurElement( __int64& fmt_header, unsigned char& size)
{
	if( hFile == NULL ) return -1;
	size_t bytereads=0;
	__int64 savepos = _ftelli64_nolock(hFile);
	bytereads += _fread_nolock_s(&fmt_header, 8, 8, 1, hFile);
	bytereads += _fread_nolock_s(&size, 1, 1, 1, hFile);
	_fseeki64_nolock(hFile, savepos, SEEK_SET);
	if( bytereads != 2 ) return 1;
	return 0;
}

int binFile::skipCurElement( )
{
	if( hFile == NULL ) return -1;

	__int64 fmt_header=0;
	unsigned char size=0;
	size_t bytereads=0;
	bytereads += _fread_nolock_s(&fmt_header, 8, 8, 1, hFile);
	bytereads += _fread_nolock_s(&size, 1, 1, 1, hFile);
	if( bytereads != 2 ) return 1;

	_fseeki64_nolock(hFile, size, SEEK_CUR);

	return 0;
}

int binFile::setArray( __int64  fmt, __int64  count, unsigned char  size, short  groups, wchar_t*  name, void* Buf )
{
 if( hFile == NULL ) return -1;
 if(Buf == NULL) return -1;

 //wchar_t fname[32];
 __int64 index=0;
 size_t bytewrites;

 bytewrites = _fwrite_nolock(&fmt, 8, 1, hFile);
 bytewrites += _fwrite_nolock(&count, 8, 1, hFile);
 bytewrites += _fwrite_nolock(&size, 1, 1, hFile);
 bytewrites += _fwrite_nolock(&index, 8, 1, hFile);
 bytewrites += _fwrite_nolock(&groups, 2, 1, hFile);

 if( bytewrites != 5 ) return 1;

 groups = (groups<0i16) ? -groups : groups;
 _fwrite_nolock( Buf, size, count * groups, hFile );

 return 0;
}

void* binFile::getArray( __int64& fmt, __int64& count, unsigned char& size, short& groups, wchar_t** name, void* Buf, bool bNextChunk )
{
	if( hFile == NULL ) return NULL;

	__int64 index=0;
	size_t bytereads;

	bytereads = _fread_nolock_s(&fmt, 8, 8, 1, hFile);
	bytereads += _fread_nolock_s(&count, 8, 8, 1, hFile);
	bytereads += _fread_nolock_s(&size, 1, 1, 1, hFile);
	bytereads += _fread_nolock_s(&index, 8, 8, 1, hFile);
	bytereads += _fread_nolock_s(&groups, 2, 2, 1, hFile);

	if( bytereads != 5 ) return NULL;

	short tg = (groups<0i16) ? -groups : groups;

	//if( *Buf != NULL ) { _aligned_free( *Buf ); *Buf = NULL; }
	//*Buf = _aligned_malloc( count * tg * size, 16 );
	Buf = _aligned_realloc(Buf, count * tg * size, 16);
	if(Buf == NULL)
	{
		_fseeki64_nolock(hFile, count * tg * size, SEEK_CUR);
		return NULL;
	}
	_fread_nolock_s( Buf, count * tg * size, size, count * tg, hFile );

	return Buf;
}

int binFile::FreeArray( void* Buf )
{
	if( hFile == NULL ) return -1;
	if( Buf == NULL ) return -1;
	_aligned_free( Buf );
	Buf = NULL;
	return 0;
}


void* binFile::getElement( __int64& fmt_header, unsigned char& size, void* Buf, bool bNextChunk)
{
	if( hFile == NULL ) return NULL;

	size_t bytereads;

	bytereads = _fread_nolock_s(&fmt_header, 8, 8, 1, hFile);
	bytereads += _fread_nolock_s(&size, 1, 1, 1, hFile);

	if( bytereads != 2 ) return NULL;

	Buf = _aligned_realloc(Buf, size, 16);
	if(Buf == NULL)
	{
		_fseeki64_nolock(hFile, size, SEEK_CUR);
		return NULL;
	}
	_fread_nolock_s( Buf, size, size, 1, hFile );

	return Buf;
}


long long binFile::GetCollide( __int64 fmt_header, unsigned char fsize )
{
	if( hFile == NULL ) return -1;
	ColidPool* cpref;

	_fseeki64_nolock(hFile,0,SEEK_SET);

	cpref=ColidPool::getInstanceFromSize(fsize, true);  //for multi thread
	//-------------------------
#pragma omp parallel shared(cpref) firstprivate(fsize,fmt_header)
{
	__int64 fmthdr=0;
	unsigned int count=0;
	unsigned short size=0;
	unsigned char* xbuffer = (unsigned char*)_aligned_malloc((8<<20), 16);
	while(true)
	{
	 size_t bytereads;

 	 _lock_file(hFile);
			bytereads = _fread_nolock_s(&fmthdr, sizeof(fmthdr), sizeof(fmthdr), 1, hFile);
			bytereads += _fread_nolock_s(&size  , sizeof(size), sizeof(size), 1, hFile);
			bytereads += _fread_nolock_s(&count, sizeof(count), sizeof(count), 1, hFile);
			
			if(bytereads!=3) { _unlock_file(hFile); break; }
			if((fmt_header != fmthdr) || (fsize != size)) { _fseeki64_nolock(hFile, size*count, SEEK_CUR); _unlock_file(hFile); continue; }
			
			_fread_nolock_s(xbuffer,(8<<20),size,count,hFile);
	 _unlock_file(hFile);
	 
	 for(int i=0; i<count; ++i) cpref->add(xbuffer + i*size);
	}
	if(xbuffer!=NULL) _aligned_free(xbuffer);
}

	_fseeki64_nolock(hFile,0,SEEK_SET);

	long long retval = cpref->getCollideSum();
	delete cpref;

	return retval;
}
//-----------------------------------------------

int binFile::exportText(wchar_t* filename)
{
	FILE* tfile=NULL;
	
	if( hFile == NULL ) return -1;
	if( filename == NULL ) return 1;
	if( _wfopen_s(&tfile, filename, L"w") != 0 ) return 2;

	__int64 savepos = _ftelli64_nolock(hFile);
	_fseeki64_nolock(hFile, 0, SEEK_SET);

	int err=0;
	char* buf=NULL;
	do
	{
		__int64 fmt, count;
		unsigned char size;
		short groups;
		wchar_t* name=NULL;
		err = getCurFmt(fmt, count, size, groups, &name);
		if(err!=0) break;

		fwprintf_s(tfile, L"%0.16I64x,%I64i,%hhi,%hi", fmt, count, size, groups);
		buf=NULL;
		buf = (char*)getArray(fmt, count, size, groups, &name, buf);
		if(buf!=NULL)
		{
		 groups = (groups<0i16) ? -groups : groups;
		 for(__int64 i=0; i < (count*groups); ++i)
		 {
			if(size==1) fwprintf_s(tfile, L",%hhi", buf[i]);
			else if(size==2) fwprintf_s(tfile, L",%hi", *((short*)buf + i));
			else if(size==4) fwprintf_s(tfile, L",%i", *((int*)buf + i));
			else if(size==8) fwprintf_s(tfile, L",%I64i", *((__int64*)buf + i));
			//else if(size==16) fwprintf_s(tfile, L",%I64i", *((__int64*)buf + i*2));
		 }
		 FreeArray(buf);
		}
		//err = skipCurFmt();
		fwprintf_s(tfile,L"\r\n");
	}while(true);
	
	fclose(tfile);

	_fseeki64_nolock(hFile, savepos, SEEK_SET);
	return 0;
}