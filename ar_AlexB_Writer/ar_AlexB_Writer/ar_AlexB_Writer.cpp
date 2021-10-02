//Last update time : 05.01.2010 23:56:48
#include "stdafx.h"
#include "ar_AlexB_Writer.h"
#include "ar_AlexB_WriterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(Car_AlexB_WriterApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()
Car_AlexB_WriterApp::Car_AlexB_WriterApp() { }
Car_AlexB_WriterApp theApp;

BOOL Car_AlexB_WriterApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);
	CWinApp::InitInstance();
	SetRegistryKey(_T("Локальные приложения, созданные с помощью мастера приложений"));
	Car_AlexB_WriterDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	return FALSE;
}