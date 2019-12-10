
// AF Alarm systemDlg.cpp: 實作檔案
//

#include "stdafx.h"
#include "AF Alarm system.h"
#include "AF Alarm systemDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 根據滿版尺寸設定各元件大小
void CLayout::SetLayout(CRect & Rect) {
	m_DisplayRect = Rect;

	// 記錄寬高
	int Width = m_DisplayRect.Width();
	int Height = m_DisplayRect.Height();

	// 分配邊界
	int BoundaryLeft = (int)(Width * 0.05);
	int BoundaryRight = (int)(Width * 0.05);
	int BoundaryUpper = (int)(Height * 0.05);
	int BoundaryLower = (int)(Height * 0.05);
	int BoundaryItem = (int)(Height * 0.03);

	// 計算元件尺寸
	int HeightItem = (int)(Height - BoundaryUpper - BoundaryLower - BoundaryItem * 3) / 4;
	int WidthItem = Width - BoundaryLeft - BoundaryRight;

	// 計算每個元件的位置
	m_Item[0] = CRect(BoundaryLeft, BoundaryUpper, Width - BoundaryRight, BoundaryUpper + HeightItem);
	m_Item[1] = m_Item[0];
	m_Item[1].OffsetRect(0, BoundaryItem + HeightItem);
	m_Item[2] = m_Item[1];
	m_Item[2].OffsetRect(0, BoundaryItem + HeightItem);
	m_Item[3] = m_Item[2];
	m_Item[3].OffsetRect(0, BoundaryItem + HeightItem);
}

// 讀取監控狀態的資料
void ReadMonitorStateData(CAFAlarmsystemDlg & Dlg) {
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
		sSqlString.Format(L"SELECT * FROM[AFDBS].[dbo].[行政部門.資訊室.0250-4-A0-0103.資訊資產服務監測狀況表]");
		database.ExecuteSQL(sSqlString);

		CRecordset recset(&database);
		recset.Open(CRecordset::forwardOnly, sSqlString, CRecordset::readOnly);		

		// 取出各欄位的資訊 Loop through each record
		Dlg.m_MonitorStateData.clear();
		Dlg.m_MonitorStateColor.clear();

		CString Str;
		CString Code0, Code1, Name, User, State, Time, IP, Message;

		while (!recset.IsEOF()) {
			recset.GetFieldValue(L"資產代碼", Code0);
			recset.GetFieldValue(L"服務代碼", Code1);
			recset.GetFieldValue(L"服務名稱", Name);
			recset.GetFieldValue(L"服務管理者", User);
			recset.GetFieldValue(L"監測判定", State);
			recset.GetFieldValue(L"最後異動時間", Time);
			recset.GetFieldValue(L"最後異動_IP", IP);
			recset.GetFieldValue(L"服務最後回報狀態", Message);
			// 產生顯示字串
			Str.Format(L" %s, %s \n %s \n %s \n %s \n %s \n %s \n %s ", Code0, Code1, Name, User, State, Time.Left(16), IP, Message);
			// 根據狀態設定顏色
			if(State == L"正常")
				Dlg.m_MonitorStateColor.push_back(COLORREF(RGB(129, 216, 208)));
			else
				Dlg.m_MonitorStateColor.push_back(COLORREF(RGB(255, 0, 0)));		
			Dlg.m_MonitorStateData.push_back(Str);
			recset.MoveNext();

		}
		// 關閉資料庫連線
		database.Close();
	}
		CATCH(CDBException, e) {
		database.Close();
		TRACE(e->m_strError);
	}
	END_CATCH;
}

CAFAlarmsystemDlg::CAFAlarmsystemDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_AFALARMSYSTEM_DIALOG, pParent)
{
	m_PageIndex = 0;
	m_FontPointSize = 10;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAFAlarmsystemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAFAlarmsystemDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()	
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CAFAlarmsystemDlg 訊息處理常式

BOOL CAFAlarmsystemDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 設定此對話方塊的圖示。當應用程式的主視窗不是對話方塊時，
	// 框架會自動從事此作業
	SetIcon(m_hIcon, TRUE);			// 設定大圖示
	SetIcon(m_hIcon, FALSE);		// 設定小圖示

	// TODO: 在此加入額外的初始設定
	this->ShowWindow(SW_SHOWMAXIMIZED);

	// 讀取監控狀態資料
	ReadMonitorStateData(*this);

	// 讀取音效檔 並初始化設備
	CString Filename = L"war.wav";
	MCI_OPEN_PARMS OpenParms;
	OpenParms.lpstrElementName = (LPCWSTR)Filename;
	mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_WAIT, (DWORD)(PVOID)&OpenParms);
	m_DeviceID = OpenParms.wDeviceID;
	m_DeviceIsPlaying = false;

	SetTimer(0, 1 * 60 * 1000, NULL);
	SetTimer(1, 1 * 10 * 1000, NULL);
	return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
}

// 如果將最小化按鈕加入您的對話方塊，您需要下列的程式碼，
// 以便繪製圖示。對於使用文件/檢視模式的 MFC 應用程式，
// 框架會自動完成此作業。

void CAFAlarmsystemDlg::OnPaint()
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
		
		CPaintDC dc(this); 
		// 配置字形物件
		CFont font;
		font.CreatePointFont(m_FontPointSize, L"微軟正黑體");
		dc.SelectObject(&font);
		CSize size = dc.GetTextExtent(L"微軟正黑體");
		int H = m_Layout.m_Item[0].Height() / 7;
	
		// 繪製物件	
		bool beep = false;
		for (int i = 0; i < 4; i++) {
			int index = m_PageIndex * 4 + i;
			if (index >= (int)m_MonitorStateData.size()) {
				CRgn Rgn;
				Rgn.CreateRoundRectRgn(m_Layout.m_Item[i].left, m_Layout.m_Item[i].top, m_Layout.m_Item[i].right, m_Layout.m_Item[i].bottom, 20, 20);
				dc.SelectObject(&Rgn);

				CBrush Brush(RGB(65, 105, 225));
				dc.SetBkColor(RGB(65, 105, 225));
				dc.FillRect(m_Layout.m_Item[i], &Brush);				
			}else {
				// 深度
				CRect Rect0(m_Layout.m_Item[i].left + 20, m_Layout.m_Item[i].top + 20, m_Layout.m_Item[i].right + 20, m_Layout.m_Item[i].bottom + 20);
				CRgn Rgn0;
				Rgn0.CreateRoundRectRgn(m_Layout.m_Item[i].left + 20, m_Layout.m_Item[i].top + 20, m_Layout.m_Item[i].right + 20, m_Layout.m_Item[i].bottom + 20, 20, 20);
				dc.SelectObject(&Rgn0);
				CBrush Brush0(RGB(128, 128, 128));
				dc.FillRect(Rect0, &Brush0);

				// 黑框邊
				CRect Rect1(m_Layout.m_Item[i].left - 5, m_Layout.m_Item[i].top - 5, m_Layout.m_Item[i].right + 5, m_Layout.m_Item[i].bottom + 5);
				CRgn Rgn1;
				Rgn1.CreateRoundRectRgn(m_Layout.m_Item[i].left-5, m_Layout.m_Item[i].top-5, m_Layout.m_Item[i].right+5, m_Layout.m_Item[i].bottom+5, 20, 20);
				dc.SelectObject(&Rgn1);
				CBrush Brush1(RGB(0,0,0));
				dc.FillRect(Rect1, &Brush1);

				// 內框邊
				CRgn Rgn2;
				Rgn2.CreateRoundRectRgn(m_Layout.m_Item[i].left, m_Layout.m_Item[i].top, m_Layout.m_Item[i].right, m_Layout.m_Item[i].bottom, 20, 20);
				dc.SelectObject(&Rgn2);
				CBrush Brush2(m_MonitorStateColor[index]);
				dc.SetBkColor(m_MonitorStateColor[index]);
				dc.FillRect(m_Layout.m_Item[i], &Brush2);
				// 繪製文字
				dc.DrawTextW(m_MonitorStateData[index], m_Layout.m_Item[i], DT_LEFT);

				// 檢查告警
				if (m_MonitorStateColor[index] == RGB(255, 0, 0)) {
					beep = true;
				}
			}			
		}
		CDialogEx::OnPaint();
		// 告警音效
		if (beep == true && m_DeviceIsPlaying == false) {
			//Beep(1000, 500); Beep(2000, 500);
			//Beep(1000, 500); Beep(2000, 500);

			MCI_PLAY_PARMS mciPlayParms;			//播放設備
			mciPlayParms.dwCallback = NULL;			//回調為空
			mciPlayParms.dwFrom = 0;				//從零開始播放
			//播放歌曲
			mciSendCommand(m_DeviceID, MCI_PLAY, MCI_FROM | MCI_NOTIFY, (DWORD)(PVOID)&mciPlayParms);
		}		
	}
}

// 當使用者拖曳最小化視窗時，
// 系統呼叫這個功能取得游標顯示。
HCURSOR CAFAlarmsystemDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CAFAlarmsystemDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此加入您的訊息處理常式程式碼
	GetClientRect(&m_DisplayRect);
	m_Layout.SetLayout(m_DisplayRect);

	CDC *pDC = this->GetDC();
	CFont font, *old;
	CSize size;

	// 計算字形的最適尺寸
	int H;
	while (1) {
		font.CreatePointFont(m_FontPointSize+10, L"微軟正黑體");
		old = pDC->SelectObject(&font);

		size = pDC->GetTextExtent(L"微軟正黑體");
		H = m_Layout.m_Item[0].Height() / 7;
		if (size.cy < H) {
			m_FontPointSize += 10;
			pDC->SelectObject(old);
			font.DeleteObject();
		}
		else {
			break;
		}
	}

}


void CAFAlarmsystemDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值
	// 定時更新監控狀態
	if (nIDEvent == 0) {
		ReadMonitorStateData(*this);

	}
	// 定時切換頁面
	if (nIDEvent == 1) {
		m_PageIndex++;
		float PageNumber = m_MonitorStateData.size() / 4.0f ;
		if (m_PageIndex >= PageNumber)
			m_PageIndex = 0;
	}
	// 要求刷新頁面
	Invalidate();
	CDialogEx::OnTimer(nIDEvent);
}
