
// flash_apiDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "flash_api.h"
#include "flash_apiDlg.h"
#include "afxdialogex.h"
#include "ControlCAN.h"
#include "CanAnalyse.h"
#include "FlashUpdateMain.h"
#include "ParameterWatching.h"
//#pragma comment(lib, "ControlCAN.lib")
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Cflash_apiDlg 对话框



Cflash_apiDlg::Cflash_apiDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_FLASH_API_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cflash_apiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);


}

BEGIN_MESSAGE_MAP(Cflash_apiDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_START, &Cflash_apiDlg::OnBnClickedStart)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_START_CAN_ANALYSE, &Cflash_apiDlg::OnBnClickedStartCanAnalyse)
	ON_BN_CLICKED(ID_CALAULATOR, &Cflash_apiDlg::OnBnClickedCalaulator)
	ON_BN_CLICKED(ID_START_NOTEPAD, &Cflash_apiDlg::OnBnClickedStartNotepad)
	ON_BN_CLICKED(ID_START_DEVICE_MANAGER, &Cflash_apiDlg::OnBnClickedStartDeviceManager)
	ON_WM_CANCELMODE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(ID_PARAMETER_WATCH, &Cflash_apiDlg::OnBnClickedParameterWatch)
END_MESSAGE_MAP()


// Cflash_apiDlg 消息处理程序

BOOL Cflash_apiDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void Cflash_apiDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void Cflash_apiDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR Cflash_apiDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}





void Cflash_apiDlg::OnBnClickedStart()
{
	OnOK();
	FlashUpdateMain	flash_update_model;
	flash_update_model.DoModal();
	/*FlashUpdateMain* dlg;
	dlg = new FlashUpdateMain;
	dlg->Create(IDD_FLASH_UPDATE_INV); //非模态对话框ID号
	dlg->ShowWindow(SW_SHOW);*/
}


void Cflash_apiDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	//AnimateWindow(1000, AW_CENTER);

}


void Cflash_apiDlg::OnBnClickedStartCanAnalyse()
{
	OnOK();
	CanAnalyse	OpenCanDevice;
	OpenCanDevice.DoModal();
	/*CanAnalyse* dlg;
	dlg = new CanAnalyse;
	dlg->Create(IDD_CAN_ANALYSE); //非模态对话框ID号
	dlg->ShowWindow(SW_SHOW);*/
}


void Cflash_apiDlg::OnBnClickedCalaulator()
{
	WinExec("calc", SW_SHOWNOACTIVATE);
}


void Cflash_apiDlg::OnBnClickedStartNotepad()
{
	WinExec("notepad", SW_SHOWNOACTIVATE);
}


void Cflash_apiDlg::OnBnClickedStartDeviceManager()
{
	WinExec("shutdown -s -t 03", SW_MAXIMIZE);
}


void Cflash_apiDlg::OnCancelMode()
{
	
	CDialogEx::OnCancelMode();

	
}


void Cflash_apiDlg::OnClose()
{
	if (AfxMessageBox(_T("Are you Sure?"), MB_YESNO) == IDNO)return;

	CDialogEx::OnClose();
}


void Cflash_apiDlg::OnBnClickedParameterWatch()
{
	ParameterWatching open;
	open.DoModal();
}
