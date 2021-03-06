// CanAnalyse.cpp: 实现文件
//

#include "stdafx.h"
#include "flash_api.h"
#include "CanAnalyse.h"
#include "afxdialogex.h"
#include "ControlCAN.h"
#include "flash_apiDlg.h"
#include <afxdb.h>
#include "CAN_FLASHupdateMsgHandle.h"
// CanAnalyse 对话框

IMPLEMENT_DYNAMIC(CanAnalyse, CDialogEx)

CanAnalyse::CanAnalyse(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CAN_ANALYSE, pParent)
{

		m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
		tx_msgnew.DataLen = 0;
		tx_msgnew.ExternFlag = 0;
		tx_msgnew.ID = 0;
		tx_msgnew.RemoteFlag = 0;
		tx_msgnew.Reserved[0] = 0;
		tx_msgnew.Reserved[1] = 0;
		tx_msgnew.Reserved[2] = 0;
		tx_msgnew.SendType = 0;
		tx_msgnew.TimeFlag = 0;
		tx_msgnew.Data[0] = 0;
		tx_msgnew.Data[1] = 0;
		tx_msgnew.Data[2] = 0;
		tx_msgnew.Data[3] = 0;
		tx_msgnew.Data[4] = 0;
		tx_msgnew.Data[5] = 0;
		tx_msgnew.Data[6] = 0;
		tx_msgnew.Data[7] = 0;
}

CanAnalyse::~CanAnalyse()
{
	Cflash_apiDlg Reopen_Main;
	Reopen_Main.DoModal();
	/*Cflash_apiDlg* dlg;
	dlg = new Cflash_apiDlg;
	dlg->Create(IDD_FLASH_API_DIALOG); //非模态对话框ID号
	dlg->ShowWindow(SW_SHOW);*/
	delete []rx_msg;
}

BOOL CanAnalyse::OnInitDialog() {

	SetDlgItemText(IDC_ID_FILTER, _T("0x1FFFFFFF"));


	CDialogEx::OnInitDialog();
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);


	CListCtrl *pList = (CListCtrl*)GetDlgItem(IDC_LIST1);
	pList->InsertColumn(0, _T("Sort"), LVCFMT_LEFT, 80);
	pList->InsertColumn(1, _T("Time"), LVCFMT_LEFT, 100);
	pList->InsertColumn(2, _T("ID"), LVCFMT_LEFT, 150);
	pList->InsertColumn(3, _T("DLC"), LVCFMT_LEFT, 80);
	pList->InsertColumn(4, _T("DATA"), LVCFMT_LEFT, 390);
	// Blue Color
	pList->SetBkColor(RGB(137, 207, 240));
	pList->SetTextBkColor(RGB(135, 206, 235));
	// Pink Color
	//pList->SetBkColor(RGB(249, 204, 226));
	//pList->SetTextBkColor(RGB(249, 204, 226));
	CComboBox *pCmob = (CComboBox*)GetDlgItem(IDC_BAUD_RATE);
	pCmob->SetCurSel(7);
	pCmob = (CComboBox*)GetDlgItem(IDC_HEXADECIMAL);
	pCmob->SetCurSel(1);
	return TRUE;
}
void CanAnalyse::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CanAnalyse, CDialogEx)
	ON_BN_CLICKED(IDC_START_CAN_DEVICE, &CanAnalyse::OnBnClickedStartCanDevice)
	ON_BN_CLICKED(IDC_START_RECEIVED, &CanAnalyse::OnBnClickedStartReceived)
	ON_BN_CLICKED(IDC_CLOSE_CAN_DEVICE, &CanAnalyse::OnBnClickedCloseCanDevice)
	ON_BN_CLICKED(IDC_CLEAR_RECEIVED, &CanAnalyse::OnBnClickedClearReceived)
	ON_BN_CLICKED(IDC_STOP_RECEIVED, &CanAnalyse::OnBnClickedStopReceived)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_WM_TIMECHANGE()
	ON_BN_CLICKED(IDC_MSG_SAVED, &CanAnalyse::OnBnClickedMsgSaved)
	ON_BN_CLICKED(IDC_SEND_MESSAGE, &CanAnalyse::OnBnClickedSendMessage)
END_MESSAGE_MAP()


// CanAnalyse 消息处理程序


void CanAnalyse::OnBnClickedStartCanDevice()
{
	if (AfxMessageBox(_T("Are you Sure Open Device?"), MB_YESNO) == IDNO) return;
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
	DWORD	test;
	vic.AccCode = 0x00000000;
	vic.AccMask = 0xFFFFFFFF;
	vic.Filter = 0;
	vic.Mode = 0;
	CString BuadRateSelect;
	GetDlgItemText(IDC_BAUD_RATE, BuadRateSelect);
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
		MessageBox(_T("请选择波特率！"), _T("警告！！！"), MB_OK | MB_ICONQUESTION);
		return;
	}
	vic.Reserved = 0;
	test = VCI_OpenDevice(device_type, device_ind, can_ind);

	if (test != STATUS_OK) {
		MessageBox(_T("Open Device Failed!!!"), _T("Caution"), MB_OK | MB_ICONQUESTION);
		return;
	}

	test = VCI_InitCAN(device_type, device_ind, can_ind, &vic);
	if (test == STATUS_ERR) {
		VCI_CloseDevice(device_type, device_ind);
		MessageBox(_T("Init Device Failed！"), _T("Caution"), MB_OK | MB_ICONQUESTION);
		return;
	}

	test = VCI_StartCAN(device_type, device_ind, can_ind);
	if (test == STATUS_ERR) {
		VCI_CloseDevice(device_type, device_ind);
		MessageBox(_T("Start Device Failed！"), _T("Caution"), MB_OK | MB_ICONQUESTION);
		return;
	}
	else {
		SetWindowText(_T("Open Device Success！"));
	}
}


void CanAnalyse::OnBnClickedStartReceived()
{
	SetTimer(1, 100, NULL);
}


void CanAnalyse::OnBnClickedCloseCanDevice()
{
		
	DWORD	test;
	VCI_BOARD_INFO	vbi;
	if (!VCI_ReadBoardInfo(device_type, device_ind, &vbi)) {

		AfxMessageBox(_T("没有设备，关闭个毛，擦擦擦!!!"));
		return;
	}
	test = VCI_CloseDevice(device_type, device_ind);
	if (test == STATUS_ERR) {
		MessageBox(_T("Close Device Failed！"), _T("Caution!!!"), MB_OK | MB_ICONQUESTION);
	}
	else {
		SetWindowText(_T("Close Device Success!"));
	}
}


void CanAnalyse::OnBnClickedClearReceived()
{
	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST1);
	pList->DeleteAllItems();
}


void CanAnalyse::OnBnClickedStopReceived()
{
	KillTimer(1);
}

void CanAnalyse::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	//AnimateWindow(1000, AW_CENTER);
}


void CanAnalyse::OnTimer(UINT_PTR nIDEvent)
{

	VCI_INIT_CONFIG	vic;
	DWORD	msg_num;
	
	vic.AccCode = 0x00000000;
	vic.AccMask = 0xFFFFFFFF;
	vic.Filter = 0;
	vic.Mode = 0;
	vic.Timing0 = 0x03;
	vic.Timing1 = 0x1C;
	vic.Reserved = 0;
	DWORD count;
	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST1);
	SYSTEMTIME time;
	CString	get_id_from_dlg, msg_id;
	CString	a, b, c, d, e, f;

	msg_num = VCI_GetReceiveNum(device_type, device_ind, can_ind);
	rx_msg = new VCI_CAN_OBJ[msg_num];

	VCI_Receive(device_type, device_ind, can_ind, rx_msg, msg_num, 500);
	for (DWORD i = 0; i < msg_num; ++i) {

		msg_id.Format(_T("0X%x"), rx_msg[i].ID);
		GetDlgItemText(IDC_ID_FILTER, get_id_from_dlg);

		if ((get_id_from_dlg != msg_id)&(get_id_from_dlg != (_T("0X1FFFFFFF"))))	continue;

		a.Format(_T("0x%08x"), (int)(rx_msg[i].ID));

		b.Format(_T("%02x"), (int)(rx_msg[i].DataLen));
		c = _T("");
		for (int j = 0; j < rx_msg[i].DataLen; ++j) {

			d.Format(_T("%02x  "), rx_msg[i].Data[j]);
			c += d;
		}
		count = pList->GetItemCount();
		e.Format(_T("%d"), count);
		GetSystemTime(&time);
		f.Format(_T("%d:%d:%d:%d"), ((time.wHour + 8) > 24) ? time.wHour + 8 - 24 : time.wHour + 8, time.wMinute, time.wSecond, time.wMilliseconds);
		pList->InsertItem(0, e);
		pList->SetItemText(0, 1, f);
		pList->SetItemText(0, 2, a);
		pList->SetItemText(0, 3, b);
		pList->SetItemText(0, 4, c);

	}
	
	
	CDialogEx::OnTimer(nIDEvent);
}


/*
void CanAnalyse::OnBnClickedMsgSaved()
{
	if(AfxMessageBox(_T("Are you Sure Save Message?"), MB_YESNO) == IDNO)return;
	CFile file;
	if (!file.Open(_T("./msg.csv"), CFile::modeCreate | CFile::modeReadWrite)) {

		AfxMessageBox(_T("Save File Failed!"));
		return;
	}
	SInfo info;
	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST1);
	DWORD	i = 0, nCount = pList->GetItemCount();
	while (i < nCount) {

		info.sort = _tstoll(pList->GetItemText(i, 0));
		wcscpy_s(info.time, pList->GetItemText(i, 1));
		wcscpy_s(info.ID, pList->GetItemText(i, 2));
		wcscpy_s(info.DLC, pList->GetItemText(i, 3));
		wcscpy_s(info.DATA, pList->GetItemText(i, 4));
		file.Write(&info, sizeof(info));
		++i;
	}

	file.Close();
}
*/
//导出数据到excel中

/*
void CanAnalyse::OnBnClickedMsgSaved()
{
	//弹出保存文件对话框，输入保存的日志文件名，并选择保存类型 txt | xls  
	SYSTEMTIME st;
	GetLocalTime(&st);

	CString str;
	str.Format(_T("%4d%02d%02d%02d%02d%02d.txt"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	CFileDialog dlg(FALSE, _T("txt"), str, OFN_EXPLORER | OFN_PATHMUSTEXIST, _T("Text Files (*.txt)|*.txt|WorkSheet Files (*.xls)|*.xls||"));
	if (IDOK != dlg.DoModal())
	{
		return;
	}

	//开始导出  
	CString strPathName = dlg.GetPathName();
	CString strExt = dlg.GetFileExt();
	strExt.MakeLower();

	int ret = 0;

	if (_T("txt") == strExt)//保存txt  
	{
		ret = ExportToTXT(strPathName);
	}
	else if (_T("xls") == strExt)//保存xls  
	{
		ret = ExportToXLS(strPathName);
	}

	return;
}
int CanAnalyse::ExportToTXT(CString& strFilePath)
{
	USES_CONVERSION;

	CFile mytxtFile;
	CString strCaption, strMsg;

	if (!mytxtFile.Open(strFilePath, CFile::modeCreate | CFile::modeReadWrite))
	{
		strCaption.LoadString(IDS_MESSAGEBOX_CAPTION);
		strMsg.LoadString(IDS_OPENFILE_FAILED);

		strMsg += strFilePath;

		MessageBox(strMsg, strCaption, MB_OK | MB_ICONERROR, this);

		return FALSE;
	}

	//计算列的宽度  
	int width[5] = { 0 };
	int i = 0, j = 0, nLen = 0;
	CListCtrl* m_logList = (CListCtrl*)GetDlgItem(IDC_LIST1);
	int nCount = m_logList->GetItemCount();

	//构造格式化字符串  
	char format[512] = { 0 };
	char buf[1024] = { 0 };

	CString str0, str1, str2, str3, str4;

	str0.LoadString(IDS_COME);
	width[0] = strlen(T2A(str0));

	str1.LoadString(IDS_TIME);
	width[1] = strlen(T2A(str1));

	str2.LoadString(IDS_USERS);
	width[2] = strlen(T2A(str2));

	str3.LoadString(IDS_CONTENT);
	width[3] = strlen(T2A(str3));

	str4.LoadString(IDS_REMAKE);
	width[4] = strlen(T2A(str4));

	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < nCount; j++)
		{
			nLen = strlen(T2A(m_logList.GetItemText(j, i)));
			if (nLen > width[i]) width[i] = nLen;
		}

		//多点间隔  
		width[i] += 10;
	}

	//format  
	sprintf_s(format, "%%-%ds %%-%ds %%-%ds %%-%ds %%-%ds \r\n", width[0], width[1], width[2], width[3], width[4]);

	//打印列名  
	sprintf_s(buf, format, T2A(str0), T2A(str1), T2A(str2), T2A(str3), T2A(str4));
	mytxtFile.Write(buf, strlen(buf));

	for (i = 0; i < nCount; i++)
	{
		str0 = m_logList->GetItemText(i, 0);
		str1 = m_logList->GetItemText(i, 1);
		str2 = m_logList->GetItemText(i, 2);
		str3 = m_logList->GetItemText(i, 3);
		str4 = m_logList->GetItemText(i, 4);

		sprintf_s(buf, format, T2A(str0), T2A(str1), T2A(str2), T2A(str3), T2A(str4));
		mytxtFile.Write(buf, strlen(buf));
	}

	mytxtFile.Close();

	return TRUE;
}


int CanAnalyse::ExportToXLS(CString& strFilePath)
{
	CDatabase database;
	CString sDriver = _T("MICROSOFT EXCEL DRIVER (*.XLS)"); // Excel安装驱动  
	CString sSql;
	CString strCaption, strMsg;

	try
	{
		//检测是否安装了excel驱动程序  
		if (!CheckExcelDriver())
		{
			strCaption.LoadString(IDS_MESSAGEBOX_CAPTION);
			strMsg.LoadString(IDS_EXCEL_DRIVER_FAILED);

			MessageBox(strMsg, strCaption, MB_OK | MB_ICONERROR);
			return FALSE;
		}

		//创建excel连接字符串  
		sSql.Format(_T("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s"),
			sDriver, strFilePath, strFilePath);

		//打开xls文件失败，成功则创建xls数据库文件  
		if (!database.OpenEx(sSql, CDatabase::noOdbcDialog))
		{
			strCaption.LoadString(IDS_MESSAGEBOX_CAPTION);
			strMsg.LoadString(IDS_OPEN_EXCEL_FAILED);

			MessageBox(strMsg, strCaption, MB_OK | MB_ICONERROR);
			return FALSE;
		}

		//创建表  
		CString str0, str1, str2, str3, str4;

		str0.LoadString(IDS_COME);
		str1.LoadString(IDS_TIME);
		str2.LoadString(IDS_USERS);
		str3.LoadString(IDS_CONTENT);
		str4.LoadString(IDS_REMAKE);

		sSql.Format(_T("CREATE TABLE log (%s TEXT, %s TEXT, %s TEXT, %s TEXT, %s TEXT)"),
			str0, str1, str2, str3, str4);
		database.ExecuteSQL(sSql);

		//循环导入数据  
		int i = 0, j = 0;
		CString str5, str6, str7, str8, str9;
		CListCtrl* m_logList = (CListCtrl*)GetDlgItem(IDC_LIST1);
		int nCount = m_logList->GetItemCount();
		for (i = 0; i < nCount; i++)
		{
			str5 = m_logList->GetItemText(i, 0);
			str6 = m_logList->GetItemText(i, 1);
			str7 = m_logList->GetItemText(i, 2);
			str8 = m_logList->GetItemText(i, 3);
			str9 = m_logList->GetItemText(i, 4);

			sSql.Format(_T("INSERT INTO log (%s, %s, %s, %s, %s) values('%s', '%s', '%s', '%s', '%s')"),
				str0, str1, str2, str3, str4, str5, str6, str7, str8, str9);
			database.ExecuteSQL(sSql);
		}

		//关闭xls数据库  
		database.Close();
	}
	catch (...)
	{
		//提示excel操作出现异常  
		strCaption.LoadString(IDS_MESSAGEBOX_CAPTION);
		strMsg.LoadString(IDS_EXCEL_OPENFILE_FAILED);

		MessageBox(strMsg, strCaption, MB_OK | MB_ICONERROR);
		return FALSE;
	}

	return TRUE;
}





BOOL CanAnalyse::CheckExcelDriver()
{
	TCHAR szBuf[2001] = { 0 };
	WORD cbBufMax = 2000;
	WORD cbBufOut = 0;
	TCHAR *pszBuf = szBuf;

	//获取已安装驱动的名称  
	if (!SQLGetInstalledDrivers(szBuf, cbBufMax, &cbBufOut))
	{
		return FALSE;
	}

	//查找excel  
	while (_tcslen(pszBuf) > 0)
	{
		if (NULL != _tcsstr(pszBuf, _T("Excel")))
		{
			return TRUE;
		}

		pszBuf += _tcslen(pszBuf) + 1;
	}

	return FALSE;
}


*/



void CanAnalyse::OnBnClickedMsgSaved()
{

	if (AfxMessageBox(_T("Are you Sure Save Message?"), MB_YESNO) == IDNO)return;
	//// TODO: Add your control notification handler code here
	//首先判断列表框中是否有记录
	CListCtrl* m_list = (CListCtrl*)GetDlgItem(IDC_LIST1);
	//// TODO: Add your control notification handler code here
	//首先判断列表框中是否有记录
	if (m_list->GetItemCount() <= 0)
	{
		AfxMessageBox(_T("列表中没有记录需要保存！"));
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


/*void CExcelTestDlg::OnBnClickedInput()
{
 // TODO: Add your control notification handler code here
 CFileDialog dlg( TRUE,//TRUE或FALSE。TRUE为打开文件；FALSE为保存文件
                 _T("xls"), //为缺省的扩展名
                _T( "FileList"), //为显示在文件名组合框的编辑框的文件名，一般可选NULL 
                 OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,//为对话框风格，一般为OFN_HIDEREADONLY   |   OFN_OVERWRITEPROMPT,即隐藏只读选项和覆盖已有文件前提示。 
                 _T("Excel 文件(*.xls)|*.xls||")//为下拉列表枢中显示文件类型
                  );
 dlg.m_ofn.lpstrTitle = _T("导入数据");
 
 if (dlg.DoModal() != IDOK)
  return;
 CString strFilePath;
 //获得文件路径名
 strFilePath = dlg.GetPathName();
 //判断文件是否已经存在，存在则打开文件
 DWORD dwRe = GetFileAttributes(strFilePath);
 if ( dwRe != (DWORD)-1 )
 {
  //ShellExecute(NULL, NULL, strFilePath, NULL, NULL, SW_RESTORE); 
 }
 else return;
 CDatabase db;//数据库库需要包含头文件 #include <afxdb.h>
 CString sDriver =_T("MICROSOFT EXCEL DRIVER (*.XLS)") ; // Excel驱动
 CString sSql,arr[3];
 
 sSql.Format(_T("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s"),sDriver, strFilePath, strFilePath);
 if(!db.OpenEx(sSql,CDatabase::noOdbcDialog))//连接数据源DJB．xls
 {
  MessageBox(_T("打开EXCEL文件失败!","错误"));
  return;
 }
 //打开EXCEL表
 CRecordset pset(&db);
 m_list.DeleteAllItems();
 AfxMessageBox(_T("OK"));
sSql = "SELECT 学号,姓名,成绩 "       
               "FROM EXCELDEMO";            
			   // "ORDER BY 姓名";
sSql.Format(_T("SELECT * FROM [Sheet1$]"));
pset.Open(CRecordset::forwardOnly, sSql, CRecordset::readOnly);
while (!pset.IsEOF())
{
	pset.GetFieldValue(_T("学号"), arr[0]);//前面字段必须与表中的相同，否则出错。
	pset.GetFieldValue(_T("姓名"), arr[1]);
	pset.GetFieldValue(_T("成绩"), arr[2]);

	int count = m_list.GetItemCount();//插入到ListCtrl中
	m_list.InsertItem(count, arr[0]);
	m_list.SetItemText(count, 1, arr[1]);
	m_list.SetItemText(count, 2, arr[2]);
	pset.MoveNext();
}
db.Close();

MessageBox(_T("Excel数据成功导入系统!", "导入成功"));

}

//导出数据到excel中



void CExcelTestDlg::OnBnClickedOutput()
{
	//// TODO: Add your control notification handler code here
	//首先判断列表框中是否有记录
	if (m_list.GetItemCount() <= 0)
	{
		AfxMessageBox(_T("列表中没有记录需要保存！"));
		return;
	}
	//打开另存为对话框 ，需要包含 #include <Afxdlgs.h>
	CFileDialog dlg(FALSE,
		_T("xls"),
		_T("FileList"),
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("Excel 文件(*.xls)|*.xls||"));
	dlg.m_ofn.lpstrTitle = _T("文件列表另存为");

	if (dlg.DoModal() != IDOK)
		return;
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
	CString sDriver = _T("MICROSOFT EXCEL DRIVER (*.XLS)";)  // Excel驱动
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

		iColumnNum = m_list.GetHeaderCtrl()->GetItemCount();
		iRowCount = m_list.GetItemCount();
		sSql = _T(" CREATE TABLE DSO_DX ( ");
		strInsert = _T(" INSERT INTO DSO_DX ( ");
		//获得列标题名称
		lvCol.mask = LVCF_TEXT; //必需设置，说明LVCOLUMN变量中pszText参数有效
		lvCol.cchTextMax = 32; //必设，pszText参数所指向的字符串的大小
		lvCol.pszText = strColName.GetBuffer(32); //必设，pszText 所指向的字符串的实际存储位置。
												  //以上三个参数设置后才能通过 GetColumn()函数获得列标题的名称
		for (i = 0; i< iColumnNum; i++)
		{
			if (!(m_list.GetColumn(i,&lvCol)))
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
		char chTemp[33];
		for (j = 0; j<iRowCount; j++)
		{
			memset(chTemp,0,33);
			for (i = 0; i<iColumnNum; i++)
			{
				m_list.GetItemText(j,i,chTemp,33);
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

}*/

void CanAnalyse::OnBnClickedSendMessage()
{
	CAN_PACKED_PROTOCOL_U *tx_msg = new CAN_PACKED_PROTOCOL_U;
	tx_msg->Frame = tx_msgnew;
	tx_msg->PackedMsg.RemoteFlag = 0;// 数据帧
	tx_msg->PackedMsg.ExternFlag = 0;// 标准帧

	tx_msg->PackedMsg.b6DestinationMacId = 0x10;
	tx_msg->PackedMsg.b7ServiceCode = 0x21;
	tx_msg->PackedMsg.b10MsgClass = CAN_CTRL_MSG_CLASS;
	tx_msg->PackedMsg.b1Fragment = NONFRAG_MSG;
	tx_msg->PackedMsg.b1RsRq = RQ_MSG;
	tx_msg->PackedMsg.b6SourceMacId = MAC_ID_MON;
	tx_msg->PackedMsg.DataLen = 6;
	/*tx_msg->PackedMsg.MsgData[0] = 0x02;
	tx_msg->PackedMsg.MsgData[1] = 0x00;
	tx_msg->PackedMsg.MsgData[2] = 0x25;
	tx_msg->PackedMsg.MsgData[3] = 0x4A;*/

	tx_msg->PackedMsg.MsgData[0] = 0x00;
	tx_msg->PackedMsg.MsgData[1] = 0x00;
	tx_msg->PackedMsg.MsgData[2] = 0x00;
	tx_msg->PackedMsg.MsgData[3] = 0x00;

	VCI_Transmit(device_type, device_ind, can_ind, &tx_msg->Frame, 1);


	delete tx_msg;
	return ;
}
