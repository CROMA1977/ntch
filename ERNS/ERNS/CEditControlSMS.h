#pragma once
#include <afxwin.h>
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 發送訊息的編輯框(配合簡訊中英文長度計算規則改寫字元計算規則)
class CEditControlSMS :public CEdit {
public:
			int				m_MessageLength;						///< 訊息的長度
			int			*	m_Length1;								///< 
			int			*	m_Length2;								///<
			CStatic		*	m_StaticInfo;							///< 字元長度的顯示元件

							CEditControlSMS();						///< 建構式
			void			CalMessageLength(void);					///< 計算訊息長度
			void			SetWindowTextA(LPCTSTR lpszString);		///< 設定文字
			BOOL			PreTranslateMessage(MSG * pMsg);		///< 處理視窗訊息
};

