//Last update time : 28.07.2015 04:09:52
#include "stdafx.h"
#include "GLOG.h"
#include "Protocols.h"

/*
#include <windows.h>
void clrscr(void)
{
	COORD coords = { 0, 0 };
	HANDLE = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(handle, &csbi);
	DWORD sz = csbi.dwSize.X * csbi.dwSize.Y;
	DWORD dw;
	FillConsoleOutputCharacter(handle, ' ', sz, coords, &dw);
	FillConsoleOutputAttribute(handle, csbi.wAttributes, sz, coords, &dw);
}*/

GLOG::GLOG(wstring portname)
{
	cc.OpenPort(portname.c_str());
}

GLOG::~GLOG()
{
}

int GLOG::Run()
{
	cc.SetupPort(false, 19200, 8, 1);
	cc.SetTimeout();

	//Get Device info
	//-------------------

	//Send NULL Packet
	//cc.WritePort();
	//cc.ReadPort();
	//cc.ExtBuffer;

	prot = new pOmnicomm();
	if(prot == NULL) return -1;

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);  // Получение дескриптора устройства стандартного вывода

	CONSOLE_SCREEN_BUFFER_INFO bi;
	
	while(1)
	{
		int packetsize = prot->Pack(0, 0xFF, 5);
		cc.ReqResp((char*)prot->container, packetsize);

		memcpy_s(prot->container, sizeof(prot->container), &cc.ExtBuffer[0], cc.ExtBuffer.size());
		prot->UnPack(0, 0xFF, 5);
		
		GetConsoleScreenBufferInfo(hConsole, &bi);
		DWORD sz = bi.dwSize.X * bi.dwSize.Y;
		DWORD dw;

		prot->print();

		COORD coords = { 0, 0 };
		FillConsoleOutputCharacter(hConsole, ' ', sz, coords, &dw);
		FillConsoleOutputAttribute(hConsole, bi.wAttributes, sz, coords, &dw);
		SetConsoleCursorPosition(hConsole, bi.dwCursorPosition);
	}
	return 0;
}