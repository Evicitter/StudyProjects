//Last update time : 03.10.2015 15:27:50
#pragma once

#include <Windows.h>

#include <vector>
#include <string>

class ComChannel
{
protected:
	HANDLE hSerial;
	std::wstring portname;
	std::wstring errorstring;
	bool isOpen;
public:
	ComChannel();
	~ComChannel();

	std::vector<char> ExtBuffer;

	// Открытие порта.
	bool OpenPort(const wchar_t* PortName);
	// Настройка параметров порта.
	void SetupPort(bool bSetDefault = true, int rate = CBR_9600, int bytesize = 8, int stopbits = ONESTOPBIT, int parity = NOPARITY);
	// Настройка таймаута.
	void SetTimeout();

	// Запись в порт
	int WritePort(char* stream, size_t nsize);
	// Чтение из порта
	int ReadPort();

	// Запрос и ответ
	int ReqResp(char* stream, size_t nsize, DWORD compause = 250);

	// Закрытие порта.
	bool ClosePort();

	bool 			IsOpenPort() const { return isOpen; }
	std::wstring    GetErrorString() const { return errorstring; };
};

extern ComChannel GCom;