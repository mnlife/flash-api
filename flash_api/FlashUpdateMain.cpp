// FlashUpdateMain.cpp: 实现文件
//

#include "stdafx.h"
#include "flash_api.h"
#include "FlashUpdateMain.h"
#include "afxdialogex.h"
#include "flash_apiDlg.h"
#include "ControlCAN.h"
#include "CAN_FLASHupdateMsgHandle.h"
#include "BootFileResolve.h"
#include "OutFileResolve.h"
#include <afxdb.h>
#include "Blob.h"
#include <map>
#include <stdexcept>
// FlashUpdateMain 对话框



CString	STATE_MACHINE_MESSAGE_GET_WHAT[]{

{ _T("初始无效状态") },
{ _T("开始选择节点升级") },
{ _T("发送握手信号等待握手信号状态") },
{ _T("发送芯片解密命令等待芯片解密应答信号") },
{ _T("发送命令等待API版本确认信息") },
{ _T("发送擦除命令") },
{ _T("等待擦除完毕") },
{ _T("请求编程许可等待编程允许") },
{ _T("传输BLOCK头") },
{ _T("等待传输BLOCK头结束标志") },
{ _T("传输BLOCK数据") },
{ _T("等待传输BLOCK数据完成标志") },
{ _T("传输CHECKSUM") },
{ _T("等待CHECKSUM校验成功标志") },
{ _T("下发编程命令") },
{ _T("等待DSP回传编程状态") },
{ _T("发送FLASH 校验命令") },
{ _T("等待FLASH 校验完毕消息") },
{ _T("判断是否有BLOCK需传输, 有传输BLOCK头,否则向DSP发送重启命令") },
{ _T("等待DSP FLASHUPDATE成功标志") },
{ _T("FLASH_UPDATE_SUCCEED") },
{ _T("FLASH UPDATE 失败") },

};

IMPLEMENT_DYNAMIC(FlashUpdateMain, CDialogEx)


//CAN_FlashupdateMsgHandle * uniflash;
FlashUpdateMain::FlashUpdateMain(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FLASH_UPDATE_INV, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	time_out_cnt = 0;

	msg_init.DataLen = 0;
	msg_init.ExternFlag = 0;
	msg_init.ID = 0;
	msg_init.RemoteFlag = 0;
	msg_init.Reserved[0] = 0;
	msg_init.Reserved[1] = 0;
	msg_init.Reserved[2] = 0;
	msg_init.SendType = 0;
	msg_init.TimeFlag = 0;
	msg_init.Data[0] = 0;
	msg_init.Data[1] = 0;
	msg_init.Data[2] = 0;
	msg_init.Data[3] = 0;
	msg_init.Data[4] = 0;
	msg_init.Data[5] = 0;
	msg_init.Data[6] = 0;
	msg_init.Data[7] = 0;


}

FlashUpdateMain::~FlashUpdateMain()
{
	Cflash_apiDlg	MainMenu;
	MainMenu.DoModal();
	/*Cflash_apiDlg* dlg;
	dlg = new Cflash_apiDlg;
	dlg->Create(IDD_FLASH_API_DIALOG); //非模态对话框ID号
	dlg->ShowWindow(SW_SHOW);*/
}
#define RegisterDDX_Control(MODULE_ID)\
DDX_Control(pDX, IDC_INV_MODILE##MODULE_ID, FlashUpdateProgressCtrl[MODULE_ID])

void FlashUpdateMain::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	//for (UINT16 i = 0; i < 16; ++i) {

	//	RegisterDDX_Control(i);
	//}
	DDX_Control(pDX, IDC_INV_MODILE1, FlashUpdateProgressCtrl[0]);
	DDX_Control(pDX, IDC_INV_MODILE2, FlashUpdateProgressCtrl[1]);
	DDX_Control(pDX, IDC_INV_MODILE3, FlashUpdateProgressCtrl[2]);
	DDX_Control(pDX, IDC_INV_MODILE4, FlashUpdateProgressCtrl[3]);
	DDX_Control(pDX, IDC_INV_MODILE5, FlashUpdateProgressCtrl[4]);
	DDX_Control(pDX, IDC_INV_MODILE6, FlashUpdateProgressCtrl[5]);
	DDX_Control(pDX, IDC_INV_MODILE7, FlashUpdateProgressCtrl[6]);
	DDX_Control(pDX, IDC_INV_MODILE8, FlashUpdateProgressCtrl[7]);
	DDX_Control(pDX, IDC_INV_MODILE9, FlashUpdateProgressCtrl[8]);
	DDX_Control(pDX, IDC_INV_MODILE10, FlashUpdateProgressCtrl[9]);
	DDX_Control(pDX, IDC_INV_MODILE11, FlashUpdateProgressCtrl[10]);
	DDX_Control(pDX, IDC_INV_MODILE12, FlashUpdateProgressCtrl[11]);
	DDX_Control(pDX, IDC_INV_MODILE13, FlashUpdateProgressCtrl[12]);
	DDX_Control(pDX, IDC_INV_MODILE14, FlashUpdateProgressCtrl[13]);
	DDX_Control(pDX, IDC_INV_MODILE15, FlashUpdateProgressCtrl[14]);
	DDX_Control(pDX, IDC_INV_MODILE16, FlashUpdateProgressCtrl[15]);
	OnCbnSelchangeModuleSelect();
}


BEGIN_MESSAGE_MAP(FlashUpdateMain, CDialogEx)
	ON_BN_CLICKED(ID_START_FLASH_UPDATE, &FlashUpdateMain::OnBnClickedStartFlashUpdate)
	ON_BN_CLICKED(ID_CLOSE_CAN_DEVICE, &FlashUpdateMain::OnBnClickedCloseCanDevice)
	ON_WM_DROPFILES()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RECORD_SAVED, &FlashUpdateMain::OnBnClickedRecordSaved)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CLEAR_MSG, &FlashUpdateMain::OnBnClickedClearMsg)
	ON_WM_CANCELMODE()
	ON_WM_CANCELMODE()
	ON_BN_CLICKED(ID_BOOT_LOADER, &FlashUpdateMain::OnBnClickedBootLoader)
	ON_BN_CLICKED(IDC_START_MSG, &FlashUpdateMain::OnBnClickedStartMsg)
	ON_BN_CLICKED(IDC_STOP_MSG, &FlashUpdateMain::OnBnClickedStopMsg)
	ON_CBN_SELCHANGE(IDC_MODULE_SELECT, &FlashUpdateMain::OnCbnSelchangeModuleSelect)
	ON_BN_CLICKED(IDC_MFCCOLORBUTTON1, &FlashUpdateMain::OnBnClickedMfccolorbutton1)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// FlashUpdateMain 消息处理程序


BOOL FlashUpdateMain::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SYSTEMTIME time;

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);
	CListCtrl *pList = (CListCtrl*)GetDlgItem(IDC_LIST2);
	//CSize	a;
	//a.SetSize(100, 200);
	//pList->ApproximateViewRect(a);
	pList->InsertColumn(0, _T("ModuleID"), LVCFMT_LEFT, 66);
	pList->InsertColumn(1, _T("Module FlashUpdate Message"), LVCFMT_LEFT, 566);
	pList->InsertColumn(2, _T("Time"), LVCFMT_LEFT, 100);
	// Blue Color
	pList->SetBkColor(RGB(137, 207, 240));
	pList->SetTextBkColor(RGB(135, 206, 235));

	// Pink Color
	//pList->SetBkColor(RGB(249, 204, 226));
	//pList->SetTextBkColor(RGB(249, 204, 226));
	// Blue Color
	//pList->SetBkColor(RGB(21, 174, 103));
	//pList->SetTextBkColor(RGB(21, 174, 103));

	// Green Color
	//pList->SetBkColor(RGB(233, 71, 41));
	//pList->SetTextBkColor(RGB(233, 71, 41));
	CString	module_num_display, module_msg_display, module_msg_time;
	GetSystemTime(&time);
	for (int i = 0; i < 16; ++i) {

		module_num_display.Format(_T("%02d"), i+1);
		pList->InsertItem(0, module_num_display);
		module_msg_display.Format(_T("Module %02d：use CAN flashupdate Ultra Fast!!!"), i+1);
		pList->SetItemText(0, 1, module_msg_display);
		
		module_msg_time.Format(_T("%d:%d:%d.%d"), ((time.wHour + 8) > 24) ? time.wHour + 8 - 24 : time.wHour + 8, time.wMinute, time.wSecond, time.wMilliseconds);
		pList->SetItemText(0, 2, module_msg_time);
	}

	
	CComboBox *pCmob = (CComboBox*)GetDlgItem(IDC_DEVICE_SELECT);
	pCmob->SetCurSel(0);
	pCmob = (CComboBox*)GetDlgItem(IDC_BAUD_RATE_SELECT);
	pCmob->SetCurSel(7);
	pCmob = (CComboBox*)GetDlgItem(IDC_FLASH_UPDATE_TYPE);
	pCmob->SetCurSel(1);
	pCmob = (CComboBox*)GetDlgItem(IDC_DSP_SELECT);
	pCmob->SetCurSel(2);
	//pCmob = (CComboBox*)GetDlgItem(IDC_SOFTWARE_SELECT);
	//pCmob->SetCurSel(0);
	pCmob = (CComboBox*)GetDlgItem(IDC_module_select);
	pCmob->SetCurSel(0);
	/*
	ShowWindow(SW_MAXIMIZE);
	CWnd *pButtom1 = NULL;
	pButtom1 = GetDlgItem(IDD_FLASH_UPDATE_MAIN);
	CRect rs;
	//GetClientRect(rs);
	GetClientRect(&rs);
	//if(pButtom1)pButtom1-> MoveWindow(0, 0,50,50); 
	if (pButtom1)pButtom1->MoveWindow((rs.right - rs.left) / 2, (rs.bottom - rs.top) / 2, 50, 50);*/
	GetClientRect(&myRect);
	return TRUE;
}


/****************************************************************************
pInitConfig->AccCode AccCode 对应SJA1000 中的四个寄存器ACR0，ACR1，ACR2，
ACR3，其中高字节对应ACR0，低字节对应ACR3；AccMask
对应SJA1000 中的四个寄存器AMR0，AMR1，AMR2，AMR3，
其中高字节对应AMR0，低字节对应AMR3。（请看表后说明）
pInitConfig->AccMask
pInitConfig->Reserve
d
保留
pInitConfig->Filter 滤波方式，1 表示单滤波，0 表示双滤波
pInitConfig->Timing0 波特率定时器0，详见VCI_INIT_CONFIG
pInitConfig->Timing1 波特率定时器1，详见VCI_INIT_CONFIG
pInitConfig->Mode 模式，0 表示正常模式，1 表示只听模式

CAN		波特率		定时器0		定时器1
5Kbps		0xBF		0xFF
10Kbps		0x31		0x1C
20Kbps		0x18		0x1C
40Kbps		0x87		0xFF
50Kbps		0x09		0x1C
80Kbps		0x83		0Xff
100Kbps		0x04		0x1C
125Kbps		0x03		0x1C
200Kbps		0x81		0xFA
250Kbps		0x01		0x1C
400Kbps		0x80		0xFA
500Kbps		0x00		0x1C
666Kbps		0x80		0xB6
800Kbps		0x00		0x16
1000Kbps	0x00		0x14
**************************************************************************/
int FlashUpdateMain::OnBnClickedOpenCanDevice()
{

#define	TIMING0_5Kbps	0xBF
#define	TIMING1_5Kbps	0xFF

#define	TIMING0_10Kbps	0x31
#define	TIMING1_10Kbps	0x1C

#define	TIMING0_20Kbps	0x18
#define	TIMING1_20Kbps	0x1C

#define	TIMING0_40Kbps	0x87
#define	TIMING1_40Kbps	0xFF

#define	TIMING0_50Kbps	0x09
#define	TIMING1_50Kbps	0x1C

#define	TIMING0_80Kbps	0x83
#define	TIMING1_80Kbps	0xFF

#define	TIMING0_100Kbps	0x04
#define	TIMING1_100Kbps	0x1C

#define	TIMING0_125Kbps	0x03
#define	TIMING1_125Kbps	0x1C

#define	TIMING0_200Kbps	0x81
#define	TIMING1_200Kbps	0xFA

#define	TIMING0_250Kbps	0x01
#define	TIMING1_250Kbps	0x1C

#define	TIMING0_400Kbps	0x80
#define	TIMING1_400Kbps	0xFA

#define	TIMING0_500Kbps	0x00
#define	TIMING1_500Kbps	0x1C

#define	TIMING0_666Kbps	0x80
#define	TIMING1_666Kbps	0xB6

#define	TIMING0_800Kbps	0x00
#define	TIMING1_800Kbps	0x16

#define	TIMING0_1000Kbps	0x00
#define	TIMING1_1000Kbps	0x14


	VCI_INIT_CONFIG	vic;
	vic.AccCode = 0x00000000;
	vic.AccMask = 0xFFFFFFFF;
	vic.Filter = 0;
	vic.Mode = 0;
	CString BuadRateSelect;
	GetDlgItemText(IDC_BAUD_RATE_SELECT, BuadRateSelect);
	if (BuadRateSelect == _T("5kbps")) {
		vic.Timing0 = TIMING0_5Kbps;
		vic.Timing1 = TIMING1_5Kbps;
	}
	else if (BuadRateSelect == _T("10kbps")) {
		vic.Timing0 = TIMING0_10Kbps;
		vic.Timing1 = TIMING1_10Kbps;
	}
	else if (BuadRateSelect == _T("20kbps")) {
		vic.Timing0 = TIMING0_20Kbps;
		vic.Timing1 = TIMING1_20Kbps;
	}
	else if (BuadRateSelect == _T("40kbps")) {
		vic.Timing0 = TIMING0_40Kbps;
		vic.Timing1 = TIMING1_40Kbps;
	}
	else if (BuadRateSelect == _T("50kbps")) {
		vic.Timing0 = TIMING0_50Kbps;
		vic.Timing1 = TIMING1_50Kbps;
	}
	else if (BuadRateSelect == _T("80kbps")) {
		vic.Timing0 = TIMING0_80Kbps;
		vic.Timing1 = TIMING1_80Kbps;
	}
	else if (BuadRateSelect == _T("100kbps")) {
		vic.Timing0 = TIMING0_100Kbps;
		vic.Timing1 = TIMING1_100Kbps;
	}
	else if (BuadRateSelect == _T("125kbps")) {
		vic.Timing0 = TIMING0_125Kbps;
		vic.Timing1 = TIMING1_125Kbps;
	}
	else if (BuadRateSelect == _T("200kbps")) {
		vic.Timing0 = TIMING0_200Kbps;
		vic.Timing1 = TIMING1_200Kbps;
	}
	else if (BuadRateSelect == _T("250kbps")) {
		vic.Timing0 = TIMING0_250Kbps;
		vic.Timing1 = TIMING1_250Kbps;
	}
	else if (BuadRateSelect == _T("400kbps")) {
		vic.Timing0 = TIMING0_400Kbps;
		vic.Timing1 = TIMING1_400Kbps;
	}
	else if (BuadRateSelect == _T("500kbps")) {
		vic.Timing0 = TIMING0_500Kbps;
		vic.Timing1 = TIMING1_500Kbps;
	}
	else if (BuadRateSelect == _T("666kbps")) {
		vic.Timing0 = TIMING0_666Kbps;
		vic.Timing1 = TIMING1_666Kbps;
	}
	else if (BuadRateSelect == _T("800kbps")) {
		vic.Timing0 = TIMING0_800Kbps;
		vic.Timing1 = TIMING1_800Kbps;
	}
	else if (BuadRateSelect == _T("1000kbps")) {
		vic.Timing0 = TIMING0_1000Kbps;
		vic.Timing1 = TIMING1_1000Kbps;
	}
	else {

		throw std::runtime_error("Please choose Baud Rate！");
	}
	vic.Reserved = 0;

	if (VCI_OpenDevice(device_type, device_ind, can_ind) != STATUS_OK) {

		SetWindowText(_T("Open Device Failed！"));
		throw std::runtime_error("Open Device Failed!!!");
	}

	if (VCI_InitCAN(device_type, device_ind, can_ind, &vic) == STATUS_ERR) {

		VCI_CloseDevice(device_type, device_ind);
		throw std::runtime_error("Init Device Failed！");
	}

	if (VCI_StartCAN(device_type, device_ind, can_ind) == STATUS_ERR) {

		VCI_CloseDevice(device_type, device_ind);
		throw std::runtime_error("Start Device Failed！");
	}
	else {

		SetWindowText(_T("Open Device Success！"));
		return 0;
	}
}

void FlashUpdateMain::OnBnClickedStartFlashUpdate() {
	
	TryCloseDevice();
	try {

		CString file_path;
		ParameterRefresh();
		OnBnClickedOpenCanDevice();

		GetDlgItemText(IDC_MFCEDITBROWSE1, file_path);
		Solver.reset(new OutFileResolve(file_path));

		Solver->FileResolve();

		uniflash.reset(new CAN_FlashupdateMsgHandle(*Solver));
		FlashUpdateGetNode();
	}catch (std::runtime_error err) {

		AfxMessageBox(CString(err.what()));
		return;
	}
	CButton *button = (CButton*)GetDlgItem(ID_START_FLASH_UPDATE);
	button->EnableWindow(0);

	button = (CButton*)GetDlgItem(ID_BOOT_LOADER);
	button->EnableWindow(0);

	SendFlashUpdateOrder();
	uniflash->m_pHostModuleItc->u16FlashupdateStatus = FLASH_UPDATE_START;
	if (uniflash->m_u16UpdaingNodeAdd == 0x3F) {

		SetTimer(2, 100, NULL);
	}
	else {

		SetTimer(2, 888, NULL);
	}
	SetTimer(3, 1000, NULL);
}


void FlashUpdateMain::OnBnClickedCloseCanDevice()
{
	ShowButton();

	KillTimer(2);
	KillTimer(3);
	KillTimer(4);
	//(uniflash == NULL) ? 1 : (delete uniflash);
	//(Solver == NULL) ? 1 : (delete Solver);

	VCI_BOARD_INFO	vbi;
	if (!VCI_ReadBoardInfo(device_type, device_ind, &vbi)) {

		AfxMessageBox(_T("No device!!!"));
		return;
	}
	if (VCI_CloseDevice(device_type, device_ind) == STATUS_ERR) {

		AfxMessageBox(_T("Close Device Failed！"));
		return;
	}
	else {
		SetWindowText(_T("Close Device Success!"));
	}

}
void FlashUpdateMain::TryCloseDevice() {

	VCI_BOARD_INFO	vbi;
	if (!VCI_ReadBoardInfo(device_type, device_ind, &vbi)) {
		return;
		//throw std::runtime_error("No Device");
	}
	if (VCI_CloseDevice(device_type, device_ind) == STATUS_ERR) {

		return;
		//throw std::runtime_error("Close Device Failed");
	}
	else {
		SetWindowText(_T("Close Device Success!"));
	}
}


int FlashUpdateMain::FlashUpdateProgressSetPos(){
	
	SYSTEMTIME	time;
	CString module_num_display, module_msg_display, module_msg_time;
	CListCtrl *pList = (CListCtrl*)GetDlgItem(IDC_LIST2);
	GetSystemTime(&time);

	for (int i = 0; i < 16; ++i) {

		FlashUpdateProgressCtrl[i].SetPos(int((uniflash->FlashUpdateProgress[i+uniflash->NodeOffset]*100)/(Solver->BlockCount-1)));
		module_num_display.Format(_T("%02d"), i + 1);
		if (uniflash->m_pHostModuleItcBak.u16FlashupdateStatus != uniflash->m_pHostModuleItc->u16FlashupdateStatus) {

			if (uniflash->FlashUpdateErrorMsg[i + uniflash->NodeOffset].ErrorCnt == 123) {
				
				pList->InsertItem(0, module_num_display);
				module_msg_display.Format(_T("模块%02d：")+
					STATE_MACHINE_MESSAGE_GET_WHAT[int(uniflash->FlashUpdateErrorMsg[i + uniflash->NodeOffset].ErrorStateSaved)] +
					_T("超时!!!"),
					i + 1);
				pList->SetItemText(0, 1, module_msg_display);

			}
			else {

				module_num_display.Format(_T("%02d"), i + 1);
				pList->InsertItem(0, module_num_display);
				module_msg_display.Format(_T("模块%02d：") +
					
					STATE_MACHINE_MESSAGE_GET_WHAT[int(uniflash->m_pHostModuleItc->u16FlashupdateStatus)]
					, i + 1);
				pList->SetItemText(0, 1, module_msg_display);

			}
			module_msg_time.Format(_T("%d:%d:%d.%d"), ((time.wHour + 8) > 24) ? time.wHour + 8 - 24 : time.wHour + 8, time.wMinute, time.wSecond, time.wMilliseconds);
			pList->SetItemText(0, 2, module_msg_time);
			time_out_cnt = 0;
		}
		else {

			time_out_cnt++;
		}
		if ((time_out_cnt > (50 << 4)) && (uniflash->m_pHostModuleItc->u16FlashupdateStatus != FLASH_UPDATE_SUCCEED)) {

			pList->InsertItem(0, module_num_display);
			module_msg_display.Format(_T("模块%02d：连接超时!!!"), i+1);
			pList->SetItemText(0, 1, module_msg_display);

			module_msg_time.Format(_T("%d:%d:%d.%d"), ((time.wHour + 8) > 24) ? time.wHour + 8 - 24 : time.wHour + 8, time.wMinute, time.wSecond, time.wMilliseconds);
			pList->SetItemText(0, 2, module_msg_time);
			uniflash->FlashUpdateErrorMsgBak[i + uniflash->NodeOffset] = uniflash->FlashUpdateErrorMsg[i + uniflash->NodeOffset];
		}	
	}
	uniflash->m_pHostModuleItcBak.u16FlashupdateStatus = uniflash->m_pHostModuleItc->u16FlashupdateStatus;
	return 0;
}


void FlashUpdateMain::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 2) {
		uniflash->GetFlashUpdateRoutine();
		
		//SetTimer(2, 10, NULL);
	}
	else if (nIDEvent == 3) {

		FlashUpdateProgressSetPos();
	}
	else if (nIDEvent == 4) {

		uniflash->GetBootLoaderRoutine();
	}

	(uniflash->m_pHostModuleItc->u16FlashupdateStatus == FLASH_UPDATE_SUCCEED) ? ShowButton() : 1;
	
	CDialogEx::OnTimer(nIDEvent);
}


bool FlashUpdateMain::SendFlashUpdateOrder()
{
	CAN_PACKED_PROTOCOL_U *tx_msg = new CAN_PACKED_PROTOCOL_U;
	tx_msg->Frame = msg_init;
	tx_msg->PackedMsg.RemoteFlag = 0;// 数据帧
	tx_msg->PackedMsg.ExternFlag = 0;// 标准帧

	tx_msg->PackedMsg.b6DestinationMacId = uniflash->m_u16UpdaingNodeAdd;
	tx_msg->PackedMsg.b7ServiceCode = service_code;
	tx_msg->PackedMsg.b10MsgClass = CAN_CTRL_MSG_CLASS;
	tx_msg->PackedMsg.b1Fragment = NONFRAG_MSG;
	tx_msg->PackedMsg.b1RsRq = RQ_MSG;
	tx_msg->PackedMsg.b6SourceMacId = MAC_ID_MON;
	tx_msg->PackedMsg.DataLen = 6;
	tx_msg->PackedMsg.MsgData[0] = tx_fu_data[0];
	tx_msg->PackedMsg.MsgData[1] = tx_fu_data[1];
	tx_msg->PackedMsg.MsgData[2] = tx_fu_data[2];
	tx_msg->PackedMsg.MsgData[3] = tx_fu_data[3];


	VCI_Transmit(device_type, device_ind, can_ind, &tx_msg->Frame, 1);

	delete tx_msg;
	return TRUE;
}


void FlashUpdateMain::OnBnClickedRecordSaved()
{
	

	if (AfxMessageBox(_T("Are you Sure Save Message?"), MB_YESNO) == IDNO)return;
	//// TODO: Add your control notification handler code here
	//首先判断列表框中是否有记录
	CListCtrl* m_list = (CListCtrl*)GetDlgItem(IDC_LIST1);
	//// TODO: Add your control notification handler code here
	//首先判断列表框中是否有记录
	if (m_list->GetItemCount() <= 0)
	{
		AfxMessageBox(_T("没有升级记录需要保存！"));
		return;
	}
	//打开另存为对话框 ，需要包含 #include <Afxdlgs.h>
	CFileDialog dlg(FALSE,
		_T("xls"),
		_T("FileList"),
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("Excel 文件(*.xls)|*.xls||"));
	dlg.m_ofn.lpstrTitle = _T("文件列表另存为");

	if (dlg.DoModal() != IDOK)return;
	CString strFilePath;
	//获得文件路径名
	strFilePath = dlg.GetPathName();
	//判断文件是否已经存在，存在则删除重建
	DWORD dwRe = GetFileAttributes(strFilePath);
	if (dwRe != (DWORD)-1)
	{
		DeleteFile(strFilePath);
	}
	CDatabase database;//数据库库需要包含头文件 #include <afxdb.h>
	CString sDriver = _T("MICROSOFT EXCEL DRIVER (*.XLS)"); // Excel驱动
	CString sSql, strInsert;
	TRY
	{
		// 创建进行存取的字符串
		sSql.Format(_T("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s"),sDriver, strFilePath, strFilePath);

	// 创建数据库 (既Excel表格文件)
	if (database.OpenEx(sSql,CDatabase::noOdbcDialog))
	{
		//获得列别框总列数
		int iColumnNum,iRowCount;
		LVCOLUMN lvCol;
		CString strColName; //用于保存列标题名称
		int i,j; //列、行循环参数

		iColumnNum = m_list->GetHeaderCtrl()->GetItemCount();
		iRowCount = m_list->GetItemCount();
		sSql = _T(" CREATE TABLE DSO_DX ( ");
		strInsert = _T(" INSERT INTO DSO_DX ( ");
		//获得列标题名称
		lvCol.mask = LVCF_TEXT; //必需设置，说明LVCOLUMN变量中pszText参数有效
		lvCol.cchTextMax = 32; //必设，pszText参数所指向的字符串的大小
		lvCol.pszText = strColName.GetBuffer(32); //必设，pszText 所指向的字符串的实际存储位置。
												  //以上三个参数设置后才能通过 GetColumn()函数获得列标题的名称
		for (i = 0; i< iColumnNum; i++)
		{
			if (!(m_list->GetColumn(i,&lvCol)))
				return;
			if (i<iColumnNum - 1)
			{
				sSql = sSql + lvCol.pszText + _T(" TEXT , ");
				strInsert = strInsert + lvCol.pszText + _T(" , ");
			}
			else
			{
				sSql = sSql + lvCol.pszText + _T(" TEXT ) ");
				strInsert = strInsert + lvCol.pszText + _T(" )  VALUES ( ");
			}
		}
		//创建Excel表格文件
		database.ExecuteSQL(sSql);
		//循环提取记录并插入到EXCEL中
		sSql = strInsert;
		CString chTemp;
		for (j = 0; j<iRowCount; j++)
		{

			for (i = 0; i<iColumnNum; i++)
			{
				chTemp = m_list->GetItemText(j, i);
				if (i < (iColumnNum - 1))
				{
					sSql = sSql + _T("'") + chTemp + _T("' , ");
				}
				else
				{
					sSql = sSql + _T("'") + chTemp + _T("' ) ");
				}
			}
			//将记录插入到表格中
			database.ExecuteSQL(sSql);
			sSql = strInsert;
		}
	}
	// 关闭Excel表格文件
	database.Close();
	AfxMessageBox(_T("保存查询结果为Excel文件成功！"));
	}
		CATCH_ALL(e)
	{
		//错误类型很多，根据需要进行报错。
		AfxMessageBox(_T("Excel文件保存失败。"));
	}
	END_CATCH_ALL;
}





int FlashUpdateMain::FlashUpdateGetNode()
{
	UINT16 module_id_uint = GetDlgItemInt(IDC_module_select, NULL, 0);;
	CString model_id_string;
	GetDlgItemText(IDC_module_select, model_id_string);
	CString software_type;
	GetDlgItemText(IDC_SOFTWARE_SELECT, software_type);
	if (software_type == _T(""))
		throw std::runtime_error("Please Choose Software Type!");
	else if (software_type == _T("REC")) {

		service_code = 0x20;
		tx_fu_data[2] = 0x14;
		tx_fu_data[3] = 0x55;
		if (model_id_string == _T("")) 
			throw std::runtime_error("Please choose module!");
		else if (model_id_string == _T("all module")) {

			tx_fu_data[0] = 0xFF;
			tx_fu_data[1] = 0xFF;
			uniflash->m_u16UpdaingNodeAdd = 0x3F;
			uniflash->NodeOffset = 0x20;
		}
		else {

			uniflash->Module_number = module_id_uint;
			uniflash->NodeOffset = 0x20;
			uniflash->m_u16UpdaingNodeAdd = 0x1F + module_id_uint;
			*(UINT16*)tx_fu_data = 1 << module_id_uint;
		}
	}
	else if (software_type == _T("INV")) {

		service_code = 0x21;
		tx_fu_data[2] = 0x25;
		tx_fu_data[3] = 0x4A;
		if (model_id_string == _T(""))
			throw std::runtime_error("Please choose module!");

		else if (model_id_string == _T("all module")) {

			tx_fu_data[0] = 0xFF;
			tx_fu_data[1] = 0xFF;
			uniflash->NodeOffset = 0x10;
			uniflash->m_u16UpdaingNodeAdd = 0x3F;
		}
		else {

			uniflash->Module_number = module_id_uint;
			uniflash->NodeOffset = 0x10;
			uniflash->m_u16UpdaingNodeAdd = 0x0F + module_id_uint;
			*(UINT16*)tx_fu_data = 1 << module_id_uint;
		}
	}
	else if (software_type == _T("BYP")) {

		service_code = 0x22;
		tx_fu_data[0] = 0x00;
		tx_fu_data[1] = 0x00;
		tx_fu_data[2] = 0xAA;
		tx_fu_data[3] = 0x55;
		uniflash->Module_number = module_id_uint;
		uniflash->NodeOffset = 0x00;
		uniflash->m_u16UpdaingNodeAdd = 0x0F;
	}

	
	return TRUE;
}

int FlashUpdateMain::ParameterRefresh()
{
	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST2);
	pList->DeleteAllItems();
	time_out_cnt = 0;

	return 0;
}


void FlashUpdateMain::OnBnClickedClearMsg()
{
	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST2);
	pList->DeleteAllItems();
}


void FlashUpdateMain::OnClose()
{
	if (AfxMessageBox(_T("Are you Sure Exit FlashUpdate?"), MB_YESNO) == IDNO)return;

	CDialogEx::OnClose();
	CDialogEx::OnCancel();
}


void FlashUpdateMain::OnOK()
{

	//CDialogEx::OnOK();
}


void FlashUpdateMain::OnCancel()
{

	//CDialogEx::OnCancel();
}


void FlashUpdateMain::OnBnClickedBootLoader(){

	TryCloseDevice();
	try {
		CString file_path;
		OnBnClickedOpenCanDevice();

		GetDlgItemText(IDC_MFCEDITBROWSE1, file_path);
		Solver.reset(new BootFileResolve(file_path));
	
		Solver->FileResolve();
	}catch (std::runtime_error err) {

		AfxMessageBox(CString(err.what()));
		return;
	}
	SendFlashUpdateOrder();

	CButton *button = (CButton*)GetDlgItem(ID_START_FLASH_UPDATE);
	button->EnableWindow(0);

	button = (CButton*)GetDlgItem(ID_BOOT_LOADER);
	button->EnableWindow(0);

	uniflash.reset(new CAN_FlashupdateMsgHandle(*Solver));
	SetTimer(4, 500, NULL);

}


void FlashUpdateMain::OnCbnSelchangeModuleSelect()
{
	for (int i = 0; i < 16; ++i) {

		FlashUpdateProgressCtrl[i].ShowWindow(1);
	}
}


void FlashUpdateMain::ShowButton()
{
	CButton *button = (CButton*)GetDlgItem(ID_START_FLASH_UPDATE);
	button->EnableWindow(1);

	button = (CButton*)GetDlgItem(ID_BOOT_LOADER);
	button->EnableWindow(1);
	return;
}

void FlashUpdateMain::OnBnClickedStartMsg()
{
	
	SetTimer(3, 1000, NULL);
}


void FlashUpdateMain::OnBnClickedStopMsg()
{
	KillTimer(3);
}


void FlashUpdateMain::OnBnClickedMfccolorbutton1()
{
	CMFCColorButton *list_color = (CMFCColorButton*)GetDlgItem(IDC_MFCCOLORBUTTON1);
	DWORD color = list_color->GetColor();

	CListCtrl *pList = (CListCtrl*)GetDlgItem(IDC_LIST2);
	pList->SetBkColor(color);
	pList->SetTextBkColor(color);
}


bool FlashUpdateMain::ChangeSize(UINT id, int x, int y)
{
	CWnd *pwnd;
	pwnd = (CWnd*)GetDlgItem(id);
	if (pwnd != NULL) {

		CRect current_rect;
		pwnd->GetWindowRect(&current_rect);
		ScreenToClient(&current_rect);
		current_rect.left = current_rect.left*x / myRect.Width();
		current_rect.right = current_rect.right*x / myRect.Width();

		current_rect.top = current_rect.top*y / myRect.Height();
		current_rect.bottom = current_rect.bottom*y / myRect.Height();

		pwnd->MoveWindow(current_rect);
	}
	return false;
}


void FlashUpdateMain::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (nType != SIZE_MINIMIZED) {
		ChangeSize(IDC_MFCEDITBROWSE1, cx, cy);
		ChangeSize(IDC_DEVICE_SELECT, cx, cy);
		ChangeSize(IDC_BAUD_RATE_SELECT, cx, cy);
		ChangeSize(IDC_FLASH_UPDATE_TYPE, cx, cy);
		ChangeSize(IDC_DSP_SELECT, cx, cy);
		ChangeSize(IDC_FLASH_UPDATE_NODE_GET, cx, cy);
		ChangeSize(ID_START_FLASH_UPDATE, cx, cy);
		ChangeSize(IDC_MODULE_SELECT, cx, cy);
		ChangeSize(ID_CLOSE_CAN_DEVICE, cx, cy);
		ChangeSize(ID_BOOT_LOADER, cx, cy);
		ChangeSize(IDC_LIST2, cx, cy);
		ChangeSize(IDC_START_MSG, cx, cy);
		ChangeSize(IDC_STOP_MSG, cx, cy);
		ChangeSize(IDC_CLEAR_MSG, cx, cy);
		ChangeSize(IDC_RECORD_SAVED, cx, cy);
		ChangeSize(IDC_INV_MODILE16, cx, cy);
		ChangeSize(IDC_INV_MODILE15, cx, cy);
		ChangeSize(IDC_INV_MODILE14, cx, cy);
		ChangeSize(IDC_INV_MODILE13, cx, cy);
		ChangeSize(IDC_INV_MODILE12, cx, cy);
		ChangeSize(IDC_INV_MODILE11, cx, cy);
		ChangeSize(IDC_INV_MODILE10, cx, cy);
		ChangeSize(IDC_INV_MODILE9, cx, cy);
		ChangeSize(IDC_INV_MODILE8, cx, cy);
		ChangeSize(IDC_INV_MODILE7, cx, cy);
		ChangeSize(IDC_INV_MODILE6, cx, cy);
		ChangeSize(IDC_INV_MODILE5, cx, cy);
		ChangeSize(IDC_INV_MODILE4, cx, cy);
		ChangeSize(IDC_INV_MODILE3, cx, cy);
		ChangeSize(IDC_INV_MODILE2, cx, cy);
		ChangeSize(IDC_INV_MODILE1, cx, cy);
		ChangeSize(IDC_MFCCOLORBUTTON1, cx, cy);
		ChangeSize(IDC_STATIC, cx, cy);
		ChangeSize(IDC_STATIC1, cx, cy);
		ChangeSize(IDC_STATIC2, cx, cy);
		ChangeSize(IDC_STATIC3, cx, cy);
		ChangeSize(IDC_STATIC4, cx, cy);
		ChangeSize(IDC_STATIC5, cx, cy);
		ChangeSize(IDC_STATIC6, cx, cy);
		ChangeSize(IDC_STATIC7, cx, cy);
		ChangeSize(IDC_STATIC8, cx, cy);
		ChangeSize(IDC_STATIC9, cx, cy);
		ChangeSize(IDC_STATIC10, cx, cy);
		ChangeSize(IDC_STATIC11, cx, cy);
		ChangeSize(IDC_STATIC12, cx, cy);
		ChangeSize(IDC_STATIC13, cx, cy);
		ChangeSize(IDC_STATIC14, cx, cy);
		ChangeSize(IDC_STATIC15, cx, cy);
		ChangeSize(IDC_STATIC16, cx, cy);
		ChangeSize(IDC_STATIC17, cx, cy);
		ChangeSize(IDC_STATIC18, cx, cy);
		ChangeSize(IDC_STATIC19, cx, cy);
		ChangeSize(IDC_STATIC20, cx, cy);
		ChangeSize(IDC_STATIC21, cx, cy);
		ChangeSize(IDC_STATIC22, cx, cy);
		ChangeSize(IDC_STATIC23, cx, cy);
		ChangeSize(IDC_STATIC24, cx, cy);

		GetClientRect(&myRect);
	}
}
