//Last update time : 02.02.2010 05:44:32
#pragma once
#include "afxwin.h"

class Car_AlexB_WriterDlg : public CDialog
{
public:
	Car_AlexB_WriterDlg(CWnd* pParent = NULL);
	enum { IDD = IDD_AR_ALEXB_WRITER_DIALOG };
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
protected:
	HICON m_hIcon;
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	CComboBox mcb_Timer;
	afx_msg void BC_Ok();
	CEdit me_Count;
	CEdit mc_Bytes;
	CButton mb_StopTime;
	afx_msg void BC_StopTimer();
};