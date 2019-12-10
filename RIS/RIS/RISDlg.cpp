
// RISDlg.cpp: 實作檔案
//

#include "stdafx.h"
#include "RIS.h"
#include "RISDlg.h"
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

// 清除資料表
void ClearTable(void) {
	// 設定資料庫連線的相關參數
	CString sDriver = L"SQL Server";
	CString sServer = L"192.168.255.15";
	CString sDsn = L"WebPageUpdater";
	CString sUID = L"sa";
	CString sPWD = L"Pa55w0rd";

	// 產生資料庫的連線字串 DSN or DSN LESS
	CString sConnectStringDsn, sConnectStringDsnLess;
	sConnectStringDsn.Format(L"DSN=%s;UID=%s;PWD=%s", sDsn, sUID, sPWD);
	sConnectStringDsnLess.Format(L"DRIVER={%s};SERVER=%s;UID=%s;PWD=%s;", sDriver, sServer, sUID, sPWD);

	CDatabase database;
	TRY{
		// 連線資料庫 DSN or DSN LESS
		database.OpenEx(sConnectStringDsnLess);
		//-- 更新資料
		CString sSqlString;
		sSqlString.Format(L"DELETE FROM [AFDBS].[dbo].[醫療部門.健檢中心.兵役體檢上傳資料]");
		database.ExecuteSQL(sSqlString);
		// 關閉資料庫連線
		database.Close();
	}CATCH(CDBException, e) {
		database.Close();
		TRACE(e->m_strError);
	}
	END_CATCH;
}

// 產生檢查資料
void GenerateMedicalExaminationData(CString DateStart, CString DateEnd) {
	// 設定資料庫連線的相關參數
	CString sDriver = L"SQL Server";
	CString sServer = L"192.168.255.15";
	CString sDsn = L"WebPageUpdater";
	CString sUID = L"sa";
	CString sPWD = L"Pa55w0rd";

	// 產生資料庫的連線字串 DSN or DSN LESS
	CString sConnectStringDsn, sConnectStringDsnLess;
	sConnectStringDsn.Format(L"DSN=%s;UID=%s;PWD=%s", sDsn, sUID, sPWD);
	sConnectStringDsnLess.Format(L"DRIVER={%s};SERVER=%s;UID=%s;PWD=%s;", sDriver, sServer, sUID, sPWD);

	// 讀取 SQL 指令
	FILE *fptr;
	_wfopen_s(&fptr,L"GMED.SQL", L"r,ccs=UTF-8");
	if (fptr == NULL) {
		AfxMessageBox(L"附屬資料 GMED.SQL 開啟失敗，請檢查軟體的完整性.");
		return;		
	}
	TCHAR Format[10240];
	fgetws(Format, 10240, fptr);
	fclose(fptr);

	// 處理資料庫
	CDatabase database;
	TRY{
		// 連線資料庫 DSN or DSN LESS
		database.OpenEx(sConnectStringDsnLess);
		//-- 更新資料
		CString sSqlString;
		sSqlString.Format(Format, DateStart, DateEnd);
		database.ExecuteSQL(sSqlString);
		// 關閉資料庫連線
		database.Close();
	}CATCH(CDBException, e) {
		database.Close();
		TRACE(e->m_strError);
	}
	END_CATCH;
}

// 輸出資料到 CSV 檔
void WriteFile(FILE * fptr, CString (&ValueU)[14]) {
	char ValueA[14][1024];
	LPWSTR pElementText;
	int    iTextLen;
	// 從 TCHAR 轉換成 ANSI
	for (int i = 0; i < 14; i++) {
		pElementText = ValueU[i].GetBuffer();
		iTextLen = WideCharToMultiByte(CP_ACP, 0, pElementText, -1, NULL, 0, NULL, NULL);
		memset((void*)ValueA[i], 0, sizeof(char) * 1024);
		if (iTextLen > 1024)
			iTextLen = 1024;
		::WideCharToMultiByte(CP_ACP, 0, pElementText, -1, ValueA[i], iTextLen, NULL, NULL);
		ValueU[i].ReleaseBuffer();
	}
	fprintf(fptr, "%010s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n", ValueA[0], ValueA[1], ValueA[2], ValueA[3], ValueA[4], ValueA[5], ValueA[6], ValueA[7], ValueA[8], ValueA[9], ValueA[10], ValueA[11], ValueA[12], ValueA[13]);
}

// 產生報告檔
CString GenerateReport(CString DateStart, CString DateEnd) {
	// 設定資料庫連線的相關參數
	CString sDriver = L"SQL Server";
	CString sServer = L"192.168.255.15";
	CString sDsn = L"WebPageUpdater";
	CString sUID = L"sa";
	CString sPWD = L"Pa55w0rd";

	// 產生資料庫的連線字串 DSN or DSN LESS
	CString sConnectStringDsn, sConnectStringDsnLess;
	sConnectStringDsn.Format(L"DSN=%s;UID=%s;PWD=%s", sDsn, sUID, sPWD);
	sConnectStringDsnLess.Format(L"DRIVER={%s};SERVER=%s;UID=%s;PWD=%s;", sDriver, sServer, sUID, sPWD);

	// 讀取資料庫指令
	FILE *fptr = NULL;
	_wfopen_s(&fptr, L"GR.SQL", L"r,ccs=UTF-8");
	if (fptr == NULL) {
		AfxMessageBox(L"附屬資料 GR.SQL 開啟失敗，請檢查軟體的完整性.");
		return CString(L"附屬資料 GR.SQL 開啟失敗，請檢查軟體的完整性.");
	}
	TCHAR Format[10240];
	fgetws(Format, 10240, fptr);
	fclose(fptr);

	CTime CurrentTime = CTime::GetCurrentTime();

	// 根據日期時間參數產生對應的檔名
	//ris20191111134610811031081109
	//ris201911111346 1081103 1081109
	//ris 西元年的執行時間 民國年的資料開始日期 民國年的資料結束日期
	CString FileName;
	FileName.Format(L"ris%4d%02d%02d%02d%02d%7s%7s.csv", CurrentTime.GetYear(), CurrentTime.GetMonth(), CurrentTime.GetDay(), CurrentTime.GetHour(), CurrentTime.GetMinute(), DateStart, DateEnd);
	_wfopen_s(&fptr, FileName, L"w");

	CDatabase database;
	TRY{
		// 連線資料庫 DSN or DSN LESS
		database.OpenEx(sConnectStringDsnLess);
		//-- 更新資料
		CString sSqlString;
		sSqlString.Format(Format);
		database.ExecuteSQL(sSqlString);

		CRecordset recset(&database);
		recset.Open(CRecordset::forwardOnly, sSqlString, CRecordset::readOnly);

		// 對資料進行分析與轉換
		CString Value[14];
		CString RunState = L"正常執行";
		while (!recset.IsEOF()) {
			// 讀取對應的欄位
			recset.GetFieldValue(L"醫事機構代碼", Value[0]);
			recset.GetFieldValue(L"役男身分證號", Value[1]);
			recset.GetFieldValue(L"徵兵體檢日期", Value[2]);
			recset.GetFieldValue(L"愛滋病毒檢查", Value[3]);
			recset.GetFieldValue(L"梅毒試驗(RPR/VDRL)", Value[4]);
			recset.GetFieldValue(L"尿糖", Value[5]);
			recset.GetFieldValue(L"尿蛋白", Value[6]);
			recset.GetFieldValue(L"白血球計數", Value[7]);
			recset.GetFieldValue(L"紅血球計數", Value[8]);
			recset.GetFieldValue(L"血色素檢查", Value[9]);
			recset.GetFieldValue(L"血球比容值測定Hct", Value[10]);
			recset.GetFieldValue(L"平均紅血球容積(MCV)", Value[11]);
			recset.GetFieldValue(L"血清麩胺酸苯醋酸轉氨基脢(S-GOT/AST)", Value[12]);
			recset.GetFieldValue(L"血清麩胺酸丙酮酸轉氨基脢(S-GPT/ALT)", Value[13]);
			// 對資料進行清洗與正規化
			for (int i = 0; i < 14; i++) {

				if (Value[i] == L"") {
					RunState = L"資料發生缺漏，請調整時間區間";
					continue;
				}
				// 愛滋病毒檢查
				if (i == 3) {
					if (Value[3].Find(L"Non Reactive") == -1) {
						if (Value[3].Find(L"Reactive") != -1) {
							Value[3] = L"2";
							continue;
						}
						Value[3] = L"3";
						continue;
					}
					Value[3] = L"1";
					continue;
				}
				// 梅毒試驗
				if (i == 4) {
					if (Value[4].Find(L"Non Reactive") == -1) {
						if (Value[4].Find(L"Reactive") != -1) {
							Value[4] = L"2";
							continue;
						}
						Value[4] = L"3";
						continue;
					}
					Value[4] = L"1";
					continue;
				}
				// 尿糖
				if (i == 5) {
					if (Value[5] == L"Negative") {
						Value[5] = L"1";
						continue;
					}
					if (Value[5] == L"1+") {
						Value[5] = L"1";
						continue;
					}
					if (Value[5] == L"2+") {
						Value[5] = L"2";
						continue;
					}
					if (Value[5] == L"3+") {
						Value[5] = L"2";
						continue;
					}
					if (Value[5] == L"4+") {
						Value[5] = L"2";
						continue;
					}
					Value[5] = L"3";
					continue;
				}
				// 尿蛋白
				if (i == 6) {
					if (Value[6] == L"Negative") {
						Value[6] = L"1";
						continue;
					}
					if (Value[6] == L"1+") {
						Value[6] = L"1";
						continue;
					}
					if (Value[6] == L"2+") {
						Value[6] = L"1";
						continue;
					}
					if (Value[6] == L"3+") {
						Value[6] = L"2";
						continue;
					}
					if (Value[6] == L"4+") {
						Value[6] = L"2";
						continue;
					}
					if (Value[6] == L"5+") {
						Value[6] = L"2";
						continue;
					}
					Value[6] = L"3";
					continue;
				}
				if (i >= 7 ) {
					if (Value[i].Find(L"<") != -1) {
						Value[i].Replace(L"<", L"");
					}
				}
			}
			// 輸出資料到 CSV 檔
			WriteFile(fptr, Value);
			// 清空資料表
			for (int i = 0; i < 14; i++) {
				Value[i] = L"";
			}
			recset.MoveNext();
		}
		// 關閉資料庫連線
		database.Close();		
		fclose(fptr);
		return RunState;
	}CATCH(CDBException, e) {
		database.Close();
		TRACE(e->m_strError);
		return CString(L"SQL 執行失敗");
	}	
	END_CATCH;	
}

// CRISDlg 對話方塊
CRISDlg::CRISDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RIS_DIALOG, pParent)
	, m_DateStart(_T(""))
	, m_DateEnd(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRISDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DATE_START, m_DateStart);
	DDV_MaxChars(pDX, m_DateStart, 7);
	DDX_Text(pDX, IDC_EDIT_DATE_END, m_DateEnd);
	DDV_MaxChars(pDX, m_DateEnd, 7);
	DDX_Control(pDX, IDC_EDIT_STATE, m_ControlRunState);
	DDX_Control(pDX, IDC_EDIT_DATE_START, m_ControlDateStart);
	DDX_Control(pDX, IDC_EDIT_DATE_END, m_ControlDateEnd);
}

BEGIN_MESSAGE_MAP(CRISDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	
	ON_BN_CLICKED(IDC_EXPORT_DATA, &CRISDlg::OnBnClickedExportData)
	ON_BN_CLICKED(IDC_OPEN_FOLDER, &CRISDlg::OnBnClickedOpenFolder)
END_MESSAGE_MAP()


// CRISDlg 訊息處理常式

BOOL CRISDlg::OnInitDialog()
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
	CTime CurrentTime = CTime::GetCurrentTime();
	int week = CurrentTime.GetDayOfWeek();
	CTime LastSunday = CurrentTime - CTimeSpan(7 + week - 1, 0, 0, 0);
	CTime LastStaturday = CurrentTime - CTimeSpan(1 + week - 1, 0, 0, 0);

	// 預設以上一週為時間區間進行初始化
	m_DateStart.Format(L"%03d%02d%02d", LastSunday.GetYear() - 1911, LastSunday.GetMonth(), LastSunday.GetDay());
	m_DateEnd.Format(L"%03d%02d%02d", LastStaturday.GetYear() - 1911, LastStaturday.GetMonth(), LastStaturday.GetDay());

	m_ControlDateStart.SetWindowTextW(m_DateStart);
	m_ControlDateEnd.SetWindowTextW(m_DateEnd);
	m_ControlDateStart.UpdateData(TRUE);
	m_ControlDateEnd.UpdateData(TRUE);
	return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
}

void CRISDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CRISDlg::OnPaint()
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
HCURSOR CRISDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CRISDlg::OnBnClickedExportData()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	// 執行資料轉換作業
	// 取得時間區間
	m_ControlDateStart.GetWindowTextW(m_DateStart);
	m_ControlDateEnd.GetWindowTextW(m_DateEnd);
	// 清除資料表後產生新的檢查報告
	ClearTable();
	GenerateMedicalExaminationData(m_DateStart, m_DateEnd);	
	CString State = GenerateReport(m_DateStart, m_DateEnd);
	// 更新執行的狀態
	m_ControlRunState.SetWindowTextW(State);
	m_ControlRunState.UpdateData(TRUE);
}

void CRISDlg::OnBnClickedOpenFolder()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	// 開啟資料所在的資料夾
	TCHAR szDir[256];
	GetCurrentDirectory(256, szDir);
	ShellExecute(NULL, L"open", NULL, NULL, szDir, SW_SHOWNORMAL);
}
