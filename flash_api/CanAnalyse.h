#pragma once

#include "ControlCAN.h"
// CanAnalyse 对话框

class CanAnalyse : public CDialogEx
{
	DECLARE_DYNAMIC(CanAnalyse)

public:
	CanAnalyse(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CanAnalyse();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAN_ANALYSE };
#endif

protected:
	HICON m_hIcon;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedStartCanDevice();
	afx_msg void OnBnClickedStartReceived();
	afx_msg void OnBnClickedCloseCanDevice();
	afx_msg void OnBnClickedClearReceived();

private:
	int mnlife;
	struct SInfo {
		INT64	sort;
		TCHAR	time[20];
		TCHAR	ID[15];
		TCHAR	DLC[5];
		TCHAR	DATA[52];
	};

	VCI_CAN_OBJ *rx_msg;
	VCI_CAN_OBJ tx_msgnew;
	int device_type = 4;	// CANalyst-II
	int device_ind = 0;		// first device
	int can_ind = 0;		// CAN channel 0
public:
	afx_msg void OnBnClickedStopReceived();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedMsgSaved();
	afx_msg void OnBnClickedSendMessage();
};
