
// AF Alarm systemDlg.h: 標頭檔

// 以全螢幕為準 留白率 90 %
// 

#pragma once

// 版面分配控制元件
class CLayout {
public:
								CLayout() {};
	virtual						~CLayout() {};

			CRect				m_DisplayRect;
			CRect				m_Item[4];

			void				SetLayout(CRect & Rect);
};

// CAFAlarmsystemDlg 對話方塊
class CAFAlarmsystemDlg : public CDialogEx
{
// 建構
public:
	CAFAlarmsystemDlg(CWnd* pParent = nullptr);	// 標準建構函式

// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AFALARMSYSTEM_DIALOG };
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
	bool		m_DeviceIsPlaying;
	int			m_PageIndex;
	int			m_FontPointSize;
	MCIDEVICEID m_DeviceID;
	CRect		m_DisplayRect;
	CLayout		m_Layout;

	std::vector<CString>m_MonitorStateData;
	std::vector<COLORREF>m_MonitorStateColor;

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
