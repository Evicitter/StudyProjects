//Date: 18/07/2015 13:30:09
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
public:
	ComChannel();
	~ComChannel();

	std::vector<char> ExtBuffer;

	// Открытие порта.
	void OpenPort(const wchar_t* PortName);
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
};