//Date: 24/08/2015 03:16:22

#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include <vector>
#include "GLOG.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	GLOG MainDevice(L"COM1");
	
	MainDevice.Run();

	/*Omnicom  sensor;
	Gadjet   Bob;
	Terminal Alisa;

	Bob.Connect(&sensor);
	Alisa.Connect(&Bob);

	int counter1 = 0;
	while (1)
	{
		sensor.Gen();
		Sleep(30);
		Bob.Tick();
		Sleep(50);
		Alisa.Tick();
		++counter1;
		//if(counter1 & 31 == 0) Alisa.Send
	}*/

	_wsystem(L"PAUSE");

	//	char resp[] = { 0x3E, 0x00, 0x06, 0x1E, 0x19, 0x00, 0xE6, 0x0F, 0x1C };

	return 0;
}