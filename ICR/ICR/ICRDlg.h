
// ICRDlg.h: 標頭檔
//

#pragma once


// CICRDlg 對話方塊
class CICRDlg : public CDialogEx
{
// 建構
public:
	CICRDlg(CWnd* pParent = nullptr);	// 標準建構函式

// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ICR_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支援


// 程式碼實作
protected:
	HICON m_hIcon;

	// 產生的訊息對應函式
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonExportData();
	afx_msg void OnBnClickedButtonOpenFolder();	

	CEdit m_EditExportState;

	CString m_StringReportDateRun;
	CEdit	m_EditReportDateRun;
};
