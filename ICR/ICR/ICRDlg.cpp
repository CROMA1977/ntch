
// ICRDlg.cpp: 實作檔案
//

#include "stdafx.h"
#include "ICR.h"
#include "ICRDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CString GenerateMedicalExaminationDataA(const CString & StringReportDateRun) {
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
	_wfopen_s(&fptr, L"GMED-A.SQL", L"r,ccs=UTF-8");
	if (fptr == NULL) {
		AfxMessageBox(L"附屬資料 GMED-A.SQL 開啟失敗，請檢查軟體的完整性.");
		return CString(L"附屬資料 GMED-A.SQL 開啟失敗，請檢查軟體的完整性.");
	}

	TCHAR Format[10240];
	fgetws(Format, 10240, fptr);
	fclose(fptr);

	CTime CurrentTime = CTime::GetCurrentTime();

	// 根據日期時間參數產生對應的檔名
	//ICR-A-20191111134610811
	//ICR-A-201911111346 10811
	//ICR-A- 西元年的執行時間 民國年月
	CString FileName;
	FileName.Format(L"ICR-A-%4d%02d%02d%02d%02d%5s.csv", CurrentTime.GetYear(), CurrentTime.GetMonth(), CurrentTime.GetDay(), CurrentTime.GetHour(), CurrentTime.GetMinute(), StringReportDateRun);
	_wfopen_s(&fptr, FileName, L"w");

	CDatabase database;
	TRY{
		// 連線資料庫 DSN or DSN LESS
		database.OpenEx(sConnectStringDsnLess);

		//-- 更新資料
		CString sSqlString;
		sSqlString.Format(Format, StringReportDateRun, StringReportDateRun, StringReportDateRun, StringReportDateRun, StringReportDateRun);
		database.ExecuteSQL(sSqlString);

		CRecordset recset(&database);
		recset.Open(CRecordset::forwardOnly, sSqlString, CRecordset::readOnly);

		// 對資料進行分析與轉換
		CString ValueU[5];
		CString RunState = L"正常執行";

		char ValueA[5][1024];
		LPWSTR pElementText;
		int    iTextLen;

		while (!recset.IsEOF()) {
			// 讀取對應的欄位
			recset.GetFieldValue(L"T3_費用年月", ValueU[0]);
			recset.GetFieldValue(L"D2_流水編號", ValueU[1]);
			recset.GetFieldValue(L"D3_身份證統一編號", ValueU[2]);
			recset.GetFieldValue(L"D30_診治醫事人員代號", ValueU[3]);
			recset.GetFieldValue(L"姓名", ValueU[4]);
		
			// 輸出資料到 CSV 檔
			// 從 TCHAR 轉換成 ANSI
			for (int i = 0; i < 5; i++) {
				pElementText = ValueU[i].GetBuffer();
				iTextLen = WideCharToMultiByte(CP_ACP, 0, pElementText, -1, NULL, 0, NULL, NULL);
				memset((void*)ValueA[i], 0, sizeof(char) * 1024);
				if (iTextLen > 1024)
					iTextLen = 1024;
				::WideCharToMultiByte(CP_ACP, 0, pElementText, -1, ValueA[i], iTextLen, NULL, NULL);
				ValueU[i].ReleaseBuffer();
			}
			fprintf(fptr, "%s,%s,%s,%s,%s\n", ValueA[0], ValueA[1], ValueA[2], ValueA[3], ValueA[4]);
			// 清空資料表
			for (int i = 0; i < 5; i++) {
				ValueU[i] = L"";
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
CString GenerateMedicalExaminationDataB(const CString & StringReportDateRun) {
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
	_wfopen_s(&fptr, L"GMED-B.SQL", L"r,ccs=UTF-8");
	if (fptr == NULL) {
		AfxMessageBox(L"附屬資料 GMED-B.SQL 開啟失敗，請檢查軟體的完整性.");
		return CString(L"附屬資料 GMED-B.SQL 開啟失敗，請檢查軟體的完整性.");
	}

	TCHAR Format[10240];
	fgetws(Format, 10240, fptr);
	fclose(fptr);

	CTime CurrentTime = CTime::GetCurrentTime();

	// 根據日期時間參數產生對應的檔名
	//ICR-B-20191111134610811
	//ICR-B-201911111346 10811
	//ICR-B- 西元年的執行時間 民國年月
	CString FileName;
	FileName.Format(L"ICR-B-%4d%02d%02d%02d%02d%5s.csv", CurrentTime.GetYear(), CurrentTime.GetMonth(), CurrentTime.GetDay(), CurrentTime.GetHour(), CurrentTime.GetMinute(), StringReportDateRun);
	_wfopen_s(&fptr, FileName, L"w");

	CDatabase database;
	TRY{
		// 連線資料庫 DSN or DSN LESS
		database.OpenEx(sConnectStringDsnLess);

		//-- 更新資料
		CString sSqlString;
		sSqlString.Format(Format, StringReportDateRun, StringReportDateRun, StringReportDateRun);
		database.ExecuteSQL(sSqlString);

		CRecordset recset(&database);
		recset.Open(CRecordset::forwardOnly, sSqlString, CRecordset::readOnly);

		// 對資料進行分析與轉換
		CString ValueU[6];
		CString RunState = L"正常執行";

		char ValueA[6][1024];
		LPWSTR pElementText;
		int    iTextLen;

		while (!recset.IsEOF()) {
			// 讀取對應的欄位
			recset.GetFieldValue(L"T3_費用年月", ValueU[0]);
			recset.GetFieldValue(L"D2_流水編號", ValueU[1]);
			recset.GetFieldValue(L"D3_身份證統一編號", ValueU[2]);
			recset.GetFieldValue(L"D30_診治醫事人員代號", ValueU[3]);
			recset.GetFieldValue(L"姓名", ValueU[4]);
			recset.GetFieldValue(L"P4_藥品項目代號", ValueU[5]);

			// 輸出資料到 CSV 檔
			// 從 TCHAR 轉換成 ANSI
			for (int i = 0; i < 6; i++) {
				pElementText = ValueU[i].GetBuffer();
				iTextLen = WideCharToMultiByte(CP_ACP, 0, pElementText, -1, NULL, 0, NULL, NULL);
				memset((void*)ValueA[i], 0, sizeof(char) * 1024);
				if (iTextLen > 1024)
					iTextLen = 1024;
				::WideCharToMultiByte(CP_ACP, 0, pElementText, -1, ValueA[i], iTextLen, NULL, NULL);
				ValueU[i].ReleaseBuffer();
			}
			fprintf(fptr, "%s,%s,%s,%s,%s,%s\n", ValueA[0], ValueA[1], ValueA[2], ValueA[3], ValueA[4], ValueA[5]);
			// 清空資料表
			for (int i = 0; i < 6; i++) {
				ValueU[i] = L"";
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

CString GenerateMedicalExaminationDataC(const CString & StringReportDateRun) {
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
	_wfopen_s(&fptr, L"GMED-C.SQL", L"r,ccs=UTF-8");
	if (fptr == NULL) {
		AfxMessageBox(L"附屬資料 GMED-C.SQL 開啟失敗，請檢查軟體的完整性.");
		return CString(L"附屬資料 GMED-C.SQL 開啟失敗，請檢查軟體的完整性.");
	}

	TCHAR Format[10240];
	fgetws(Format, 10240, fptr);
	fclose(fptr);

	CTime CurrentTime = CTime::GetCurrentTime();

	// 根據日期時間參數產生對應的檔名
	//ICR-C-20191111134610811
	//ICR-C-201911111346 10811
	//ICR-C- 西元年的執行時間 民國年月
	CString FileName;
	FileName.Format(L"ICR-C-%4d%02d%02d%02d%02d%5s.csv", CurrentTime.GetYear(), CurrentTime.GetMonth(), CurrentTime.GetDay(), CurrentTime.GetHour(), CurrentTime.GetMinute(), StringReportDateRun);
	_wfopen_s(&fptr, FileName, L"w");

	CDatabase database;
	TRY{
		// 連線資料庫 DSN or DSN LESS
		database.OpenEx(sConnectStringDsnLess);

		//-- 更新資料
		CString sSqlString;
		sSqlString.Format(Format, StringReportDateRun);

		//_wfopen_s(&fptr, L"SQL.txt", L"w,ccs=UTF-8");
		//fputws(sSqlString.GetBuffer(), fptr);
		//fclose(fptr);

		database.ExecuteSQL(sSqlString);

		CRecordset recset(&database);
		recset.Open(CRecordset::forwardOnly, sSqlString, CRecordset::readOnly);

		// 對資料進行分析與轉換
		CString ValueU[19];
		CString RunState = L"正常執行";

		char ValueA[19][1024];
		LPWSTR pElementText;
		int    iTextLen;

		while (!recset.IsEOF()) {
			// 讀取對應的欄位
			recset.GetFieldValue(L"T3_費用年月", ValueU[0]);
			recset.GetFieldValue(L"D8_就醫科別", ValueU[1]);
			recset.GetFieldValue(L"D9_就醫日期", ValueU[2]);
			recset.GetFieldValue(L"D3_身份證統一編號", ValueU[3]);
			recset.GetFieldValue(L"病歷號碼", ValueU[4]);
			recset.GetFieldValue(L"病人姓名", ValueU[5]);
			recset.GetFieldValue(L"D11_出生年月日", ValueU[6]);
			recset.GetFieldValue(L"疾病碼一", ValueU[7]);
			recset.GetFieldValue(L"疾病名一", ValueU[8]);
			recset.GetFieldValue(L"疾病碼二", ValueU[9]);
			recset.GetFieldValue(L"疾病名二", ValueU[10]);
			recset.GetFieldValue(L"疾病碼三", ValueU[11]);
			recset.GetFieldValue(L"疾病名三", ValueU[12]);
			recset.GetFieldValue(L"疾病碼四", ValueU[13]);
			recset.GetFieldValue(L"疾病名四", ValueU[14]);
			recset.GetFieldValue(L"疾病碼五", ValueU[15]);
			recset.GetFieldValue(L"疾病名五", ValueU[16]);
			recset.GetFieldValue(L"D30_診治醫事人員代號", ValueU[17]);
			recset.GetFieldValue(L"姓名", ValueU[18]);



			// 輸出資料到 CSV 檔
			// 從 TCHAR 轉換成 ANSI
			for (int i = 0; i < 19; i++) {
				pElementText = ValueU[i].GetBuffer();
				iTextLen = WideCharToMultiByte(CP_ACP, 0, pElementText, -1, NULL, 0, NULL, NULL);
				memset((void*)ValueA[i], 0, sizeof(char) * 1024);
				if (iTextLen > 1024)
					iTextLen = 1024;
				::WideCharToMultiByte(CP_ACP, 0, pElementText, -1, ValueA[i], iTextLen, NULL, NULL);
				ValueU[i].ReleaseBuffer();
			}
			fprintf(fptr, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n", ValueA[0], ValueA[1], ValueA[2], ValueA[3], ValueA[4], ValueA[5], ValueA[6], ValueA[7], ValueA[8], ValueA[9], ValueA[10], ValueA[11], ValueA[12], ValueA[13], ValueA[14], ValueA[15], ValueA[16], ValueA[17], ValueA[18]);
			// 清空資料表
			for (int i = 0; i < 19; i++) {
				ValueU[i] = L"";
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

CString ExportData(const CString & StringReportDateRun) {
	CString State, StateA, StateB, StateC;
	StateA = GenerateMedicalExaminationDataA(StringReportDateRun);
	StateB = GenerateMedicalExaminationDataB(StringReportDateRun);
	StateC = GenerateMedicalExaminationDataC(StringReportDateRun);
	State.Format(L"流程 A = %s 流程 B = %s 流程 C = %s", StateA, StateB, StateC);
	return State;
}


// CICRDlg 對話方塊



CICRDlg::CICRDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ICR_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CICRDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DATE_RUN, m_StringReportDateRun);
	DDV_MaxChars(pDX, m_StringReportDateRun, 5);

	DDX_Control(pDX, IDC_EDIT_STATE, m_EditExportState);
	DDX_Control(pDX, IDC_EDIT_DATE_RUN, m_EditReportDateRun);
	
}

BEGIN_MESSAGE_MAP(CICRDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_EXPORT_DATA, &CICRDlg::OnBnClickedButtonExportData)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_FOLDER, &CICRDlg::OnBnClickedButtonOpenFolder)
END_MESSAGE_MAP()


// CICRDlg 訊息處理常式

BOOL CICRDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 設定此對話方塊的圖示。當應用程式的主視窗不是對話方塊時，
	// 框架會自動從事此作業
	SetIcon(m_hIcon, TRUE);			// 設定大圖示
	SetIcon(m_hIcon, FALSE);		// 設定小圖示

	// TODO: 在此加入額外的初始設定
	CTime CurrentTime = CTime::GetCurrentTime();
	int Day = CurrentTime.GetDay() + 1;
	CTime RunTimeYearMon = CurrentTime - CTimeSpan(Day, 0, 0, 0);

	// 預設以上一週為時間區間進行初始化
	m_StringReportDateRun.Format(L"%03d%02d", RunTimeYearMon.GetYear() - 1911, RunTimeYearMon.GetMonth());
	m_EditReportDateRun.SetWindowTextW(m_StringReportDateRun);
	m_EditReportDateRun.UpdateData(TRUE);

	return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
}

// 如果將最小化按鈕加入您的對話方塊，您需要下列的程式碼，
// 以便繪製圖示。對於使用文件/檢視模式的 MFC 應用程式，
// 框架會自動完成此作業。

void CICRDlg::OnPaint()
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
HCURSOR CICRDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CICRDlg::OnBnClickedButtonExportData()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	m_EditReportDateRun.GetWindowTextW(m_StringReportDateRun);
	CString State = ExportData(m_StringReportDateRun);

	// 更新執行的狀態
	m_EditExportState.SetWindowTextW(State);
	m_EditExportState.UpdateData(TRUE);

}


void CICRDlg::OnBnClickedButtonOpenFolder()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	TCHAR szDir[256];
	GetCurrentDirectory(256, szDir);
	ShellExecute(NULL, L"open", NULL, NULL, szDir, SW_SHOWNORMAL);
}
