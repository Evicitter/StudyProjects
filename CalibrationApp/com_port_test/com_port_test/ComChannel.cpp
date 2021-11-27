//Date: 28/07/2015 05:38:56

#include <stdio.h>
#include <tchar.h>
#include "ComChannel.h"

ComChannel::ComChannel()
{
	portname.clear();	
	ExtBuffer.reserve(512);
}

ComChannel::~ComChannel()
{
	portname.clear();
	ExtBuffer.clear();
}

// Открытие порта.
void ComChannel::OpenPort(const wchar_t* PortName)
{
	errorstring.clear();
	portname = PortName;
	//hSerial = ::CreateFile(portname.c_str(), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING | FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 0);
	hSerial = ::CreateFile(portname.c_str(), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING | FILE_ATTRIBUTE_NORMAL, 0);
	if (hSerial == INVALID_HANDLE_VALUE)
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
			errorstring = L"serial port does not exist.\n";
		errorstring.append(L"some other error occurred.\n");
	}
}

// Настройка параметров порта.
void ComChannel::SetupPort(bool bSetDefault, int rate, int bytesize, int stopbits, int parity)
{
	errorstring.clear();

	DCB dcbSerialParams = { 0 };
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	if (!GetCommState(hSerial, &dcbSerialParams))
		errorstring = L"getting state error\n";

	if(!bSetDefault)
	{
	 dcbSerialParams.BaudRate = rate;
	 dcbSerialParams.ByteSize = bytesize;
	 dcbSerialParams.StopBits = stopbits;
	 dcbSerialParams.Parity = parity;
	 if (!SetCommState(hSerial, &dcbSerialParams))
		errorstring = L"error setting serial port state\n";
	}
}

// Настройка таймаута.
void ComChannel::SetTimeout()
{
	errorstring.clear();

	COMMTIMEOUTS m_CommTimeouts;

	if (!GetCommTimeouts(hSerial, &m_CommTimeouts))
		errorstring = L"getting timeouts error\n";
	
	m_CommTimeouts.ReadIntervalTimeout			= 50;
	m_CommTimeouts.ReadTotalTimeoutConstant		= 50;
	m_CommTimeouts.ReadTotalTimeoutMultiplier	= 10;
	m_CommTimeouts.WriteTotalTimeoutConstant	= 50;
	m_CommTimeouts.WriteTotalTimeoutMultiplier	= 10;
	SetCommTimeouts(hSerial, &m_CommTimeouts);
}

// Запись в порт
int ComChannel::WritePort(char* stream, size_t nsize)
{
	DWORD dwSize = (DWORD)nsize;
	DWORD dwBytesWritten;
	//LPOVERLAPPED ov;

	int iRet = WriteFile(hSerial, stream, dwSize, &dwBytesWritten, NULL);

    //errorstring = dwSize  << L" Bytes in string. " << dwBytesWritten << " Bytes sended. ";

	return iRet;
}

// Чтение из порта
int ComChannel::ReadPort()
{
	//LPOVERLAPPED ov;
	DWORD iSize;
	char sReceivedChar[8] = { '\0' };
	ExtBuffer.clear();
	while (true)
	{
		ReadFile(hSerial, sReceivedChar, 1, &iSize, 0);
		if (iSize > 0)
		{
			for (DWORD i = 0; i < iSize; ++i)
			{
				//wprintf_s(L"%0.2x,", (unsigned char)sReceivedChar[i]);
				ExtBuffer.push_back(sReceivedChar[i]);
			}
		}
		else
		{
			//wprintf_s(L"\n");
			break;
		}
	}
	return ExtBuffer.size();
}

// Запрос и ответ
int ComChannel::ReqResp(char* stream, size_t nsize, DWORD compause)
{
	while (1) //Get Req 1
	{
		WritePort(stream, nsize);
		Sleep(compause);
		ReadPort();
		if (ExtBuffer.size() > 0) break;
	}
	return ExtBuffer.size();
}