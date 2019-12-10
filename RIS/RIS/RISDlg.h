
// RISDlg.h: 標頭檔
//

#pragma once


// CRISDlg 對話方塊
class CRISDlg : public CDialogEx
{
// 建構
public:
	CRISDlg(CWnd* pParent = nullptr);	// 標準建構函式

// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RIS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支援


// 程式碼實作
protected:
	HICON m_hIcon;

	// 產生的訊息對應函式
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:

	CString m_DateStart;
	CString m_DateEnd;
	CEdit m_ControlRunState;						
	CEdit m_ControlDateStart;
	CEdit m_ControlDateEnd;

	afx_msg void OnBnClickedExportData();
	afx_msg void OnBnClickedOpenFolder();
};
