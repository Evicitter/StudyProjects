///////////////////////////////////////////////////////////
//  TransAppMsg.cpp
//  Implementation of the Class TransAppMsg
//  Created on:      06-май-2010 8:28:57
///////////////////////////////////////////////////////////

#include "TransAppMsg.h"
#include <Windows.h>

DWORD WINAPI IdleEndProccess( LPVOID p )
{
	register TransAppMsg* lnk = (TransAppMsg*)(p);
	unsigned char buffer[65536];

	while( !lnk->bShutDown )
	{
		if (lnk->gMS !=INVALID_HANDLE_VALUE )
		{
			DWORD nBytesRead;
			unsigned short nucSize;
			memset(buffer,0,sizeof(buffer));
			if(ReadFile(lnk->gMS, &buffer, sizeof(buffer), &nBytesRead, NULL)!=0)
			{
				nucSize = (buffer[0] << 8) | buffer[1];
				if(lnk->gCBFunc[ buffer[3] ] != NULL)
				   lnk->gCBFunc[ buffer[3] ]( buffer+4 , nucSize );
			}
		}
		Sleep(100ul);
	}
	ExitThread( 0 );
	return 0;
}





TransAppMsg::TransAppMsg(){
 gMS=gCT=NULL;
 bShutDown=bThisServer=bExtClock=bOnceWork=false;
}



TransAppMsg::~TransAppMsg(){
 bShutDown=true;
 if(gMS != NULL) CloseHandle(gMS);
 if(gCT != NULL) { TerminateThread(gCT,0); CloseHandle(gCT); }
}





/**
* Вызов функции на сервере и ожидание пакетов.
*/
bool TransAppMsg::Host(wchar_t* chanelname, bool bIsLocal, bool bIsOnce, bool bIsExternalClock, unsigned long ITimeOutForReadMSG)
{
	if( chanelname == NULL ) return false;
	bOnceWork = bIsOnce;
	bExtClock = bIsExternalClock;
	wchar_t p[256];
	wcscpy(p,L"\\\\.\\mailslot\\");
	wcscat(p,chanelname);
	gMS = CreateMailslot(p,0,ITimeOutForReadMSG,NULL);
	gCT = CreateThread(NULL,0,&IdleEndProccess, this ,0, &inThreadID);
	if(gMS == INVALID_HANDLE_VALUE) return false;
	return true;
}


/**
* Вызов функции на клиенте и отправка пакетов серверу.
*/
bool TransAppMsg::Join(wchar_t* chanelname, bool bIsLocal){
	if( chanelname == NULL ) return false;
	wchar_t p[256];
	wcscpy(p,L"\\\\.\\mailslot\\");
	wcscat(p,chanelname);
	gMS = CreateFile(p,GENERIC_WRITE,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(gMS == INVALID_HANDLE_VALUE) return false;
	gCT = NULL;
	return true;
}

/**
* Вызов функции по завершению приложения для коректного завершения компонентов порождённых классом.
*/
bool TransAppMsg::Disconnect()
{
	bShutDown=true;
	bool berret = (gCT == NULL) || (gMS == NULL);
	if (gCT != NULL)
	{
		if( TerminateThread( gCT, 0 ) == 0 )  berret = berret || true;
		if( CloseHandle(gCT) != S_OK)		berret = berret || true;
		gCT = NULL;
	}
	if (gMS != NULL)
	{	
	 if( CloseHandle(gMS) != S_OK ) berret = berret || true;
	 gMS = NULL;
	}
	return !berret;
}


/**
 * Отсылка сообщений серверу.
 */
void TransAppMsg::SendMSG(void* _buffer, unsigned short bufsize_inbyte, unsigned char flag_msg, unsigned char indexcallback)
{
	if (gMS!=INVALID_HANDLE_VALUE)
	{
		unsigned char exheader[4] = { (bufsize_inbyte>>8)&0xff, bufsize_inbyte&0xff, flag_msg, indexcallback};
		unsigned char sndmsg[65536];
		memcpy(sndmsg, exheader, sizeof(exheader));
		memcpy(sndmsg+sizeof(exheader), _buffer, bufsize_inbyte);
		DWORD dwwr;
		if (WriteFile(gMS, sndmsg, bufsize_inbyte + 4,&dwwr,NULL)!=0)
		{ //successful;
		}
	}
}


/**
 * Отсылка сообщений серверу.
 */
void TransAppMsg::SendMSG_L64(__int64 value, unsigned char flag_msg, unsigned char indexcallback)
{
	if (gMS!=INVALID_HANDLE_VALUE)
	{
		unsigned char exheader[4] = { 0, sizeof(value), flag_msg, indexcallback};
		unsigned char sndmsg[16];
		memcpy(sndmsg, exheader, sizeof(exheader));
		memcpy(sndmsg+sizeof(exheader), &value, sizeof(value));
		DWORD dwwr;
		if (WriteFile(gMS, sndmsg, sizeof(sndmsg) ,&dwwr,NULL)!=0)
		{ //successful;
		}
	}
}


/**
 * Отсылка сообщений серверу.
 */
void TransAppMsg::SendMSG_long(long value, unsigned char flag_msg, unsigned char indexcallback)
{
	if (gMS!=INVALID_HANDLE_VALUE)
	{
		unsigned char exheader[4] = { 0, sizeof(value), flag_msg, indexcallback};
		unsigned char sndmsg[8];
		memcpy(sndmsg, exheader, sizeof(exheader));
		memcpy(sndmsg+sizeof(exheader), &value, sizeof(value));
		DWORD dwwr;
		if (WriteFile(gMS, sndmsg, sizeof(sndmsg) ,&dwwr,NULL)!=0)
		{ //successful;
		}
	}
}


/**
 * Установка функции с обратным вызовом на сервере.
 * Вызывается когда приходит сообщение с индексом этой функции
 */
void TransAppMsg::SetCallBack(cbTransAppMsg funct, unsigned char index)
{
 gCBFunc[index] = funct;
}