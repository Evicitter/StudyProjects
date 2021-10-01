//Last update time : 28.11.2011 07:03:26

#include "stdafx.h"
#include "FileShareProtocol.h"

#include <malloc.h>
#include <intrin.h>

#include <Windows.h>
#include <Wincrypt.h>

//---------------------------
#include "CMemStream.hpp"

#include <iostream>
#include <sstream>
#include <string>

using namespace std;

FileShareProtocol globalFSC;

int MD5genB(unsigned char* buffer, unsigned int bufsize_inbyte, unsigned char** out)
{
	HCRYPTHASH hHash;
	HCRYPTPROV hProv;
	BYTE md5hash[16];
	DWORD md5hash_size,dwSize;

	if( buffer == NULL ) return 1;
	if( out == NULL ) { return 2; }
	if( *out == NULL ) return 3;

	dwSize=sizeof(md5hash_size);
	memset( md5hash, 0, sizeof(md5hash) );

	CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, 0);
	CryptCreateHash(hProv,CALG_MD5,0,0,&hHash);
	CryptHashData(hHash,buffer,bufsize_inbyte,0);
	CryptGetHashParam(hHash,HP_HASHSIZE,(BYTE *)&md5hash_size,&dwSize,0);
	CryptGetHashParam(hHash,HP_HASHVAL,md5hash,&md5hash_size,0);
	CryptDestroyHash(hHash);
	CryptReleaseContext(hProv, 0);

	memcpy( *out, md5hash, sizeof(md5hash) );
	
	return 0;
}

int __fastcall MD5genF(wchar_t* filename, unsigned char** out)
{
	HCRYPTHASH hHash;
	HCRYPTPROV hProv;
	BYTE md5hash[16];
	DWORD md5hash_size,dwSize;
	FILE* hFile;
	int readn;
	unsigned char buffile[2048];

	if( filename == NULL ) return 1;
	if( out == NULL ) { return 2; }
	if( *out == NULL ) return 3;
	if( buffile == NULL ) return 4;

	if( _wfopen_s(&hFile, filename, L"rb") != 0 ) return 5;

	dwSize=sizeof(md5hash_size);
	memcpy( md5hash, 0, sizeof(md5hash) );

	CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, 0);
	CryptCreateHash(hProv,CALG_MD5,0,0,&hHash);

	do 
	{
		readn = fread_s(buffile, 2048, 1, 2048, hFile);
		CryptHashData(hHash,buffile,readn,0);
	} while ( readn == 2048 );

	fclose(hFile);
	
	CryptGetHashParam(hHash,HP_HASHSIZE,(BYTE *)&md5hash_size,&dwSize,0);
	CryptGetHashParam(hHash,HP_HASHVAL,md5hash,&md5hash_size,0);
	CryptDestroyHash(hHash);
	CryptReleaseContext(hProv, 0);

	memcpy( *out, md5hash, sizeof(md5hash) );

	return 0;
}

//-----------------------------------------------------------------------------
FileShareProtocol::FileShareProtocol(void)
{
	idCurrentClient=64;
	idCurrentServer=64;
	lerror=0;
	HashB = &MD5genB;
	HashF = &MD5genF;
	w_profilename = (wchar_t*)_aligned_malloc( 64 * sizeof(wchar_t), 16 );
}

FileShareProtocol::~FileShareProtocol(void)
{
	if(w_profilename != NULL) { _aligned_free(w_profilename); w_profilename=NULL; }
}

//Установка индетификатора клиента и сервера, и имени профиля
int FileShareProtocol::setup(int ids, int idc, const wchar_t* profilename)
{
 lerror=0;
 if( profilename == NULL ) return ++lerror;
 if( w_profilename == NULL ) return ++lerror;
 if( wcslen(profilename) == 0 ) return ++lerror;
 
 wcscpy_s(w_profilename,64,profilename);

 idCurrentServer = ids;
 idCurrentClient = idc;

 return lerror=0;
}

// Генерирует заголовок по типу, и возвращает его размер
int FileShareProtocol::genheader(unsigned char** buf, int idtype)
{
	if( buf == NULL ) return 0;
	if( *buf == NULL ) return 0;
	unsigned char* pbuf = *buf;
	*pbuf++ = 0xff;
	*pbuf++ = 0xff;
	*pbuf++ = 'F';
	*pbuf++ = 'S';
	*pbuf++ = 'P';
	*pbuf++ = '0'+idtype;
	*pbuf++ = 0xff;
	*pbuf++ = 0xff;
	return pbuf - *buf;
}

// Функция возвращает тип заголовка, иначе -1 если ошибка
int FileShareProtocol::getheader(unsigned char** buf, int buflen)
{
	if( buf == NULL ) return -1;
	if( *buf == NULL ) return -1;
	if( buflen < 8 ) return -1;
	unsigned char* pbuf = *buf;
	int itype;

	if( *pbuf++ != 0xff ) return -1;
	if( *pbuf++ != 0xff ) return -1;
	if( *pbuf++ != 'F' ) return -1;
	if( *pbuf++ != 'S' ) return -1;
	if( *pbuf++ != 'P' ) return -1;
	itype = *pbuf++;
	if( *pbuf++ != 0xff ) return -1;
	if( *pbuf++ != 0xff ) return -1;
	return itype - '0';
}

// Освобождает выделеный буфер
int FileShareProtocol::gq_FreeResource(unsigned char** fbuffer)
{
	lerror=0;
	if( fbuffer == NULL ) return ++lerror;
	if( *fbuffer == NULL ) return ++lerror;
	_aligned_free( *fbuffer );
	*fbuffer=NULL;
	return lerror=0;
}

// Сборка пакета запроса scanusers
int FileShareProtocol::gq_ScanUsers_sen(unsigned char** inbuf, int& len, s_scanusers_inout* in_param)
{
	lerror=0;
	if( inbuf == NULL ) return ++lerror;
	if( *inbuf != NULL ) return ++lerror;
	if( HashB == NULL ) return ++lerror;
	if( in_param == NULL ) return ++lerror;
	if( in_param->profile == NULL ) return ++lerror;

	unsigned char buf[16];
	unsigned char* pbuf = buf;
	int sizeh = genheader(&pbuf, (in_param->query) ? 0 : 1 );

	CMemStream cms;
	cms.ResetWriteB();
	cms.SetSize(512);
	cms.SetPosition(0);

	cms << wpairarr(buf, sizeh);
	cms << in_param->iclient;
	cms << in_param->iserver;
	cms << in_param->query;
	cms << wpairarr(in_param->profile, sizeof(in_param->profile));
	cms << wpairarr(in_param->rcompName, sizeof(in_param->rcompName));
	cms << wpairarr(in_param->md5hash, sizeof(in_param->md5hash));

	*inbuf = (unsigned char*)_aligned_malloc( len = (int)cms.GetPosition() , 16);
	memcpy( *inbuf, cms.Pointer(), len );

	return lerror=0;
}

// Сборка пакета ответа и его проверка на запрос
int FileShareProtocol::gq_ScanUsers_rec(unsigned char* inbuf, int len, s_scanusers_inout** out_param)
{
	lerror=0;
	if( inbuf == NULL ) return ++lerror;
	if( HashB == NULL ) return ++lerror;
	if( out_param == NULL ) return ++lerror;
	//if( len > 512 ) return ++lerror;

	CMemStream cms;
	cms.ResetReadB();
	cms.ResetWriteB();
	cms.SetSize(512);
	cms.SetPosition(0);
	cms.Write( inbuf, len );

	*out_param = (s_scanusers_inout*)_aligned_malloc( sizeof(s_scanusers_inout), 16 );

	//cms.SetPosition(0);
	//unsigned char buf[16];
	cms.SetPosition( 4 + 8 );
	rpairarr ppa;

	cms >> (*out_param)->iclient;
	cms >> (*out_param)->iserver;
	cms >> (*out_param)->query;
	cms >> ppa;
	memcpy( (*out_param)->profile, ppa.data, ppa.size );

	cms >> ppa;
	memcpy( (*out_param)->rcompName, ppa.data, ppa.size );

	cms >> ppa; 
	memcpy( (*out_param)->md5hash, ppa.data, ppa.size );
	return lerror=0;
}

// Сборка пакета запроса scanfiles
int FileShareProtocol::gq_ScanFiles_sen(unsigned char** inbuf, int& len, s_scanfiles_inout* in_param)
{
	lerror=0;
	if( inbuf == NULL ) return ++lerror;
	if( *inbuf != NULL ) return ++lerror;
	if( HashB == NULL ) return ++lerror;
	if( in_param == NULL ) return ++lerror;
	if( in_param->r_profile == NULL ) return ++lerror;
	if( in_param->s_profile == NULL ) return ++lerror;

	unsigned char buf[16];
	unsigned char* pbuf = buf;
	int sizeh = genheader(&pbuf, (in_param->query) ? 2 : 3 );

	CMemStream cms;
	cms.ResetWriteB();
	cms.SetSize(2048);
	cms.SetPosition(0);

	cms << wpairarr(buf, sizeh);
	cms << in_param->iclient;
	cms << in_param->iserver;
	cms << in_param->query;
	cms << wpairarr(in_param->r_profile, sizeof(in_param->r_profile));
	cms << wpairarr(in_param->s_profile, sizeof(in_param->s_profile));
	cms << wpairarr(in_param->md5hash, sizeof(in_param->md5hash));
	cms << wpairarr(in_param->rcompName, sizeof(in_param->rcompName));
	cms << wpairarr(in_param->r_filename, sizeof(in_param->r_filename));

	*inbuf = (unsigned char*)_aligned_malloc( len = (int)cms.GetPosition() , 16);
	memcpy( *inbuf, cms.Pointer(), len );

	return lerror=0;
}

// Сборка пакета ответа и его проверка на запрос
int FileShareProtocol::gq_ScanFiles_rec(unsigned char* inbuf, int len, s_scanfiles_inout** out_param)
{
	lerror=0;
	if( inbuf == NULL ) return ++lerror;
	if( HashB == NULL ) return ++lerror;
	if( out_param == NULL ) return ++lerror;

	//unsigned char buf[16];
	//unsigned char* pbuf = buf;
	//int sizeh = 8;

	CMemStream cms;
	cms.ResetReadB();
	cms.ResetWriteB();
	cms.SetSize(512);
	cms.SetPosition(0);
	cms.Write( inbuf, len );

	*out_param = (s_scanfiles_inout*)_aligned_malloc( sizeof(s_scanfiles_inout), 16 );

	//cms.SetPosition(0);
	//unsigned char buf[16];
	cms.SetPosition( 4 + 8 );
	rpairarr ppa;

	cms >> (*out_param)->iclient;
	cms >> (*out_param)->iserver;
	cms >> (*out_param)->query;
	cms >> ppa;
	memcpy( (*out_param)->r_profile, ppa.data, ppa.size );

	cms >> ppa; 
	memcpy( (*out_param)->s_profile, ppa.data, ppa.size );

	cms >> ppa; 
	memcpy( (*out_param)->md5hash, ppa.data, ppa.size );

	cms >> ppa; 
	memcpy( (*out_param)->rcompName, ppa.data, ppa.size );

	cms >> ppa;
	memcpy( (*out_param)->r_filename, ppa.data, ppa.size );

	return lerror=0;
}

// Сборка пакета запроса getfile
int FileShareProtocol::gq_GetFile_sen(unsigned char** inbuf, int& len, s_getfile_inout* in_param)
{
	lerror=0;
	if( inbuf == NULL ) return ++lerror;
	if( *inbuf != NULL ) return ++lerror;
	if( HashB == NULL ) return ++lerror;
	if( in_param == NULL ) return ++lerror;
	if( in_param->r_profile == NULL ) return ++lerror;
	if( in_param->s_profile == NULL ) return ++lerror;

	unsigned char buf[16];
	unsigned char* pbuf = buf;
	int sizeh = genheader(&pbuf, (in_param->query) ? 4 : 5 );

	CMemStream cms;
	cms.ResetWriteB();
	cms.SetSize(2048);
	cms.SetPosition(0);

	cms << wpairarr(buf, sizeh);
	cms << in_param->iclient;
	cms << in_param->iserver;
	cms << in_param->query;
	cms << wpairarr(in_param->r_profile, sizeof(in_param->r_profile));
	cms << wpairarr(in_param->s_profile, sizeof(in_param->s_profile));
	cms << wpairarr(in_param->md5hash_chunk, sizeof(in_param->md5hash_chunk));
	cms << wpairarr(in_param->rcompName, sizeof(in_param->rcompName));
	cms << wpairarr(in_param->r_filename, sizeof(in_param->r_filename));
	
	cms << in_param->position;
	cms << in_param->bufsize;
	cms << wpairarr(in_param->chunk, sizeof(in_param->chunk));

	*inbuf = (unsigned char*)_aligned_malloc( len = (int)cms.GetPosition() , 16);
	memcpy( *inbuf, cms.Pointer(), len );

	return lerror=0;
}

// Сборка пакета ответа и его проверка на запрос
int FileShareProtocol::gq_GetFile_rec(unsigned char* inbuf, int len, s_getfile_inout** out_param)
{
	lerror=0;
	if( inbuf == NULL ) return ++lerror;
	if( HashB == NULL ) return ++lerror;
	if( out_param == NULL ) return ++lerror;

	//unsigned char buf[16];
	//unsigned char* pbuf = buf;
	//int sizeh = 8;

	CMemStream cms;
	cms.ResetReadB();
	cms.ResetWriteB();
	cms.SetSize(512);
	cms.SetPosition(0);
	cms.Write( inbuf, len );

	*out_param = (s_getfile_inout*)_aligned_malloc( sizeof(s_getfile_inout), 16 );

	//cms.SetPosition(0);
	//unsigned char buf[16];
	cms.SetPosition( 4 + 8 );
	rpairarr ppa;

	cms >> (*out_param)->iclient;
	cms >> (*out_param)->iserver;
	cms >> (*out_param)->query;
	cms >> ppa;
	memcpy( (*out_param)->r_profile, ppa.data, ppa.size );

	cms >> ppa; 
	memcpy( (*out_param)->s_profile, ppa.data, ppa.size );

	cms >> ppa; 
	memcpy( (*out_param)->md5hash_chunk, ppa.data, ppa.size );

	cms >> ppa; 
	memcpy( (*out_param)->rcompName, ppa.data, ppa.size );

	cms >> ppa;
	memcpy( (*out_param)->r_filename, ppa.data, ppa.size );

	cms >> (*out_param)->position;
	cms >> (*out_param)->bufsize;

	cms >> ppa;
	memcpy( (*out_param)->chunk, ppa.data, ppa.size );

	return lerror=0;
}

// Сборка пакета запроса checkfile
int FileShareProtocol::gq_CheckFile_sen(unsigned char** inbuf, int& len, s_checkfile_inout* in_param)
{
	lerror=0;
	if( inbuf == NULL ) return ++lerror;
	if( *inbuf != NULL ) return ++lerror;
	if( HashB == NULL ) return ++lerror;
	if( in_param == NULL ) return ++lerror;
	if( in_param->r_profile == NULL ) return ++lerror;
	if( in_param->s_profile == NULL ) return ++lerror;

	unsigned char buf[16];
	unsigned char* pbuf = buf;
	int sizeh = genheader(&pbuf, (in_param->query) ? 6 : 7 );

	CMemStream cms;
	cms.ResetWriteB();
	cms.SetSize(2048);
	cms.SetPosition(0);

	cms << wpairarr(buf, sizeh);
	cms << in_param->iclient;
	cms << in_param->iserver;
	cms << in_param->query;
	cms << wpairarr(in_param->r_profile, sizeof(in_param->r_profile));
	cms << wpairarr(in_param->s_profile, sizeof(in_param->s_profile));
	cms << wpairarr(in_param->md5hash_file, sizeof(in_param->md5hash_file));
	cms << wpairarr(in_param->rcompName, sizeof(in_param->rcompName));
	cms << wpairarr(in_param->r_filename, sizeof(in_param->r_filename));
	cms << in_param->filelength;

	*inbuf = (unsigned char*)_aligned_malloc( len = (int)cms.GetPosition() , 16);
	memcpy( *inbuf, cms.Pointer(), len );

	return lerror=0;
}
// Сборка пакета ответа и его проверка на запрос
int FileShareProtocol::gq_CheckFile_rec(unsigned char* inbuf, int len, s_checkfile_inout** out_param)
{
	lerror=0;
	if( inbuf == NULL ) return ++lerror;
	if( HashB == NULL ) return ++lerror;
	if( out_param == NULL ) return ++lerror;

	//unsigned char buf[16];
	//unsigned char* pbuf = buf;
	//int sizeh = 8;

	CMemStream cms;
	cms.ResetReadB();
	cms.ResetWriteB();
	cms.SetSize(512);
	cms.SetPosition(0);
	cms.Write( inbuf, len );

	*out_param = (s_checkfile_inout*)_aligned_malloc( sizeof(s_checkfile_inout), 16 );

	//cms.SetPosition(0);
	//unsigned char buf[16];
	cms.SetPosition( 4 + 8 );
	rpairarr ppa;

	cms >> (*out_param)->iclient;
	cms >> (*out_param)->iserver;
	cms >> (*out_param)->query;
	cms >> ppa;
	memcpy( (*out_param)->r_profile, ppa.data, ppa.size );

	cms >> ppa; 
	memcpy( (*out_param)->s_profile, ppa.data, ppa.size );

	cms >> ppa; 
	memcpy( (*out_param)->md5hash_file, ppa.data, ppa.size );

	cms >> ppa; 
	memcpy( (*out_param)->rcompName, ppa.data, ppa.size );

	cms >> ppa;
	memcpy( (*out_param)->r_filename, ppa.data, ppa.size );

	cms >> (*out_param)->filelength;

	return lerror=0;
}