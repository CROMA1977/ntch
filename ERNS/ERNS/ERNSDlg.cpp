
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
	m_ScheduledSenderNumber = 0;	
	m_SenderCompletedNumber = 0;	
	m_RepliesCompletedNumber = 0;	
	m_RepliesState1Number = 0;		
	m_RepliesState2Number = 0;		
	m_RepliesState3Number = 0;		
	m_RepliesState4Number = 0;		
	m_RepliesState5Number = 0;		
	m_RepliesState6Number = 0;		

	// 產生範例檔
	FILE *fptr = NULL;
	fopen_s(&fptr, "SMS_SendList_Demo.csv", "rt");
	if (fptr == NULL) {
		fopen_s(&fptr, "SMS_SendList_Demo.csv", "at");
		if (fptr != NULL) {
			fprintf(fptr, "097869xxxx,姓名,群組,職稱,科室\n");
			fprintf(fptr, "090522xxxx,姓名,群組,職稱,科室");
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
	DDX_Control(pDX, IDC_STATIC_STATE8, m_StaticState8);
	DDX_Control(pDX, IDC_STATIC_INFO, m_StaticInfo);

	DDX_Control(pDX, IDC_EDIT_SEND_MESSAGE_LIST_FILENAME, m_Filename);
	DDX_Control(pDX, IDC_ERNS_STATE_LIST, m_ListCtrlRecallSituation);
	DDX_Control(pDX, IDC_STATIC_STATE, m_UpdateTime);

	DDX_Control(pDX, IDC_IDC_COMBO_TEMPLATE, m_ComboTemplate);
	DDX_Control(pDX, IDC_IDC_COMBO_DISTRICT, m_ComboDistrict);
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
	ON_BN_CLICKED(IDC_BUTTON_GET_STATE, &CERNSDlg::OnBnClickedButtonGetState)
	ON_BN_CLICKED(IDC_BUTTON_USE_TEMPLATE, &CERNSDlg::OnBnClickedButtonUseTemplate)
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

	m_EditSmsMessage2.m_Length1 = &m_EditSmsMessage1.m_MessageLength;
	m_EditSmsMessage2.m_Length2 = &m_EditSmsMessage2.m_MessageLength;
	m_EditSmsMessage2.m_StaticInfo = &this->m_StaticInfo;
	
	CRect rect;
	m_ListCtrlRecallSituation.m_CERNSDlg = this;
	m_ListCtrlRecallSituation.GetClientRect(&rect);
	m_ListCtrlRecallSituation.SetExtendedStyle(m_ListCtrlRecallSituation.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_AUTOARRANGE | LVS_EX_INFOTIP);
	
	m_ListCtrlRecallSituation.InsertColumn(0, _T("群組"), LVCFMT_CENTER,118);
	m_ListCtrlRecallSituation.InsertColumn(1, _T("預定召回"), LVCFMT_CENTER,75);
	m_ListCtrlRecallSituation.InsertColumn(2, _T("已確認動態"), LVCFMT_CENTER,90);
	m_ListCtrlRecallSituation.InsertColumn(3, _T("已返院"), LVCFMT_CENTER,65);
	m_ListCtrlRecallSituation.InsertColumn(4, _T("10 分鐘內返院"), LVCFMT_CENTER,110);
	m_ListCtrlRecallSituation.InsertColumn(5, _T("20 分鐘內返院"), LVCFMT_CENTER,110);
	m_ListCtrlRecallSituation.InsertColumn(6, _T("30 分鐘內返院"), LVCFMT_CENTER,110);
	m_ListCtrlRecallSituation.InsertColumn(7, _T("60 分鐘內返院"), LVCFMT_CENTER, 110);
	m_ListCtrlRecallSituation.InsertColumn(8, _T("無法返院"), LVCFMT_CENTER,80);
	m_ListCtrlRecallSituation.InsertColumn(9, _T("尚未回覆"), LVCFMT_CENTER,80);

	m_ComboTemplate.AddString("測試用模板(990)");
	m_ComboTemplate.AddString("嚴重火災模板(999)");
	m_ComboTemplate.AddString("嚴重地震模板(998)");
	m_ComboTemplate.AddString("嚴重車禍模板(997)");
	m_ComboTemplate.AddString("核生化模板(996)");
	m_ComboTemplate.AddString("院內資安模板(995)");
	m_ComboTemplate.AddString("院內消防模板(994)");
	m_ComboTemplate.AddString("院內電力模板(993)");
	m_ComboTemplate.AddString("院內水力模板(992)");
	m_ComboTemplate.AddString("院內空調模板(991)");
	m_ComboTemplate.SetCurSel(0);

	m_SMS_TemplateListA.push_back("新北市立聯合醫院%s進行 [緊急召回系統稽核]，請回覆簡訊進行稽核:");
	m_SMS_TemplateListA.push_back("新北市立聯合醫院%s出現 [大量火災傷患]，須緊急召回人員，請回傳預定回院時間:");
	m_SMS_TemplateListA.push_back("新北市立聯合醫院%s出現 [大量地震傷患]，須緊急召回人員，請回傳預定回院時間:");
	m_SMS_TemplateListA.push_back("新北市立聯合醫院%s出現 [大量車禍傷患]，須緊急召回人員，請回傳預定回院時間:");
	m_SMS_TemplateListA.push_back("新北市立聯合醫院%s出現 [大量核生化傷患]，須緊急召回人員，請回傳預定回院時間:");
	m_SMS_TemplateListA.push_back("新北市立聯合醫院%s發生 [資訊系統中斷事件]，須緊急召回人員，請回傳預定回院時間:");
	m_SMS_TemplateListA.push_back("新北市立聯合醫院%s發生 [消防事件]，須緊急召回人員，請回傳預定回院時間:");
	m_SMS_TemplateListA.push_back("新北市立聯合醫院%s發生 [電力失效事件]，須緊急召回人員，請回傳預定回院時間:");
	m_SMS_TemplateListA.push_back("新北市立聯合醫院%s發生 [水力失效事件]，須緊急召回人員，請回傳預定回院時間:");
	m_SMS_TemplateListA.push_back("新北市立聯合醫院%s發生 [空調失效事件]，須緊急召回人員，請回傳預定回院時間:");

	m_SMS_TemplateListB.push_back("在院(%s0),10分到院(%s1),20分到院(%s2),30分到院(%s3),60分到院(%s6),無法60分回院(%s9)");
	m_SMS_TemplateListB.push_back("在院(%s0),10分到院(%s1),20分到院(%s2),30分到院(%s3),60分到院(%s6),無法60分回院(%s9)");
	m_SMS_TemplateListB.push_back("在院(%s0),10分到院(%s1),20分到院(%s2),30分到院(%s3),60分到院(%s6),無法60分回院(%s9)");
	m_SMS_TemplateListB.push_back("在院(%s0),10分到院(%s1),20分到院(%s2),30分到院(%s3),60分到院(%s6),無法60分回院(%s9)");
	m_SMS_TemplateListB.push_back("在院(%s0),10分到院(%s1),20分到院(%s2),30分到院(%s3),60分到院(%s6),無法60分回院(%s9)");
	m_SMS_TemplateListB.push_back("在院(%s0),10分到院(%s1),20分到院(%s2),30分到院(%s3),60分到院(%s6),無法60分回院(%s9)");
	m_SMS_TemplateListB.push_back("在院(%s0),10分到院(%s1),20分到院(%s2),30分到院(%s3),60分到院(%s6),無法60分回院(%s9)");
	m_SMS_TemplateListB.push_back("在院(%s0),10分到院(%s1),20分到院(%s2),30分到院(%s3),60分到院(%s6),無法60分回院(%s9)");
	m_SMS_TemplateListB.push_back("在院(%s0),10分到院(%s1),20分到院(%s2),30分到院(%s3),60分到院(%s6),無法60分回院(%s9)");
	m_SMS_TemplateListB.push_back("在院(%s0),10分到院(%s1),20分到院(%s2),30分到院(%s3),60分到院(%s6),無法60分回院(%s9)");

	m_SMS_MessageCodeList.push_back("990");
	m_SMS_MessageCodeList.push_back("999");
	m_SMS_MessageCodeList.push_back("998");
	m_SMS_MessageCodeList.push_back("997");
	m_SMS_MessageCodeList.push_back("996");
	m_SMS_MessageCodeList.push_back("995");
	m_SMS_MessageCodeList.push_back("994");
	m_SMS_MessageCodeList.push_back("993");
	m_SMS_MessageCodeList.push_back("992");
	m_SMS_MessageCodeList.push_back("991");

	m_ComboDistrict.AddString("全院區");
	m_ComboDistrict.AddString("三重院區");
	m_ComboDistrict.AddString("板橋院區");
	m_ComboDistrict.SetCurSel(0);

	m_SMS_DistrictCodeList.push_back(" 兩院區 ");
	m_SMS_DistrictCodeList.push_back(" 三重院區 ");
	m_SMS_DistrictCodeList.push_back(" 板橋院區 ");
	UseTemplateSetMessage();

	ReadSendList("SMS_SendList_DEF.csv", "SMS_SendList_DEF.csv");
	Update();

	EnableToolTips(TRUE);
	m_ListCtrlRecallSituation.EnableToolTips(TRUE);

	SetTimer(0, 60 * 3000 * 1, NULL);
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
		CString ErrorMessage;
		ErrorMessage.Format("開啟檔案 \"%s\" 失敗", FileName);
		AfxMessageBox(ErrorMessage);
		return;
	}
	m_SMS_SendList.clear();
	m_SMS_GroupList.clear();
	m_SMS_DepartmentList.clear();
	m_Filename.SetWindowTextA(FileName);
	int i,index;
	char Buffer[128];
	CString String[5];
	CString & Phone			= String[0];
	CString & Name			= String[1];
	CString & Group			= String[2];
	CString & JobTitle		= String[3];
	CString & Department	= String[4];

	CSenderInfo  Info;
	int LineID = 1;
	while (!feof(fptr)) {
		Buffer[0] = '\0';
		fgets(Buffer, 128, fptr);
		i = strlen(Buffer);
		// 去除換行符號
		while ((Buffer[i - 1] == 0x09 || Buffer[i - 1] == 0x0a) && i-- > 0);
		Buffer[i] = '\0';

		if (strlen(Buffer) == 0)
			continue;

		// 分割資料
		String[0] = Buffer;
		for (int i = 0; i < 4; i++) {
			index = String[i].Find(_T(","));
			if (index == -1) {
				CString ErrorMessage;
				ErrorMessage.Format("第 %d 行資料欄位異常 每一行必須包含 電話,姓名,群組,職稱,科室", LineID);
				AfxMessageBox(ErrorMessage);
				break;
			}
			String[i+1] = String[i].Right(String[i].GetLength() - index - 1);
			String[i]	= String[i].Left(index);
		}
		
		Info.m_Phone		= Phone;
		Info.m_Name			= Name;
		Info.m_Group		= Group;
		Info.m_JobTitle		= JobTitle;
		Info.m_Department	= Department;

		if (Phone != "") {
			if (Phone.Left(1) != "0") {
				CString ErrorMessage;
				ErrorMessage.Format("第 %d 行資料欄位異常 電話號碼第一碼不為 0，請檢查是否在輸出成 CSV 檔時的欄位型態", LineID);
				AfxMessageBox(ErrorMessage);
			}
			m_SMS_SendList.push_back(Info);
		}

		Phone = "";
		Name = "";
		Group = "";
		JobTitle = "";
		Department = "";
		LineID++;
	};
	fclose(fptr);

	m_ScheduledSenderNumber = m_SMS_SendList.size();	

	// 產生群組資料表
	for (unsigned int i = 0, j = 0; i < m_SMS_SendList.size(); i++) {		
		for (j = 0; j < m_SMS_GroupList.size(); j++) {
			if (m_SMS_SendList[i].m_Group == m_SMS_GroupList[j])
				break;
		}
		if (j == m_SMS_GroupList.size()) {
			m_SMS_GroupList.push_back(m_SMS_SendList[i].m_Group);
		}
	}
	// 產生部門清單
	for (unsigned int i = 0, j = 0; i < m_SMS_SendList.size(); i++) {
		for (j = 0; j < m_SMS_DepartmentList.size(); j++) {
			if (m_SMS_SendList[i].m_Department == m_SMS_DepartmentList[j])
				break;
		}
		if (j == m_SMS_DepartmentList.size()) {
			m_SMS_DepartmentList.push_back(m_SMS_SendList[i].m_Department);
		}
	}

	m_ListCtrlRecallSituation.DeleteAllItems();

	for (unsigned int i = 0; i < m_SMS_GroupList.size(); i++) {
		m_ListCtrlRecallSituation.InsertItem(i, m_SMS_GroupList[i]);
		m_ListCtrlRecallSituation.SetItemText(i, 1, _T("0"));
		m_ListCtrlRecallSituation.SetItemText(i, 2, _T("0"));
		m_ListCtrlRecallSituation.SetItemText(i, 3, _T("0"));
		m_ListCtrlRecallSituation.SetItemText(i, 4, _T("0"));
		m_ListCtrlRecallSituation.SetItemText(i, 5, _T("0"));
		m_ListCtrlRecallSituation.SetItemText(i, 6, _T("0"));
		m_ListCtrlRecallSituation.SetItemText(i, 7, _T("0"));
		m_ListCtrlRecallSituation.SetItemText(i, 8, _T("0"));
		m_ListCtrlRecallSituation.SetItemText(i, 9, _T("0"));
	}
	m_ListCtrlRecallSituation.InsertItem(m_SMS_GroupList.size(), "-----------");
	m_ListCtrlRecallSituation.SetItemText(m_SMS_GroupList.size(), 1, "-----------");
	m_ListCtrlRecallSituation.SetItemText(m_SMS_GroupList.size(), 2, "-----------");
	m_ListCtrlRecallSituation.SetItemText(m_SMS_GroupList.size(), 3, "-----------");
	m_ListCtrlRecallSituation.SetItemText(m_SMS_GroupList.size(), 4, "-----------");
	m_ListCtrlRecallSituation.SetItemText(m_SMS_GroupList.size(), 5, "-----------");
	m_ListCtrlRecallSituation.SetItemText(m_SMS_GroupList.size(), 6, "-----------");
	m_ListCtrlRecallSituation.SetItemText(m_SMS_GroupList.size(), 7, "-----------");
	m_ListCtrlRecallSituation.SetItemText(m_SMS_GroupList.size(), 8, "-----------");
	m_ListCtrlRecallSituation.SetItemText(m_SMS_GroupList.size(), 9, "-----------");

	for (unsigned int i = 0; i < m_SMS_DepartmentList.size(); i++) {
		m_ListCtrlRecallSituation.InsertItem(i + m_SMS_GroupList.size() + 1, m_SMS_DepartmentList[i]);
		m_ListCtrlRecallSituation.SetItemText(i + m_SMS_GroupList.size() + 1, 1, _T("0"));
		m_ListCtrlRecallSituation.SetItemText(i + m_SMS_GroupList.size() + 1, 2, _T("0"));
		m_ListCtrlRecallSituation.SetItemText(i + m_SMS_GroupList.size() + 1, 3, _T("0"));
		m_ListCtrlRecallSituation.SetItemText(i + m_SMS_GroupList.size() + 1, 4, _T("0"));
		m_ListCtrlRecallSituation.SetItemText(i + m_SMS_GroupList.size() + 1, 5, _T("0"));
		m_ListCtrlRecallSituation.SetItemText(i + m_SMS_GroupList.size() + 1, 6, _T("0"));
		m_ListCtrlRecallSituation.SetItemText(i + m_SMS_GroupList.size() + 1, 7, _T("0"));
		m_ListCtrlRecallSituation.SetItemText(i + m_SMS_GroupList.size() + 1, 8, _T("0"));
		m_ListCtrlRecallSituation.SetItemText(i + m_SMS_GroupList.size() + 1, 9, _T("0"));
	}
}
void CERNSDlg::Update(void)
{
	// 取出訊息碼
	CString MessageCode = m_SMS_MessageCodeList[m_NowMessageID];	
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
		
		// 查詢 24 小時內 ERNS 系統發送的電話號碼
		sSqlString.Format("select count(a.Tophone) as ScheduledSenderNumber from (select * from chtsms.fromuseridtosend where FromUserID = 'ERNS%s' AND SendTime >= (NOW() - interval 2 hour) group by ToPhone) a; ", MessageCode);
		database.ExecuteSQL(sSqlString);
		CRecordset recset1(&database);
		recset1.Open(CRecordset::forwardOnly, sSqlString, CRecordset::readOnly);
		CString ScheduledSenderNumber;
		recset1.GetFieldValue("ScheduledSenderNumber", ScheduledSenderNumber);
		m_ScheduledSenderNumber = _ttoi(ScheduledSenderNumber);

		// 查詢 24 小時內 ERNS 系統完成發送的電話號碼
		sSqlString.Format("select count(a.Tophone) as SenderCompletedNumber from (select * from chtsms.fromuseridtosend where FromUserID = 'ERNS%s' AND SendTime >= (NOW() - interval 2 hour) AND isSend = '1' group by ToPhone) a;", MessageCode);
		database.ExecuteSQL(sSqlString);
		CRecordset recset2(&database);
		recset2.Open(CRecordset::forwardOnly, sSqlString, CRecordset::readOnly);
		CString SenderCompletedNumber;
		recset2.GetFieldValue("SenderCompletedNumber", SenderCompletedNumber);
		m_SenderCompletedNumber = _ttoi(SenderCompletedNumber);

		// 查詢 24 小時內 ERNS 系統完成發送的電話號碼已回復的數量
		sSqlString.Format("select count(e.SendID) as RepliesCompletedNumber from(select d.ID as SendID, d.FromUserID as FromUserID, d.ToPhone as ToPhone, d.SendTime as SendTime, d.isSend as isSend, c.MessageBody as RecvMessageBody, c.RecvTime as RecvTime from(select * from chtsms.fromuseridtosend where FromUserID = 'ERNS%s' AND SendTime >= (NOW() - interval 2 hour) group by ToPhone) as d left join(select b.* from(select h24.FromPhone, max(h24.recvTime) as recvTime from(select * from chtsms.receivemessage	where recvTime >= (NOW() - interval 2 hour) AND MessageBody like '%s%%') h24	group by h24.FromPhone) as a left join chtsms.receivemessage as b on a.recvTime = b.recvTime AND a.FromPhone = b.FromPhone) as c on d.ToPhone = c.FromPhone) as e where e.RecvTime is not null;", MessageCode, MessageCode);
		database.ExecuteSQL(sSqlString);
		CRecordset recset3(&database);
		recset3.Open(CRecordset::forwardOnly, sSqlString, CRecordset::readOnly);
		CString RepliesCompletedNumber;
		recset3.GetFieldValue("RepliesCompletedNumber", RepliesCompletedNumber);
		m_RepliesCompletedNumber = _ttoi(RepliesCompletedNumber);

		// 查詢 24 小時內 ERNS 系統完成發送的電話號碼回復為 0 的數量		
		sSqlString.Format("select count(e.SendID) as RepliesState1Number from(select d.ID as SendID, d.FromUserID as FromUserID, d.ToPhone as ToPhone, d.SendTime as SendTime, d.isSend as isSend, c.MessageBody as RecvMessageBody, c.RecvTime as RecvTime from(select * from chtsms.fromuseridtosend where FromUserID = 'ERNS%s' AND SendTime >= (NOW() - interval 2 hour) group by ToPhone) as d left join(select b.* from(select h24.FromPhone, max(h24.recvTime) as recvTime from(select * from chtsms.receivemessage	where recvTime >= (NOW() - interval 2 hour) AND MessageBody like '%s%%') h24	group by h24.FromPhone) as a left join chtsms.receivemessage as b on a.recvTime = b.recvTime AND a.FromPhone = b.FromPhone) as c on d.ToPhone = c.FromPhone) as e where e.RecvTime is not null AND e.RecvMessageBody = '%s0';", MessageCode, MessageCode, MessageCode);
		database.ExecuteSQL(sSqlString);
		CRecordset recset4(&database);
		recset4.Open(CRecordset::forwardOnly, sSqlString, CRecordset::readOnly);
		CString RepliesState1Number;
		recset4.GetFieldValue("RepliesState1Number", RepliesState1Number);
		m_RepliesState1Number = _ttoi(RepliesState1Number);

		// 查詢 24 小時內 ERNS 系統完成發送的電話號碼回復為 1 的數量
		sSqlString.Format("select count(e.SendID) as RepliesState2Number from(select d.ID as SendID, d.FromUserID as FromUserID, d.ToPhone as ToPhone, d.SendTime as SendTime, d.isSend as isSend, c.MessageBody as RecvMessageBody, c.RecvTime as RecvTime from(select * from chtsms.fromuseridtosend where FromUserID = 'ERNS%s' AND SendTime >= (NOW() - interval 2 hour) group by ToPhone) as d left join(select b.* from(select h24.FromPhone, max(h24.recvTime) as recvTime from(select * from chtsms.receivemessage	where recvTime >= (NOW() - interval 2 hour) AND MessageBody like '%s%%') h24	group by h24.FromPhone) as a left join chtsms.receivemessage as b on a.recvTime = b.recvTime AND a.FromPhone = b.FromPhone) as c on d.ToPhone = c.FromPhone) as e where e.RecvTime is not null AND e.RecvMessageBody = '%s1';", MessageCode, MessageCode, MessageCode);
		database.ExecuteSQL(sSqlString);
		CRecordset recset5(&database);
		recset5.Open(CRecordset::forwardOnly, sSqlString, CRecordset::readOnly);
		CString RepliesState2Number;
		recset5.GetFieldValue("RepliesState2Number", RepliesState2Number);
		m_RepliesState2Number = _ttoi(RepliesState2Number);

		// 查詢 24 小時內 ERNS 系統完成發送的電話號碼回復為 2 的數量
		sSqlString.Format("select count(e.SendID) as RepliesState3Number from(select d.ID as SendID, d.FromUserID as FromUserID, d.ToPhone as ToPhone, d.SendTime as SendTime, d.isSend as isSend, c.MessageBody as RecvMessageBody, c.RecvTime as RecvTime from(select * from chtsms.fromuseridtosend where FromUserID = 'ERNS%s' AND SendTime >= (NOW() - interval 2 hour) group by ToPhone) as d left join(select b.* from(select h24.FromPhone, max(h24.recvTime) as recvTime from(select * from chtsms.receivemessage	where recvTime >= (NOW() - interval 2 hour) AND MessageBody like '%s%%') h24	group by h24.FromPhone) as a left join chtsms.receivemessage as b on a.recvTime = b.recvTime AND a.FromPhone = b.FromPhone) as c on d.ToPhone = c.FromPhone) as e where e.RecvTime is not null AND e.RecvMessageBody = '%s2';", MessageCode, MessageCode, MessageCode);
		database.ExecuteSQL(sSqlString);
		CRecordset recset6(&database);
		recset6.Open(CRecordset::forwardOnly, sSqlString, CRecordset::readOnly);
		CString RepliesState3Number;
		recset6.GetFieldValue("RepliesState3Number", RepliesState3Number);
		m_RepliesState3Number = _ttoi(RepliesState3Number);

		// 查詢 24 小時內 ERNS 系統完成發送的電話號碼回復為 3 的數量
		sSqlString.Format("select count(e.SendID) as RepliesState4Number from(select d.ID as SendID, d.FromUserID as FromUserID, d.ToPhone as ToPhone, d.SendTime as SendTime, d.isSend as isSend, c.MessageBody as RecvMessageBody, c.RecvTime as RecvTime from(select * from chtsms.fromuseridtosend where FromUserID = 'ERNS%s' AND SendTime >= (NOW() - interval 2 hour) group by ToPhone) as d left join(select b.* from(select h24.FromPhone, max(h24.recvTime) as recvTime from(select * from chtsms.receivemessage	where recvTime >= (NOW() - interval 2 hour) AND MessageBody like '%s%%') h24	group by h24.FromPhone) as a left join chtsms.receivemessage as b on a.recvTime = b.recvTime AND a.FromPhone = b.FromPhone) as c on d.ToPhone = c.FromPhone) as e where e.RecvTime is not null AND e.RecvMessageBody = '%s3';", MessageCode, MessageCode, MessageCode);
		database.ExecuteSQL(sSqlString);
		CRecordset recset7(&database);
		recset7.Open(CRecordset::forwardOnly, sSqlString, CRecordset::readOnly);
		CString RepliesState4Number;
		recset7.GetFieldValue("RepliesState4Number", RepliesState4Number);
		m_RepliesState4Number = _ttoi(RepliesState4Number);

		// 查詢 24 小時內 ERNS 系統完成發送的電話號碼回復為 6 的數量		
		sSqlString.Format("select count(e.SendID) as RepliesState5Number from(select d.ID as SendID, d.FromUserID as FromUserID, d.ToPhone as ToPhone, d.SendTime as SendTime, d.isSend as isSend, c.MessageBody as RecvMessageBody, c.RecvTime as RecvTime from(select * from chtsms.fromuseridtosend where FromUserID = 'ERNS%s' AND SendTime >= (NOW() - interval 2 hour) group by ToPhone) as d left join(select b.* from(select h24.FromPhone, max(h24.recvTime) as recvTime from(select * from chtsms.receivemessage	where recvTime >= (NOW() - interval 2 hour) AND MessageBody like '%s%%') h24	group by h24.FromPhone) as a left join chtsms.receivemessage as b on a.recvTime = b.recvTime AND a.FromPhone = b.FromPhone) as c on d.ToPhone = c.FromPhone) as e where e.RecvTime is not null AND e.RecvMessageBody = '%s6';", MessageCode, MessageCode, MessageCode);
		database.ExecuteSQL(sSqlString);
		CRecordset recset8(&database);
		recset8.Open(CRecordset::forwardOnly, sSqlString, CRecordset::readOnly);
		CString RepliesState5Number;
		recset8.GetFieldValue("RepliesState5Number", RepliesState5Number);
		m_RepliesState5Number = _ttoi(RepliesState5Number);

		// 查詢 24 小時內 ERNS 系統完成發送的電話號碼回復為 9 的數量
		sSqlString.Format("select count(e.SendID) as RepliesState6Number from(select d.ID as SendID, d.FromUserID as FromUserID, d.ToPhone as ToPhone, d.SendTime as SendTime, d.isSend as isSend, c.MessageBody as RecvMessageBody, c.RecvTime as RecvTime from(select * from chtsms.fromuseridtosend where FromUserID = 'ERNS%s' AND SendTime >= (NOW() - interval 2 hour) group by ToPhone) as d left join(select b.* from(select h24.FromPhone, max(h24.recvTime) as recvTime from(select * from chtsms.receivemessage	where recvTime >= (NOW() - interval 2 hour) AND MessageBody like '%s%%') h24	group by h24.FromPhone) as a left join chtsms.receivemessage as b on a.recvTime = b.recvTime AND a.FromPhone = b.FromPhone) as c on d.ToPhone = c.FromPhone) as e where e.RecvTime is not null AND e.RecvMessageBody = '%s9';", MessageCode, MessageCode, MessageCode);
		database.ExecuteSQL(sSqlString);
		CRecordset recset9(&database);
		recset9.Open(CRecordset::forwardOnly, sSqlString, CRecordset::readOnly);
		CString RepliesState6Number;
		recset9.GetFieldValue("RepliesState6Number", RepliesState6Number);
		m_RepliesState6Number = _ttoi(RepliesState6Number);

		// 刷新回覆現況
		sSqlString.Format("SELECT d.ID AS SendID, d.FromUserID AS FromUserID, d.ToPhone AS ToPhone, d.SendTime AS SendTime, d.isSend AS isSend, c.MessageBody AS RecvMessageBody, c.RecvTime AS RecvTime	FROM(SELECT * FROM chtsms.fromuseridtosend WHERE FromUserID = 'ERNS%s' AND SendTime >= (NOW() - interval 2 hour)	GROUP BY ToPhone) AS d LEFT JOIN (SELECT b.* FROM(SELECT h24.FromPhone,	MAX(h24.recvTime) AS recvTime FROM(SELECT *	FROM chtsms.receivemessage WHERE recvTime >= (NOW() - interval 2 hour)	AND MessageBody LIKE '%s%%') h24 GROUP BY	h24.FromPhone) AS a	LEFT JOIN chtsms.receivemessage AS b ON	a.recvTime = b.recvTime	AND     a.FromPhone = b.FromPhone) AS c	ON d.ToPhone = c.FromPhone;", MessageCode, MessageCode);
		database.ExecuteSQL(sSqlString);
		CRecordset recset10(&database);
		recset10.Open(CRecordset::forwardOnly, sSqlString, CRecordset::readOnly);

		CString ToPhone, RecvMessageBody;
		while (!recset10.IsEOF()) {
			recset10.GetFieldValue("ToPhone", ToPhone);
			recset10.GetFieldValue("RecvMessageBody", RecvMessageBody);

			for (unsigned int i = 0; i < m_SMS_SendList.size(); i++) {
				if (m_SMS_SendList[i].m_Phone == ToPhone) {
					m_SMS_SendList[i].m_RecvMessageBody = RecvMessageBody;
					break;
				}
			}
			recset10.MoveNext();
		}
		// 關閉資料庫連線
		database.Close();
	}
	CATCH(CDBException, e) {
		database.Close();
		CString ErrorString = "資料庫指令異常，錯誤訊息為 " + e->m_strError + "。";
		AfxMessageBox(ErrorString);
	}
	END_CATCH;

	CString State;
	State.Format("發送狀態(已發送人數/預定發送人數) : %d / %d", m_SenderCompletedNumber, m_ScheduledSenderNumber);
	m_StaticState1.SetWindowTextA(State);
	State.Format("回覆狀態(已回覆人數/已發送人數): %d / %d", m_RepliesCompletedNumber, m_SenderCompletedNumber);
	m_StaticState2.SetWindowTextA(State);
	State.Format("已抵達院內(抵達人數/已發送人數): %d / %d", m_RepliesState1Number, m_SenderCompletedNumber);
	m_StaticState3.SetWindowTextA(State);
	State.Format("10 分鐘內返院(回覆人數/已發送人數): %d / %d", m_RepliesState2Number, m_SenderCompletedNumber);
	m_StaticState4.SetWindowTextA(State);
	State.Format("20 分鐘內返院(回覆人數/已發送人數): %d / %d", m_RepliesState3Number, m_SenderCompletedNumber);
	m_StaticState5.SetWindowTextA(State);
	State.Format("30 分鐘內返院(回覆人數/已發送人數): %d / %d", m_RepliesState4Number, m_SenderCompletedNumber);
	m_StaticState6.SetWindowTextA(State);
	State.Format("60 分鐘內返院(回覆人數/已發送人數): %d / %d", m_RepliesState5Number, m_SenderCompletedNumber);
	m_StaticState7.SetWindowTextA(State);
	State.Format("無法返院(無法返院人數/已發送人數): %d / %d", m_RepliesState6Number, m_SenderCompletedNumber);
	m_StaticState8.SetWindowTextA(State);

	// 計算群組的狀態並更新 List
	int Number[9] = { 0 };
	CString TipText[9],Text;
	for (unsigned int i = 0; i < m_SMS_GroupList.size(); i++) {
		for (unsigned int j = 0; j < m_SMS_SendList.size(); j++) {
			if (m_SMS_SendList[j].m_Group == m_SMS_GroupList[i]) {
				Number[0]++;
				if (TipText[0] == "") {
					Text.Format("%s %s %s", m_SMS_SendList[j].m_Department, m_SMS_SendList[j].m_Name, m_SMS_SendList[j].m_JobTitle);
					TipText[0] = Text;
				} else {
					Text.Format("/%s %s %s", m_SMS_SendList[j].m_Department, m_SMS_SendList[j].m_Name, m_SMS_SendList[j].m_JobTitle);
					TipText[0] += Text;
				}

				if (m_SMS_SendList[j].m_RecvMessageBody != "") {
					Number[1]++;					
					if (TipText[1] == "") {
						Text.Format("%s %s %s", m_SMS_SendList[j].m_Department, m_SMS_SendList[j].m_Name, m_SMS_SendList[j].m_JobTitle);
						TipText[1] = Text;
					} else {
						Text.Format("/%s %s %s", m_SMS_SendList[j].m_Department, m_SMS_SendList[j].m_Name, m_SMS_SendList[j].m_JobTitle);
						TipText[1] += Text;
					}
				} else {
					Number[8]++;
					if (TipText[8] == "") {
						Text.Format("%s %s %s", m_SMS_SendList[j].m_Department, m_SMS_SendList[j].m_Name, m_SMS_SendList[j].m_JobTitle);
						TipText[8] = Text;
					}else {
						Text.Format("/%s %s %s", m_SMS_SendList[j].m_Department, m_SMS_SendList[j].m_Name, m_SMS_SendList[j].m_JobTitle);
						TipText[8] += Text;
					}
				}
				Text.Format("%s0", MessageCode);
				if (m_SMS_SendList[j].m_RecvMessageBody == Text) {
					Number[2]++;
					if (TipText[2] == "") {
						Text.Format("%s %s %s", m_SMS_SendList[j].m_Department, m_SMS_SendList[j].m_Name, m_SMS_SendList[j].m_JobTitle);
						TipText[2] = Text;
					} else {
						Text.Format("/%s %s %s", m_SMS_SendList[j].m_Department, m_SMS_SendList[j].m_Name, m_SMS_SendList[j].m_JobTitle);
						TipText[2] += Text;
					}
					continue;
				}
				Text.Format("%s1", MessageCode);
				if (m_SMS_SendList[j].m_RecvMessageBody == Text) {
					Number[3]++;
					if (TipText[3] == "") {
						Text.Format("%s %s %s", m_SMS_SendList[j].m_Department, m_SMS_SendList[j].m_Name, m_SMS_SendList[j].m_JobTitle);
						TipText[3] = Text;
					} else {
						Text.Format("/%s %s %s", m_SMS_SendList[j].m_Department, m_SMS_SendList[j].m_Name, m_SMS_SendList[j].m_JobTitle);
						TipText[3] += Text;
					}
					continue;
				}
				Text.Format("%s2", MessageCode);
				if (m_SMS_SendList[j].m_RecvMessageBody == Text) {
					Number[4]++;
					if (TipText[4] == "") {
						Text.Format("%s %s %s", m_SMS_SendList[j].m_Department, m_SMS_SendList[j].m_Name, m_SMS_SendList[j].m_JobTitle);
						TipText[4] = Text;
					} else {
						Text.Format("/%s %s %s", m_SMS_SendList[j].m_Department, m_SMS_SendList[j].m_Name, m_SMS_SendList[j].m_JobTitle);
						TipText[4] += Text;
					}
					continue;
				}
				Text.Format("%s3", MessageCode);
				if (m_SMS_SendList[j].m_RecvMessageBody == Text) {
					Number[5]++;
					if (TipText[5] == "") {
						Text.Format("%s %s %s", m_SMS_SendList[j].m_Department, m_SMS_SendList[j].m_Name, m_SMS_SendList[j].m_JobTitle);
						TipText[5] = Text;
					} else {
						Text.Format("/%s %s %s", m_SMS_SendList[j].m_Department, m_SMS_SendList[j].m_Name, m_SMS_SendList[j].m_JobTitle);
						TipText[5] += Text;
					}
					continue;
				}
				Text.Format("%s6", MessageCode);
				if (m_SMS_SendList[j].m_RecvMessageBody == Text) {
					Number[6]++;
					if (TipText[6] == "") {
						Text.Format("%s %s %s", m_SMS_SendList[j].m_Department, m_SMS_SendList[j].m_Name, m_SMS_SendList[j].m_JobTitle);
						TipText[6] = Text;
					} else {
						Text.Format("/%s %s %s", m_SMS_SendList[j].m_Department, m_SMS_SendList[j].m_Name, m_SMS_SendList[j].m_JobTitle);
						TipText[6] += Text;
					}
					continue;
				}
				Text.Format("%s9", MessageCode);
				if (m_SMS_SendList[j].m_RecvMessageBody == Text) {
					Number[7]++;
					if (TipText[7] == "") {
						Text.Format("%s %s %s", m_SMS_SendList[j].m_Department, m_SMS_SendList[j].m_Name, m_SMS_SendList[j].m_JobTitle);
						TipText[7] = Text;
					}
					else {
						Text.Format("/%s %s %s", m_SMS_SendList[j].m_Department, m_SMS_SendList[j].m_Name, m_SMS_SendList[j].m_JobTitle);
						TipText[7] += Text;
					}
					continue;
				}
			}
		}
		for (int j = 0; j < 9; j++) {
			Text.Format("%d", Number[j]);
			m_ListCtrlRecallSituation.SetItemText(i, j+1, (LPTSTR)(LPCTSTR)Text);
			m_ListCtrlRecallSituation.SetItemToolTipText(i, j + 1, (LPTSTR)(LPCTSTR)TipText[j]);
			Number[j] = 0;
			TipText[j] = "";
		}
	}	
	for (unsigned int i = 0; i < m_SMS_DepartmentList.size(); i++) {
		for (unsigned int j = 0; j < m_SMS_SendList.size(); j++) {
			if (m_SMS_SendList[j].m_Department == m_SMS_DepartmentList[i]) {
				Number[0]++;
				if (TipText[0] == "") {
					Text.Format("%s %s %s", m_SMS_SendList[j].m_Department, m_SMS_SendList[j].m_Name, m_SMS_SendList[j].m_JobTitle);
					TipText[0] = Text;
				} else {
					Text.Format("/%s %s %s", m_SMS_SendList[j].m_Department, m_SMS_SendList[j].m_Name, m_SMS_SendList[j].m_JobTitle);
					TipText[0] += Text;
				}
				if (m_SMS_SendList[j].m_RecvMessageBody != "") {
					Number[1]++;
					if (TipText[1] == "") {
						Text.Format("%s %s %s", m_SMS_SendList[j].m_Department, m_SMS_SendList[j].m_Name, m_SMS_SendList[j].m_JobTitle);
						TipText[1] = Text;
					} else {
						Text.Format("/%s %s %s", m_SMS_SendList[j].m_Department, m_SMS_SendList[j].m_Name, m_SMS_SendList[j].m_JobTitle);
						TipText[1] += Text;
					}
				} else {
					Number[8]++;
					if (TipText[8] == "") {
						Text.Format("%s %s %s", m_SMS_SendList[j].m_Department, m_SMS_SendList[j].m_Name, m_SMS_SendList[j].m_JobTitle);
						TipText[8] = Text;
					} else {
						Text.Format("/%s %s %s", m_SMS_SendList[j].m_Department, m_SMS_SendList[j].m_Name, m_SMS_SendList[j].m_JobTitle);
						TipText[8] += Text;
					}
				}
				Text.Format("%s0", MessageCode);
				if (m_SMS_SendList[j].m_RecvMessageBody == Text) {
					Number[2]++;
					if (TipText[2] == "") {
						Text.Format("%s %s %s", m_SMS_SendList[j].m_Department, m_SMS_SendList[j].m_Name, m_SMS_SendList[j].m_JobTitle);
						TipText[2] = Text;
					} else {
						Text.Format("/%s %s %s", m_SMS_SendList[j].m_Department, m_SMS_SendList[j].m_Name, m_SMS_SendList[j].m_JobTitle);
						TipText[2] += Text;
					}
					continue;
				}
				Text.Format("%s1", MessageCode);
				if (m_SMS_SendList[j].m_RecvMessageBody == Text) {
					Number[3]++;
					if (TipText[3] == "") {
						Text.Format("%s %s %s", m_SMS_SendList[j].m_Department, m_SMS_SendList[j].m_Name, m_SMS_SendList[j].m_JobTitle);
						TipText[3] = Text;
					} else {
						Text.Format("/%s %s %s", m_SMS_SendList[j].m_Department, m_SMS_SendList[j].m_Name, m_SMS_SendList[j].m_JobTitle);
						TipText[3] += Text;
					}
					continue;
				}
				Text.Format("%s2", MessageCode);
				if (m_SMS_SendList[j].m_RecvMessageBody == Text) {
					Number[4]++;
					if (TipText[4] == "") {
						Text.Format("%s %s %s", m_SMS_SendList[j].m_Department, m_SMS_SendList[j].m_Name, m_SMS_SendList[j].m_JobTitle);
						TipText[4] = Text;
					} else {
						Text.Format("/%s %s %s", m_SMS_SendList[j].m_Department, m_SMS_SendList[j].m_Name, m_SMS_SendList[j].m_JobTitle);
						TipText[4] += Text;
					}
					continue;
				}
				Text.Format("%s3", MessageCode);
				if (m_SMS_SendList[j].m_RecvMessageBody == Text) {
					Number[5]++;
					if (TipText[5] == "") {
						Text.Format("%s %s %s", m_SMS_SendList[j].m_Department, m_SMS_SendList[j].m_Name, m_SMS_SendList[j].m_JobTitle);
						TipText[5] = Text;
					} else {
						Text.Format("/%s %s %s", m_SMS_SendList[j].m_Department, m_SMS_SendList[j].m_Name, m_SMS_SendList[j].m_JobTitle);
						TipText[5] += Text;
					}
					continue;
				}
				Text.Format("%s6", MessageCode);
				if (m_SMS_SendList[j].m_RecvMessageBody == Text) {
					Number[6]++;
					if (TipText[6] == "") {
						Text.Format("%s %s %s", m_SMS_SendList[j].m_Department, m_SMS_SendList[j].m_Name, m_SMS_SendList[j].m_JobTitle);
						TipText[6] = Text;
					} else {
						Text.Format("/%s %s %s", m_SMS_SendList[j].m_Department, m_SMS_SendList[j].m_Name, m_SMS_SendList[j].m_JobTitle);
						TipText[6] += Text;
					}
					continue;
				}
				Text.Format("%s9", MessageCode);
				if (m_SMS_SendList[j].m_RecvMessageBody == Text) {
					Number[7]++;
					if (TipText[7] == "") {
						Text.Format("%s %s %s", m_SMS_SendList[j].m_Department, m_SMS_SendList[j].m_Name, m_SMS_SendList[j].m_JobTitle);
						TipText[7] = Text;
					}
					else {
						Text.Format("/%s %s %s", m_SMS_SendList[j].m_Department, m_SMS_SendList[j].m_Name, m_SMS_SendList[j].m_JobTitle);
						TipText[7] += Text;
					}
					continue;
				}
			}
		}
		for (int j = 0; j < 9; j++) {
			CString Text;
			Text.Format("%d", Number[j]);
			m_ListCtrlRecallSituation.SetItemText(i+ m_SMS_GroupList.size()+1, j + 1, (LPTSTR)(LPCTSTR)Text);
			m_ListCtrlRecallSituation.SetItemToolTipText(i + m_SMS_GroupList.size() + 1, j + 1, (LPTSTR)(LPCTSTR)TipText[j]);
			Number[j] = 0;
			TipText[j] = "";
		}
	}

	CTime CurrentTime = CTime::GetCurrentTime();

	CString UpdateTime;
	UpdateTime.Format("召回情況[更新時間:%02d/%02d %02d:%02d](每3分鐘自動更新)", CurrentTime.GetMonth(), CurrentTime.GetDay(), CurrentTime.GetHour(), CurrentTime.GetMinute());	
	m_UpdateTime.SetWindowTextA(UpdateTime);
}

void CERNSDlg::OnBnClickedButtonSendSms()
{
	// 取出訊息碼
	CString MessageCode = m_SMS_MessageCodeList[m_NowMessageID];
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
			if (m_SMS_SendList[i].m_Phone != "") {
				sSqlString.Format("INSERT INTO `chtsms`.`fromuseridtosend` (`FromUserID`, `ToPhone`, `MessageBody`, `SendTime`, `isSend`) VALUES ('ERNS%s', '%s', '%s', NOW(), '0');", MessageCode, m_SMS_SendList[i].m_Phone, Text1);
				database.ExecuteSQL(sSqlString);
				sSqlString.Format("INSERT INTO `chtsms`.`fromuseridtosend` (`FromUserID`, `ToPhone`, `MessageBody`, `SendTime`, `isSend`) VALUES ('ERNS%s', '%s', '%s', NOW(), '0');", MessageCode, m_SMS_SendList[i].m_Phone, Text2);
				database.ExecuteSQL(sSqlString);
			}
		}
		// 關閉資料庫連線
		database.Close();
	}
	CATCH(CDBException, e) {
		database.Close();
		CString ErrorString = "資料庫指令異常，錯誤訊息為 " + e->m_strError + "。";
		AfxMessageBox(ErrorString);
	}
	END_CATCH;
	AfxMessageBox("資料寫入完畢開始發送訊息");
}


void CERNSDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值
	Update();
	OutputState(false);
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
	CString szFilter = "SMS File(*.csv)|*.csv";
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
		Update();
	}
}

void CERNSDlg::OnBnClickedButtonGetState()
{	
	OutputState(true);
}
void CERNSDlg::OutputList(int Item, int iSubItem) {
	FILE *fptr = NULL;
	fopen_s(&fptr, "SMS_SendList_List.txt", "wt");
	if ((unsigned int)Item < m_SMS_GroupList.size()) {
		OutputGroupList(fptr, m_SMS_GroupList[Item]);
		fclose(fptr);
		HINSTANCE hRslt = ShellExecute(NULL, _T("open"), _T("notepad.EXE"), _T("SMS_SendList_List.txt"), NULL, SW_SHOWNORMAL);
		assert(hRslt > (HINSTANCE)HINSTANCE_ERROR);
	}
	if ((unsigned int)Item > m_SMS_GroupList.size() && (unsigned int)Item <= (m_SMS_GroupList.size() + m_SMS_DepartmentList.size()) ) {
		Item -= (unsigned int)(m_SMS_GroupList.size() + 1);
		OutputDepartmentList(fptr, m_SMS_DepartmentList[Item]);
		fclose(fptr);
		HINSTANCE hRslt = ShellExecute(NULL, _T("open"), _T("notepad.EXE"), _T("SMS_SendList_List.txt"), NULL, SW_SHOWNORMAL);
		assert(hRslt > (HINSTANCE)HINSTANCE_ERROR);
	}
	fclose(fptr);
}

void CERNSDlg::OutputState(bool openFile)
{
	// 取出訊息碼
	CString MessageCode = m_SMS_MessageCodeList[m_NowMessageID];
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

	CString ToPhone, RecvMessageBody;
	CDatabase database;

	TRY{
		// 連線資料庫 DSN or DSN LESS
		database.OpenEx(sConnectStringDsnLess);

		sSqlString.Format("SELECT d.ID AS SendID, d.FromUserID AS FromUserID, d.ToPhone AS ToPhone, d.SendTime AS SendTime, d.isSend AS isSend, c.MessageBody AS RecvMessageBody, c.RecvTime AS RecvTime	FROM(SELECT * FROM chtsms.fromuseridtosend WHERE FromUserID = 'ERNS%s' AND SendTime >= (NOW() - interval 2 hour)	GROUP BY ToPhone) AS d LEFT JOIN (SELECT b.* FROM(SELECT h24.FromPhone,	MAX(h24.recvTime) AS recvTime FROM(SELECT *	FROM chtsms.receivemessage WHERE recvTime >= (NOW() - interval 2 hour)	AND MessageBody LIKE '%s%%') h24 GROUP BY	h24.FromPhone) AS a	LEFT JOIN chtsms.receivemessage AS b ON	a.recvTime = b.recvTime	AND     a.FromPhone = b.FromPhone) AS c	ON d.ToPhone = c.FromPhone;", MessageCode, MessageCode);
		database.ExecuteSQL(sSqlString);
		CRecordset recset(&database);
		recset.Open(CRecordset::forwardOnly, sSqlString, CRecordset::readOnly);

		while (!recset.IsEOF()) {
			recset.GetFieldValue("ToPhone", ToPhone);
			recset.GetFieldValue("RecvMessageBody", RecvMessageBody);

			for (unsigned int i = 0; i < m_SMS_SendList.size(); i++) {
				if (m_SMS_SendList[i].m_Phone == ToPhone) {
					m_SMS_SendList[i].m_RecvMessageBody = RecvMessageBody;
					break;
				}
			}
			recset.MoveNext();
		}
		// 關閉資料庫連線
		database.Close();
	}
		CATCH(CDBException, e) {
		database.Close();
		CString ErrorString = "資料庫指令異常，錯誤訊息為 " + e->m_strError + "。";
		AfxMessageBox(ErrorString);
	}
	END_CATCH;

	CTime CurrentTime = CTime::GetCurrentTime();

	FILE *fptr = NULL;
	char FileName[1024];
	sprintf_s(FileName, 1024, "SMS_SendList_Report-%04d-%02d%02d-%02d%02d.txt", CurrentTime.GetYear(), CurrentTime.GetMonth(), CurrentTime.GetDay(), CurrentTime.GetHour(), CurrentTime.GetMinute());
	fopen_s(&fptr, FileName, "wt");
	fprintf(fptr, "新北市立聯合醫院 %04d/%02d/%02d %02d:%02d 緊急召回情況\n", CurrentTime.GetYear(), CurrentTime.GetMonth(), CurrentTime.GetDay(), CurrentTime.GetHour(), CurrentTime.GetMinute());
	fprintf(fptr, "\n================================================各群組召回情況========================================================\n");

	for (unsigned int i = 0; i < m_SMS_GroupList.size(); i++) {
		if (i != 0)
			fprintf(fptr, "========================================================================================================\n");
		OutputGroupList(fptr, m_SMS_GroupList[i]);
	}
	fprintf(fptr, "\n================================================各部門召回情況========================================================\n");
	for (unsigned int i = 0; i < m_SMS_DepartmentList.size(); i++) {
		if (i != 0)
			fprintf(fptr, "========================================================================================================\n");
		OutputDepartmentList(fptr, m_SMS_DepartmentList[i]);
	}
	fclose(fptr);
	// TODO: 在此加入控制項告知處理常式程式碼
	if (openFile == true) {
		HINSTANCE hRslt = ShellExecute(NULL, _T("open"), _T("notepad.EXE"), _T(FileName), NULL, SW_SHOWNORMAL);
		assert(hRslt > (HINSTANCE)HINSTANCE_ERROR);
	}
}

// 應修改為函數指標型來取得比對參數
void  CERNSDlg::OutputGroupList(FILE *fptr, CString Group) {
	// Type 預定召回 Number[0] 人，已有 Number[1] 人確定動態，其中 Number[2] 人已抵達院內，另有 Number[3] 人預定在 10 分鐘內返院 Number[4] 人在 20 分鐘內返院， Number[5] 人在 30 分鐘內返院 Number[6] 人在 60 分鐘內返院	Number[7] 確定無法返院
	// 取出訊息碼
	CString MessageCode = m_SMS_MessageCodeList[m_NowMessageID];
	int Number[9] = { 0 };
	CString Text;
	for (unsigned int i = 0; i < m_SMS_SendList.size(); i++) {
		if (m_SMS_SendList[i].m_Group == Group) {
			Number[0]++;
			if (m_SMS_SendList[i].m_RecvMessageBody != "") {
				Number[1]++;
			}else {
				Number[8]++;
			}
			Text.Format("%s0", MessageCode);
			if (m_SMS_SendList[i].m_RecvMessageBody == Text) {
				Number[2]++;
				continue;
			}
			Text.Format("%s1", MessageCode);
			if (m_SMS_SendList[i].m_RecvMessageBody == Text) {
				Number[3]++;
				continue;
			}
			Text.Format("%s2", MessageCode);
			if (m_SMS_SendList[i].m_RecvMessageBody == Text) {
				Number[4]++;
				continue;
			}
			Text.Format("%s3", MessageCode);
			if (m_SMS_SendList[i].m_RecvMessageBody == Text) {
				Number[5]++;
				continue;
			}
			Text.Format("%s6", MessageCode);
			if (m_SMS_SendList[i].m_RecvMessageBody == Text) {
				Number[6]++;
				continue;
			}
			Text.Format("%s9", MessageCode);
			if (m_SMS_SendList[i].m_RecvMessageBody == Text) {
				Number[7]++;
				continue;
			}

		}
	}
	// 如果沒有該項目則返回
	if (Number[0] == 0)
		return;
	//
	fprintf(fptr, "\n%s 預定召回 %d 人，已有 %d 人確定動態，其中 %d 人已抵達院內", Group.GetBuffer(), Number[0], Number[1], Number[2]);
	Group.ReleaseBuffer();

	if (Number[3] != 0 || Number[4] != 0 || Number[5] != 0 || Number[6] != 0 || Number[7] != 0) {
		fprintf(fptr, "，另有");
		if (Number[3] != 0) {
			fprintf(fptr, " %d 人在 10 分鐘內返院",Number[3]);
		}
		if (Number[4] != 0) {
			if(Number[3] != 0)
				fprintf(fptr, "，");
			fprintf(fptr, " %d 人在 20 分鐘返院", Number[4]);
		}
		if (Number[5] != 0) {
			if (Number[3] != 0 || Number[4] != 0)
				fprintf(fptr, "，");
			fprintf(fptr, " %d 人在 30 分鐘返院", Number[5]);
		}
		if (Number[6] != 0) {
			if (Number[3] != 0 || Number[4] != 0 || Number[5] != 0)
				fprintf(fptr, "，");
			fprintf(fptr, " %d 人在 60 分鐘返院", Number[6]);
		}
		if (Number[7] != 0) {
			if (Number[3] != 0 || Number[4] != 0 || Number[5] != 0 || Number[6] != 0)
				fprintf(fptr, "，");
			fprintf(fptr, " %d 人確定無法返院", Number[7]);
		}
		fprintf(fptr, "。");
	} else {
		fprintf(fptr, "。");
	}

	fprintf(fptr, "\n\n人員狀態如下：");
	if (Number[2] != 0) {
		fprintf(fptr, "\n\n已抵達院內：\n");
		Text.Format("%s0", MessageCode);
		for (unsigned int i = 0, j = 1; i < m_SMS_SendList.size(); i++) {
			if (m_SMS_SendList[i].m_Group == Group && m_SMS_SendList[i].m_RecvMessageBody == Text) {
				fprintf(fptr, "%s %s %s	", m_SMS_SendList[i].m_Name.GetBuffer(), m_SMS_SendList[i].m_JobTitle.GetBuffer(), m_SMS_SendList[i].m_Phone.GetBuffer());
				m_SMS_SendList[i].m_Name.ReleaseBuffer();
				m_SMS_SendList[i].m_JobTitle.ReleaseBuffer();
				m_SMS_SendList[i].m_Phone.ReleaseBuffer();
				if ((j++ % 5) == 0) {
					fprintf(fptr, "\n");
				}
			}
		}
	}
	if (Number[3] != 0) {
		fprintf(fptr, "\n\n10 分鐘內返院：\n");
		Text.Format("%s1", MessageCode);
		for (unsigned int i = 0, j = 1; i < m_SMS_SendList.size(); i++) {
			if (m_SMS_SendList[i].m_Group == Group && m_SMS_SendList[i].m_RecvMessageBody == Text) {
				fprintf(fptr, "%s %s %s	", m_SMS_SendList[i].m_Name.GetBuffer(), m_SMS_SendList[i].m_JobTitle.GetBuffer(), m_SMS_SendList[i].m_Phone.GetBuffer());
				m_SMS_SendList[i].m_Name.ReleaseBuffer();
				m_SMS_SendList[i].m_JobTitle.ReleaseBuffer();
				m_SMS_SendList[i].m_Phone.ReleaseBuffer();
				if ((j++ % 5) == 0) {
					fprintf(fptr, "\n");
				}
			}
		}
	}
	if (Number[4] != 0) {
		fprintf(fptr, "\n\n20 分鐘內返院\n");
		Text.Format("%s2", MessageCode);
		for (unsigned int i = 0, j = 1; i < m_SMS_SendList.size(); i++) {
			if (m_SMS_SendList[i].m_Group == Group && m_SMS_SendList[i].m_RecvMessageBody == Text) {
				fprintf(fptr, "%s %s %s	", m_SMS_SendList[i].m_Name.GetBuffer(), m_SMS_SendList[i].m_JobTitle.GetBuffer(), m_SMS_SendList[i].m_Phone.GetBuffer());
				m_SMS_SendList[i].m_Name.ReleaseBuffer();
				m_SMS_SendList[i].m_JobTitle.ReleaseBuffer();
				m_SMS_SendList[i].m_Phone.ReleaseBuffer();
				if ((j++ % 5) == 0) {
					fprintf(fptr, "\n");
				}
			}
		}
	}
	if (Number[5] != 0) {
		fprintf(fptr, "\n\n30 分鐘內返院\n");
		Text.Format("%s3", MessageCode);
		for (unsigned int i = 0, j = 1; i < m_SMS_SendList.size(); i++) {
			if (m_SMS_SendList[i].m_Group == Group && m_SMS_SendList[i].m_RecvMessageBody == Text) {
				fprintf(fptr, "%s %s %s	", m_SMS_SendList[i].m_Name.GetBuffer(), m_SMS_SendList[i].m_JobTitle.GetBuffer(), m_SMS_SendList[i].m_Phone.GetBuffer());
				m_SMS_SendList[i].m_Name.ReleaseBuffer();
				m_SMS_SendList[i].m_JobTitle.ReleaseBuffer();
				m_SMS_SendList[i].m_Phone.ReleaseBuffer();
				if ((j++ % 5) == 0) {
					fprintf(fptr, "\n");
				}
			}
		}
	}
	if (Number[6] != 0) {
		fprintf(fptr, "\n\n60 分鐘內返院\n");
		Text.Format("%s6", MessageCode);
		for (unsigned int i = 0, j = 1; i < m_SMS_SendList.size(); i++) {
			if (m_SMS_SendList[i].m_Group == Group && m_SMS_SendList[i].m_RecvMessageBody == Text) {
				fprintf(fptr, "%s %s %s	", m_SMS_SendList[i].m_Name.GetBuffer(), m_SMS_SendList[i].m_JobTitle.GetBuffer(), m_SMS_SendList[i].m_Phone.GetBuffer());
				m_SMS_SendList[i].m_Name.ReleaseBuffer();
				m_SMS_SendList[i].m_JobTitle.ReleaseBuffer();
				m_SMS_SendList[i].m_Phone.ReleaseBuffer();
				if ((j++ % 5) == 0) {
					fprintf(fptr, "\n");
				}
			}
		}
	}

	if (Number[8] != 0) {
		fprintf(fptr, "\n\n尚未回覆人員名單：\n");
		for (unsigned int i = 0, j = 1; i < m_SMS_SendList.size(); i++) {
			if (m_SMS_SendList[i].m_Group == Group && m_SMS_SendList[i].m_RecvMessageBody == "") {
				fprintf(fptr, "%s %s %s	", m_SMS_SendList[i].m_Name.GetBuffer(), m_SMS_SendList[i].m_JobTitle.GetBuffer(), m_SMS_SendList[i].m_Phone.GetBuffer());
				m_SMS_SendList[i].m_Name.ReleaseBuffer();
				m_SMS_SendList[i].m_JobTitle.ReleaseBuffer();
				m_SMS_SendList[i].m_Phone.ReleaseBuffer();
				if ((j++ % 5) == 0) {
					fprintf(fptr, "\n");
				}
			}
		}
	}
	if (Number[7] != 0) {
		fprintf(fptr, "\n\n無法返院人員名單：\n");
		Text.Format("%s9", MessageCode);
		for (unsigned int i = 0, j = 1; i < m_SMS_SendList.size(); i++) {
			if (m_SMS_SendList[i].m_Group == Group && m_SMS_SendList[i].m_RecvMessageBody == Text) {
				fprintf(fptr, "%s %s %s	", m_SMS_SendList[i].m_Name.GetBuffer(), m_SMS_SendList[i].m_JobTitle.GetBuffer(), m_SMS_SendList[i].m_Phone.GetBuffer());
				m_SMS_SendList[i].m_Name.ReleaseBuffer();
				m_SMS_SendList[i].m_JobTitle.ReleaseBuffer();
				m_SMS_SendList[i].m_Phone.ReleaseBuffer();
				if ((j++ % 5) == 0) {
					fprintf(fptr, "\n");
				}
			}
		}
	}
	fprintf(fptr, "\n");
	fprintf(fptr, "\n");	
}

void CERNSDlg::OutputDepartmentList(FILE * fptr, CString Department) {
	// Type 預定召回 Number[0] 人，已有 Number[1] 人確定動態，其中 Number[2] 人已抵達院內，另有 Number[3] 人預定在 10 分鐘內返院 Number[4] 人在 20 分鐘內返院， Number[5] 人在 30 分鐘內返院 Number[6] 人在 60 分鐘內返院	Number[7] 確定無法返院
	// 取出訊息碼
	CString MessageCode = m_SMS_MessageCodeList[m_NowMessageID];
	int Number[9] = { 0 };
	CString Text;
	for (unsigned int i = 0; i < m_SMS_SendList.size(); i++) {
		if (m_SMS_SendList[i].m_Department == Department) {
			Number[0]++;
			if (m_SMS_SendList[i].m_RecvMessageBody != "") {
				Number[1]++;
			} else {
				Number[8]++;
			}
			Text.Format("%s0", MessageCode);
			if (m_SMS_SendList[i].m_RecvMessageBody == Text) {
				Number[2]++;
				continue;
			}
			Text.Format("%s1", MessageCode);
			if (m_SMS_SendList[i].m_RecvMessageBody == Text) {
				Number[3]++;
				continue;
			}
			Text.Format("%s2", MessageCode);
			if (m_SMS_SendList[i].m_RecvMessageBody == Text) {
				Number[4]++;
				continue;
			}
			Text.Format("%s3", MessageCode);
			if (m_SMS_SendList[i].m_RecvMessageBody == Text) {
				Number[5]++;
				continue;
			}
			Text.Format("%s6", MessageCode);
			if (m_SMS_SendList[i].m_RecvMessageBody == Text) {
				Number[6]++;
				continue;
			}
			Text.Format("%s9", MessageCode);
			if (m_SMS_SendList[i].m_RecvMessageBody == Text) {
				Number[7]++;
				continue;
			}

		}
	}
	// 如果沒有該項目則返回
	if (Number[0] == 0)
		return;
	//
	fprintf(fptr, "\n%s 預定召回 %d 人，已有 %d 人確定動態，其中 %d 人已抵達院內", Department.GetBuffer(), Number[0], Number[1], Number[2]);
	Department.ReleaseBuffer();

	if (Number[3] != 0 || Number[4] != 0 || Number[5] != 0 || Number[6] != 0 || Number[7] != 0) {
		fprintf(fptr, "，另有");
		if (Number[3] != 0) {
			fprintf(fptr, " %d 人在 10 分鐘內返院", Number[3]);
		}
		if (Number[4] != 0) {
			if (Number[3] != 0)
				fprintf(fptr, "，");
			fprintf(fptr, " %d 人在 20 分鐘內返院", Number[4]);
		}
		if (Number[5] != 0) {
			if (Number[3] != 0 || Number[4] != 0)
				fprintf(fptr, "，");
			fprintf(fptr, " %d 人在 30 分鐘內返院", Number[5]);
		}
		if (Number[6] != 0) {
			if (Number[3] != 0 || Number[4] != 0 || Number[5] != 0)
				fprintf(fptr, "，");
			fprintf(fptr, " %d 人在 60 分鐘內返院", Number[6]);
		}
		if (Number[7] != 0) {
			if (Number[3] != 0 || Number[4] != 0 || Number[5] != 0 || Number[6] != 0)
				fprintf(fptr, "，");
			fprintf(fptr, " %d 人確定無法返院", Number[7]);
		}
		fprintf(fptr, "。");
	} else {
		fprintf(fptr, "。");
	}

	fprintf(fptr, "\n\n人員狀態如下：");
	if (Number[2] != 0) {
		fprintf(fptr, "\n\n已抵達院內：\n");
		Text.Format("%s0", MessageCode);
		for (unsigned int i = 0, j = 1; i < m_SMS_SendList.size(); i++) {
			if (m_SMS_SendList[i].m_Department == Department && m_SMS_SendList[i].m_RecvMessageBody == Text) {
				fprintf(fptr, "%s %s %s	", m_SMS_SendList[i].m_Name.GetBuffer(), m_SMS_SendList[i].m_JobTitle.GetBuffer(), m_SMS_SendList[i].m_Phone.GetBuffer());
				m_SMS_SendList[i].m_Name.ReleaseBuffer();
				m_SMS_SendList[i].m_JobTitle.ReleaseBuffer();
				m_SMS_SendList[i].m_Phone.ReleaseBuffer();
				if ((j++ % 5) == 0) {
					fprintf(fptr, "\n");
				}
			}
		}
	}
	if (Number[3] != 0) {
		fprintf(fptr, "\n\n10 分鐘內返院：\n");
		Text.Format("%s1", MessageCode);
		for (unsigned int i = 0, j = 1; i < m_SMS_SendList.size(); i++) {
			if (m_SMS_SendList[i].m_Department == Department && m_SMS_SendList[i].m_RecvMessageBody == Text) {
				fprintf(fptr, "%s %s %s	", m_SMS_SendList[i].m_Name.GetBuffer(), m_SMS_SendList[i].m_JobTitle.GetBuffer(), m_SMS_SendList[i].m_Phone.GetBuffer());
				m_SMS_SendList[i].m_Name.ReleaseBuffer();
				m_SMS_SendList[i].m_JobTitle.ReleaseBuffer();
				m_SMS_SendList[i].m_Phone.ReleaseBuffer();
				if ((j++ % 5) == 0) {
					fprintf(fptr, "\n");
				}
			}
		}
	}
	if (Number[4] != 0) {
		fprintf(fptr, "\n\n20 分鐘內返院：\n");
		Text.Format("%s2", MessageCode);
		for (unsigned int i = 0, j = 1; i < m_SMS_SendList.size(); i++) {
			if (m_SMS_SendList[i].m_Department == Department && m_SMS_SendList[i].m_RecvMessageBody == Text) {
				fprintf(fptr, "%s %s %s	", m_SMS_SendList[i].m_Name.GetBuffer(), m_SMS_SendList[i].m_JobTitle.GetBuffer(), m_SMS_SendList[i].m_Phone.GetBuffer());
				m_SMS_SendList[i].m_Name.ReleaseBuffer();
				m_SMS_SendList[i].m_JobTitle.ReleaseBuffer();
				m_SMS_SendList[i].m_Phone.ReleaseBuffer();
				if ((j++ % 5) == 0) {
					fprintf(fptr, "\n");
				}
			}
		}
	}
	if (Number[5] != 0) {
		fprintf(fptr, "\n\n30 分鐘內返院：\n");
		Text.Format("%s3", MessageCode);
		for (unsigned int i = 0, j = 1; i < m_SMS_SendList.size(); i++) {
			if (m_SMS_SendList[i].m_Department == Department && m_SMS_SendList[i].m_RecvMessageBody == Text) {
				fprintf(fptr, "%s %s %s	", m_SMS_SendList[i].m_Name.GetBuffer(), m_SMS_SendList[i].m_JobTitle.GetBuffer(), m_SMS_SendList[i].m_Phone.GetBuffer());
				m_SMS_SendList[i].m_Name.ReleaseBuffer();
				m_SMS_SendList[i].m_JobTitle.ReleaseBuffer();
				m_SMS_SendList[i].m_Phone.ReleaseBuffer();
				if ((j++ % 5) == 0) {
					fprintf(fptr, "\n");
				}
			}
		}
	}
	if (Number[6] != 0) {
		fprintf(fptr, "\n\n60 分鐘內返院：\n");
		Text.Format("%s6", MessageCode);
		for (unsigned int i = 0, j = 1; i < m_SMS_SendList.size(); i++) {
			if (m_SMS_SendList[i].m_Department == Department && m_SMS_SendList[i].m_RecvMessageBody == Text) {
				fprintf(fptr, "%s %s %s	", m_SMS_SendList[i].m_Name.GetBuffer(), m_SMS_SendList[i].m_JobTitle.GetBuffer(), m_SMS_SendList[i].m_Phone.GetBuffer());
				m_SMS_SendList[i].m_Name.ReleaseBuffer();
				m_SMS_SendList[i].m_JobTitle.ReleaseBuffer();
				m_SMS_SendList[i].m_Phone.ReleaseBuffer();
				if ((j++ % 5) == 0) {
					fprintf(fptr, "\n");
				}
			}
		}
	}

	if (Number[8] != 0) {
		fprintf(fptr, "\n\n尚未回覆人員名單：\n");
		for (unsigned int i = 0, j = 1; i < m_SMS_SendList.size(); i++) {
			if (m_SMS_SendList[i].m_Department == Department && m_SMS_SendList[i].m_RecvMessageBody == "") {
				fprintf(fptr, "%s %s %s	", m_SMS_SendList[i].m_Name.GetBuffer(), m_SMS_SendList[i].m_JobTitle.GetBuffer(), m_SMS_SendList[i].m_Phone.GetBuffer());
				m_SMS_SendList[i].m_Name.ReleaseBuffer();
				m_SMS_SendList[i].m_JobTitle.ReleaseBuffer();
				m_SMS_SendList[i].m_Phone.ReleaseBuffer();
				if ((j++ % 5) == 0) {
					fprintf(fptr, "\n");
				}
			}
		}
	}
	if (Number[7] != 0) {
		fprintf(fptr, "\n\n無法返院人員名單：\n");
		Text.Format("%s9", MessageCode);
		for (unsigned int i = 0, j = 1; i < m_SMS_SendList.size(); i++) {
			if (m_SMS_SendList[i].m_Department == Department && m_SMS_SendList[i].m_RecvMessageBody == Text) {
				fprintf(fptr, "%s %s %s	", m_SMS_SendList[i].m_Name.GetBuffer(), m_SMS_SendList[i].m_JobTitle.GetBuffer(), m_SMS_SendList[i].m_Phone.GetBuffer());
				m_SMS_SendList[i].m_Name.ReleaseBuffer();
				m_SMS_SendList[i].m_JobTitle.ReleaseBuffer();
				m_SMS_SendList[i].m_Phone.ReleaseBuffer();
				if ((j++ % 5) == 0) {
					fprintf(fptr, "\n");
				}
			}
		}
	}
	fprintf(fptr, "\n");
	fprintf(fptr, "\n");
}

void CERNSDlg::UseTemplateSetMessage(void) {
	/*
	m_ComboTemplate.AddString("測試用模板(9990)");
	m_ComboTemplate.AddString("嚴重火災模板(9999)");
	m_ComboTemplate.AddString("嚴重地震模板(9998)");
	m_ComboTemplate.AddString("嚴重車禍模板(9997)");
	m_ComboTemplate.AddString("核生化模板(9996)");
	m_ComboTemplate.AddString("院內資安模板(9995)");
	m_ComboTemplate.AddString("院內消防模板(9994)");
	m_ComboTemplate.AddString("院內電力模板(9993)");
	m_ComboTemplate.AddString("院內水力模板(9992)");
	m_ComboTemplate.AddString("院內空調模板(9991)");
	m_ComboTemplate.SetCurSel(0);

	m_SMS_TemplateListA.push_back("新北市立聯合醫院%s發生 [緊急召回系統測試]，請回覆簡訊進行測試:");
	m_SMS_TemplateListA.push_back("新北市立聯合醫院%s發生 [大量火災傷患]，須緊急召回人員，請回傳預定回院時間:");
	m_SMS_TemplateListA.push_back("新北市立聯合醫院%s發生 [大量地震傷患]，須緊急召回人員，請回傳預定回院時間:");
	m_SMS_TemplateListA.push_back("新北市立聯合醫院%s發生 [大量車禍傷患]，須緊急召回人員，請回傳預定回院時間:");
	m_SMS_TemplateListA.push_back("新北市立聯合醫院%s發生 [大量核生化傷患]，須緊急召回人員，請回傳預定回院時間:");
	m_SMS_TemplateListA.push_back("新北市立聯合醫院%s發生 [資訊系統中斷事件]，須緊急召回人員，請回傳預定回院時間:");
	m_SMS_TemplateListA.push_back("新北市立聯合醫院%s發生 [火災事件]，須緊急召回人員，請回傳預定回院時間:");
	m_SMS_TemplateListA.push_back("新北市立聯合醫院%s發生 [電力失效事件]，須緊急召回人員，請回傳預定回院時間:");
	m_SMS_TemplateListA.push_back("新北市立聯合醫院%s發生 [水力失效事件]，須緊急召回人員，請回傳預定回院時間:");
	m_SMS_TemplateListA.push_back("新北市立聯合醫院%s發生 [空調失效事件]，須緊急召回人員，請回傳預定回院時間:");

	m_SMS_TemplateListB.push_back("在院(%s0),10分到院(%s1),20分到院(%s2),30分到院(%s3),60分到院(%s6),無法60分回院(%s9)");
	m_SMS_TemplateListB.push_back("在院(%s0),10分到院(%s1),20分到院(%s2),30分到院(%s3),60分到院(%s6),無法60分回院(%s9)");
	m_SMS_TemplateListB.push_back("在院(%s0),10分到院(%s1),20分到院(%s2),30分到院(%s3),60分到院(%s6),無法60分回院(%s9)");
	m_SMS_TemplateListB.push_back("在院(%s0),10分到院(%s1),20分到院(%s2),30分到院(%s3),60分到院(%s6),無法60分回院(%s9)");
	m_SMS_TemplateListB.push_back("在院(%s0),10分到院(%s1),20分到院(%s2),30分到院(%s3),60分到院(%s6),無法60分回院(%s9)");
	m_SMS_TemplateListB.push_back("在院(%s0),10分到院(%s1),20分到院(%s2),30分到院(%s3),60分到院(%s6),無法60分回院(%s9)");
	m_SMS_TemplateListB.push_back("在院(%s0),10分到院(%s1),20分到院(%s2),30分到院(%s3),60分到院(%s6),無法60分回院(%s9)");
	m_SMS_TemplateListB.push_back("在院(%s0),10分到院(%s1),20分到院(%s2),30分到院(%s3),60分到院(%s6),無法60分回院(%s9)");
	m_SMS_TemplateListB.push_back("在院(%s0),10分到院(%s1),20分到院(%s2),30分到院(%s3),60分到院(%s6),無法60分回院(%s9)");
	m_SMS_TemplateListB.push_back("在院(%s0),10分到院(%s1),20分到院(%s2),30分到院(%s3),60分到院(%s6),無法60分回院(%s9)");

	m_SMS_MessageCodeList.push_back("9990");
	m_SMS_MessageCodeList.push_back("9999");
	m_SMS_MessageCodeList.push_back("9998");
	m_SMS_MessageCodeList.push_back("9997");
	m_SMS_MessageCodeList.push_back("9996");
	m_SMS_MessageCodeList.push_back("9995");
	m_SMS_MessageCodeList.push_back("9994");
	m_SMS_MessageCodeList.push_back("9993");
	m_SMS_MessageCodeList.push_back("9992");
	m_SMS_MessageCodeList.push_back("9991");

	m_ComboDistrict.AddString("全院區");
	m_ComboDistrict.AddString("三重院區");
	m_ComboDistrict.AddString("板橋院區");
	m_ComboDistrict.SetCurSel(0);

	m_SMS_DistrictCodeList.push_back(" 兩院區 ");
	m_SMS_DistrictCodeList.push_back(" 三重院區 ");
	m_SMS_DistrictCodeList.push_back(" 板橋院區 ");
	*/

	int UseTemplateID = m_ComboTemplate.GetCurSel();
	int UseDistrictID = m_ComboDistrict.GetCurSel();
	m_NowMessageID = UseTemplateID;

	CString Message1, Message2;
	Message1.Format(m_SMS_TemplateListA[UseTemplateID], m_SMS_DistrictCodeList[UseDistrictID]);
	Message2.Format(m_SMS_TemplateListB[UseTemplateID], m_SMS_MessageCodeList[UseTemplateID], m_SMS_MessageCodeList[UseTemplateID], m_SMS_MessageCodeList[UseTemplateID], m_SMS_MessageCodeList[UseTemplateID], m_SMS_MessageCodeList[UseTemplateID], m_SMS_MessageCodeList[UseTemplateID]);
	m_EditSmsMessage1.SetWindowTextA(Message1);
	m_EditSmsMessage2.SetWindowTextA(Message2);
}

void CERNSDlg::OnBnClickedButtonUseTemplate()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	UseTemplateSetMessage();
}
