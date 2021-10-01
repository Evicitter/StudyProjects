///////////////////////////////////////////////////////////
//	TransAppMsg.h
//	Implementation of the Class TransAppMsg
//	Created on:      06.05.2010 8:28:57
//	Last update time : 28.11.2011 02:56:28
///////////////////////////////////////////////////////////

#if !defined(EA_0F676E3E_5A33_4acc_9B68_43C9EB7AE68A__INCLUDED_)
#define EA_0F676E3E_5A33_4acc_9B68_43C9EB7AE68A__INCLUDED_

#include <Windows.h>

class TransAppMsg
{
	typedef void (__fastcall *cbTransAppMsg)(void* buffer, unsigned short bufsize_inbyte, unsigned char flag);

public:
	TransAppMsg();
	virtual ~TransAppMsg();

	virtual bool Host(const wchar_t* chanelname, bool bIsLocal = true, bool bIsOnce = false, bool bIsExternalClock = false, unsigned long ITimeOutForReadMSG = 1000ul);
	virtual bool Join(const wchar_t* chanelname, const wchar_t* compname = L"", bool bIsLocal = true);
	virtual bool Disconnect();

	virtual void SendMSG(void* _buffer, unsigned short bufsize_inbyte, unsigned char flag_msg = 255ui8, unsigned char indexcallback = 0ui8);
	virtual void SendMSG_L64(__int64 value, unsigned char flag_msg = 255ui8, unsigned char indexcallback = 0ui8);
	virtual void SendMSG_long(long value, unsigned char flag_msg = 255ui8, unsigned char indexcallback = 0ui8);
	virtual void SendMSG_string(const wchar_t* str, unsigned char flag_msg = 255ui8, unsigned char indexcallback = 0ui8);
	virtual void SetCallBack(cbTransAppMsg funct, unsigned char index = 0ui8);

public:
	cbTransAppMsg gCBFunc[256];
	HANDLE gMS;
	HANDLE gCT;
	unsigned long inThreadID;
	unsigned long lerror;
	unsigned short last_len;
	unsigned char last_flag;
	unsigned char last_cbid;
	bool bOnceWork;
	bool bExtClock;
	bool bThisServer;
	bool bShutDown;
};
#endif // !defined(EA_0F676E3E_5A33_4acc_9B68_43C9EB7AE68A__INCLUDED_)
