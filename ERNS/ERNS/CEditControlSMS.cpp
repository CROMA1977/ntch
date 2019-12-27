#include "stdafx.h"
#include "CEditControlSMS.h"


CEditControlSMS::CEditControlSMS() {
	m_Length1 = NULL;
	m_Length2 = NULL;
	m_MessageLength = 0;
	m_StaticInfo = NULL;
	CEdit::CEdit();
}

// 計算訊息長度
void CEditControlSMS::CalMessageLength(void) {
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
	if (m_StaticInfo != NULL)
		m_StaticInfo->SetWindowTextA(State);
}

void CEditControlSMS::SetWindowTextA(LPCTSTR lpszString) {
	CEdit::SetWindowTextA(lpszString);
	CalMessageLength();
}

BOOL CEditControlSMS::PreTranslateMessage(MSG* pMsg) {
	BOOL bDone = FALSE;
	CalMessageLength();
	return bDone;
}// end of PreTranslateMessage