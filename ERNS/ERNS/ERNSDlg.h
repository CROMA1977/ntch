
// ERNSDlg.h: 標頭檔
//

#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 簡訊發送人員資訊
class CSenderInfo {
public:
			CString			m_Phone;					///< 接收簡訊的行動電話號碼
			CString			m_Name;						///< 簡訊接收人的姓名
			CString			m_Group;					///< 簡訊接收人的群組
			CString			m_JobTitle;					///< 簡訊接收人的職稱
			CString			m_Department;				///< 簡訊接收人的部門
			CString			m_RecvMessageBody;			///< 簡訊接收人的回覆訊息
							CSenderInfo() {}
	virtual					~CSenderInfo() {}
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
			HICON						m_hIcon;
	// 產生的訊息對應函式
										
	virtual BOOL						OnInitDialog();										///< 初始化對話框
	afx_msg void						OnSysCommand(UINT nID, LPARAM lParam);				///< 系統訊息										
	afx_msg void						OnPaint();											///< 更新畫面
	afx_msg HCURSOR						OnQueryDragIcon();									///< 繪製 ICON		
	afx_msg void						OnBnClickedButtonSendSms();							///< 發送簡訊										
	afx_msg void						OnBnClickedButtonUpdate();							///< 強制更新										
	afx_msg void						OnBnClickedButtonSelectSendMessageListFile();		///< 載入發送資料										
	afx_msg void						OnBnClickedButtonGetState();						///< 取得回報資料			
	afx_msg void						OnBnClickedButtonUseTemplate();						///< 套用簡訊模板
	afx_msg void						OnTimer(UINT_PTR nIDEvent);							///< 定時器
	DECLARE_MESSAGE_MAP()
public:
			int							m_NowMessageID;				///< 現在的訊息編號
			int							m_MessageInfoLength;		///< 發送訊息的長度
			int							m_ScheduledSenderNumber;	///< 預定發送人數
			int							m_SenderCompletedNumber;	///< 完成訊息發送人數
			int							m_RepliesCompletedNumber;	///< 完成回覆人數
			int							m_RepliesState1Number;		///< 狀態一人數
			int							m_RepliesState2Number;		///< 狀態二人數
			int							m_RepliesState3Number;		///< 狀態三人數
			int							m_RepliesState4Number;		///< 狀態四人數
			int							m_RepliesState5Number;		///< 狀態五人數
			int							m_RepliesState6Number;		///< 狀態六人數

			std::vector<CSenderInfo>	m_SMS_SendList;				///< 通知人員資訊
			std::vector<CString>		m_SMS_GroupList;			///< 群組清單	
			std::vector<CString>		m_SMS_DepartmentList;		///< 部門清單
			std::vector<CString>		m_SMS_MessageCodeList;		///< 訊息代碼
			std::vector<CString>		m_SMS_DistrictCodeList;		///< 院區代碼
			std::vector<CString>		m_SMS_TemplateListA;		///< 簡訊模板A
			std::vector<CString>		m_SMS_TemplateListB;		///< 簡訊模板B

			CComboBox					m_ComboTemplate;			///< 簡訊模板選單
			CComboBox					m_ComboDistrict;			///< 院區模板選單

			CEditControlSMS				m_EditSmsMessage1;			///< 簡訊編輯框1
			CEditControlSMS				m_EditSmsMessage2;			///< 簡訊編輯框2

			CStatic						m_StaticInfo;				///< 簡訊的長度
			CStatic						m_UpdateTime;				///< 召回情況與更新時間
			CStatic						m_StaticState1;				///< 發送狀態
			CStatic						m_StaticState2;				///< 回覆狀態
			CStatic						m_StaticState3;				///< 已抵達人數
			CStatic						m_StaticState4;				///< 10 分鐘內返院
			CStatic						m_StaticState5;				///< 20 分鐘內返院
			CStatic						m_StaticState6;				///< 30 分鐘內返院
			CStatic						m_StaticState7;				///< 60 分鐘內返院
			CStatic						m_StaticState8;				///< 無法返院

			CEdit						m_Filename;					///< 目前使用的檔案名稱
			CToolTipListCtrl			m_ListCtrlRecallSituation;	///< 召回群組清單元件
										
			void						ReadSendList(CString PathName, CString FileName);		///< 讀取發送資料的訊息檔
			void						Update(void);											///< 更新訊息										
			void						OutputList(int Item, int iSubItem);						///< 輸出 List Item 狀態
private:								
			void						OutputState(bool openFile);
			void						OutputGroupList(FILE *fptr, CString Group);				///< 輸出群組的狀態										
			void						OutputDepartmentList(FILE *fptr, CString Department);	///< 輸出部門的狀態
			void						UseTemplateSetMessage(void);
public:
};
