
// ERNSDlg.cpp: 實作檔案
//

#include "stdafx.h"
#include "ERNS.h"
#include "ERNSDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 對 App About 使用 CAboutDlg 對話方塊

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

// 程式碼實作
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




// CERNSDlg 對話方塊
CERNSDlg::CERNSDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ERNS_DIALOG, pParent)
{
	m_ScheduledSenderNumber = 0;	///< 預定發送人數
	m_SenderCompletedNumber = 0;	///< 完成訊息發送人數
	m_RepliesCompletedNumber = 0;	///< 完成回覆人數
	m_RepliesState1Number = 0;		///< 狀態一人數
	m_RepliesState2Number = 0;		///< 狀態二人數
	m_RepliesState3Number = 0;		///< 狀態三人數
	m_RepliesState4Number = 0;		///< 狀態四人數
	m_RepliesState5Number = 0;		///< 狀態五人數

	// 產生範例檔
	FILE *fptr = NULL;
	fopen_s(&fptr, "SMS_SendList_Demo.txt", "rt");
	if (fptr == NULL) {
		fopen_s(&fptr, "SMS_SendList_Demo.txt", "at");
		if (fptr != NULL) {
			fprintf(fptr, "097869xxxx\n");
			fprintf(fptr, "090522xxxx\n");
			fclose(fptr);
		}
	}

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CERNSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_SMS_MESSAGE1, m_EditSmsMessage1);
	DDX_Control(pDX, IDC_EDIT_SMS_MESSAGE2, m_EditSmsMessage2);
	DDX_Control(pDX, IDC_STATIC_STATE1, m_StaticState1);
	DDX_Control(pDX, IDC_STATIC_STATE2, m_StaticState2);
	DDX_Control(pDX, IDC_STATIC_STATE3, m_StaticState3);
	DDX_Control(pDX, IDC_STATIC_STATE4, m_StaticState4);
	DDX_Control(pDX, IDC_STATIC_STATE5, m_StaticState5);
	DDX_Control(pDX, IDC_STATIC_STATE6, m_StaticState6);
	DDX_Control(pDX, IDC_STATIC_STATE7, m_StaticState7);
	DDX_Control(pDX, IDC_STATIC_INFO, m_StaticInfo);

	DDX_Control(pDX, IDC_EDIT_SEND_MESSAGE_LIST_FILENAME, m_Filename);
}

BEGIN_MESSAGE_MAP(CERNSDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SEND_SMS, &CERNSDlg::OnBnClickedButtonSendSms)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CERNSDlg::OnBnClickedButtonUpdate)
	ON_WM_KEYDOWN()
	ON_BN_CLICKED(IDC_BUTTON_SELECT_SEND_MESSAGE_LIST_FILE, &CERNSDlg::OnBnClickedButtonSelectSendMessageListFile)
END_MESSAGE_MAP()


// CERNSDlg 訊息處理常式

BOOL CERNSDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 將 [關於...] 功能表加入系統功能表。

	// IDM_ABOUTBOX 必須在系統命令範圍之中。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 設定此對話方塊的圖示。當應用程式的主視窗不是對話方塊時，
	// 框架會自動從事此作業
	SetIcon(m_hIcon, TRUE);			// 設定大圖示
	SetIcon(m_hIcon, FALSE);		// 設定小圖示

	// TODO: 在此加入額外的初始設定
	m_EditSmsMessage1.m_Length1 = &m_EditSmsMessage1.m_MessageLength;
	m_EditSmsMessage1.m_Length2 = &m_EditSmsMessage2.m_MessageLength;
	m_EditSmsMessage1.m_StaticInfo = &this->m_StaticInfo;
	m_EditSmsMessage1.SetWindowTextA("新北市立聯合醫院發生 XXXX 事件進行緊急召回，請回傳緊急召回代碼 66999+代碼：");

	m_EditSmsMessage2.m_Length1 = &m_EditSmsMessage1.m_MessageLength;
	m_EditSmsMessage2.m_Length2 = &m_EditSmsMessage2.m_MessageLength;
	m_EditSmsMessage2.m_StaticInfo = &this->m_StaticInfo;
	m_EditSmsMessage2.SetWindowTextA("如1.在院內為(669991)、2.三十分鐘內抵院(669992)依序、3.一小時內抵院、4.兩小時內抵院、5.無法返院。");

	ReadSendList("SMS_SendList_DEF.txt", "SMS_SendList_DEF.txt");
	Update();
	SetTimer(0, 60 * 1000 * 1, NULL);
	return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
}

void CERNSDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果將最小化按鈕加入您的對話方塊，您需要下列的程式碼，
// 以便繪製圖示。對於使用文件/檢視模式的 MFC 應用程式，
// 框架會自動完成此作業。

void CERNSDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 繪製的裝置內容

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 將圖示置中於用戶端矩形
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 描繪圖示
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 當使用者拖曳最小化視窗時，
// 系統呼叫這個功能取得游標顯示。
HCURSOR CERNSDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CERNSDlg::ReadSendList(CString PathName, CString FileName) {
	char * ptr = PathName.GetBuffer();
	FILE * fptr = NULL;
	fopen_s(&fptr, ptr, "rt");

	if (fptr == NULL) {
		AfxMessageBox("開啟檔案失敗.");
		return;
	}
	m_SMS_SendList.clear();
	m_Filename.SetWindowTextA(FileName);
	int i;
	char Buffer[16];
	CString phone;

	while (!feof(fptr)) {
		fgets(Buffer, 16, fptr);
		i = strlen(Buffer);
		while ((Buffer[i - 1] == 0x09 || Buffer[i - 1] == 0x0a) && i-- > 0);
		Buffer[i] = '\0';
		phone = Buffer;
		if (phone != "") {
			m_SMS_SendList.push_back(phone);
		}
		phone = "";
	};
	fclose(fptr);
	m_ScheduledSenderNumber = m_SMS_SendList.size();	
}
void CERNSDlg::Update(void)
{
	// 進行資料庫資料更新作業
	CString sSqlString, sConnectStringDsn, sConnectStringDsnLess;

	// 設定資料庫連線的相關參數
	CString sDriver = "MySQL ODBC 3.51 Driver";
	CString sServer = "192.168.255.196";
	CString sPort = "3306";
	CString sDsn = "sms";
	CString sUID = "cht";
	CString sPWD = "cht_odbc";
	CString sDatabase = "chtsms";

	// 產生資料庫的連線字串 DSN or DSN LESS
	sConnectStringDsn.Format("DSN=%s;UID=%s;PWD=%s", sDsn, sUID, sPWD);
	sConnectStringDsnLess.Format("DRIVER={%s};Server=%s;Database=%s;User=%s;Password=%s;Option=3;", sDriver, sServer, sDatabase, sUID, sPWD);

	CDatabase database;
	TRY{
		// 連線資料庫 DSN or DSN LESS
		database.OpenEx(sConnectStringDsnLess);
		
		//
		sSqlString = "select count(a.Tophone) as ScheduledSenderNumber from (select * from chtsms.fromuseridtosend where FromUserID = 'ERNS' AND SendTime >= (NOW() - interval 24 hour) group by ToPhone) a; ";
		database.ExecuteSQL(sSqlString);
		CRecordset recset1(&database);
		recset1.Open(CRecordset::forwardOnly, sSqlString, CRecordset::readOnly);
		CString ScheduledSenderNumber;
		recset1.GetFieldValue("ScheduledSenderNumber", ScheduledSenderNumber);
		m_ScheduledSenderNumber = _ttoi(ScheduledSenderNumber);

		//
		sSqlString = "select count(a.Tophone) as SenderCompletedNumber from (select * from chtsms.fromuseridtosend where FromUserID = 'ERNS' AND SendTime >= (NOW() - interval 24 hour) AND isSend = '1' group by ToPhone) a;";
		database.ExecuteSQL(sSqlString);
		CRecordset recset2(&database);
		recset2.Open(CRecordset::forwardOnly, sSqlString, CRecordset::readOnly);
		CString SenderCompletedNumber;
		recset2.GetFieldValue("SenderCompletedNumber", SenderCompletedNumber);
		m_SenderCompletedNumber = _ttoi(SenderCompletedNumber);

		//
		sSqlString = "select count(e.SendID) as RepliesCompletedNumber from(select d.ID as SendID, d.FromUserID as FromUserID, d.ToPhone as ToPhone, d.SendTime as SendTime, d.isSend as isSend, c.MessageBody as RecvMessageBody, c.RecvTime as RecvTime from(select * from chtsms.fromuseridtosend where FromUserID = 'ERNS' AND SendTime >= (NOW() - interval 24 hour) group by ToPhone) as d left join(select b.* from(select h24.FromPhone, max(h24.recvTime) as recvTime from(select * from chtsms.receivemessage	where recvTime >= (NOW() - interval 24 hour) AND MessageBody like '66999%') h24	group by h24.FromPhone) as a left join chtsms.receivemessage as b on a.recvTime = b.recvTime AND a.FromPhone = b.FromPhone) as c on d.ToPhone = c.FromPhone) as e where e.RecvTime is not null;";
		database.ExecuteSQL(sSqlString);
		CRecordset recset3(&database);
		recset3.Open(CRecordset::forwardOnly, sSqlString, CRecordset::readOnly);
		CString RepliesCompletedNumber;
		recset3.GetFieldValue("RepliesCompletedNumber", RepliesCompletedNumber);
		m_RepliesCompletedNumber = _ttoi(RepliesCompletedNumber);

		//
		sSqlString = "select count(e.SendID) as RepliesState1Number from(select d.ID as SendID, d.FromUserID as FromUserID, d.ToPhone as ToPhone, d.SendTime as SendTime, d.isSend as isSend, c.MessageBody as RecvMessageBody, c.RecvTime as RecvTime from(select * from chtsms.fromuseridtosend where FromUserID = 'ERNS' AND SendTime >= (NOW() - interval 24 hour) group by ToPhone) as d left join(select b.* from(select h24.FromPhone, max(h24.recvTime) as recvTime from(select * from chtsms.receivemessage	where recvTime >= (NOW() - interval 24 hour) AND MessageBody like '66999%') h24	group by h24.FromPhone) as a left join chtsms.receivemessage as b on a.recvTime = b.recvTime AND a.FromPhone = b.FromPhone) as c on d.ToPhone = c.FromPhone) as e where e.RecvTime is not null AND e.RecvMessageBody = '669991';";
		database.ExecuteSQL(sSqlString);
		CRecordset recset4(&database);
		recset4.Open(CRecordset::forwardOnly, sSqlString, CRecordset::readOnly);
		CString RepliesState1Number;
		recset4.GetFieldValue("RepliesState1Number", RepliesState1Number);
		m_RepliesState1Number = _ttoi(RepliesState1Number);

		//
		sSqlString = "select count(e.SendID) as RepliesState2Number from(select d.ID as SendID, d.FromUserID as FromUserID, d.ToPhone as ToPhone, d.SendTime as SendTime, d.isSend as isSend, c.MessageBody as RecvMessageBody, c.RecvTime as RecvTime from(select * from chtsms.fromuseridtosend where FromUserID = 'ERNS' AND SendTime >= (NOW() - interval 24 hour) group by ToPhone) as d left join(select b.* from(select h24.FromPhone, max(h24.recvTime) as recvTime from(select * from chtsms.receivemessage	where recvTime >= (NOW() - interval 24 hour) AND MessageBody like '66999%') h24	group by h24.FromPhone) as a left join chtsms.receivemessage as b on a.recvTime = b.recvTime AND a.FromPhone = b.FromPhone) as c on d.ToPhone = c.FromPhone) as e where e.RecvTime is not null AND e.RecvMessageBody = '669992';";
		database.ExecuteSQL(sSqlString);
		CRecordset recset5(&database);
		recset5.Open(CRecordset::forwardOnly, sSqlString, CRecordset::readOnly);
		CString RepliesState2Number;
		recset5.GetFieldValue("RepliesState2Number", RepliesState2Number);
		m_RepliesState2Number = _ttoi(RepliesState2Number);

		//
		sSqlString = "select count(e.SendID) as RepliesState3Number from(select d.ID as SendID, d.FromUserID as FromUserID, d.ToPhone as ToPhone, d.SendTime as SendTime, d.isSend as isSend, c.MessageBody as RecvMessageBody, c.RecvTime as RecvTime from(select * from chtsms.fromuseridtosend where FromUserID = 'ERNS' AND SendTime >= (NOW() - interval 24 hour) group by ToPhone) as d left join(select b.* from(select h24.FromPhone, max(h24.recvTime) as recvTime from(select * from chtsms.receivemessage	where recvTime >= (NOW() - interval 24 hour) AND MessageBody like '66999%') h24	group by h24.FromPhone) as a left join chtsms.receivemessage as b on a.recvTime = b.recvTime AND a.FromPhone = b.FromPhone) as c on d.ToPhone = c.FromPhone) as e where e.RecvTime is not null AND e.RecvMessageBody = '669993';";
		database.ExecuteSQL(sSqlString);
		CRecordset recset6(&database);
		recset6.Open(CRecordset::forwardOnly, sSqlString, CRecordset::readOnly);
		CString RepliesState3Number;
		recset6.GetFieldValue("RepliesState3Number", RepliesState3Number);
		m_RepliesState3Number = _ttoi(RepliesState3Number);

		//
		sSqlString = "select count(e.SendID) as RepliesState4Number from(select d.ID as SendID, d.FromUserID as FromUserID, d.ToPhone as ToPhone, d.SendTime as SendTime, d.isSend as isSend, c.MessageBody as RecvMessageBody, c.RecvTime as RecvTime from(select * from chtsms.fromuseridtosend where FromUserID = 'ERNS' AND SendTime >= (NOW() - interval 24 hour) group by ToPhone) as d left join(select b.* from(select h24.FromPhone, max(h24.recvTime) as recvTime from(select * from chtsms.receivemessage	where recvTime >= (NOW() - interval 24 hour) AND MessageBody like '66999%') h24	group by h24.FromPhone) as a left join chtsms.receivemessage as b on a.recvTime = b.recvTime AND a.FromPhone = b.FromPhone) as c on d.ToPhone = c.FromPhone) as e where e.RecvTime is not null AND e.RecvMessageBody = '669994';";
		database.ExecuteSQL(sSqlString);
		CRecordset recset7(&database);
		recset7.Open(CRecordset::forwardOnly, sSqlString, CRecordset::readOnly);
		CString RepliesState4Number;
		recset7.GetFieldValue("RepliesState4Number", RepliesState4Number);
		m_RepliesState4Number = _ttoi(RepliesState4Number);

		//
		sSqlString = "select count(e.SendID) as RepliesState5Number from(select d.ID as SendID, d.FromUserID as FromUserID, d.ToPhone as ToPhone, d.SendTime as SendTime, d.isSend as isSend, c.MessageBody as RecvMessageBody, c.RecvTime as RecvTime from(select * from chtsms.fromuseridtosend where FromUserID = 'ERNS' AND SendTime >= (NOW() - interval 24 hour) group by ToPhone) as d left join(select b.* from(select h24.FromPhone, max(h24.recvTime) as recvTime from(select * from chtsms.receivemessage	where recvTime >= (NOW() - interval 24 hour) AND MessageBody like '66999%') h24	group by h24.FromPhone) as a left join chtsms.receivemessage as b on a.recvTime = b.recvTime AND a.FromPhone = b.FromPhone) as c on d.ToPhone = c.FromPhone) as e where e.RecvTime is not null AND e.RecvMessageBody = '669995';";
		database.ExecuteSQL(sSqlString);
		CRecordset recset8(&database);
		recset8.Open(CRecordset::forwardOnly, sSqlString, CRecordset::readOnly);
		CString RepliesState5Number;
		recset8.GetFieldValue("RepliesState5Number", RepliesState5Number);
		m_RepliesState5Number = _ttoi(RepliesState5Number);

		// 關閉資料庫連線
		database.Close();
	}
	CATCH(CDBException, e) {
		database.Close();
	}
	END_CATCH;

	CString State;
	State.Format("發送狀態(已發送人數/預定發送人數) : %d / %d", m_SenderCompletedNumber, m_ScheduledSenderNumber);
	m_StaticState1.SetWindowTextA(State);
	State.Format("回覆狀態(已回覆人數/已發送人數): %d / %d", m_RepliesCompletedNumber, m_SenderCompletedNumber);
	m_StaticState2.SetWindowTextA(State);
	State.Format("已抵達院內(抵達人數/已發送人數): %d / %d", m_RepliesState1Number, m_SenderCompletedNumber);
	m_StaticState3.SetWindowTextA(State);
	State.Format("30 分鐘內返院(回覆人數/已發送人數): %d / %d", m_RepliesState2Number, m_SenderCompletedNumber);
	m_StaticState4.SetWindowTextA(State);
	State.Format("一小時內返院(回覆人數/已發送人數): %d / %d", m_RepliesState3Number, m_SenderCompletedNumber);
	m_StaticState5.SetWindowTextA(State);
	State.Format("兩小時內返院(抵達人數/已發送人數): %d / %d", m_RepliesState4Number, m_SenderCompletedNumber);
	m_StaticState6.SetWindowTextA(State);
	State.Format("無法返院(無法返院人數/已發送人數): %d / %d", m_RepliesState5Number, m_SenderCompletedNumber);
	m_StaticState7.SetWindowTextA(State);
}


void CERNSDlg::OnBnClickedButtonSendSms()
{
	CString Text1, Text2;
	m_EditSmsMessage1.GetWindowTextA(Text1);
	m_EditSmsMessage2.GetWindowTextA(Text2);

	if (m_EditSmsMessage1.m_MessageLength > 70 || m_EditSmsMessage2.m_MessageLength > 70) {
		AfxMessageBox("訊息長度超過簡訊允許長度，請調整後再行發送。");
		return;
	}

	// 進行資料庫資料更新作業
	CString sSqlString, sConnectStringDsn, sConnectStringDsnLess;

	// 設定資料庫連線的相關參數
	CString sDriver = "MySQL ODBC 3.51 Driver";
	CString sServer = "192.168.255.196";
	CString sPort = "3306";
	CString sDsn = "sms";
	CString sUID = "cht";
	CString sPWD = "cht_odbc";
	CString sDatabase = "chtsms";

	// 產生資料庫的連線字串 DSN or DSN LESS
	sConnectStringDsn.Format("DSN=%s;UID=%s;PWD=%s", sDsn, sUID, sPWD);
	sConnectStringDsnLess.Format("DRIVER={%s};Server=%s;Database=%s;User=%s;Password=%s;Option=3;", sDriver, sServer, sDatabase, sUID, sPWD);
	

	CDatabase database;
	TRY{
		// 連線資料庫 DSN or DSN LESS
		database.OpenEx(sConnectStringDsnLess);
		// 發送訊息給召回人員
		for (unsigned int i = 0; i < m_SMS_SendList.size(); i++) {
			if (m_SMS_SendList[i] != "") {
				sSqlString.Format("INSERT INTO `chtsms`.`fromuseridtosend` (`FromUserID`, `ToPhone`, `MessageBody`, `SendTime`, `isSend`) VALUES ('ERNS', '%s', '%s', NOW(), '0');", m_SMS_SendList[i], Text1);
				database.ExecuteSQL(sSqlString);
				sSqlString.Format("INSERT INTO `chtsms`.`fromuseridtosend` (`FromUserID`, `ToPhone`, `MessageBody`, `SendTime`, `isSend`) VALUES ('ERNS', '%s', '%s', NOW(), '0');", m_SMS_SendList[i], Text2);
				database.ExecuteSQL(sSqlString);
			}
		}
		// 關閉資料庫連線
		database.Close();
	}
	CATCH(CDBException, e) {
		database.Close();
	}
	END_CATCH;
	AfxMessageBox("資料寫入完畢開始發送訊息");
}


void CERNSDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值
	Update();
	CDialogEx::OnTimer(nIDEvent);
}


void CERNSDlg::OnBnClickedButtonUpdate()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	Update();
}

void CERNSDlg::OnBnClickedButtonSelectSendMessageListFile()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	CString szFilter = "SMS File(*.txt)|*.txt";
	CFileDialog Dialog(true, NULL, NULL, OFN_HIDEREADONLY, szFilter, NULL);

	TCHAR szDir[256];
	GetCurrentDirectory(256, szDir);
	Dialog.m_ofn.lpstrInitialDir = szDir;
	//按下OK
	if (Dialog.DoModal() == IDOK) {
		//開啟檔案成功
		CString szFileTitle = Dialog.GetFileName(); //取得開啟檔案的全名(包含路徑)
		CString szFileName = Dialog.GetPathName();	//取得開啟檔案的全名(包含路徑)
		ReadSendList(szFileName, szFileTitle);
	}
}
