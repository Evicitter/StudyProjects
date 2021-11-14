//Last update time : 24.10.2010 03:33:28
#include "SevenForceLib.h"
#include <Windows.h>
#include <intrin.h>
#include <WinIoCtl.h>

#include "md5/md5.h"


//for hdd device
//#include <stdio.h> 
//#include <stdlib.h> 
//#include <stddef.h>
//#include <windows.h>  
#include <initguid.h>   // Guid definition
#include <devguid.h>    // Device guids
#include <setupapi.h>   // for SetupDiXxx functions.
#include <cfgmgr32.h>   // for SetupDiXxx functions.
//#include <winioctl.h>  
#include <ntddscsi.h>

#pragma comment(lib, "setupapi.lib")

i128 __fastcall GetHashKBD(i128 curHash, int p1, int p2)
{
 i128 rval; __stosd( (PDWORD)rval.x,0, 4);
 
 wchar_t* plex;

 md5_context ctx;
 md5_starts( &ctx );
 md5_update( &ctx, (unsigned char*)curHash.x, sizeof(curHash) );

 switch( GetKeyboardType(0) )
 {
  case 1: plex=L"IBM PC/XT or compatible (83-key) keyboard"; break;
  case 2: plex=L"Olivetti \"ICO\" (102-key) keyboard"; break;
  case 3: plex=L"IBM PC/AT (84-key) or similar keyboard"; break;
  case 4: plex=L"IBM enhanced (101- or 102-key) keyboard"; break;
  case 5: plex=L"Nokia 1050 and similar keyboards"; break;
  case 6: plex=L"Nokia 9140 and similar keyboards"; break;
  case 7: plex=L"Japanese keyboard"; break;
  default: plex=L"06f7af7cf887af78acaf78af9878545a4c545af45af"; break;
 }

 md5_update( &ctx, (unsigned char*)plex, wcslen(plex)*2 );

 p1 = GetKeyboardType(1);
 switch( p1 )
 {
  case 1: plex=L"Number of function keys - 10"; break;
  case 2: plex=L"Number of function keys - 12 (sometimes 18)"; break;
  case 3: plex=L"3Number of function keys - 10"; break;
  case 4: plex=L"Number of function keys - 12"; break;
  case 5: plex=L"5Number of function keys - 10"; break;
  case 6: plex=L"Number of function keys - 24"; break;
  case 7: plex=L"Hardware dependent and specified by the OEM"; break;
  case 81: plex=L" USB keyboard is connected to the computer"; break;
  default: plex=L"482138431627951915679521315498213461529189"; break;
 }

 p2=GetKeyboardType(2);

 md5_update( &ctx, (unsigned char*)plex, wcslen(plex)*2 );
 md5_update( &ctx, (unsigned char*)&p1, sizeof(p1) );
 md5_update( &ctx, (unsigned char*)&p2, sizeof(p2) );

 md5_finish( &ctx, rval.x );

 return rval;
}

i128 __fastcall GetHashMSE( i128 curHash, int p1, int p2 )
{
	i128 rval; __stosd( (PDWORD)rval.x,0, 4);
	md5_context ctx;
	md5_starts( &ctx );
	md5_update( &ctx, (unsigned char*)curHash.x, sizeof(curHash) );
	p1 = GetSystemMetrics(SM_MOUSEPRESENT);
	p2 = GetSystemMetrics(SM_MOUSEWHEELPRESENT);
	md5_update( &ctx, (unsigned char*)&p1, sizeof(p1) );
	md5_update( &ctx, (unsigned char*)&p2, sizeof(p2) );
	md5_finish( &ctx, rval.x );
	return rval;
}

i128 __fastcall GetHashRAM( i128 curHash, int p1, int p2 )
{
	i128 rval; __stosd( (PDWORD)rval.x,0, 4);

	md5_context ctx;
	md5_starts( &ctx );
	md5_update( &ctx, (unsigned char*)curHash.x, sizeof(curHash) );

	MEMORYSTATUSEX msx;
	msx.dwLength = sizeof(msx);
	GlobalMemoryStatusEx(&msx);
	
	md5_update( &ctx, (unsigned char*)&msx.ullTotalPhys, sizeof(msx.ullTotalPhys) );
	md5_finish( &ctx, rval.x);
	return rval;
}

i128 __fastcall GetHashCPU( i128 curHash, int p1, int p2 )
{
	i128 rval; __stosd( (PDWORD)rval.x,0, 4);
	md5_context ctx;
	md5_starts( &ctx );
	md5_update( &ctx, (unsigned char*)curHash.x, sizeof(curHash) );
	/*p1 = IsProcessorFeaturePresent(PF_3DNOW_INSTRUCTIONS_AVAILABLE)	+ __lzcnt(PF_3DNOW_INSTRUCTIONS_AVAILABLE);
	p2 = IsProcessorFeaturePresent(PF_CHANNELS_ENABLED)				+ __lzcnt(PF_CHANNELS_ENABLED);
	p1 += IsProcessorFeaturePresent(PF_COMPARE_EXCHANGE_DOUBLE)			+ __lzcnt(PF_COMPARE_EXCHANGE_DOUBLE);
	p2 += IsProcessorFeaturePresent(PF_COMPARE_EXCHANGE128)				+ __lzcnt(PF_COMPARE_EXCHANGE128);
	p1 += IsProcessorFeaturePresent(PF_COMPARE64_EXCHANGE128)			+ __lzcnt(PF_COMPARE64_EXCHANGE128);
	p2 += IsProcessorFeaturePresent(PF_FLOATING_POINT_EMULATED)			+ __lzcnt(PF_FLOATING_POINT_EMULATED);
	p1 += IsProcessorFeaturePresent(PF_FLOATING_POINT_PRECISION_ERRATA)	+ __lzcnt(PF_FLOATING_POINT_PRECISION_ERRATA);
	p2 += IsProcessorFeaturePresent(PF_MMX_INSTRUCTIONS_AVAILABLE)		+ __lzcnt(PF_MMX_INSTRUCTIONS_AVAILABLE);
	p1 += IsProcessorFeaturePresent(PF_NX_ENABLED)						+ __lzcnt(PF_NX_ENABLED);
	p2 += IsProcessorFeaturePresent(PF_PAE_ENABLED)						+ __lzcnt(PF_PAE_ENABLED);
	p1 += IsProcessorFeaturePresent(PF_RDTSC_INSTRUCTION_AVAILABLE)		+ __lzcnt(PF_RDTSC_INSTRUCTION_AVAILABLE);
	p2 += IsProcessorFeaturePresent(PF_SSE3_INSTRUCTIONS_AVAILABLE)		+ __lzcnt(PF_SSE3_INSTRUCTIONS_AVAILABLE);
	p1 += IsProcessorFeaturePresent(PF_XMMI_INSTRUCTIONS_AVAILABLE)		+ __lzcnt(PF_XMMI_INSTRUCTIONS_AVAILABLE);
	p2 += IsProcessorFeaturePresent(PF_XMMI64_INSTRUCTIONS_AVAILABLE)	+ __lzcnt(PF_XMMI64_INSTRUCTIONS_AVAILABLE);
	p1 += IsProcessorFeaturePresent(17)									+ __lzcnt(17);
	md5_update( &ctx, (unsigned char*)&p1, sizeof(p1) );
	md5_update( &ctx, (unsigned char*)&p2, sizeof(p2) );*/

	//SYSTEM_INFO si;
	//GetNativeSystemInfo( &si );
	
	int CPUInfo[4];
//	unsigned    nIds, nExIds, i;

	// __cpuid with an InfoType argument of 0 returns the number of
	// valid Ids in CPUInfo[0] and the CPU identification string in
	// the other three array elements. The CPU identification string is
	// not in linear order. The code below arranges the information 
	// in a human readable form.
	__stosd((PDWORD)CPUInfo, 0, 4);
	__cpuid(CPUInfo, 0);
	//nIds = CPUInfo[0];
	md5_update(&ctx, (unsigned char*)CPUInfo, 16);
	// Get the information associated with each valid Id
	/*for (i=1; i<=nIds; ++i)
	{
		__cpuid(CPUInfo, i);
		md5_update(&ctx, (unsigned char*)CPUInfo, 16);
	}*/
	// Calling __cpuid with 0x80000000 as the InfoType argument
	// gets the number of valid extended IDs.
	/*__cpuid(CPUInfo, 0x80000000);
	nExIds = CPUInfo[0];

	// Get the information associated with each extended ID.
	for (i=0x80000000; i<=nExIds; ++i)
	{
		__cpuid(CPUInfo, i);
		md5_update(&ctx, (unsigned char*)CPUInfo, 16);
	}*/

	// Display all the information in user-friendly format.
	/*for (i=0;;i++)
	{
		__cpuidex(CPUInfo, 0x4, i);
		if(!(CPUInfo[0] & 0xf0)) break;
		md5_update(&ctx, (unsigned char*)CPUInfo, 16);
	}*/

	md5_finish( &ctx, rval.x );
	return rval;
}

i128 __fastcall GetHashGPU( i128 curHash, int p1, int p2 )
{
	i128 rval; __stosd( (PDWORD)rval.x,0, 4);
	return rval;
}

i128 __fastcall GetHashHDD( i128 curHash, int p1, int p2 )
{
	SP_DEVICE_INTERFACE_DATA            interfaceData;
	PSP_DEVICE_INTERFACE_DETAIL_DATA    interfaceDetailData = NULL;
	STORAGE_PROPERTY_QUERY              query;
	PSTORAGE_ADAPTER_DESCRIPTOR         adpDesc;
	PSTORAGE_DEVICE_DESCRIPTOR          devDesc;
	//SCSI_PASS_THROUGH_WITH_BUFFERS      sptwb;
	HANDLE                              hDevice;
	PUCHAR                              p;
	UCHAR                               outBuf[512];
	ULONG                               length = 0, returned = 0, returnedLength;
	DWORD                               interfaceDetailDataSize, reqSize, errorCode;


	i128 rval; __stosd( (PDWORD)rval.x,0, 4);
	md5_context ctx;
	md5_starts( &ctx );
	md5_update( &ctx, (unsigned char*)curHash.x, sizeof(curHash) );

	HDEVINFO        hIntDevInfo;
	DWORD           index=0;
	BOOL            status;

	// Open the device using device interface registered by the driver
	// Get the interface device information set that contains all devices of event class.
	hIntDevInfo = SetupDiGetClassDevs ((LPGUID)&DiskClassGuid,NULL,NULL,(DIGCF_PRESENT | DIGCF_INTERFACEDEVICE));

	//  Enumerate all the disk devices
	while(true) 
	{
		//DebugPrint( 1, "Properties for Device %d", index+1);
		interfaceDetailData = NULL;
		length = 0, returned = 0, returnedLength=0;
		interfaceDetailDataSize=0; reqSize=0; errorCode=0;

		interfaceData.cbSize = sizeof (SP_INTERFACE_DEVICE_DATA);
		status = SetupDiEnumDeviceInterfaces (hIntDevInfo,0,(LPGUID)&DiskClassGuid, index, &interfaceData );
		if ( status == FALSE )
		{
			errorCode = GetLastError();
			if ( errorCode == ERROR_NO_MORE_ITEMS ) 
			{
				*((int*)rval.x)=ERROR_NO_MORE_ITEMS;
				md5_update( &ctx, (unsigned char*)rval.x, sizeof(rval) );
			}
			else {
				*((int*)rval.x)=errorCode;
				md5_update( &ctx, (unsigned char*)rval.x, sizeof(rval) );
			}
			break;
		}

		status = SetupDiGetDeviceInterfaceDetail (hIntDevInfo, &interfaceData, NULL, 0, &reqSize, NULL);
		if ( status == FALSE )
		{
			if ( GetLastError() != ERROR_INSUFFICIENT_BUFFER ) {
				*((int*)rval.x)=~ERROR_INSUFFICIENT_BUFFER;
				md5_update( &ctx, (unsigned char*)rval.x, sizeof(rval) );
				break;
			}
		}

		interfaceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc (reqSize);
		if ( interfaceDetailData == NULL ) {
			*((int*)rval.x + 2) = 0xffffffff;
			md5_update( &ctx, (unsigned char*)rval.x, sizeof(rval) );
			break;
		}
		interfaceDetailDataSize = reqSize;
		interfaceDetailData->cbSize = sizeof (SP_INTERFACE_DEVICE_DETAIL_DATA);
		status = SetupDiGetDeviceInterfaceDetail (hIntDevInfo, &interfaceData, interfaceDetailData, interfaceDetailDataSize, &reqSize, NULL);                    
		if ( status == FALSE ) {
			*((int*)rval.x + 2) = 0x22222222;
			*((int*)rval.x + 1) = 0x11111111;
			md5_update( &ctx, (unsigned char*)rval.x, sizeof(rval) );
			break;
		}

		hDevice = CreateFile(interfaceDetailData->DevicePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
		// We have the handle to talk to the device. 
		// So we can release the interfaceDetailData buffer
		free (interfaceDetailData);

		if (hDevice == INVALID_HANDLE_VALUE) {
			*((int*)rval.x + 2) = 0xffffffff;
			md5_update( &ctx, (unsigned char*)rval.x, sizeof(rval) );
			++index;
			continue; //return true;
		}

		query.PropertyId = StorageAdapterProperty;
		query.QueryType = PropertyStandardQuery;

		status = DeviceIoControl(hDevice, IOCTL_STORAGE_QUERY_PROPERTY, &query, sizeof( STORAGE_PROPERTY_QUERY ), &outBuf, 512, &returnedLength, NULL);
		if ( !status ) {
			*((int*)rval.x + 3) = 0x56455645;
			*((int*)rval.x + 2) = 0x44444444;
			*((int*)rval.x + 1) = 0x33333333;
			md5_update( &ctx, (unsigned char*)rval.x, sizeof(rval) );
		}
		else {
			adpDesc = (PSTORAGE_ADAPTER_DESCRIPTOR) outBuf;
			//"UNKNOWN"						"IEEE 1394"					"USB"
			if((adpDesc->BusType == 0) || (adpDesc->BusType == 4) || (adpDesc->BusType == 7))
			{
				CloseHandle(hDevice);
				++index;
				continue; //return To next device
			}

			md5_update( &ctx, (unsigned char*)&adpDesc->MaximumTransferLength, sizeof(adpDesc->MaximumTransferLength) );
			md5_update( &ctx, (unsigned char*)&adpDesc->MaximumPhysicalPages, sizeof(adpDesc->MaximumPhysicalPages) );
			md5_update( &ctx, (unsigned char*)&adpDesc->AlignmentMask, sizeof(adpDesc->AlignmentMask) );
			
			query.PropertyId = StorageDeviceProperty;
			query.QueryType = PropertyStandardQuery;
			status = DeviceIoControl(hDevice, IOCTL_STORAGE_QUERY_PROPERTY,	&query,	sizeof( STORAGE_PROPERTY_QUERY ),&outBuf, 512, &returnedLength,	NULL);
			if ( !status ) {
				*((int*)rval.x + 3) = 0xfcdefcde;
				*((int*)rval.x + 2) = 0xab3ab3ab;
				*((int*)rval.x + 1) = 0xbabababa;
				md5_update( &ctx, (unsigned char*)rval.x, sizeof(rval) );
			}
			else {
				//DebugPrint( 1, "\nDevice Properties\n");
				devDesc = (PSTORAGE_DEVICE_DESCRIPTOR) outBuf;
				p = (PUCHAR) outBuf;
				md5_update( &ctx, p, returnedLength );

				// Our device table can handle only 16 devices.
				//DebugPrint( 1, "Device Type     : %s (0x%X)\n", 
				//DeviceType[devDesc->DeviceType > 0x0F? 0x0F: devDesc->DeviceType ], devDesc->DeviceType);


				//md5_update( &ctx, (unsigned char*)devDesc->DeviceTypeModifier, sizeof(devDesc->DeviceTypeModifier) );
				//md5_update( &ctx, (unsigned char*)devDesc->RemovableMedia, sizeof(devDesc->RemovableMedia) );

				

				/*if ( devDesc->VendorIdOffset && p[devDesc->VendorIdOffset] ) {
					DebugPrint( 1, "Vendor ID       : " );
					for ( i = devDesc->VendorIdOffset; p[i] != (UCHAR) NULL && i < returnedLength; i++ ) {
						DebugPrint( 1, "%c", p[i] );
					}
					DebugPrint( 1, "\n");
				}
				if ( devDesc->ProductIdOffset && p[devDesc->ProductIdOffset] ) {
					//DebugPrint( 1, "Product ID      : " );
					//for ( i = devDesc->ProductIdOffset; p[i] != (UCHAR) NULL && i < returnedLength; i++ ) {
					//	DebugPrint( 1, "%c", p[i] );
					//}
					//DebugPrint( 1, "\n");
				}

				if ( devDesc->ProductRevisionOffset && p[devDesc->ProductRevisionOffset] ) {
					//DebugPrint( 1, "Product Revision: " );
					//for ( i = devDesc->ProductRevisionOffset; p[i] != (UCHAR) NULL && i < returnedLength; i++ ) {
					//	DebugPrint( 1, "%c", p[i] );
					//}
					//DebugPrint( 1, "\n");
				}

				if ( devDesc->SerialNumberOffset && p[devDesc->SerialNumberOffset] ) {
					//DebugPrint( 1, "Serial Number   : " );
					//for ( i = devDesc->SerialNumberOffset; p[i] != (UCHAR) NULL && i < returnedLength; i++ ) {
					//	DebugPrint( 1, "%c", p[i] );
					//}
					//DebugPrint( 1, "\n");
				}*/
			}
		}

		// Close handle the driver
		CloseHandle(hDevice);

		//status = GetDeviceProperty( hIntDevInfo, index );
		//if ( status == FALSE ) {
		//	break;
		//}
		index++;
	}
	//DebugPrint( 1, "\r ***  End of Device List  *** \n");
	SetupDiDestroyDeviceInfoList(hIntDevInfo);

	md5_finish(&ctx, rval.x);

	return rval;
}

i128 __fastcall GetHashNET( i128 curHash, int p1, int p2 )
{
	i128 rval; __stosd( (PDWORD)rval.x,0, 4);
	return rval;
}

i128 __fastcall GetHashMON( i128 curHash, int p1, int p2 )
{
	i128 rval; __stosd( (PDWORD)rval.x,0, 4);

	md5_context ctx;
	md5_starts( &ctx );
	md5_update( &ctx, (unsigned char*)curHash.x, sizeof(curHash) );

	DISPLAY_DEVICE dd; 
	dd.cb = sizeof(dd); 
	DWORD dev = 0; 
	// device index 
	int id = 1; 
	// monitor number, as used by Display Properties > Settings

	while (EnumDisplayDevices(0, dev, &dd, 0))
	{
		DISPLAY_DEVICE ddMon;
		__stosb((PBYTE)&ddMon, 0, sizeof(ddMon)); //ZeroMemory(&ddMon, sizeof(ddMon));
		ddMon.cb = sizeof(ddMon);
		DWORD devMon = 0;

		while (EnumDisplayDevices(dd.DeviceName, devMon, &ddMon, 0))
		{
			if (ddMon.StateFlags & DISPLAY_DEVICE_ACTIVE && 
				!(ddMon.StateFlags & DISPLAY_DEVICE_MIRRORING_DRIVER))
			{
				md5_update( &ctx, (unsigned char*)ddMon.DeviceID, sizeof(ddMon.DeviceID) );
				md5_update( &ctx, (unsigned char*)ddMon.DeviceKey, sizeof(ddMon.DeviceKey) );
			}
			devMon++;

			__stosb((PBYTE)&ddMon, 0, sizeof(ddMon));
			ddMon.cb = sizeof(ddMon);
		}

		__stosb((PBYTE)&ddMon, 0, sizeof(ddMon));
		dd.cb = sizeof(dd);
		dev++; 
	}

	md5_finish(&ctx, rval.x);
	
	return rval;
}

i128 __fastcall GetHashRSM(i128 curHash, int p1, int p2) //RSMB - 8
{
	i128 rval; __stosd( (PDWORD)rval.x,0, 4);

	/*GetSystemFirmwareTable(
		__in DWORD FirmwareTableProviderSignature,
		__in DWORD FirmwareTableID,
		__out_bcount_part_opt(BufferSize, return) PVOID pFirmwareTableBuffer,
		__in DWORD BufferSize
		);*/
typedef UINT (WINAPI *GSFT)(DWORD , DWORD , PVOID, DWORD);

	GSFT tick=NULL;
	unsigned char *newSmbiosBuffer = NULL;
	int iBufferSizeNeeded;

	md5_context ctx;
	md5_starts( &ctx );
	md5_update( &ctx, (unsigned char*)curHash.x, sizeof(curHash) );

	HMODULE hdll = LoadLibraryW(L"Kernel32.dll");
	if(hdll == INVALID_HANDLE_VALUE)
	{
		char* oo="00000000000000000000000000000000000000000000000000000000000000";
		md5_update( &ctx, (unsigned char*)&iBufferSizeNeeded, sizeof(curHash) );
		md5_update( &ctx, (unsigned char*)oo, strlen(oo) );
		md5_finish(&ctx, rval.x);
		return rval;
	}
	tick = (GSFT)GetProcAddress( hdll, "GetSystemFirmwareTable" );
	if( (tick == NULL) || (tick == INVALID_HANDLE_VALUE) )
	{
		unsigned int t = p1;
		short Fresnes[4];
		Fresnes[0] =  ( ((t = t * 214013L	+ 2531011L) >> 16) & 0x7fff );
		Fresnes[1] =  ( ((t = t * 214013L	+ 2531011L) >> 16) & 0x7fff );
		Fresnes[2] =  ( ((t = t * 214013L	+ 2531011L) >> 16) & 0x7fff );
		Fresnes[3] =  ( ((t = t * 214013L	+ 2531011L) >> 16) & 0x7fff );
		md5_update( &ctx, (unsigned char*)&iBufferSizeNeeded, sizeof(curHash) );
		md5_update( &ctx, (unsigned char*)Fresnes, sizeof(Fresnes) );
		md5_finish(&ctx, rval.x);
		return rval;
	}


	iBufferSizeNeeded = tick('RSMB', 0, 0, 0); //GetSystemFirmwareTable( 'RSMB', 0, 0, 0 );
	if( iBufferSizeNeeded<=0 )
	{
		char* oo="Diflexant00_9877_PGI0543_PCIInput8673_8766333542_Bus01_Rev98";
		md5_update( &ctx, (unsigned char*)&iBufferSizeNeeded, sizeof(curHash) );
		md5_update( &ctx, (unsigned char*)oo, strlen(oo) );
		md5_finish(&ctx, rval.x);
		return rval;
	}

	newSmbiosBuffer = (unsigned char*)malloc( iBufferSizeNeeded * sizeof(unsigned char));
	__stosb(newSmbiosBuffer, 0, iBufferSizeNeeded);

	tick('RSMB', 0, newSmbiosBuffer, iBufferSizeNeeded); //GetSystemFirmwareTable( 'RSMB', 0, newSmbiosBuffer, iBufferSizeNeeded );
	md5_update( &ctx, (unsigned char*)&iBufferSizeNeeded, sizeof(iBufferSizeNeeded) );

# define MS_RSMB_HEADER_SIZE 8
	__movsb(newSmbiosBuffer, newSmbiosBuffer + MS_RSMB_HEADER_SIZE, iBufferSizeNeeded - MS_RSMB_HEADER_SIZE);
	__stosb(newSmbiosBuffer  + iBufferSizeNeeded - MS_RSMB_HEADER_SIZE, 0, MS_RSMB_HEADER_SIZE);
	md5_update( &ctx, newSmbiosBuffer, iBufferSizeNeeded - MS_RSMB_HEADER_SIZE );

 md5_finish(&ctx, rval.x);
 return rval;
}

bool __fastcall GetVerOS_XP32( __int32 fparam, __int32 sparam )
{
	OSVERSIONINFOEX osvi;
	SYSTEM_INFO si;
	BOOL bOsVersionInfoEx;
	//DWORD dwType;

	__stosb((PBYTE)&si, 0, sizeof(SYSTEM_INFO));
	__stosb((PBYTE)&osvi, 0, sizeof(OSVERSIONINFOEX));

	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
		return true; //error

	// Call GetNativeSystemInfo if supported or GetSystemInfo otherwise.
	//GetNativeSystemInfo(&si);

	if ( VER_PLATFORM_WIN32_NT!=osvi.dwPlatformId )
		return false;

	if ( osvi.dwMajorVersion == 5 )
	{
		GetNativeSystemInfo(&si);
		if( si.wProcessorArchitecture != PROCESSOR_ARCHITECTURE_AMD64 )
			return true;
	}
 return false;
}