
// ERNSDlg.h: 標頭檔
//

#pragma once
class CEditControlSMS :public CEdit {

public:

	// 關鍵片段:
	int				m_MessageLength;
	int			*	m_Length1;
	int			*	m_Length2;
	CStatic		*	m_StaticInfo;

	CEditControlSMS() {
		m_Length1 = NULL;
		m_Length2 = NULL;
		m_MessageLength = 0;
		m_StaticInfo = NULL;
		CEdit::CEdit();
	}
	void CalMessageLength(void) {
		CString Text;
		this->GetWindowTextA(Text);
		char * ptr = Text.GetBuffer();
		int TextNumber = 0;
		for (unsigned int i = 0; i < strlen(ptr); i++) {
			unsigned char a = ptr[i];
			unsigned char b = ptr[i + 1];
			if (((a >= 0xA4 && a <= 0xC6) || (a >= 0xC9 && a <= 0xF9)) && ((b >= 0x40 && b <= 0x7E) || (a >= 0xA1 && b <= 0xFE))) {
				i++;
				TextNumber++;
				continue;
			}
			TextNumber++;
		}
		m_MessageLength = TextNumber;

		int MessageLength1 = 0;
		int MessageLength2 = 0;

		if (m_Length1 != NULL)
			MessageLength1 = *m_Length1;
		if (m_Length2 != NULL)
			MessageLength2 = *m_Length2;

		CString State;
		State.Format("召回訊息(%d/70 字元內 : %d/70 字元內)", MessageLength1, MessageLength2);
		if(m_StaticInfo != NULL)
			m_StaticInfo->SetWindowTextA(State);
	}
	void SetWindowTextA(LPCTSTR lpszString) {
		CEdit::SetWindowTextA(lpszString);
		CalMessageLength();
	}
	BOOL PreTranslateMessage(MSG* pMsg) {
		BOOL bDone = FALSE;
		CalMessageLength();
		return bDone;
	}// end of PreTranslateMessage
};

// CERNSDlg 對話方塊
class CERNSDlg : public CDialogEx
{
// 建構
public:
	CERNSDlg(CWnd* pParent = nullptr);	// 標準建構函式

// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ERNS_DIALOG };
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
	int						m_MessageInfoLength;		///< 發送訊息的長度
	int						m_ScheduledSenderNumber;	///< 預定發送人數
	int						m_SenderCompletedNumber;	///< 完成訊息發送人數
	int						m_RepliesCompletedNumber;	///< 完成回覆人數
	int						m_RepliesState1Number;		///< 狀態一人數
	int						m_RepliesState2Number;		///< 狀態二人數
	int						m_RepliesState3Number;		///< 狀態三人數
	int						m_RepliesState4Number;		///< 狀態四人數
	int						m_RepliesState5Number;		///< 狀態五人數

	std::vector<CString>	m_SMS_SendList;				///< 發送電話清單
	
	//CEdit					m_EditSmsMessage;
	CEditControlSMS			m_EditSmsMessage1;
	CEditControlSMS			m_EditSmsMessage2;

	CStatic					m_StaticState1;
	CStatic					m_StaticState2;
	CStatic					m_StaticState3;
	CStatic					m_StaticState4;
	CStatic					m_StaticState5;
	CStatic					m_StaticState6;
	CStatic					m_StaticState7;
	CStatic					m_StaticInfo;

	void ReadSendList(CString PathName, CString FileName);

	void Update(void);

	afx_msg void OnBnClickedButtonSendSms();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg void OnBnClickedButtonUpdate();


	CEdit m_22342;
	afx_msg void OnBnClickedButtonSelectSendMessageListFile();
	CEdit m_Filename;
};
