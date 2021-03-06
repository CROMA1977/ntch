// WebPageUpdater.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//

#include "pch.h"
#include "WebPageUpdater.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

LPWSTR a2t(char *str) {
	USES_CONVERSION;
	return A2T(str);
}
// 院區急診資訊 預期使用的欄位
//m_BedChange							= false;			// 床位狀態變動
//m_BedExhausted						= L"否";			// 滿床位
//m_BedExhaustedTime					= L"201811010000";	// 滿床位起始時間 YYYYMMDDHHMM
//m_NumberOfWaitingOfVisits				= L"0";				// 等待看診人數(已使用)
//m_NumberOfWaitingOfPushers			= L"0";				// 等待推床人數
//m_NumberOfWaitingOfHospitalization	= L"0";				// 等待住院人數(已使用)
//m_NumberOfWaitingOfICU				= L"0";				// 等待 ICU 人數
/// 院區急診資訊
struct EmergencyInfo {
public:
										 EmergencyInfo();
	bool								m_BedChang;							// 床位狀態變動
	CString								m_BedExhausted;						// 滿床位
	CString								m_BedExhaustedTime;					// 滿床位起始時間 YYYYMMDDHHMM
	CString								m_NumberOfWaitingOfVisits;			///< 等待看診人數 數字 兩字元
	CString								m_NumberOfWaitingOfHospitalization;	///< 等待住院人數 數字 兩字元
}District[2];

EmergencyInfo & DistrictSanChong	= District[0];												///< 三重院區
EmergencyInfo & DistrictBanQiao		= District[1];												///< 板橋院區

// 初始化 
EmergencyInfo::EmergencyInfo() {
	// 當沒有資料時的預設值
	m_BedChang							= false;
	m_BedExhausted						= L"否";			
	m_BedExhaustedTime					= L"201811010000";	
	m_NumberOfWaitingOfVisits			= L"0";		
	m_NumberOfWaitingOfHospitalization	= L"0";
}

// 唯一一個應用程式物件

CWinApp theApp;

using namespace std;

int main() {
    int nRetCode = 0;

    HMODULE hModule = ::GetModuleHandle(nullptr);

    if (hModule != nullptr) {
        // 將 MFC 初始化並於失敗時列印錯誤
        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0)) {
            // TODO: 在此撰寫應用程式行為的程式碼。
            wprintf(L"嚴重錯誤: MFC 初始化失敗\n");
            nRetCode = 1;
        } else {
            // TODO: 在此撰寫應用程式行為的程式碼。	
			RunStat = L"";
			ReadInfoForSqlServer();											// 初次從 SQL 讀入相關資訊			
			UpdateRunTimeInfo();											// 更新執行階段的資訊
			ReadInfoForOracleDatabase();									// 從 Oracle 讀入相關資訊
			ReadInfoForSqlServer();											// 從 SQL	 讀入相關資訊
			WriteInfoToSqlServer();											// 寫入資訊到 SQL Server
			UpdateRunStat();
			wprintf(L"\n[%s][Run Cycle over]", UpdateTime.GetBuffer() );
			UpdateTime.ReleaseBuffer();
			Sleep(1000 * 30);
		}
    } else {
        // TODO: 配合您的需要變更錯誤碼
        wprintf(L"嚴重錯誤: GetModuleHandle 失敗\n");
        nRetCode = 1;
    }
    return nRetCode;
}

// 更新執行階段的資訊
void UpdateRunTimeInfo(void) {
	// 取得現在的日期字串 格式(民國) YYYMMDD
	CurrentTime = CTime::GetCurrentTime();
	TodayYYYMMDD.Format(L"%03d%02d%02d", CurrentTime.GetYear() - 1911, CurrentTime.GetMonth(), CurrentTime.GetDay());
	// 取得一天前的日期字串 格式(民國) YYYMMDD
	OneDaysAgo = CurrentTime - CTimeSpan(1, 0, 0, 0);
	OneDayAgoYYYMMDD.Format(L"%03d%02d%02d", OneDaysAgo.GetYear() - 1911, OneDaysAgo.GetMonth(), OneDaysAgo.GetDay());
	// 取得兩天前的日期字串 格式(民國) YYYMMDD
	TwoDaysAgo = CurrentTime - CTimeSpan(2, 0, 0, 0);
	TwoDayAgoYYYMMDD.Format(L"%03d%02d%02d", TwoDaysAgo.GetYear() - 1911, TwoDaysAgo.GetMonth(), TwoDaysAgo.GetDay());

	// 取得更新的時間字串 格式 MM/DD HH:MM
	UpdateTime.Format(L"%02d/%02d %02d:%02d", CurrentTime.GetMonth(), CurrentTime.GetDay(), CurrentTime.GetHour(), CurrentTime.GetMinute());
	// 取得更新的時間字串 格式 YYYYMMDDHHMM YYY-MM-DD HH:MM:00.000
	UpdateTimeYYYMMDD.Format(L"%04d-%02d-%02d %02d:%02d:00.000", CurrentTime.GetYear(), CurrentTime.GetMonth(), CurrentTime.GetDay(), CurrentTime.GetHour(), CurrentTime.GetMinute());
	// 取得滿床位更新時間字串 格式 YYYYMMDDHHMM
	BedChangTime.Format(L"%04d%02d%02d%02d%02d", CurrentTime.GetYear(), CurrentTime.GetMonth(), CurrentTime.GetDay(), CurrentTime.GetHour(), CurrentTime.GetMinute());

	

	char szBuffer[1024];

	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 0);
	if (::WSAStartup(wVersionRequested, &wsaData) != 0)
		return ;

	if (gethostname(szBuffer, sizeof(szBuffer)) == SOCKET_ERROR) {
		WSACleanup();
		return ;
	}

	struct hostent *host = gethostbyname(szBuffer);
	if (host == NULL) {
		WSACleanup();
		return ;
	}

	LocalIP = inet_ntoa(*(struct in_addr *)*host->h_addr_list);
	
	//Obtain the computer's IP
	//myIP.b1 = ((struct in_addr *)(host->h_addr))->S_un.S_un_b.s_b1;
	//myIP.b2 = ((struct in_addr *)(host->h_addr))->S_un.S_un_b.s_b2;
	//myIP.b3 = ((struct in_addr *)(host->h_addr))->S_un.S_un_b.s_b3;
	//myIP.b4 = ((struct in_addr *)(host->h_addr))->S_un.S_un_b.s_b4;

	WSACleanup();
}

// 從 Oracle 資料庫讀取資訊
void ReadInfoForOracleDatabase(void) {
	CString sSqlString, sConnectStringDsn, sConnectStringDsnLess;
	
	// 設定資料庫連線的相關參數
	CString sDriver = L"Microsoft ODBC for Oracle";
	CString sServer = L"10.35.250.150:1521/health";
	CString sDsn	= L"TCHOST";
	CString sUID	= L"q0004";
	CString sPWD	= L"q0004";

	// 產生資料庫的連線字串 DSN or DSN LESS
	sConnectStringDsnLess.Format( L"DRIVER={%s};SERVER=%s;UID=%s;PWD=%s;", sDriver, sServer, sUID, sPWD);

	CDatabase database;
	TRY{
		// 連線資料庫 DSN or DSN LESS
		database.OpenEx(sConnectStringDsnLess);

		//--等待看診人數
		sSqlString.Format(	L"select b.area, count(*) "
							L"from "
							L"fpr005k a, fth007k b "
							L"where a.sdate >= '%s' "
							L"and a.dept = b.dept "
							L"and (a.status <> '3' or a.status is null) "
							L"and a.source = '2' "
							L"and (a.see_doct = '' or a.see_doct is null) group by b.area", OneDayAgoYYYMMDD);
		database.ExecuteSQL(sSqlString);

		CRecordset recset(&database);
		recset.Open(CRecordset::forwardOnly, sSqlString, CRecordset::readOnly);

		// 取出各欄位的資訊
		CString sArea, sCount;
		// Loop through each record
		while (!recset.IsEOF()) {
			recset.GetFieldValue(L"AREA", sArea);
			recset.GetFieldValue(L"COUNT(*)", sCount);
			recset.MoveNext();

			// 根據院區編號寫入對應的變數
			if (sArea == L"A") {
				DistrictSanChong.m_NumberOfWaitingOfVisits = sCount;
				continue;
			}
			if (sArea == L"B") {
				DistrictBanQiao.m_NumberOfWaitingOfVisits = sCount;
				continue;
			}
		}
		/*	select AREA,count(*) from (
			select Q.AREA
			from
			FAM005K A,FTH008K B,FAM020K E,FTH013K G,FTH013K H,FOM003K I,FTH010K K,FTH005K L,FTH012K M,FTH005K N,FTH019K O,FTH024K P,FTH007K Q
			where
			A.STATUS = '1' and A.REGNO = B.REGNO(+) and A.CASENO = E.CASENO(+) and E.TYPE(+) = '1'
			and E.CREC(+) = 1 and A.COME = G.CODE(+) and G.TYPE(+) = '9' and A.P = H.CODE(+)
			and H.TYPE(+) = '8' and E.ICDCODE = I.CODE(+) and A.MDOCT = K.DOCT(+) and K.LOGIN_ID = L.LOGIN_ID(+)
			and K.SDATE >= all (select SDATE from FTH010K where DOCT = K.DOCT)
			and B.CITY_ID = M.CITY(+) and A.OK_USER = N.LOGIN_ID(+)
			and A.IPD_DEPT=Q.DEPT(+)
			and A.POSI = O.POSI(+) and A.PAY_NO = P.HOSP(+) and P.TYPE(+) = 'I'
			and A.OK_DATE >= '1071103' and A.OK_DATE <= '1071105'
			and A.COME in ('1','2')
			)A
			group by Area
		*/

		//--待床中(許可證登入)
		sSqlString.Format(	L"select AREA,count(*) from ( "
							L"select Q.AREA "
							L"from "
							L"FAM005K A, FTH008K B, FAM020K E, FTH013K G, FTH013K H, FOM003K I, FTH010K K, FTH005K L, FTH012K M, FTH005K N, FTH019K O, FTH024K P, FTH007K Q "
							L"where "
							L"A.STATUS = '1' and A.REGNO = B.REGNO(+) and A.CASENO = E.CASENO(+) and E.TYPE(+) = '1' "
							L"and A.ocaseno in (select caseno from fpr005k where source='2' and status='1' and sdate>='%s') "
							L"and E.CREC(+) = 1 and A.COME = G.CODE(+) and G.TYPE(+) = '9' and A.P = H.CODE(+) "
							L"and H.TYPE(+) = '8' and E.ICDCODE = I.CODE(+) and A.MDOCT = K.DOCT(+) and K.LOGIN_ID = L.LOGIN_ID(+) "
							L"and K.SDATE >= all(select SDATE from FTH010K where DOCT = K.DOCT) "
							L"and B.CITY_ID = M.CITY(+) and A.OK_USER = N.LOGIN_ID(+) "
							L"and A.IPD_DEPT = Q.DEPT(+) "
							L"and A.POSI = O.POSI(+) and A.PAY_NO = P.HOSP(+) and P.TYPE(+) = 'I' "
							L"and A.OK_DATE >= '%s' and A.OK_DATE <= '%s' "
							L"and A.COME in('1', '2') "
							L")A "
							L"group by Area", TwoDayAgoYYYMMDD, TwoDayAgoYYYMMDD, TodayYYYMMDD);
		database.ExecuteSQL(sSqlString);

		// 取出各欄位的資訊
		CRecordset recset1(&database);
		recset1.Open(CRecordset::forwardOnly, sSqlString, CRecordset::readOnly);

		while (!recset1.IsEOF()) {
			recset1.GetFieldValue(L"AREA", sArea);
			recset1.GetFieldValue(L"COUNT(*)", sCount);
			recset1.MoveNext();
			// 根據院區編號寫入對應的變數
			if (sArea == L"A") {
				DistrictSanChong.m_NumberOfWaitingOfHospitalization = sCount;
				continue;
			}
			if (sArea == L"B") {
				DistrictBanQiao.m_NumberOfWaitingOfHospitalization = sCount;
				continue;
			}
		}
		// 關閉資料庫連線
		database.Close();
	}	
	CATCH(CDBException, e) {
		database.Close();
		// If a database exception occured, show error msg
		wprintf(L"\n[%s][Read Info For Oracle Database Database error:%s]", UpdateTime.GetBuffer(), e->m_strError.GetBuffer());
		e->m_strError.ReleaseBuffer();
		UpdateTime.ReleaseBuffer();
	}
	END_CATCH;
}

// 讀取 Sql Server 資訊
void ReadInfoForSqlServer(void) {
	CString sSqlString, sConnectStringDsn, sConnectStringDsnLess;

	// 設定資料庫連線的相關參數
	CString sDriver = L"SQL Server";
	CString sServer = L"192.168.255.15";
	CString sDsn = L"WebPageUpdater";
	CString sUID = L"WebPageUpdater";
	CString sPWD = L"q0004";

	// 產生資料庫的連線字串 DSN or DSN LESS
	sConnectStringDsn.Format(L"DSN=%s;UID=%s;PWD=%s", sDsn, sUID, sPWD);
	sConnectStringDsnLess.Format(L"DRIVER={%s};SERVER=%s;UID=%s;PWD=%s;", sDriver, sServer, sUID, sPWD);

	CDatabase database;
	TRY{
		// 連線資料庫 DSN or DSN LESS
		database.OpenEx(sConnectStringDsnLess);

		/*
		SELECT [院區]
	    ,[是否滿床]
		,[候診人數]
	    ,[等待推床人數]
	    ,[等待住院人數]
	    ,[等待ICU人數]
		,[更新時間]
		,[滿床時間]
		FROM [dbo].[急診公開資訊觀測]
		*/

		//-- 更新資料
		sSqlString.Format(	L"SELECT"
							L" [院區] "
							L",[是否滿床] "
							L",[滿床時間] "
							L"FROM [dbo].[急診公開資訊觀測] ");
		database.ExecuteSQL(sSqlString);

		CRecordset recset(&database);
		recset.Open(CRecordset::forwardOnly, sSqlString, CRecordset::readOnly);

		// 取出各欄位的資訊

		//sDistrict							= L"三重院區"		// [院區]
		//sBedExhausted						= L"否";			// [是否滿床]
		//sBedExhaustedTime					= L"201811010000";	// [滿床時間] 滿床位起始時間 YYYYMMDDHHMM
		CString sDistrict, sBedExhausted, sBedExhaustedTime;
		// Loop through each record
		while (!recset.IsEOF()) {
			recset.GetFieldValue(L"院區",		sDistrict);
			recset.GetFieldValue(L"是否滿床",	sBedExhausted);
			recset.GetFieldValue(L"滿床時間",	sBedExhaustedTime);
			recset.MoveNext();

			// 根據院區編號紀錄床位狀態是否改變 現有的床位狀況 床位變化的時間
			if (sDistrict == L"三重院區") {
				// 如果滿床位狀態變化 否->是
				if (DistrictSanChong.m_BedExhausted != sBedExhausted && sBedExhausted != L"否") {
					// 紀錄狀態變化
					DistrictSanChong.m_BedChang = true;
				}
				// 記錄滿床位狀態和滿床位時間
				DistrictSanChong.m_BedExhausted			= sBedExhausted;
				DistrictSanChong.m_BedExhaustedTime		= sBedExhaustedTime;				
				continue;
			}
			if (sDistrict == L"板橋院區") {
				// 如果滿床位狀態變化 否->是
				if (DistrictBanQiao.m_BedExhausted != sBedExhausted && sBedExhausted != L"否") {
					// 紀錄狀態變化
					DistrictBanQiao.m_BedChang = true;			
				}
				// 記錄滿床位狀態和滿床位時間
				DistrictBanQiao.m_BedExhausted			= sBedExhausted;
				DistrictBanQiao.m_BedExhaustedTime		= sBedExhaustedTime;
				continue;
			}	
		}

		// 關閉資料庫連線
		database.Close();
	}
	CATCH(CDBException, e) {
		database.Close();
		// If a database exception occured, show error msg
		//AfxMessageBox(L"Database error: " + e->m_strError);
		RunStat += e->m_strError;		
		wprintf(L"\n[%s][Read Info For SQL Server Database error:%s]", UpdateTime.GetBuffer(), e->m_strError.GetBuffer());
		e->m_strError.ReleaseBuffer();
		UpdateTime.ReleaseBuffer();
	}
	END_CATCH;
}

// 寫入 SQL Server 的資訊
void WriteInfoToSqlServer(void) {
	CString sSqlString, sConnectStringDsn, sConnectStringDsnLess;

	// 設定資料庫連線的相關參數
	CString sDriver = L"SQL Server";
	CString sServer = L"192.168.255.15";
	CString sDsn	= L"WebPageUpdater";
	CString sUID	= L"WebPageUpdater";
	CString sPWD	= L"q0004";

	// 產生資料庫的連線字串 DSN or DSN LESS
	sConnectStringDsn.Format(L"DSN=%s;UID=%s;PWD=%s", sDsn, sUID, sPWD);
	sConnectStringDsnLess.Format(L"DRIVER={%s};SERVER=%s;UID=%s;PWD=%s;", sDriver, sServer, sUID, sPWD);	

	CDatabase database;
	TRY{
		// 連線資料庫 DSN or DSN LESS
		database.OpenEx(sConnectStringDsnLess);

		/*	
			UPDATE [dbo].[急診公開資訊觀測]
			SET [候診人數]		= <候診人數, nchar(2),>,
			    [等待住院人數]	= <等待住院人數, nchar(2),>,
				[更新時間]		= <更新時間, nChar(10),>     -- MM/DD HH:MM
			WHERE [院區] = "三重院區"
		*/

		//-- 更新資料
		sSqlString.Format(	L"UPDATE [dbo].[急診公開資訊觀測] "
							L"SET"
							L" [候診人數] = '%s' "
							L",[等待住院人數] = '%s' "
							L",[更新時間] = '%s' "
							L"WHERE [院區] = '三重院區' ", 
							DistrictSanChong.m_NumberOfWaitingOfVisits, DistrictSanChong.m_NumberOfWaitingOfHospitalization, UpdateTime);
		database.ExecuteSQL(sSqlString);
		
		sSqlString.Format(	L"UPDATE [dbo].[急診公開資訊觀測] "
							L"SET"
							L" [候診人數] = '%s' "
							L",[等待住院人數] = '%s' "
							L",[更新時間] = '%s' "
							L"WHERE [院區] = '板橋院區' ",
							DistrictBanQiao.m_NumberOfWaitingOfVisits, DistrictBanQiao.m_NumberOfWaitingOfHospitalization, UpdateTime);
		database.ExecuteSQL(sSqlString);

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// 當滿床位狀態發生變化時系統更新滿床位發生時間
		if (DistrictSanChong.m_BedChang == true) {
			DistrictSanChong.m_BedChang = false;

			sSqlString.Format(L"UPDATE [dbo].[急診公開資訊觀測] "
				L"SET "
				L"[滿床時間] = '%s' "
				L"WHERE [院區] = '三重院區' ",
				BedChangTime);
			database.ExecuteSQL(sSqlString);
			DistrictSanChong.m_BedExhaustedTime = BedChangTime;
		}
		if (DistrictBanQiao.m_BedChang == true) {
			DistrictBanQiao.m_BedChang = false;

			sSqlString.Format(L"UPDATE [dbo].[急診公開資訊觀測] "
				L"SET "
				L"[滿床時間] = '%s' "
				L"WHERE [院區] = '板橋院區' ",
				BedChangTime);
			database.ExecuteSQL(sSqlString);
			DistrictBanQiao.m_BedExhaustedTime = BedChangTime;
		}
		/////////////////////////////////////////////
		// 當滿床位狀態時 開始檢查滿床位時間 兩小時後自動關閉
		if (DistrictSanChong.m_BedExhausted != L"否") {
			CTime BedExhaustedTime = ConversionTimeFormat(DistrictSanChong.m_BedExhaustedTime);
			// 如果現在時間超過 滿床設定時間 兩個小時 將 滿床狀態關閉
			
			if (CurrentTime > (BedExhaustedTime + CTimeSpan(0, 2, 0, 0) ) ){
				sSqlString.Format(L"UPDATE [dbo].[急診公開資訊觀測] "
					L"SET "
					L"[是否滿床] = '否' "
					L"WHERE [院區] = '三重院區' ");
				database.ExecuteSQL(sSqlString);				
			}			
		}
		
		if (DistrictBanQiao.m_BedExhausted != L"否") {
			CTime BedExhaustedTime = ConversionTimeFormat(DistrictBanQiao.m_BedExhaustedTime);
				// 如果現在時間超過 滿床設定時間 兩個小時 將 滿床狀態關閉
				if (CurrentTime > (BedExhaustedTime + CTimeSpan(0, 2, 0, 0) ) ) {
					sSqlString.Format(L"UPDATE [dbo].[急診公開資訊觀測] "
						L"SET "
						L"[是否滿床] = '否' "
						L"WHERE [院區] = '板橋院區' ");
					database.ExecuteSQL(sSqlString);
				}
		}
		
		// 關閉資料庫連線
		database.Close();
	}
	CATCH(CDBException, e) {
		database.Close();
		// If a database exception occured, show error msg
		//AfxMessageBox(L"Database error: " + e->m_strError);
		RunStat += e->m_strError;
		wprintf(L"[%s][Write Info To SQL Server Database error:%s]", UpdateTime.GetBuffer(), e->m_strError.GetBuffer());
		e->m_strError.ReleaseBuffer();
		UpdateTime.ReleaseBuffer();
	}
	END_CATCH;
}

// 讀取 Sybase Server 資訊
void ReadInfoForSybaseServer(void) {
	CString sSqlString, sConnectStringDsn, sConnectStringDsnLess;

	// 設定資料庫連線的相關參數
	CString sDriver = L"Sybase ASE ODBC Driver";
	CString sServer = L"192.168.10.251,8501;DB=hmisa1";
	CString sDsn = L"hmisa";
	CString sUID = L"hmisa";
	CString sPWD = L"hmisa95";

	// aseCon = New AseConnection("DataSource=192.168.10.251;Port=8501;Database=hmisa1;UID=hmisa;PWD=hmisa95;charset=big5")
	// _SqlCommand = "SELECT pat_no,pat_idno FROM hh_mpat where pat_no = 445644"
	// 61744

	// 產生資料庫的連線字串 DSN or DSN LESS
	//Driver={Sybase ASE ODBC Driver};NetworkAddress=myServerAddress,5000;Db = myDataBase; Uid = myUsername; Pwd = myPassword;
	sConnectStringDsn.Format(L"DSN=%s;UID=%s;PWD=%s", sDsn, sUID, sPWD);
	sConnectStringDsnLess.Format(L"DRIVER={%s};NetworkAddress=%s;UID=%s;PWD=%s;", sDriver, sServer, sUID, sPWD);
	CDatabase database;
	TRY{
		// 連線資料庫 DSN or DSN LESS
		//database.OpenEx(sConnectStringDsn);
		database.OpenEx(sConnectStringDsnLess);

		//-- 更新資料
		sSqlString.Format(L"SELECT pat_no,pat_idno, pat_name, pat_birth_dt FROM hh_mpat where pat_no = 445644");
		database.ExecuteSQL(sSqlString);

		CRecordset recset(&database);
		recset.Open(CRecordset::forwardOnly, sSqlString, CRecordset::readOnly);

		// 取出各欄位的資訊

		CString sPat_no, sPat_idno, sPat_name, sPat_birthe_dt;
		// Loop through each record
		while (!recset.IsEOF()) {
			recset.GetFieldValue(L"pat_no", sPat_no);
			recset.GetFieldValue(L"pat_idno", sPat_idno);
			recset.GetFieldValue(L"pat_name", sPat_name);
			recset.GetFieldValue(L"pat_birth_dt", sPat_birthe_dt);

			recset.MoveNext();
		}
		// 關閉資料庫連線
		database.Close();
	}
	CATCH(CDBException, e) {
		database.Close();
		// If a database exception occured, show error msg
		// AfxMessageBox(L"Database error: " + e->m_strError);
		RunStat += e->m_strError;
		wprintf(L"\n[%s][Read Info For SQL Server Database error:%s]", UpdateTime.GetBuffer(), e->m_strError.GetBuffer());
		e->m_strError.ReleaseBuffer();
		UpdateTime.ReleaseBuffer();
	}
	END_CATCH;
}

// 將 YYYYMMDDHHMM 的時間字串資料轉換成時間物件
CTime ConversionTimeFormat(CString & sTime) {
	int year, mon, day, hr, min;
	_stscanf_s(sTime.GetBuffer(), L"%4d%2d%2d%2d%2d", &year, &mon, &day, &hr, &min);
	sTime.ReleaseBuffer();
	return CTime(year, mon, day, hr, min, 0);
}

void UpdateRunStat(void) {
	// 資產代碼與服務代碼(重要)
	CString AssetCode = L"2019021202";
	CString ServiceCode = L"0001";
	/*
	// 取得現在的日期字串 格式(民國) YYYMMDD
	CurrentTime = CTime::GetCurrentTime();
	// 取得更新的時間字串 格式 YYYYMMDDHHMM YYY-MM-DD HH:MM:00.000
	UpdateTimeYYYMMDD.Format(L"%04d-%02d-%02d %02d:%02d:00.000", CurrentTime.GetYear(), CurrentTime.GetMonth(), CurrentTime.GetDay(), CurrentTime.GetHour(), CurrentTime.GetMinute());

	char szBuffer[1024];
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 0);
	if (::WSAStartup(wVersionRequested, &wsaData) != 0)
		return;
	if (gethostname(szBuffer, sizeof(szBuffer)) == SOCKET_ERROR) {
		WSACleanup();
		return;
	}

	struct hostent *host = gethostbyname(szBuffer);
	if (host == NULL) {
		WSACleanup();
		return;
	}

	LocalIP = inet_ntoa(*(struct in_addr *)*host->h_addr_list);
	WSACleanup();
	*/
	// 進行資料庫資料更新作業
	CString sSqlString, sConnectStringDsn, sConnectStringDsnLess;
	// 設定資料庫連線的相關參數
	CString sDriver = L"SQL Server";
	CString sServer = L"192.168.255.15";
	CString sDsn = L"WebPageUpdater";
	CString sUID = L"sa";
	CString sPWD = L"Pa55w0rd";

	// 產生資料庫的連線字串 DSN or DSN LESS
	sConnectStringDsn.Format(L"DSN=%s;UID=%s;PWD=%s", sDsn, sUID, sPWD);
	sConnectStringDsnLess.Format(L"DRIVER={%s};SERVER=%s;UID=%s;PWD=%s;", sDriver, sServer, sUID, sPWD);

	if (RunStat == L"")
		RunStat = L"正常執行";

	CDatabase database;
	TRY{
		// 連線資料庫 DSN or DSN LESS
		database.OpenEx(sConnectStringDsnLess);

		// 檢查資料是否存在
		sSqlString.Format(L"SELECT [資產代碼], [服務代碼] FROM [AFDBS].[dbo].[行政部門.資訊室.0250-4-A0-0101.資訊資產服務狀況回報表] WHERE [資產代碼] = '%s' AND [服務代碼] = '%s'", AssetCode, ServiceCode);
		database.ExecuteSQL(sSqlString);

		CRecordset recset(&database);
		recset.Open(CRecordset::forwardOnly, sSqlString, CRecordset::readOnly);

		// 檢查資料是否存在
		if (recset.IsEOF() == TRUE) {
			database.Close();
			database.OpenEx(sConnectStringDsnLess);
			// 當不存在該筆資料時插入一筆新的資料
			sSqlString.Format(	L"INSERT INTO [AFDBS].[dbo].[行政部門.資訊室.0250-4-A0-0101.資訊資產服務狀況回報表] "
								L"([資產代碼], [服務代碼], [最後異動時間], [最後異動_IP], [服務回報狀態]) "
								L"VALUES('%s', '%s', '%s', '%s', '%s')", AssetCode, ServiceCode, UpdateTimeYYYMMDD, LocalIP, RunStat);
			database.ExecuteSQL(sSqlString);
		}else {
			database.Close();
			database.OpenEx(sConnectStringDsnLess);
			// 當該筆資料存在時更新資料
			sSqlString.Format(	L"UPDATE [AFDBS].[dbo].[行政部門.資訊室.0250-4-A0-0101.資訊資產服務狀況回報表] "
								L"SET "
								L"   [最後異動時間] = '%s' "
								L"  ,[最後異動_IP] = '%s' "
								L"  ,[服務回報狀態] = '%s' "
								L"WHERE [資產代碼] = '%s' and [服務代碼] = '%s'",
								UpdateTimeYYYMMDD, LocalIP, RunStat, AssetCode, ServiceCode);
			database.ExecuteSQL(sSqlString);
		}
		// 關閉資料庫連線
		database.Close();
	}
	CATCH(CDBException, e) {
			database.Close();
			RunStat += e->m_strError;
			wprintf(L"[%s][Write Info To SQL Server Database error:%s]", UpdateTimeYYYMMDD.GetBuffer(), (e->m_strError.GetBuffer()));
			e->m_strError.ReleaseBuffer();
			UpdateTimeYYYMMDD.ReleaseBuffer();
	}
	END_CATCH;
}

