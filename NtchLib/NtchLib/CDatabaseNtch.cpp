#include "pch.h"
//#include <atlconv.h>

#include "CDatabaseNtch.h"


CDatabaseNtchDsn::CDatabaseNtchDsn() {
	m_StandbySetup  = false;
	m_DSN			= L"未知的";
	m_UID			= L"未知的";
	m_PWD			= L"未知的";
}


CDatabaseNtchDsn::~CDatabaseNtchDsn() {
	if (this->IsOpen() == TRUE) {
		this->Close();
	}
}

void CDatabaseNtchDsn::Assign(CString DSN, CString UID, CString PWD) {
	m_DSN = DSN;
	m_UID = UID;
	m_PWD = PWD;
	m_StandbySetup = true;
}

void CDatabaseNtchDsn::AssignOracle(void) {
	m_DSN = L"TCHOST";
	m_UID = L"q0004";
	m_PWD = L"q0004";
	m_StandbySetup = true;
}

void CDatabaseNtchDsn::AssignMsServer(void) {
	m_DSN = L"WebPageUpdater";
	m_UID = L"WebPageUpdater";
	m_PWD = L"q0004";
	m_StandbySetup = true;
}

void CDatabaseNtchDsn::AssignSybase(void) {
	m_DSN = L"hmisa";
	m_UID = L"hmisa";
	m_PWD = L"hmisa95";
	m_StandbySetup = true;
}

BOOL CDatabaseNtchDsn::OpenDatabase(void) {
	if (m_StandbySetup == false) {
		AfxMessageBox(L"CDatabaseNtchDsn 還未完成設定");
		return FALSE;
	}

	CString sConnectStringDsn;
	// 產生資料庫的連線字串 DSN or DSN LESS
	sConnectStringDsn.Format(L"DSN=%s;UID=%s;PWD=%s", m_DSN, m_UID, m_PWD);
	
	TRY{
		// 連線資料庫 DSN or DSN LESS
		OpenEx(sConnectStringDsn);
	}
	CATCH(CDBException, e) {
		// If a database exception occured, show error msg
		CDatabaseNtchDsn::DisplayErrorMsg(this, e->m_strError);
	}
	END_CATCH;
	return TRUE;
}

void CDatabaseNtchDsn::DisplayErrorMsg(CDatabaseNtchDsn * Ptr, CString Msg) {
	CTime tCurrentTime = CTime::GetCurrentTime();
	CString sCurrentTime = tCurrentTime.Format(L"%Y-%m-%d %H-%M-%S");

	USES_CONVERSION;
	CString sInfo;
	sInfo.Format(L"[%s][Database %s error: %s]", sCurrentTime, Ptr->m_DSN, Msg);
	printf("\n%s", T2A(sInfo.GetBuffer()));
	sInfo.ReleaseBuffer();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CDatabaseNtchDsnLess::CDatabaseNtchDsnLess() {
	m_StandbySetup	= false;
	m_IP			= L"未知的";
	m_DatabaseName	= L"未知的";
	m_UID			= L"未知的";
	m_PWD			= L"未知的";
	m_Func			= NULL;
}

CDatabaseNtchDsnLess::~CDatabaseNtchDsnLess() {
	if (this->IsOpen() == TRUE) {
		this->Close();
	}
}
void CDatabaseNtchDsnLess::Assign(DatabaseType Type, CString DatabaseName, CString UID, CString PWD, CString IP, CString Port) {
	m_DatabaseType  = Type;
	m_IP			= IP;
	m_Port			= Port;
	m_DatabaseName	= DatabaseName;
	m_UID			= UID;
	m_PWD			= PWD;	

	switch (Type) {
		case DT_Oracle:
			if (m_Port = L"")
				m_Port = L"1521";
			m_Func = &CDatabaseNtchDsnLess::GetConnectStringForOracle;
			break;
		case DT_MS_Server:
			if (m_Port = L"")
				m_Port = L"1433";
			m_Func = &CDatabaseNtchDsnLess::GetConnectStringForMsServer;
			break;
		case DT_Sybase:
			if (m_Port = L"")
				m_Port = L"8501";
			m_Func = &CDatabaseNtchDsnLess::GetConnectStringForSybase;
			break;
		default:
			m_StandbySetup	= false;
			m_IP			= L"未知的";
			m_DatabaseName	= L"未知的";
			m_UID			= L"未知的";
			m_PWD			= L"未知的";
			m_Func			= NULL;
			return;
	}
	m_StandbySetup = true;
}
void CDatabaseNtchDsnLess::AssignOracle(void) {
	m_IP			= L"10.35.250.150";
	m_Port			= L"1521";
	m_DatabaseName	= L"health";
	m_UID			= L"q0004";
	m_PWD			= L"q0004";
	m_Func			= &CDatabaseNtchDsnLess::GetConnectStringForOracle;
	m_StandbySetup	= true;
}

void CDatabaseNtchDsnLess::AssignMsServer(void) {
	m_IP			= L"192.168.255.15";
	m_Port			= L"1433";
	m_DatabaseName	= L"急診醫學科";
	m_UID			= L"WebPageUpdater";
	m_PWD			= L"q0004";
	m_Func			= &CDatabaseNtchDsnLess::GetConnectStringForMsServer;
	m_StandbySetup	= true;
}
void CDatabaseNtchDsnLess::AssignSybase(void) {
	m_IP			= L"192.168.10.251";
	m_Port			= L"8501";
	m_DatabaseName	= L"hmisa1";
	m_UID			= L"hmisa";
	m_PWD			= L"hmisa95";
	m_Func			= &CDatabaseNtchDsnLess::GetConnectStringForSybase;
	m_StandbySetup	= true;
}

BOOL CDatabaseNtchDsnLess::OpenDatabase(void) {
	if (m_StandbySetup == false || m_Func == NULL) {
		AfxMessageBox(L"CDatabaseNtchDsnLess 還未完成設定");
		return FALSE;
	}

	// 產生資料庫的連線字串 DSN LESS
	CString sConnectStringDsnLess = m_Func(this);
	TRY{
		// 連線資料庫 DSN LESS
		OpenEx(sConnectStringDsnLess);
	}
	CATCH(CDBException, e) {
		// If a database exception occured, show error msg
		CDatabaseNtchDsnLess::DisplayErrorMsg(this, e->m_strError);
	}
	END_CATCH;
	return TRUE;
}

CString	CDatabaseNtchDsnLess::ExecuteSQL(CSqlOrder & Order) {

	TRY{
		CString SqlOrder = Order.GetSqlOrder();

		CDatabase * Database = this;
		Database->ExecuteSQL(SqlOrder);

		CRecordset recset(Database);
		recset.Open(CRecordset::forwardOnly, SqlOrder, CRecordset::readOnly);
		Order.SqlOrderProcFunc(recset);
	}
	CATCH(CDBException, e) {
		// If a database exception occured, show error msg
		AfxMessageBox(L"Database error: " + e->m_strError);
		return e->m_strError;
	}
	END_CATCH;
	return L"Correct execution";
}

// 產生各種資料庫所使用的連接字串
CString CDatabaseNtchDsnLess::GetConnectStringForOracle(CDatabaseNtchDsnLess * Ptr) {
	CString ConnectString;
	ConnectString.Format(L"DRIVER={Microsoft ODBC for Oracle};SERVER=%s:%s/%s;UID=%s;PWD=%s;", Ptr->m_IP, Ptr->m_Port, Ptr->m_DatabaseName, Ptr->m_UID, Ptr->m_PWD);
	return ConnectString;
}
CString CDatabaseNtchDsnLess::GetConnectStringForMsServer(CDatabaseNtchDsnLess * Ptr) {
	CString ConnectString;
	ConnectString.Format(L"DRIVER={SQL Server};SERVER=%s,%s;Database=%s;UID=%s;PWD=%s;", Ptr->m_IP, Ptr->m_Port, Ptr->m_DatabaseName, Ptr->m_UID, Ptr->m_PWD);
	return ConnectString;
}
CString CDatabaseNtchDsnLess::GetConnectStringForSybase(CDatabaseNtchDsnLess * Ptr) {
	CString ConnectString;
	ConnectString.Format(L"DRIVER={Sybase ASE ODBC Driver};NetworkAddress=%s,%s;DB=%s;UID=%s;PWD=%s;", Ptr->m_IP, Ptr->m_Port, Ptr->m_DatabaseName, Ptr->m_UID, Ptr->m_PWD);
	return ConnectString;
}

void CDatabaseNtchDsnLess::DisplayErrorMsg(CDatabaseNtchDsnLess * Ptr, CString Msg) {
	CTime tCurrentTime = CTime::GetCurrentTime();
	CString sCurrentTime = tCurrentTime.Format(L"%Y-%m-%d %H-%M-%S");

	USES_CONVERSION;
	CString sInfo;
	sInfo.Format(L"[%s][Database %s:%s error: %s]", sCurrentTime, Ptr->m_IP, Ptr->m_Port, Msg);
	printf("\n%s", T2A(sInfo.GetBuffer()));
	sInfo.ReleaseBuffer();
}

////////////////////////////////////////

CTimeNtch::CTimeNtch() {
}
//CTimeNtch(const CTime & Obj);
CTimeNtch::CTimeNtch(const CTimeNtch & Obj) :
CTime(Obj)
{
}

CTimeNtch::~CTimeNtch() {
}

void CTimeNtch::SetCurrentTime(void) {
	CTime *Time = this;
	*Time = CTime::GetCurrentTime();
}
void CTimeNtch::SetTimeC3YMMDD(CString TimeString) {
	USES_CONVERSION;
	int Year, Mon, Day;
	sscanf_s(T2A(TimeString.GetBuffer()), "%3d%2d%2d", &Year, &Mon, &Day);
	TimeString.ReleaseBuffer();
	CTime * Ptr = this;
	*Ptr = CTime(Year + 1911, Mon, Day, 0, 0, 0);
}
void CTimeNtch::SetTimeC4YMMDD(CString TimeString) {
	USES_CONVERSION;
	int Year, Mon, Day;
	sscanf_s(T2A(TimeString.GetBuffer()), "%4d%2d%2d", &Year, &Mon, &Day);
	TimeString.ReleaseBuffer();
	//  第一碼代表民國前 1001 = 民國前一年 = 1911 - 1
	if (Year > 1000) {
		Year = 0 - (Year % 1000);
	}
	CTime * Ptr = this;
	*Ptr = CTime(Year + 1911, Mon, Day, 0, 0, 0);
}

CString CTimeNtch::GetDateStringC3YMMDD(void) {
	CString TimeString;
	TimeString.Format(L"%03d%02d%02d", this->GetYear() - 1911, this->GetMonth(), this->GetDay());
	return TimeString;
}

CString CTimeNtch::GetDateStringC4YMMDD(void) {
	CString TimeString;
	TimeString.Format(L"%04d%02d%02d", (this->GetYear() >= 1911) ? (this->GetYear() - 1911):(1000-(1911-this->GetYear())), this->GetMonth(), this->GetDay());
	return TimeString;
}

CTimeNtch & CTimeNtch::operator = (const CTimeNtch & Obj) {
	CTime * Ptr1 = this;
	const CTime * Ptr2 = &Obj;
	*Ptr1 = *Ptr2;
	return *this;
}