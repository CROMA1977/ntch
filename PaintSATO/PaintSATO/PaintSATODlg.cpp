
// PaintSATODlg.cpp: 實作檔案
//

#include "stdafx.h"
#include "PaintSATO.h"
#include "PaintSATODlg.h"
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


// CPaintSATODlg 對話方塊



CPaintSATODlg::CPaintSATODlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PAINTSATO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPaintSATODlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPaintSATODlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_PRINT_SATO, &CPaintSATODlg::OnBnClickedButtonPrintSato)
END_MESSAGE_MAP()


// CPaintSATODlg 訊息處理常式

BOOL CPaintSATODlg::OnInitDialog()
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

	return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
}

void CPaintSATODlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPaintSATODlg::OnPaint()
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
HCURSOR CPaintSATODlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CPaintSATODlg::OnBnClickedButtonPrintSato()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	// 宣告 Print Dialog
	CPrintDialog PrintDialog(FALSE);
	// 取得預設的印表機
	PrintDialog.GetDefaults();
	// 宣告列印用的 DC
	CDC PrintDC;
	// PrintDlg 的 DC 進行連接 失敗的話 顯示訊息
	if (!PrintDC.Attach(PrintDialog.GetPrinterDC())) {
		AfxMessageBox(L"Invalid Printer.");
		return;
	}
	// 取出列印裝置的解析度
	PrintDC.m_bPrinting = TRUE;
	int PrintWidth  = PrintDC.GetDeviceCaps(HORZRES);
	int PrintHeight = PrintDC.GetDeviceCaps(VERTRES);

	// 設定列印的資訊
	CPrintInfo Info;
	Info.SetMaxPage(1);
	Info.m_rectDraw.SetRect(0, 0, PrintWidth, PrintHeight);

	// 文件格式設定列印用的格式
	DOCINFO di;
	::ZeroMemory(&di, sizeof(DOCINFO));
	di.cbSize		= sizeof(DOCINFO);
	di.lpszDocName	= L"SATO WS408";
	
	BOOL bPrintingOK = PrintDC.StartDocW(&di);

	// 宣告 DC 與記憶的 DC
	CDC PrintBufferDC;
	// 記憶用的 DC 根據 PrintDC 建立相容性的 DC 用在 Preview 
	PrintBufferDC.CreateCompatibleDC(&PrintDC);

	// 宣告顯示用的 Bitmap
	CBitmap PrintBufferBMP;
	// 建立一個相容  Print DC 格式的 Bmp 大小與 PrintDC 的大小一致
	PrintBufferBMP.CreateCompatibleBitmap(&PrintDC, Info.m_rectDraw.Width(), Info.m_rectDraw.Height());
	// 選擇物件並記錄舊物件
	CBitmap * pOldBMP = PrintBufferDC.SelectObject(&PrintBufferBMP);

	// 選擇背景模式
	PrintBufferDC.SetBkMode(TRANSPARENT);
	// 設定文字顏色
	PrintBufferDC.SetTextColor(RGB(0, 0, 0));
	// 填滿背景
	PrintBufferDC.PatBlt(0, 0, Info.m_rectDraw.Width(), Info.m_rectDraw.Height(), WHITENESS);

	// 宣告字形與恢復用的指標
	CFont Font;
	// 設定點的尺寸
	int FontPointSize = 100;
	// 設定筆觸的字形大小
	Font.CreatePointFont(FontPointSize, L"微軟正黑體", &PrintBufferDC);
	// 測試字串
	CString strWords;
	strWords.Format(L"R122707165");
	// 設定使用的筆物件
	CFont * pOldFnt = PrintBufferDC.SelectObject(&Font);
	// 取得文字區域的尺寸
	CSize zMetrix = PrintBufferDC.GetTextExtent(strWords);
	// Write string or Paint something
	int iPos = 0;
	int iMaksimal = 0;
	int iLineHeight = 1;
	int iLoop;
	// Calculate how many lines we could fit
	// 計算可以容納的行數
	for (iLoop = 1; iLoop < 100; iLoop++) {
		if (((zMetrix.cy + iLineHeight)*iLoop) < PrintHeight)
			iMaksimal++;
	}
	// 顯示訊息
	CString strPuncak;
	//strPuncak.Format(L"Maximum Amount of line(s) for %d	points are %d lines", FontPointSize, iMaksimal);
	strPuncak.Format(L"Maximum Amount of line(s) for %d	points are %d lines", FontPointSize, iMaksimal);
	iMaksimal = 3;
	// 輸出顯示的資訊
	for (iLoop = 0; iLoop < iMaksimal; iLoop++) {
		strWords.Format(L"This is line %d", iLoop);
		PrintBufferDC.TextOut(0, iLoop*(zMetrix.cy + iLineHeight), strWords);
	}

	// Reseting font
	// 還原字形
	PrintBufferDC.SelectObject(pOldFnt);

	// 逐頁輸出
	for (UINT Page = Info.GetMinPage(); Page <= Info.GetMaxPage() && bPrintingOK; Page++) {
		PrintDC.StartPage();
		Info.m_nCurPage = Page;

		PrintBufferDC.SetMapMode(PrintDC.GetMapMode());
		PrintDC.SetStretchBltMode(HALFTONE);
		// now stretchblt to maximum width on page
		PrintDC.StretchBlt(0, 0, Info.m_rectDraw.Width(), Info.m_rectDraw.Height(), &PrintBufferDC, 0, 0, Info.m_rectDraw.Width(), Info.m_rectDraw.Height(), SRCCOPY);
		// clean up
		PrintBufferDC.SelectObject(pOldBMP);
		bPrintingOK = (PrintDC.EndPage() > 0); // end page
	}

	if (bPrintingOK)
		PrintDC.EndDoc();	// end a print job
	else
		PrintDC.AbortDoc(); // abort job.
}
