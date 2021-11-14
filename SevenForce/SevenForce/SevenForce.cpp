//Last update time : 24.10.2010 03:35:13

#include "stdafx.h"
#include "SevenForce.h"
#include "SevenForceDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "..\\SevenForceLib\\SevenForceLib.h"
#include <intrin.h>

BEGIN_MESSAGE_MAP(CSevenForceApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

CSevenForceApp::CSevenForceApp()
{
}

CSevenForceApp theApp;

BOOL CSevenForceApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);
	CWinAppEx::InitInstance();

	CSevenForceDlg dlg;
	m_pMainWnd = &dlg;

	i128 dCmpHash[7]; __stosd( (PDWORD)dCmpHash, 0 , sizeof(dCmpHash)/4);

#if 0
	CStdioFile csf;
	csf.Open(L"Test.txt", CFile::modeCreate | CFile::modeWrite | CFile::typeText);
#define WriteStr(x) { csf.WriteString(x); csf.Close(); }
#else
#define WriteStr(x) 
#endif


	HMODULE hMod=GetModuleHandleW(NULL);
	// если ресурс лежит в другом файле, то параметр для GetModuleHandle
	// можно получить с помощью LoadLibrary(...);
	if(!hMod)
	{
	} 
	//Получаем дескриптор информационного блока нашего ресурса 
	HRSRC hRes[7];
	hRes[0]=FindResourceW(hMod,(LPCWSTR)131,RT_RCDATA);
	hRes[1]=FindResourceW(hMod,(LPCWSTR)132,RT_RCDATA);
	hRes[2]=FindResourceW(hMod,(LPCWSTR)133,RT_RCDATA);
	hRes[3]=FindResourceW(hMod,(LPCWSTR)134,RT_RCDATA);
	hRes[4]=FindResourceW(hMod,(LPCWSTR)135,RT_RCDATA);
	hRes[5]=FindResourceW(hMod,(LPCWSTR)136,RT_RCDATA);
	hRes[6]=FindResourceW(hMod,(LPCWSTR)137,RT_RCDATA);
	//Если ResurceName не было определено как число, то ResurceName 
	//берется в кавычках. Иначе надо (LPSTR)ResurceName
	//Если тип ресурса был задан как WAVE, то последний параметр должен
	// быть "WAVE" - в кавычках!
	if(!hRes[0] || !hRes[1] || !hRes[2] || !hRes[3] || !hRes[4] || !hRes[5] || !hRes[6]){
		WriteStr(L"FindResourceW\n");
		return FALSE;
	}
	//Теперь загружаем ресурс в память, получая дескриптор загру-
	//женного блока памяти.
	int iterres=0;
	while(true)
	{
		if(iterres > 6) break;

		HGLOBAL hGlob=LoadResource(hMod,hRes[iterres]);
		//Опять же, для ресурса в "текущем" модуле, первым параметром можно
		//передавать NULL
		if(!hGlob) {
			CString cs; cs.Format(L"LoadResource on %i\n", iterres);
			WriteStr(cs);
			return FALSE;
		}
		//И, наконец, последнее - получаем указатель на начало массива
		BYTE *lpbArray=(BYTE*)LockResource(hGlob); 
		if(!lpbArray)
		{
			WriteStr(L"LockResource\n");
			return FALSE;
		}

	  if( SizeofResource(hMod,hRes[iterres]) != 16 )
		{
			WriteStr(L"SizeofResource\n");
			return FALSE;
		}

		__movsd((PDWORD)(&dCmpHash[iterres]), (PDWORD)lpbArray, 4);

		FreeResource(hGlob);
		++iterres;
	}

	i128 d[7];__stosd( (PDWORD)d, 0 , sizeof(d)/4);
	register DWORD64 pppt = __rdtsc(); pppt = (pppt >> 32) + (pppt & 0x00000000ffffffff);
	srand( pppt );

	d[0] = GetHashMSE(d[0],rand(),rand());
	d[1] = GetHashKBD(d[1],rand(),rand());
	d[2] = GetHashRAM(d[2],rand(),rand());
	d[3] = GetHashMON(d[3],rand(),rand());
	d[4] = GetHashCPU(d[4],rand(),rand());
	d[5] = GetHashHDD(d[5],rand(),rand());
	d[6] = GetHashRSM(d[6],rand(),rand());
/*
CString css; 

csf.WriteString(L"Test Cmp\n");
for(int i=0; i<7; ++i)
{
 css.Format(L"d[%i]=", i); csf.WriteString(css);
 for(int y=0; y<16; ++y)
 {
  css.Format(L"%x",d[i].x[y]); csf.WriteString(css);
 }
 csf.WriteString(L"\n");
}

for(int i=0; i<7; ++i)
{
	css.Format(L"dCmpHash[%i]=", i); csf.WriteString(css);
	for(int y=0; y<16; ++y)
	{
		css.Format(L"%x",dCmpHash[i].x[y]); csf.WriteString(css);
	}
	csf.WriteString(L"\n");
}

csf.Close();
*/

	for(int i=0; i<6; ++i)
	{
		for(int y=0; y<16; ++y)
			if( dCmpHash[i].x[y] != d[i].x[y] )
			{				
				return FALSE;
			}
	}

	if( !GetVerOS_XP32(0,0) )
	{
		for(int y=0; y<16; ++y)
			if( dCmpHash[6].x[y] != d[6].x[y] )
			{				
				return FALSE;
			}
	}


	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}
	return FALSE;
}
