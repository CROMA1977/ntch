#pragma once
#include <afxwin.h>
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// �o�e�T�����s���(�t�X²�T���^����׭p��W�h��g�r���p��W�h)
class CEditControlSMS :public CEdit {
public:
			int				m_MessageLength;						///< �T��������
			int			*	m_Length1;								///< 
			int			*	m_Length2;								///<
			CStatic		*	m_StaticInfo;							///< �r�����ת���ܤ���

							CEditControlSMS();						///< �غc��
			void			CalMessageLength(void);					///< �p��T������
			void			SetWindowTextA(LPCTSTR lpszString);		///< �]�w��r
			BOOL			PreTranslateMessage(MSG * pMsg);		///< �B�z�����T��
};

