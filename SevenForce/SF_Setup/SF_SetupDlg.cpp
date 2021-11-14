//Last update time : 24.10.2010 02:16:50
#include "SF_Setup.h"
#include "SF_SetupDlg.h"
#include "..\SevenForceLib/SevenForceLib.h"
#include "..\SevenForceLib/md5/md5.h"

#include <intrin.h>

#define SystemFunction036 NTAPI SystemFunction036
#include <NTSecAPI.h>
#undef SystemFunction036
//#pragma comment(lib,"ntsecapi.lib")

/*

#include <windows.h>
#include <setupapi.h>
#include <devguid.h>
#include <regstr.h>
#pragma comment(lib,"Setupapi.lib")
*/

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CString spath;
i128 dHashes[7];

CString GetHash(i128 d)
{
	CString ff;
	ff.Format(L"%x%x%x%x%x%x%x%x%x%x%x%x%x%x%x%x", d.x[15],d.x[14],d.x[13],d.x[12],d.x[11],d.x[10],d.x[9],d.x[8],d.x[7],d.x[6],d.x[5],d.x[4],d.x[3],d.x[2],d.x[1],d.x[0]);
	return ff;
}

unsigned int __fastcall GetRnd()
{
	unsigned int rsp;
	
	if( RtlGenRandom(&rsp, 4) != 0 ) //if( rand_s(&rsp) != 0 )
		rsp = rand() | (rand() << 16);
	return rsp;
}


CSF_SetupDlg::CSF_SetupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSF_SetupDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	__stosd( (PDWORD)dHashes, 0 , sizeof(dHashes)/4);
}

void CSF_SetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, mc_Path);
}

BEGIN_MESSAGE_MAP(CSF_SetupDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDOK, &CSF_SetupDlg::BC_Setup)
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL CSF_SetupDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	i128 d[7];__stosd( (PDWORD)d, 0 , sizeof(d)/4);
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	register DWORD64 pppt = __rdtsc(); pppt = (pppt >> 32) + (pppt & 0x00000000ffffffff);
	srand( pppt );

	//CStdioFile ss;
	//ss.Open(L"Test.txt", CFile::modeCreate | CFile::modeWrite | CFile::typeText);

	d[0] = GetHashMSE(d[0],GetRnd(),GetRnd());
	d[1] = GetHashKBD(d[1],GetRnd(),GetRnd());
	d[2] = GetHashRAM(d[2],GetRnd(),GetRnd());
	d[3] = GetHashMON(d[3],GetRnd(),GetRnd());
	d[4] = GetHashCPU(d[4],GetRnd(),GetRnd());
	d[5] = GetHashHDD(d[5],GetRnd(),GetRnd());
	d[6] = GetHashRSM(d[6],GetRnd(),GetRnd());

	__movsd( (PDWORD)dHashes, (PDWORD)d, sizeof(d)/4);
	__stosd( (PDWORD)d, 0 , sizeof(d)/4);

	/*ff.Format(L"KBD = %s\n", GetHash( GetHashKBD(d,0,0) ));	ss.WriteString(ff);
	ff.Format(L"MSE = %s\n", GetHash( GetHashMSE(d,0,0) ));	ss.WriteString(ff);
	ff.Format(L"RAM = %s\n", GetHash( GetHashRAM(d,0,0) ));	ss.WriteString(ff);
	ff.Format(L"CPU = %s\n", GetHash( GetHashCPU(d,0,0) ));	ss.WriteString(ff);
	ff.Format(L"GPU = %s\n", GetHash( GetHashGPU(d,0,0) ));	ss.WriteString(ff);
	ff.Format(L"HDD = %s\n", GetHash( GetHashHDD(d,0,0) ));	ss.WriteString(ff);
	ff.Format(L"MON = %s\n", GetHash( GetHashMON(d,0,0) ));	ss.WriteString(ff);
	ff.Format(L"RSM = %s\n", GetHash( GetHashRSM(d,rand(),rand()) ));	ss.WriteString(ff);
	ss.Close();*/

	wchar_t ppath[256];
	GetCurrentDirectoryW(sizeof(ppath), ppath);
	mc_Path.SetWindowText(ppath);
	spath = ppath;
	
	return TRUE;
}

void CSF_SetupDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		CRect rect;
		GetClientRect(&rect);
		dc.DrawIcon((rect.Width() - GetSystemMetrics(SM_CXICON) + 1) >> 1, (rect.Height() - GetSystemMetrics(SM_CYICON) + 1) >> 1, m_hIcon);
	}
	else
		CDialog::OnPaint();
}

HCURSOR CSF_SetupDlg::OnQueryDragIcon() { return static_cast<HCURSOR>(m_hIcon); }

void CSF_SetupDlg::OnDropFiles(HDROP hDropInfo)
{
	int nFiles = DragQueryFile(hDropInfo, -1, NULL, NULL);
	wchar_t path[256];
	DragQueryFile(hDropInfo, 0, path, sizeof(path));
	DragFinish(hDropInfo);
	if( nFiles<=1 )
	{
		if( GetFileAttributes(path) & FILE_ATTRIBUTE_DIRECTORY )
		{
			mc_Path.SetWindowText( path );
			spath.Empty();
			spath = path;
		}
	}
	CDialog::OnDropFiles(hDropInfo);
}

void CSF_SetupDlg::BC_Setup()
{
	register DWORD64 pppt = __rdtsc(); pppt = (pppt >> 32) + (pppt & 0x00000000ffffffff);
	srand( (rand() * rand()) + rand() + pppt );
	if( GetFileAttributesW( spath + L"\\SevenForce.exe" ) != -1 )
		return;

	/*md5_context ctx;
	md5_starts( &ctx );
	md5_update( &ctx, (unsigned char*)&rsp, sizeof(rsp) );*/

	HMODULE hMod=GetModuleHandleW(NULL);
	// если ресурс лежит в другом файле, то параметр для GetModuleHandle
	// можно получить с помощью LoadLibrary(...);
	if(!hMod)
	{
		SetTimer(0x11111111,10,NULL);
		return;
	} 
	//Получаем дескриптор информационного блока нашего ресурса 
	HRSRC hRes=FindResourceW(hMod,(LPCWSTR)IDR_RCDATA1,RT_RCDATA);
	//Если ResurceName не было определено как число, то ResurceName 
	//берется в кавычках. Иначе надо (LPSTR)ResurceName
	//Если тип ресурса был задан как WAVE, то последний параметр должен
	// быть "WAVE" - в кавычках!
	if(!hRes){
		SetTimer(0x22222222,10,NULL);
		return;
	}
	//Теперь загружаем ресурс в память, получая дескриптор загру-
	//женного блока памяти.
	HGLOBAL hGlob=LoadResource(hMod,hRes);
	//Опять же, для ресурса в "текущем" модуле, первым параметром можно
	//передавать NULL
	if(!hGlob) {
		SetTimer(0x33333333,10,NULL);
		return;
	}
	//И, наконец, последнее - получаем указатель на начало массива
	BYTE *lpbArray=(BYTE*)LockResource(hGlob); 
	if(!lpbArray)
	{
		SetTimer(0x44444444,10,NULL);
		return;
	}

//SevenForce
	CFile sss;
	if( !sss.Open(spath + L"\\SevenForce.exe", CFile::modeCreate | CFile::modeWrite | CFile::typeBinary) )
	{
		SetTimer(0x55555555,100,NULL);
		return;
	}
	sss.Write(lpbArray, SizeofResource(hMod,hRes));
	sss.Close();

	FreeResource(hGlob);
	
	//Open EXE
	HANDLE hUpdateRes=NULL;  // update resource handle
	BOOL bResult;

	hUpdateRes = BeginUpdateResourceW(spath + L"\\SevenForce.exe", FALSE);
	if (hUpdateRes == NULL)
	{
		SetTimer(0x77777777,12,NULL);
		return;
	}
	bResult  = UpdateResourceW(hUpdateRes, RT_RCDATA, MAKEINTRESOURCE(131), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), dHashes[0].x, sizeof(dHashes[0].x));
	bResult |= UpdateResourceW(hUpdateRes, RT_RCDATA, MAKEINTRESOURCE(132), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), dHashes[1].x, sizeof(dHashes[1].x));
	bResult |= UpdateResourceW(hUpdateRes, RT_RCDATA, MAKEINTRESOURCE(133), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), dHashes[2].x, sizeof(dHashes[2].x));
	bResult |= UpdateResourceW(hUpdateRes, RT_RCDATA, MAKEINTRESOURCE(134), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), dHashes[3].x, sizeof(dHashes[3].x));
	bResult |= UpdateResourceW(hUpdateRes, RT_RCDATA, MAKEINTRESOURCE(135), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), dHashes[4].x, sizeof(dHashes[4].x));
	bResult |= UpdateResourceW(hUpdateRes, RT_RCDATA, MAKEINTRESOURCE(136), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), dHashes[5].x, sizeof(dHashes[5].x));
	bResult |= UpdateResourceW(hUpdateRes, RT_RCDATA, MAKEINTRESOURCE(137), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), dHashes[6].x, sizeof(dHashes[6].x));
	if (bResult == FALSE)
	{
		SetTimer(0x88888888,22,NULL);
		return;
	}

	unsigned char wires[16];
	md5_context ctx;
	for(int i=0; i<7; ++i)
	{
		md5_starts(&ctx);
		for(int y=0; y<16; ++y)
		{
			srand(dHashes[i].x[y]);
			wires[1]=rand();
			wires[2]=rand();
			wires[3]=rand();
			wires[4]=rand();
			wires[5]=rand();
			wires[6]=rand();
			wires[7]=rand();
			wires[8]=rand();
			wires[9]=rand();
			wires[10]=rand();
			wires[11]=rand();
			wires[12]=rand();
			wires[13]=rand();
			wires[14]=rand();
			wires[15]=rand();
			md5_update(&ctx, wires, sizeof(wires));
		}
		md5_finish(&ctx,wires);
		bResult  = UpdateResourceW(hUpdateRes, RT_RCDATA, MAKEINTRESOURCE(138 + i), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), wires, sizeof(wires));
	}
	if (!EndUpdateResourceW(hUpdateRes, FALSE))
	{
		SetTimer(0x99999999,22,NULL);
		return;
	}
}

void CSF_SetupDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == 0x11111111)
	{
		KillTimer(nIDEvent);
		MessageBox(L"Внутреняя ошибка распаковки №981 (Слепое приложение)",L"Ошибка",MB_ICONERROR);
		return;
	}

	if(nIDEvent == 0x22222222)
	{
		KillTimer(nIDEvent);
		MessageBox(L"Внутреняя ошибка распаковки №1 (Не найден код приложения)",L"Ошибка",MB_ICONERROR);
		return;
	}

	if(nIDEvent == 0x33333333)
	{
		KillTimer(nIDEvent);
		MessageBox(L"Внутреняя ошибка распаковки №2 (Не загружается внутренний индентификатор)",L"Ошибка",MB_ICONERROR);
		return;
	}

	if(nIDEvent == 0x44444444)
	{
		KillTimer(nIDEvent);
		MessageBox(L"Внутреняя ошибка распаковки №3 (Не возможен захват сегмента)",L"Ошибка",MB_ICONERROR);
		return;
	}

	if(nIDEvent == 0x55555555)
	{
		KillTimer(nIDEvent);
		MessageBox(L"Внутреняя ошибка создания файла №1 (Нет доступа для записи)",L"Ошибка",MB_ICONERROR);
		return;
	}

	if(nIDEvent == 0x77777777)
	{
		KillTimer(nIDEvent);
		MessageBox(L"Внутреняя ошибка создания файла №2 (Невозможно открыть файл для записи)",L"Ошибка",MB_ICONERROR);
		return;
	}

	if(nIDEvent == 0x88888888)
	{
		KillTimer(nIDEvent);
		MessageBox(L"Внутреняя ошибка создания файла №3 (Невозможно обновить данные)",L"Ошибка",MB_ICONERROR);
		return;
	}

	if(nIDEvent == 0x99999999)
	{
		KillTimer(nIDEvent);
		MessageBox(L"Внутреняя ошибка создания файла №4 (Could not write changes to file.)",L"Ошибка",MB_ICONERROR);
		return;
	}
	CDialog::OnTimer(nIDEvent);
}