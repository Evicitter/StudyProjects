//Last update time : 21.07.2009 23:55:26
#include "stdafx.h"
#include <windows.h>
#include <winbase.h>

#include <Pdh.h>
#include <stdio.h>

#pragma comment(lib,"pdh.lib")

int _tmain(int argc, _TCHAR* argv[])
{
	int err = -1;
                
	PDH_STATUS  status;
	HQUERY	    perfQuery = NULL;
	HCOUNTER    uptimeCounter;
	PDH_FMT_COUNTERVALUE uptimeValue;

	// Check for NT
	//
	OSVERSIONINFO osvi;    
	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	if(!GetVersionEx(&osvi) || (osvi.dwPlatformId != VER_PLATFORM_WIN32_NT))
		return err; //unknown OS or not NT based

	if(PdhOpenQuery(NULL, 0, &perfQuery) != ERROR_SUCCESS)
		return err;

	DWORD compNameSize = 80;
	char  computerName[80]; 
	GetComputerNameA(computerName, &compNameSize);


	DWORD objectNameSize = 80;
	char  uptimeObjectName[80];
	status = PdhLookupPerfNameByIndexA(computerName, 2, uptimeObjectName, &objectNameSize);

	DWORD counterNameSize = 80;
	char  uptimeCounterName[80];
	status = PdhLookupPerfNameByIndexA(computerName, 674, uptimeCounterName, &counterNameSize);

	PDH_COUNTER_PATH_ELEMENTS_A elems;
	memset(&elems, 0, sizeof(PDH_COUNTER_PATH_ELEMENTS));
	elems.szCounterName = uptimeCounterName;
	elems.szObjectName = uptimeObjectName;
	elems.szMachineName = computerName;

	DWORD counterPathSize = 80;
	char  counterPathEx[80];

	if(PdhMakeCounterPathA(&elems, counterPathEx, &counterPathSize, 0))
		return err;         

	if(PdhAddCounterA(perfQuery, counterPathEx, 0, &uptimeCounter))
		return err;         

	if(PdhCollectQueryData(perfQuery))
		return err;

	if(PdhGetFormattedCounterValue(uptimeCounter, PDH_FMT_LARGE , NULL, &uptimeValue) != ERROR_SUCCESS) 
		return err;


	PdhCloseQuery(&perfQuery);

	DWORD seconds = (DWORD) (uptimeValue.largeValue);

	printf("System was booted %d seconds ago.\n", seconds);

	return 0;
}