
// flash_apiDlg.h : 头文件
//

#pragma once


// Cflash_apiDlg 对话框
class Cflash_apiDlg : public CDialogEx
{
// 构造
public:
	Cflash_apiDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FLASH_API_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedStart();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedStartCanAnalyse();
	afx_msg void OnBnClickedCalaulator();
	afx_msg void OnBnClickedStartNotepad();
	afx_msg void OnBnClickedStartDeviceManager();
	afx_msg void OnCancelMode();
	afx_msg void OnClose();
	afx_msg void OnBnClickedParameterWatch();
};
