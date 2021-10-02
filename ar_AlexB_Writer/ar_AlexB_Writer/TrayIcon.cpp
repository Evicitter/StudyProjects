//Last update time : 13.02.2010 14:48:32
//notifyicon.cpp

#include "stdafx.h"
#include "TrayIcon.h"

UINT count = 0;

NotifyIcon::NotifyIcon()
{
	memset(&this->iconData, 0, sizeof(NOTIFYICONDATA));
	this->iconData.cbSize = NOTIFYICONDATA_V2_SIZE; //sizeof(NOTIFYICONDATA);
	this->iconData.hWnd = NULL;
	this->iconData.uID = count++;
	this->iconData.uCallbackMessage = WM_NOTIFYICON;        
}

NotifyIcon::~NotifyIcon()
{
	Shell_NotifyIcon(NIM_DELETE, &iconData);
}

void NotifyIcon::setIcon(HICON icon)
{
	this->iconData.hIcon = icon;
	if(this->visible) 
	{
		this->iconData.uFlags = NIF_ICON;
		Shell_NotifyIcon(NIM_MODIFY, &this->iconData);
	}
}

void NotifyIcon::setTipText(LPCTSTR text)
{
	int len = _tcslen(text);
	if(len > 127) len = 127;
	_tcsncpy(this->iconData.szTip, text, len);
	this->iconData.szTip[len] = 0;

	if(visible)
	{
		this->iconData.uFlags = NIF_TIP;
		Shell_NotifyIcon(NIM_MODIFY, &this->iconData);
	}
}

void NotifyIcon::Show()
{
	this->visible = true;
	this->iconData.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	Shell_NotifyIcon(NIM_ADD, &this->iconData);
}

void NotifyIcon::Hide()
{
	this->visible = false;
	Shell_NotifyIcon(NIM_DELETE, &this->iconData);
}

void NotifyIcon::ShowBalloonTip(LPCTSTR body, LPCTSTR title, DWORD icon, UINT timeout)
{
	if(!visible) this->Show();

	this->iconData.uTimeout = timeout;
	this->iconData.dwInfoFlags = icon;

	UINT bodyLength = _tcslen(body);
	if(bodyLength > 255) bodyLength = 255;
	_tcsncpy(this->iconData.szInfo, body, bodyLength);
	this->iconData.szInfo[bodyLength] = 0;

	UINT titleLength = _tcslen(title);
	if(titleLength > 63) titleLength = 63;
	_tcsncpy(this->iconData.szInfoTitle, title, titleLength);
	this->iconData.szInfoTitle[titleLength] = 0;

	this->iconData.uFlags = NIF_INFO;
	Shell_NotifyIcon(NIM_MODIFY, &this->iconData);
}

void NotifyIcon::setWindow( HWND owner )
{
 this->iconData.hWnd = owner;
}