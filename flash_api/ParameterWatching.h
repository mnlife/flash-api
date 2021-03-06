#pragma once


// ParameterWatching 对话框

class ParameterWatching : public CDialogEx
{
	DECLARE_DYNAMIC(ParameterWatching)

public:
	ParameterWatching(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~ParameterWatching();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PARAMETER_WATCHING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	HICON m_hIcon;
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
