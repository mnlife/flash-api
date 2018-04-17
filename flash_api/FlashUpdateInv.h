#pragma once


// FlashUpdateInv 对话框

class FlashUpdateInv : public CDialogEx
{
	DECLARE_DYNAMIC(FlashUpdateInv)
private:
	HICON m_hIcon;


public:
	FlashUpdateInv(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~FlashUpdateInv();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FLASH_UPDATE_INV };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedStartFlashUpdate();
	afx_msg void OnBnClickedOpenCanDevice();
private:
	CProgressCtrl FlashUpdateProgressCtrl[16];
	int	FlashUpdateProgress[16];
	int InvFlashUpdate();
	struct {
		DWORD	data_head;
		DWORD	data_trail;
		DWORD	data_count;
		DWORD	next_data_head;
	}transfer_data;
	int i = 0;
public:
	afx_msg void OnBnClickedCloseCanDevice();

private:
	int FlashUpdateProgressSetPos();
};
