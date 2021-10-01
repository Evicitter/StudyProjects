//Last update time : 28.11.2011 07:10:07

#include "stdafx.h"
#include <Windows.h>
#include <LM.h>
#pragma comment(lib, "netapi32.lib")

//ansi---------------
#include <process.h>
//-------------------

//std----------------
#include <iostream>
#include <string>
#include <map>
#include <vector>
//-------------------

//My---------------------------
#include "TransAppMsg.h"
#include "FileShareProtocol.h"
//-----------------------------

using namespace std;

#define DefCMD_SCANU1  L"scanusers"
#define DefCMD_SCANF1  L"scanfiles"
#define DefCMD_GETF1   L"getfile"
#define DefCMD_CHECKF1 L"checkfile"
#define DefCMD_EXIT1   L"exit"

#define DefCMD_SCANU2  L"su"
#define DefCMD_SCANF2  L"sf"
#define DefCMD_GETF2   L"gf"
#define DefCMD_CHECKF2 L"cf"
#define DefCMD_EXIT2   L"e"

int scanusers(void*);
int scanfiles(void*);
int getfile(void*);
int checkfile(void*);
int updatefile(int);

struct ArgContainer
{
	HANDLE hRec;
	HANDLE hSen;
	HWND tWin;
};

unsigned __stdcall thread1_receive(void* vstack);
unsigned __stdcall thread1_send(void* vstack);

static TransAppMsg tamServer;
static TransAppMsg tamClient;
static unsigned int idserver=64;
static unsigned int idclient=64;
static wstring profileName;
static wstring serverProfile;
static int serverIDProfile;
static map<int,wstring> OtherProf;
static map<int,wstring> OtherCompName;
static vector<wstring> serverFiles;
static vector<wstring> clientFiles;

//----------------------------------
volatile bool bExitApp=false;
volatile bool bWorkInDomain=false;
wstring wsDomainName;
//----------------------------------

void create_server()
{
 if(bWorkInDomain)
 {
		wstring ss;
		ss = wstring(L"msstwin_d");
		if( tamServer.Host(ss.c_str(),false) )
		{
			idserver = (rand() | (rand() << 16)) & 127;
			idclient = idserver;
			return;
		} else { wcout << L"Сервер не зарегестрирован: " << ss << endl; }
 }
 else
 {
		wstring ss;
		wchar_t tb[16]={L'\0'};

		int count = 0;
		while (count < 64)
		{
			_itow_s(count,tb,16,10);
			ss = wstring(L"msstwin") + tb;
			if( tamServer.Host(ss.c_str()) )
			{
				idserver = count;
				idclient = count;
				break;
			} else ++count;
		}
 }
}

void broadcast(unsigned char* buf, unsigned short inbytebuf)
{
 if(bWorkInDomain)
 {
	 wstring ss;
	 ss = wstring(L"msstwin_d");
	 if( tamClient.Join(ss.c_str(), wsDomainName.c_str(), false) )
	 {
		 tamClient.SendMSG(buf, inbytebuf, (unsigned char)idclient, 0);
		 tamClient.Disconnect();
	 } else { wcout << L"Нет соединения с: " << ss << endl; }
 }
 else
 {
		wstring ss;
		wchar_t tb[16]={L'\0'};

		int count = 0;
		while (count < 64)
		{
			if( idclient == count ) { ++count; continue; } // не отправлять пакет себе

			_itow_s(count,tb,16,10);
			ss = wstring(L"msstwin") + tb;
			if( tamClient.Join(ss.c_str()) )
			{
				tamClient.SendMSG(buf, inbytebuf, (unsigned char)idclient, 0);
				tamClient.Disconnect();
			}
			++count;
		}
 }
}

bool msendto( unsigned int idserv, const wchar_t* wTo, unsigned char* buf, unsigned short inbytebuf )
{
if(bWorkInDomain)
{
	wstring ss;
	ss = wstring(L"msstwin_d");
	if( tamClient.Join(ss.c_str(), wTo, false) )
	{
		tamClient.SendMSG(buf, inbytebuf, (unsigned char)idclient, 0);
		tamClient.Disconnect();
		return true;
	} else { wcout << L"Нет соединения с: " << wTo << endl; return false; }
}
else
 {
	wstring ss = L"msstwin";
	wchar_t tb[16]={L'\0'};
	
	if(idserv == idclient) return false; //Не посылать себе

	_itow_s(idserv,tb,16,10);
	ss = ss + tb;
	if( tamClient.Join( ss.c_str() ) )
	{
		tamClient.SendMSG( buf, inbytebuf, (unsigned char)idclient, 0 );
		tamClient.Disconnect();
		return true;
	}
 }
 return false;
}

void __fastcall tss1(void* buffer, unsigned short bufsize_inbyte, unsigned char idclnt)
{
 unsigned char* pbuffer = (unsigned char*)buffer;
 unsigned char* senBuffer;
 int slen=0;
 unsigned int lerror=0;
 
 if(bWorkInDomain && (idclnt == idserver)) return; //не принимать от себя самого в домене

 pbuffer += 4;
 int headertype = globalFSC.getheader( &pbuffer, bufsize_inbyte );
 pbuffer -= 4;

 wcout << L"receive packet server(" << idserver << L") =" << L"from client(" << idclnt << L")" << endl;
 wcout << L"header type = " << headertype << endl;

 //---------------------------------------------------------------------------------------
 if( headertype == 0 )		//Отправлен запрос на scanusers
 {
	 s_scanusers_inout* psuparam = NULL;
	 lerror = globalFSC.gq_ScanUsers_rec(pbuffer,bufsize_inbyte, &psuparam);
	 if( (psuparam == NULL) || (lerror != 0) )
	 {
		 wcout << L"log: Ошибка: принятый пакет ответа ScanUsers не был собран! Код ошибки:" << lerror << endl;
		 return;
	 }

	 senBuffer = NULL;
	 s_scanusers_inout suparam;
	 suparam.iclient = idclient;
	 suparam.iserver = idclnt;		//сервер тот кому отправляем ответ, от кого приняли запрос.
	 suparam.query = 0;
	 wcscpy_s(suparam.profile,32,profileName.c_str());

	 DWORD dwsb = sizeof(suparam.rcompName);
	 GetComputerNameW( suparam.rcompName, &dwsb );

	 lerror = globalFSC.gq_ScanUsers_sen( &senBuffer, slen, &suparam );
	 if( (senBuffer == NULL) || (lerror!=0) )
	 {
		 wcout << L"Ошибка пакет ответа ScanUsers не был собран! Код ошибки:" << lerror << endl;
		 return;
	 }
	 msendto( suparam.iserver, psuparam->rcompName, senBuffer, slen );
	 globalFSC.gq_FreeResource( &senBuffer );
 } else if(headertype == 1) //Пришёл ответ на запрос scanusers
 {
	 s_scanusers_inout* psuparam = NULL;
	 lerror = globalFSC.gq_ScanUsers_rec(pbuffer,bufsize_inbyte, &psuparam);
	 if( (psuparam == NULL) || (lerror != 0) )
	 {
		 wcout << L"log: Ошибка: принятый пакет ответа ScanUsers не был собран! Код ошибки:" << lerror << endl;
		 return;
	 }
	 wcout << L"log: Пакет принят от: " << psuparam->iclient << L" Профиль: " << psuparam->profile << endl;
	 OtherProf[psuparam->iclient]=wstring(psuparam->profile);
	 OtherCompName[psuparam->iclient]=wstring(psuparam->rcompName);
	 globalFSC.gq_FreeResource( (unsigned char**)&psuparam );
 } //---------------------------------------------------------------------------------------
 else if( headertype == 2 ) //Отправлен запрос на scanfiles
 {
	 s_scanfiles_inout* getp = NULL;
	 lerror = globalFSC.gq_ScanFiles_rec(pbuffer, bufsize_inbyte, &getp);
	 if( (getp == NULL) || (lerror != 0) )
	 {
		 wcout << L"log: Ошибка: принятый пакет зпроса ScanFiles не был собран! Код ошибки:" << lerror << endl;
		 return;
	 }

	 if( wcscmp( profileName.c_str(), getp->r_profile ) != 0 )
	 {
		 wcout << L"Ошибка: Ваш профиль не совпадает с профилем принимающего:" << getp->r_profile << endl;
		 globalFSC.gq_FreeResource( (unsigned char**)&getp ); //getp Больше не нужен
		 return;
	 }

	 senBuffer = NULL;
	 s_scanfiles_inout suparam;
	 suparam.iclient = idserver;		
	 suparam.iserver = getp->iclient; //Отправляем тому от кого приняли
	 suparam.query = 0;
	 wcscpy_s(suparam.s_profile,32,profileName.c_str());
	 wcscpy_s(suparam.r_profile,32,getp->s_profile);
	 
	 DWORD dwsb = sizeof(suparam.rcompName);
	 GetComputerNameW( suparam.rcompName, &dwsb );

	 vector<wstring>::const_iterator itrf;
	 for(itrf = clientFiles.begin(); itrf != clientFiles.end(); ++itrf)
	 {
		 wcscpy_s( suparam.r_filename, 32, (*itrf).c_str() );
		 unsigned char* phash = suparam.md5hash;				//Временая переменная 
		 globalFSC.HashF( suparam.r_filename, &phash );
		 senBuffer=NULL;
		 lerror = globalFSC.gq_ScanFiles_sen( &senBuffer, slen, &suparam );
		 if( (senBuffer==NULL) || (lerror!=0) )	wcout << L"Ошибка пакет ответа ScanFiles не был собран! Код ошибки:" << lerror << endl;

		 msendto( suparam.iserver, getp->rcompName, senBuffer, slen );
		 globalFSC.gq_FreeResource( &senBuffer );
	 }
	 globalFSC.gq_FreeResource( (unsigned char**)&getp ); //getp Больше не нужен
 } //---------------------------------------------------------------------------------------
 else if( headertype == 3 ) //Пришёл ответ на запрос scanfiles
 {
	 s_scanfiles_inout* getp = NULL;
	 lerror = globalFSC.gq_ScanFiles_rec(pbuffer, bufsize_inbyte, &getp);
	 if( (getp == NULL) || (lerror != 0) )
	 {
		 wcout << L"log: Ошибка: принятый пакет ответа ScanFiles не был собран! Код ошибки:" << lerror << endl;
		 return;
	 }

	 if( wcscmp( profileName.c_str(), getp->r_profile ) != 0 )
	 {
		 wcout << L"Ошибка: Ваш профиль не совпадает с профилем принимающего:" << getp->r_profile << endl;
		 globalFSC.gq_FreeResource( (unsigned char**)&getp ); //getp Больше не нужен
		 return;
	 }

	 if( wcslen( getp->r_filename ) == 0 )
	 {
		 wcout << L"Ошибка: файл не сканирован. Длина его имени равна 0" << endl;
		 globalFSC.gq_FreeResource( (unsigned char**)&getp );
		 return;
	 }

	 serverFiles.push_back( wstring(getp->r_filename) );
	 wcout << L"Сканированый файл: " << getp->r_filename << L" от профиля: " << getp->s_profile << L" к профилю: " << getp->r_profile << endl;

	 globalFSC.gq_FreeResource( (unsigned char**)&getp );
 } //---------------------------------------------------------------------------------------
 else if( headertype == 4 ) //Отправлен запрос на getfiles
 {
	 s_getfile_inout* getp = NULL;
	 lerror = globalFSC.gq_GetFile_rec(pbuffer, bufsize_inbyte, &getp);
	 if( (getp == NULL) || (lerror != 0) )
	 {
		 wcout << L"log: Ошибка: принятый пакет зпроса GetFile не был собран! Код ошибки:" << lerror << endl;
		 return;
	 }

	 if( wcscmp( profileName.c_str(), getp->r_profile ) != 0 )
	 {
		 wcout << L"Ошибка: профиль не совпадает с принятым профилем:" << getp->r_profile << endl;
		 globalFSC.gq_FreeResource( (unsigned char**)&getp ); //getp Больше не нужен
		 return;
	 }

	 FILE* hF;
	 s_getfile_inout suparam;
	 suparam.iclient = idserver;		//Отправляем тому от кого приняли
	 suparam.iserver = getp->iclient;
	 suparam.query = 0;
	 wcscpy_s(suparam.s_profile,32,profileName.c_str());
	 wcscpy_s(suparam.r_profile,32,getp->s_profile);
	 wcscpy_s(suparam.r_filename,32,getp->r_filename);

	 DWORD dwsb = sizeof(suparam.rcompName);
	 GetComputerNameW( suparam.rcompName, &dwsb );

	 suparam.bufsize = 0;
	 suparam.position = 0;
	 memset(suparam.chunk,0,sizeof(suparam.chunk));

	 lerror=0;
	 if( GetFileAttributesW( getp->r_filename ) == -1 )
	 {
		 suparam.position = 0xffffffffffffff01; //wcscpy_s(suparam.r_filename, 256, L"##error_1##");
		 lerror=1;
	 }
	 else
	 {
		 if( _wfopen_s(&hF, getp->r_filename, L"rb") != 0 )
		 {
			 suparam.position = 0xffffffffffffff02; //wcscpy_s(suparam.r_filename, 256, L"##error_2##");
			 lerror=2;
		 }
	 }

	 if( lerror == 0 )
	 {
	  int nread;
		fpos_t tt = 0;
		fsetpos(hF, &tt);
		do 
		{ 
			suparam.position = _ftelli64(hF);
			nread = fread_s( suparam.chunk, sizeof(suparam.chunk), 1, sizeof(suparam.chunk), hF );
			suparam.bufsize = nread;

			//calc hash
			unsigned char* phash = suparam.md5hash_chunk;
			globalFSC.HashB( suparam.chunk, nread, &phash );
			//---------------------------------------------------

			senBuffer=NULL;
			lerror = globalFSC.gq_GetFile_sen( &senBuffer, slen, &suparam );
			if( (senBuffer == NULL) || (lerror != 0) )
			{
				wcout << L"Ошибка построения пакета ответа GetFile : файл:" << suparam.r_filename << L" - не был передан блок на позиции:" << suparam.position << L" Размером: " << suparam.bufsize << endl;
				lerror = 0xffffffff;
				break;
			}
			msendto( suparam.iserver, getp->rcompName, senBuffer, slen );
			globalFSC.gq_FreeResource( &senBuffer );
			lerror=0;
		} while ( nread == sizeof(suparam.chunk) );
		fclose(hF);
	 }
		
	  memset(suparam.chunk,0,sizeof(suparam.chunk));
		suparam.bufsize = 0;

		if( lerror == 0 )		//Передаём ответ что файл полностью передан
			suparam.position = 0xffffffffffffaa00;
		else if( lerror == 0xffffffff)	//Говорим что передача файла оборвалась
			suparam.position = 0xffffffffffffaa01;
		
		senBuffer=NULL;
		lerror = globalFSC.gq_GetFile_sen( &senBuffer, slen, &suparam );
		if( (senBuffer == NULL) || (lerror != 0) )
		{
			wcout << L"Ошибка: построения пакета ответа GetFile: ответ о завершении передачи файла не был передан." << endl;
			globalFSC.gq_FreeResource( (unsigned char**)&getp ); //getp Больше не нужен
			return;
		}
		msendto( suparam.iserver, getp->rcompName, senBuffer, slen );
		globalFSC.gq_FreeResource( &senBuffer );
		globalFSC.gq_FreeResource( (unsigned char**)&getp ); //getp Больше не нужен
 } //---------------------------------------------------------------------------------------
 else if( headertype == 5 ) //Приёшл ответ на getfiles
 {
	 s_getfile_inout* getp = NULL;
	 lerror = globalFSC.gq_GetFile_rec(pbuffer, bufsize_inbyte, &getp);
	 if( (getp == NULL) || (lerror != 0) )
	 {
		 wcout << L"log: Ошибка: принятый пакет ответа GetFile не был собран! Код ошибки:" << lerror << endl;
		 return;
	 }

	 if( wcscmp( profileName.c_str(), getp->r_profile ) != 0 )
	 {
		 wcout << L"Ошибка: профиль не совпадает с принятым профилем:" << getp->r_profile << endl;
		 globalFSC.gq_FreeResource( (unsigned char**)&getp ); //getp Больше не нужен
		 return;
	 }

	 if( wcslen( getp->r_filename ) == 0 )
	 {
		 wcout << L"Ошибка: файл не принят длина его имени равна 0" << endl;
		 globalFSC.gq_FreeResource( (unsigned char**)&getp );
		 return;
	 }

	 //error register ======================
	 lerror=0;
	 if( getp->position == 0xffffffffffffaa01 ) //передача файла оборвалась
	 {
		 wcout << L"Ошибка: Передача файла:" << getp->r_filename << L" прервалась" << endl;
		 lerror = 1;
	 }
	 if( getp->position == 0xffffffffffffff01 )
	 {
		 wcout << L"Ошибка: Файл не был найден у профиля:" << getp->s_profile << endl;
		 lerror = 2;
	 }
	 if( getp->position == 0xffffffffffffff02 )
	 {
		 wcout << L"Ошибка: Файл не был успешно открыт у профиля:" << getp->s_profile << L" возможно файл заблокирован" << endl;
		 lerror = 2;
	 }
	 if( getp->position == 0xffffffffffffaa00 )
	 {
		 wcout << L"log: Конец передачи файла:" << getp->r_filename << endl;
		 lerror = 0x11111111;
	 }
	 //===================================================

	 if( lerror != 0 )
	 {
		globalFSC.gq_FreeResource( (unsigned char**)&getp );
		return;
	 }

	 //calc hash
	 unsigned char testhash[16];
	 unsigned char* phash = testhash;
	 globalFSC.HashB( getp->chunk, getp->bufsize, &phash );
	 
	 if( memcmp( testhash, getp->md5hash_chunk, 16 ) != 0)
	 {
		 wcout << L"Ошибка: файл:" << getp->r_filename << L" данные в позиции:" << getp->position << L" повреждены." << endl;
		 globalFSC.gq_FreeResource( (unsigned char**)&getp );
		 return;
	 }
//------------------------------------------------------------
	 FILE* tF;
	 if( _wfopen_s(&tF, getp->r_filename, L"ab") != 0 )
	 {
		 wcout << L"Ошибка: файл:" << getp->r_filename << L" не был открыт для записи" << endl;
		 globalFSC.gq_FreeResource( (unsigned char**)&getp );
		 return;
	 }

	 fsetpos(tF, &getp->position);
	 fwrite( getp->chunk, 1, getp->bufsize, tF );
	 fclose(tF);
	 
	 globalFSC.gq_FreeResource( (unsigned char**)&getp );
	 return;
 } //---------------------------------------------------------------------------------------
 else if( headertype == 6 ) //Отправлен запрос на checkfiles
 {
	 s_checkfile_inout* getp = NULL;
	 lerror = globalFSC.gq_CheckFile_rec(pbuffer, bufsize_inbyte, &getp);
	 if( (getp == NULL) || (lerror != 0) )
	 {
		 wcout << L"log: Ошибка: принятый пакет зпроса CheckFile не был собран! Код ошибки:" << lerror << endl;
		 return;
	 }

	 if( wcscmp( profileName.c_str(), getp->r_profile ) != 0 )
	 {
		 wcout << L"Ошибка: Ваш профиль не совпадает с профилем принимающего:" << getp->r_profile << endl;
		 globalFSC.gq_FreeResource( (unsigned char**)&getp ); //getp Больше не нужен
		 return;
	 }

	 FILE* hF;
	 s_checkfile_inout suparam;
	 suparam.iclient = idserver;		//Отправляем тому от кого приняли
	 suparam.iserver = getp->iclient;
	 suparam.query = 0;
	 wcscpy_s(suparam.s_profile,32,profileName.c_str());
	 wcscpy_s(suparam.r_profile,32,getp->s_profile);
	 wcscpy_s(suparam.r_filename,32,getp->r_filename);

	 DWORD dwsb = sizeof(suparam.rcompName);
	 GetComputerNameW( suparam.rcompName, &dwsb );

	 lerror=0;
	 if( GetFileAttributesW( getp->r_filename ) == -1 )
	 {
		 suparam.filelength = 0xffffffffffffff01; //wcscpy_s(suparam.r_filename, 256, L"##error_1##");
		 lerror=1;
	 }
	 else
	 {
		 if( _wfopen_s(&hF, getp->r_filename, L"rb") != 0 )
		 {
			 suparam.filelength = 0xffffffffffffff02; //wcscpy_s(suparam.r_filename, 256, L"##error_2##");
			 lerror=2;
		 }
	 }

	 if( lerror == 0 )
	 {
		 unsigned char thash[16]={0};

		 _fseeki64(hF, 0, SEEK_END);
		 suparam.filelength = _ftelli64(hF);
		 fclose(hF);
		 
		 unsigned char* phash = suparam.md5hash_file;
		 if( globalFSC.HashF( suparam.r_filename, &phash ) != 0 )
			 lerror = 0xffffffff;
	 }
	 
	 if( lerror == 0 )		//Передаём ответ что файл полностью передан
	 {
	 } else if( lerror == 0xffffffff)	//Говорим что передача файла оборвалась
	 {
		 suparam.filelength = 0xffffffffffffaa01;
	 }

	 senBuffer=NULL;
	 lerror = globalFSC.gq_CheckFile_sen( &senBuffer, slen, &suparam );
	 if( (senBuffer == NULL) || (lerror != 0) )
	 {
		 wcout << L"Ошибка: построения пакета ответа CheckFile: ответ о завершении передачи файла не был передан." << endl;
		 return;
	 }
	 msendto( suparam.iserver, getp->rcompName, senBuffer, slen );
	 globalFSC.gq_FreeResource( &senBuffer );
	 globalFSC.gq_FreeResource( (unsigned char**)&getp ); //getp Больше не нужен
 } //---------------------------------------------------------------------------------------
 else if( headertype == 7 ) //Принят ответ на запрос checkfiles
 {
	 s_checkfile_inout* getp = NULL;
	 lerror = globalFSC.gq_CheckFile_rec(pbuffer, bufsize_inbyte, &getp);
	 if( (getp == NULL) || (lerror != 0) )
	 {
		 wcout << L"log: Ошибка: принятый пакет ответа CheckFile не был собран! Код ошибки:" << lerror << endl;
		 return;
	 }

	 if( wcscmp( profileName.c_str(), getp->r_profile ) != 0 )
	 {
		 wcout << L"Ошибка: Ваш профиль не совпадает с профилем принимающего:" << getp->r_profile << endl;
		 globalFSC.gq_FreeResource( (unsigned char**)&getp ); //getp Больше не нужен
		 return;
	 }

	 if( wcslen( getp->r_filename ) == 0 )
	 {
		 wcout << L"Ошибка: файл не принят. Длина его имени равна 0" << endl;
		 globalFSC.gq_FreeResource( (unsigned char**)&getp );
		 return;
	 }

	 //error register ======================
	 lerror=0;
	 if( getp->filelength == 0xffffffffffffaa01 ) //передача файла оборвалась
	 {
		 wcout << L"Ошибка: Передача файла:" << getp->r_filename << L" прервалась" << endl;
		 lerror = 1;
	 }
	 if( getp->filelength == 0xffffffffffffff01 )
	 {
		 wcout << L"Ошибка: Файл не был найден у профиля:" << getp->s_profile << endl;
		 lerror = 2;
	 }
	 if( getp->filelength == 0xffffffffffffff02 )
	 {
		 wcout << L"Ошибка: Файл не был успешно открыт у профиля:" << getp->s_profile << L" возможно файл заблокирован" << endl;
		 lerror = 2;
	 }
	 //===================================================

	 if( lerror != 0 )
	 {
		 globalFSC.gq_FreeResource( (unsigned char**)&getp );
		 return;
	 }

	 if( GetFileAttributesW(getp->r_filename) == -1 )
	 {
		 wcout << L"Ошибка: файл:" << getp->r_filename << L" не существует в папке" << endl;
		 return;
	 }

	 FILE* tF;
	 if( _wfopen_s(&tF, getp->r_filename, L"rb") != 0 )
	 {
		 wcout << L"Ошибка: файл:" << getp->r_filename << L" не был открыт для чтения быть может он заблокирован" << endl;
		 globalFSC.gq_FreeResource( (unsigned char**)&getp );
		 return;
	 }

	 _fseeki64(tF, 0, SEEK_END);
	 long long s_filelength = _ftelli64(tF);
	 fclose(tF);
	 if( getp->filelength != s_filelength )
	 {
		 wcout << L"log: разный размер файла:" << getp->r_filename << L" профиль:" << getp->s_profile << L" размер(";
		 wcout << getp->filelength << L") профиль:" << getp->r_profile << L" размер(" << s_filelength << L")" << endl;
		 return;
	 }

	 //calc hash
	 unsigned char thash[16];
	 unsigned char* phash = thash;
	 globalFSC.HashF(getp->r_filename, &phash);
	 //----------------------------------------

	 if( memcmp(thash, getp->md5hash_file, 16) != 0 )
	 {
		 wcout << L"log: разное содержание файла:" << getp->r_filename << endl;
		 wcout << L"профиль:" << getp->s_profile << L" hashLE(";
		 for(int i=0; i<16;++i)
		  wprintf_s(L"%0.2x", getp->md5hash_file[i]);
		 wcout << L")" << endl;
		 wcout << L"профиль:" << getp->r_profile << L" hashLE(";
		 for(int i=0; i<16;++i)
			 wprintf_s(L"%0.2x", thash[i]);
		 wcout << L")" << endl;
		 return;
	 } else
	 {
		 wcout << L"log: Файл индентичен:" << getp->r_filename << L" профиль:" << getp->s_profile << L" профиль:" << getp->r_profile << endl;
	 }

	 globalFSC.gq_FreeResource( (unsigned char**)&getp );
	 return;
 }
}

void __fastcall tss2(void* buffer, unsigned short bufsize_inbyte, unsigned char flag)
{
}
void __fastcall tss3(void* buffer, unsigned short bufsize_inbyte, unsigned char flag)
{
}
void __fastcall tss4(void* buffer, unsigned short bufsize_inbyte, unsigned char flag)
{
}
void __fastcall tss5(void* buffer, unsigned short bufsize_inbyte, unsigned char flag)
{
}

int wmain(int argc, wchar_t* argv[])
{
	srand( (unsigned)time(NULL) );
	_wsetlocale(LC_ALL, L"RUS");

	tamServer.SetCallBack(&tss1,0);
	tamServer.SetCallBack(&tss2,1);
	tamServer.SetCallBack(&tss3,2);
	tamServer.SetCallBack(&tss4,3);
	tamServer.SetCallBack(&tss5,4);

	DWORD typeFile;

	wcout << L"Введите имя профиля: ";
	wcin >> profileName;
	profileName = profileName.substr(0,30);

	typeFile = GetFileAttributesW( profileName.c_str() );
	if( typeFile == -1 )
	{
		if( CreateDirectoryW( profileName.c_str(), NULL ) )
				wcout << L"Создан профиль: " << profileName << endl;
	}
	else if( typeFile & FILE_ATTRIBUTE_DIRECTORY  )
		wcout << L"Установлен профиль: " << profileName << endl;
	else
	{
		wcout << L"Неизвестная ошибка: Профиль не создан/не установлен." << endl;
		_wsystem(L"PAUSE");
		return 1;
	}

	if( SetConsoleTitleW( profileName.c_str() ) )
	{
		wcout << L"Соединение установлено." << endl;
	}

	wchar_t cdir[256]={L'\0'};
	GetCurrentDirectoryW(sizeof(cdir),cdir);
	wcscat_s( cdir, 256, L"\\" );
	wcscat_s( cdir, 256, profileName.c_str());
	if( !SetCurrentDirectoryW( cdir ) )
	{
		wcout << L"Ошибка: Директория для профиля не установлена" << endl;
		//return 0;
	}

	//поиск файлов -----------------------------------
	updatefile(0);
	//------------------------------------------------

	//--------------------------------------------------------------
	HWND hwnd;
	HWND ThisWindow=NULL;
	wchar_t p[512];
	hwnd = GetDesktopWindow();
	hwnd = GetWindow(hwnd, GW_CHILD);
	while (hwnd !=0)
	{
		hwnd = GetWindow(hwnd, GW_HWNDNEXT);
		GetWindowText(hwnd,p,512);
		if ( (wcslen(p)>0) && (_wcsicmp(p, profileName.c_str())==0) )
		{
			wcout << L"Валидность пройденна: " << p << endl;
			ThisWindow = hwnd;
			break;
		}
	}
	//----------------------------------------------------------------

	wstring yesno;
	wcout << L"Работать в домене? (yes/no) : "; wcin >> yesno; wcout << endl;
	if( yesno.compare(L"yes")==0 )
	{
		bWorkInDomain = true;

		DWORD dwLevel = 102;
		LPWKSTA_INFO_102 pBuf = NULL;
		LMSTR pszServerName = NULL;

		if(NetWkstaGetInfo(pszServerName, dwLevel, (LPBYTE *)&pBuf) == NERR_Success)
		{
			wsDomainName = pBuf->wki102_langroup;
			wcout << L"Домен: " << wsDomainName << endl;
		}
		else
		{
			wcout << L"Не удалось получить имя рабочей группы" << endl;
			wcout << L"Введите рабочую группу вручную: "; wcin >> yesno; wcout << endl;
		}
	}
	else
	{
		bWorkInDomain = false;
		wcout << L"Режим работы: локальный" << endl;
	}
	
	ArgContainer sAC;

	sAC.tWin = ThisWindow;

	_beginthreadex(NULL, 0, &thread1_receive, &sAC, 0, NULL);
	_beginthreadex(NULL, 0, &thread1_send, &sAC, 0, NULL);

	create_server();
	if( globalFSC.setup( idserver, idclient, profileName.c_str() ) != 0)
	{
		wcout << L"Протокол не установлен" << endl;
		Sleep(200);
	}

	wstring wcmd;
	bool bRealLoop=true;
	while ( bRealLoop )
	{
		wcin >> wcmd;

		if( (wcmd == DefCMD_SCANU1) || (wcmd == DefCMD_SCANU2) )
		{
			scanusers(&sAC);
		} else if( (wcmd == DefCMD_SCANF1) || (wcmd == DefCMD_SCANF2) )
		{
			scanfiles(&sAC);
		} else if( (wcmd == DefCMD_GETF1) || (wcmd == DefCMD_GETF2) )
		{
			getfile(&sAC);
		} else if( (wcmd == DefCMD_CHECKF1) || (wcmd == DefCMD_CHECKF2) )
		{
			checkfile(&sAC);
		} else if( (wcmd == DefCMD_EXIT1) || (wcmd == DefCMD_EXIT2) )
		{
			bRealLoop = false;
			bExitApp = true;
		} else if ( wcmd == L"cls" )
		{
			_wsystem(L"cls");
		} else
		{
			wcout << L"Неизвестная команда: поддерживаемые команды =" << endl;
			wcout << DefCMD_SCANU1 << L" или " << DefCMD_SCANU2 << L" - сканировать пользователей" << endl;
			wcout << DefCMD_SCANF1 << L" или " << DefCMD_SCANF2 << L" - сканировать файлы пользователя" << endl;
			wcout << DefCMD_GETF1 << L" или " << DefCMD_GETF2 << L" - получить файл пользователя" << endl;
			wcout << DefCMD_CHECKF1 << L" или " << DefCMD_CHECKF2 << L" - проверить файл пользователя" << endl;
			wcout << DefCMD_EXIT1 << L" или " << DefCMD_EXIT2 << L" - выйти из программы" << endl;
			wcout << endl;
		}
	}

	tamServer.Disconnect();

	//if( TerminateThread(sAC.hRec,0) ) CloseHandle(sAC.hRec);
	//if( TerminateThread(sAC.hSen,0) ) CloseHandle(sAC.hSen);

	_wsystem(L"PAUSE");

	return 0;
}

//------------------------------------------------------------------------
int scanusers(void* param)
{
 ArgContainer* psAC = (ArgContainer*)param;
 if(psAC == NULL) return 1;

 unsigned char* ppacket=NULL;
 int lenpacket;
 int lerror=0;

 s_scanusers_inout suparam;

 suparam.iclient = idclient;
 suparam.iserver = idserver;
 suparam.query = 1;
 wcscpy_s(suparam.profile,32,profileName.c_str());
 memset(suparam.md5hash,0,16);

 DWORD dwsb = sizeof(suparam.rcompName);
 GetComputerNameW( suparam.rcompName, &dwsb );

 lerror = globalFSC.gq_ScanUsers_sen( &ppacket, lenpacket, &suparam );
 if( (ppacket == NULL) || (lerror!=0) )
 {
	 wcout << L"Ошибка пакет запрос ScanUsers не был собран! Код ошибки: " << lerror << endl;
	 return 2;
 }

  OtherProf.erase(OtherProf.begin(),OtherProf.end());
	OtherCompName.erase(OtherCompName.begin(),OtherCompName.end());

	broadcast(ppacket, lenpacket);

 globalFSC.gq_FreeResource( &ppacket );

 return 0;
}

//2222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222
int scanfiles(void* param)
{
 ArgContainer* psAC = (ArgContainer*)param; 
 if(psAC == NULL) return 1;

 wcout << L"Пользователи:" << endl;
 
 map<int,wstring>::const_iterator itr;
 for(itr=OtherProf.begin(); itr!=OtherProf.end(); ++itr)
	 wcout << L"§\t" << (*itr).second << endl;

 bool		 bfind=false;
 wstring ucmd;
 wcout << L"Выберите пользователя:\t"; wcin >> ucmd; wcout<<endl;
 
 for(itr=OtherProf.begin(); itr!=OtherProf.end(); ++itr)
 {
	if( (*itr).second == ucmd )
	{
		serverProfile = (*itr).second;
		serverIDProfile = (*itr).first;

		unsigned char* ppacket=NULL;
		int lenpacket;
		int lerror=0;

		s_scanfiles_inout suparam;

		suparam.iclient = idclient;
		suparam.iserver = (*itr).first;
		suparam.query = 1;
		wcscpy_s(suparam.s_profile,32,profileName.c_str());
		wcscpy_s(suparam.r_profile,32,(*itr).second.c_str());
		memset(suparam.md5hash,0,16);
		memset(suparam.r_filename,0,sizeof(suparam.r_filename));

		DWORD dwsb = sizeof(suparam.rcompName);
		GetComputerNameW( suparam.rcompName, &dwsb );

		lerror = globalFSC.gq_ScanFiles_sen( &ppacket, lenpacket, &suparam );
		if( (ppacket == NULL) || (lerror!=0) )
		{
			wcout << L"Ошибка пакет запрос ScanFiles не был собран! Код ошибки: " << lerror << endl;
			return 2;
		}

		serverFiles.erase(serverFiles.begin(),serverFiles.end());

		msendto(suparam.iserver, OtherCompName[suparam.iserver].c_str(), ppacket, lenpacket);

		globalFSC.gq_FreeResource( &ppacket );
		bfind=true;
	}
 }

 if(!bfind)
	 wcout << L"Внимание: профиль: " << ucmd << L" Не найден!" << endl;

 return 0;
}

//3333333333333333333333333333333333333333333333333333333333333333333333333333333333333333
int getfile(void* param)
{
 ArgContainer* psAC = (ArgContainer*)param;
 if(psAC == NULL) return 1;

 //scanfiles(param);
 //------------------
 //------------------

 wcout << L"Файлы:" << endl;

 vector<wstring>::const_iterator itr;
 for(itr=serverFiles.begin(); itr!=serverFiles.end(); ++itr)
	 wcout << L"€@\t" << (*itr).c_str() << endl;

 bool		 bfind=false;
 wstring ucmd;
 wcout << L"Выберите файл:\t"; wcin >> ucmd; wcout<<endl;

 for(itr=serverFiles.begin(); itr!=serverFiles.end(); ++itr)
 {
	if( (*itr) == ucmd  )
	{
		unsigned char* ppacket=NULL;
		int lenpacket;
		int lerror=0;

		s_getfile_inout suparam;

		suparam.iclient = idclient;
		suparam.iserver = serverIDProfile;
		suparam.query = 1;
		wcscpy_s(suparam.s_profile,32,profileName.c_str());
		wcscpy_s(suparam.r_profile,32,serverProfile.c_str());
		wcscpy_s(suparam.r_filename,256, (*itr).c_str());
		memset(suparam.md5hash_chunk,0,16);
		memset(suparam.chunk,0,sizeof(suparam.chunk));
		suparam.position = 0;

		DWORD dwsb = sizeof(suparam.rcompName);
		GetComputerNameW( suparam.rcompName, &dwsb );

		lerror = globalFSC.gq_GetFile_sen( &ppacket, lenpacket, &suparam );
		if( (ppacket == NULL) || (lerror!=0) )
		{
			wcout << L"Ошибка пакет запрос GetFiles не был собран! Код ошибки: " << lerror << endl;
			return 2;
		}

		msendto(suparam.iserver, OtherCompName[suparam.iserver].c_str(), ppacket, lenpacket);

		globalFSC.gq_FreeResource( &ppacket );
		bfind=true;
	}
 }

 if(!bfind)
	 wcout << L"Внимание: файл: " << ucmd << L" Не найден!" << endl;

 return 0;
}

//4444444444444444444444444444444444444444444444444444444444444444444444444444444444444444
int checkfile(void* param)
{
	ArgContainer* psAC = (ArgContainer*)param;
	if(psAC == NULL) return 1;

	//scanfiles(param);
	//------------------
	//------------------

	wcout << L"Файлы:" << endl;

	vector<wstring>::const_iterator itr;
	for(itr=serverFiles.begin(); itr!=serverFiles.end(); ++itr)
		wcout << L"€@\t" << (*itr).c_str() << endl;

	bool		bfind=false;
	wstring ucmd;
	wcout << L"Выберите файл:\t"; wcin >> ucmd; wcout<<endl;

	for(itr=serverFiles.begin(); itr!=serverFiles.end(); ++itr)
	{
		if( (*itr) == ucmd  )
		{
			unsigned char* ppacket=NULL;
			int lenpacket;
			int lerror=0;

			s_checkfile_inout suparam;

			suparam.iclient = idclient;
			suparam.iserver = serverIDProfile;
			suparam.query = 1;
			wcscpy_s(suparam.s_profile,32,profileName.c_str());
			wcscpy_s(suparam.r_profile,32,serverProfile.c_str());
			wcscpy_s(suparam.r_filename,256, (*itr).c_str());
			memset(suparam.md5hash_file,0,16);
			suparam.filelength = 0;

			DWORD dwsb = sizeof(suparam.rcompName);
			GetComputerNameW( suparam.rcompName, &dwsb );

			lerror = globalFSC.gq_CheckFile_sen( &ppacket, lenpacket, &suparam );
			if( (ppacket == NULL) || (lerror!=0) )
			{
				wcout << L"Ошибка пакет запрос CheckFile не был собран! Код ошибки: " << lerror << endl;
				return 2;
			}

			msendto(suparam.iserver, OtherCompName[suparam.iserver].c_str(), ppacket, lenpacket);

			globalFSC.gq_FreeResource( &ppacket );
			bfind=true;
		}
	}

	if(!bfind)
		wcout << L"Внимание: файл: " << ucmd << L" Не найден!" << endl;

	return 0;
}

int updatefile(int typeupd)
{
	//поиск файлов -----------------------------------------------
	WIN32_FIND_DATAW wfd;
	memset(&wfd,0,sizeof(wfd));
	clientFiles.erase(clientFiles.begin(), clientFiles.end());
	HANDLE hFFF = FindFirstFileW( L"*.*", &wfd );
	if( hFFF != INVALID_HANDLE_VALUE )
	{
		do 
		{
			if( !(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
		 {
			 if(typeupd==0) wcout << L"В профиле найден файл: " << wfd.cFileName << endl;
			 clientFiles.push_back( wstring(wfd.cFileName) );
		 }
		} while ( FindNextFileW(hFFF, &wfd) );
		FindClose( hFFF );
	} else
	{
		wcout << L"FindFirstFileW тип обновления:" << typeupd << endl;
		return 1;
	}
	return 0;
	//------------------------------------------------------------
}

unsigned __stdcall thread1_receive(void* vstack)
{
	ArgContainer* psAC = (ArgContainer*)vstack;
	if( (psAC != NULL) && (psAC->tWin) )
	{
		psAC->hRec = GetCurrentThread();
	}

	while ( !bExitApp )
	{
		Sleep(1000ul);
		updatefile(1);
	}

	_endthreadex(0);
	return 0;
}

unsigned __stdcall thread1_send(void* vstack)
{
	ArgContainer* psAC = (ArgContainer*)vstack;
	if( (psAC != NULL) && (psAC->tWin) )
	{
		psAC->hSen = GetCurrentThread();
	}

	//while ( !bExitApp )
	//{
	//	Sleep(500ul);
	//}

	_endthreadex(0);
	return 0;
}