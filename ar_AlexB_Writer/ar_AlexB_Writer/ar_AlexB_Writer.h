//Last update time : 05.01.2010 22:22:56
#pragma once
#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif
#include "resource.h"

class Car_AlexB_WriterApp : public CWinApp
{
public:
	Car_AlexB_WriterApp();
	public:
	virtual BOOL InitInstance();
	DECLARE_MESSAGE_MAP()
};

extern Car_AlexB_WriterApp theApp;