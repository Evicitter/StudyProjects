//Last update time : 09.01.2010 17:24:16
#include <windows.h>
#include <shellapi.h>
#include <tchar.h>
#define WM_NOTIFYICON (WM_USER + 1)
class NotifyIcon
{
private:
	NOTIFYICONDATA iconData;
	bool visible;
public:
	NotifyIcon();
	~NotifyIcon();
	void setWindow(HWND owner);
	void setIcon(HICON icon);
	void setTipText(LPCTSTR text);
	void Show();
	void Hide();
	void ShowBalloonTip(LPCTSTR body, LPCTSTR title, DWORD icon, UINT timeout);
};