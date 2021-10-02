//Last update time : 15.02.2010 04:51:12

#include "stdafx.h"
#include "ar_AlexB_Writer.h"
#include "ar_AlexB_WriterDlg.h"

#include <intrin.h>

#include "TrayIcon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

NotifyIcon TrayI;
CFile FWriter;

DWORD64 lls[512];
DWORD64 nCountWrite=0ui64;
DWORD64 nByteWrite=0ui64;

static unsigned __int64 _xcvSEED[2];
void __fastcall xsrand()
{ 
	_xcvSEED[0] = _time64( NULL );
	_xcvSEED[1]=__rdtsc()<<4;
}

unsigned __int64 __fastcall xrand()
{ 
	_xcvSEED[0] = _umul128(_xcvSEED[0], 919178835918847ui64, &_xcvSEED[1]);
	_xcvSEED[0] += _xcvSEED[1] + 10870609470816256ui64 + __lzcnt64(_xcvSEED[0]) + __lzcnt64(_xcvSEED[1]);
	return _xcvSEED[0];
}

BOOL EjectMedia(wchar_t litdisk)
{
	/*HANDLE drvHandle;
	DWORD  dwAccess, dwRet;
	BOOL   bResult = FALSE;

	CString strDrive;
	CString strLitD;

	strDrive.Format(L"\\\\.\\%c:", litdisk);
	strLitD.Format(L"%c:\\", litdisk);

	//LPWSTR strDrive = L"\\\\.\\I:";

	switch(GetDriveType(strLitD))
	{
	case DRIVE_REMOVABLE: 
		dwAccess = GENERIC_READ | GENERIC_WRITE; 
		break;
	default: 
		dwAccess = GENERIC_READ;
		break;
	}

	drvHandle = CreateFile(strDrive, dwAccess, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);

	if (drvHandle != INVALID_HANDLE_VALUE)
	{
		bResult = FALSE;
		for (int j = 0; j < 10; j++) 
		{
			if (DeviceIoControl(drvHandle, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0,&dwRet, NULL))
			{
				bResult = TRUE;
				break;
			}
			Sleep(100);
		}
		if (bResult)
		{
			bResult = DeviceIoControl(drvHandle,  FSCTL_DISMOUNT_VOLUME, NULL, 0, NULL, 0,&dwRet, NULL);
			if (bResult)
			{
				bResult= DeviceIoControl(drvHandle, IOCTL_STORAGE_MEDIA_REMOVAL, NULL, 0, NULL,  0, &dwRet, NULL);
				if (bResult)
				{
					bResult = DeviceIoControl(drvHandle,IOCTL_STORAGE_EJECT_MEDIA, NULL, 0, NULL, 0,&dwRet, NULL);
					bResult = CloseHandle(drvHandle);
				}
			}
		}
	}
	else
		return FALSE;
 return bResult;*/
	return FALSE;
}

Car_AlexB_WriterDlg::Car_AlexB_WriterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(Car_AlexB_WriterDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Car_AlexB_WriterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, mcb_Timer);
	DDX_Control(pDX, IDC_EDIT1, me_Count);
	DDX_Control(pDX, IDC_EDIT2, mc_Bytes);
	DDX_Control(pDX, IDC_BUTTON2, mb_StopTime);
}

BEGIN_MESSAGE_MAP(Car_AlexB_WriterDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
//	ON_WM_DESTROY()
//	ON_WM_MENUSELECT()
	ON_WM_MENURBUTTONUP()
	ON_WM_TIMER()
	ON_WM_HELPINFO()
	ON_BN_CLICKED(IDC_BUTTON1, &Car_AlexB_WriterDlg::BC_Ok)
	ON_BN_CLICKED(IDC_BUTTON2, &Car_AlexB_WriterDlg::BC_StopTimer)
END_MESSAGE_MAP()

BOOL Car_AlexB_WriterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);
  mcb_Timer.SetCurSel(3);
	TrayI.setWindow( m_hWnd );
	TrayI.setIcon(m_hIcon);
	TrayI.setTipText(L"File Writer for hdd");
	TrayI.Show();
	FWriter.Open( L"zzConsorcium", CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::osSequentialScan | CFile::shareDenyRead );
	FWriter.SeekToBegin();	
	SetTimer(0x10101010,5000,NULL);
	SetTimer(0x40404040,20000, NULL);
	xsrand();
	return TRUE;
}

void Car_AlexB_WriterDlg::OnPaint()
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
HCURSOR Car_AlexB_WriterDlg::OnQueryDragIcon() { return static_cast<HCURSOR>(m_hIcon); }
void Car_AlexB_WriterDlg::OnDestroy()
{
	KillTimer(0x20202020);
	TrayI.Hide();
	//CString df = FWriter.GetFilePath();
	FWriter.Close();
	//CFile::Remove( df );
	CDialog::OnDestroy();
}

LRESULT Car_AlexB_WriterDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if( message == WM_NOTIFYICON )
	{
	 if( (lParam == WM_RBUTTONDOWN) || (lParam == WM_LBUTTONDOWN) )
	 {
		 if( IsWindowVisible() )
		 {
			 BC_Ok();
			 ShowWindow(SW_HIDE);
		 }
		 else
		 {
			 ShowWindow(SW_SHOW);
			 SetTimer(0x30303030,1000,NULL);
		 }
			/*CMenu cm;
			POINT cp;
			cm.CreateMenu();
			cm.CreatePopupMenu();
			cm.AppendMenu( MF_STRING, 33,L"Timer -  1 sec" );
			cm.AppendMenu( MF_STRING, 34,L"Timer -  5 sec" );
			cm.AppendMenu( MF_STRING, 35,L"Timer - 10 sec" );
			cm.AppendMenu( MF_STRING, 36,L"Timer - 30 sec" );
			cm.AppendMenu( MF_STRING, 8888,L"Exit" );
			GetCursorPos(&cp);

			KillTimer(0x20202020);

			cm.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTALIGN | TPM_RIGHTBUTTON,
				cp.x, cp.y, this);
			cm.DestroyMenu();*/
	 }
	 else if( lParam == WM_MBUTTONDOWN )
	 {
		 CStringW xccs;
		 xccs.Format(L"Total Writes = %I64u\nTotal Bytes = %I64u", nCountWrite, nByteWrite);
		 TrayI.ShowBalloonTip(xccs,L"Statistics",0,5000);
	 }
	}
	return CDialog::WindowProc(message,wParam,lParam);
}

BOOL Car_AlexB_WriterDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	/*if( lParam == 0 )
	{
	 switch(wParam)
	 {
	 case 33: SetTimer(0x20202020, 1000, NULL); break;
	 case 34: SetTimer(0x20202020, 5000, NULL); break;
	 case 35: SetTimer(0x20202020, 10000, NULL); break;
	 case 36: SetTimer(0x20202020, 30000, NULL); break;
	 case 8888:
		 OnDestroy();
		 EndDialog(0);
	 break;
	 }
	}*/
	return CDialog::OnCommand(wParam,lParam);
}

void Car_AlexB_WriterDlg::OnTimer(UINT_PTR nIDEvent)
{
	if( nIDEvent == 0x20202020 )
	{
		for(register short cx=0; cx<512; ++cx)
		{
			lls[cx]=xrand() + xrand() + __lzcnt64(lls[cx]);
		}

		++nCountWrite;
		nByteWrite += sizeof(lls);
		FWriter.Write(lls,sizeof(lls));
		return;
	}

	else if( nIDEvent == 0x10101010 )
	{
		if(IsWindowVisible())
		{
			KillTimer(0x10101010);
			SetTimer(0x20202020,10000,NULL);
			ShowWindow(SW_HIDE); 
		}
	}

	else if( nIDEvent == 0x30303030 )
	{
		CString cs;
		DWORD64 t=nByteWrite;
		int n10x=0;
		while (t>1024)
		{
			t>>=10;
			++n10x;
		}
		switch(n10x)
		{
		case 0: cs.Format(L"%I64i байт", t); break;
		case 1: cs.Format(L"%I64i Кбайт", t); break;
		case 2: cs.Format(L"%I64i Мбайт", t); break;
		case 3: cs.Format(L"%I64i Гбайт", t); break;
		}
		mc_Bytes.SetWindowText(cs);
		cs.Format(L"%I64i", nCountWrite);
		me_Count.SetWindowText(cs);
		return;
	}
	else if( nIDEvent == 0x40404040 )
		TrayI.Show();

	CDialog::OnTimer(nIDEvent);
}

BOOL Car_AlexB_WriterDlg::OnHelpInfo(HELPINFO* pHelpInfo) { return FALSE; }

void Car_AlexB_WriterDlg::BC_Ok()
{
	KillTimer(0x10101010);
	KillTimer(0x20202020);
		 if( mcb_Timer.GetCurSel() == 0 ) SetTimer( 0x20202020, 1000, NULL );
	else if( mcb_Timer.GetCurSel() == 1 ) SetTimer( 0x20202020, 2000, NULL );
	else if( mcb_Timer.GetCurSel() == 2 ) SetTimer( 0x20202020, 5000, NULL );
	else if( mcb_Timer.GetCurSel() == 3 ) SetTimer( 0x20202020, 10000, NULL );
	else if( mcb_Timer.GetCurSel() == 4 ) SetTimer( 0x20202020, 30000, NULL );

	ShowWindow(SW_HIDE);
	mb_StopTime.EnableWindow();
}

void Car_AlexB_WriterDlg::BC_StopTimer()
{
	KillTimer(0x20202020);
	KillTimer(0x30303030);
	OnTimer(0x30303030);
	mb_StopTime.EnableWindow(FALSE);
}