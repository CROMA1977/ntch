// Ping.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//

#include "pch.h"
#include "Ping.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CTime CurrentTime;							///< 現在時間資料
CString RunStat;							///< 執行狀態
CString RunStatNow;							///< 執行狀態
CString UpdateTimeYYYMMDD;					///< 上傳資料的時間 YYYYMMDDHHMM
CString LocalIP;							///< 本機 IP

struct Device {
	bool m_StateConnect;
	char m_IP[17];
	char m_name[256];
};
std::vector<Device> DeviceList;


/*
CString UpdateTime;							///< 上傳資料的時間
CString TodayYYYMMDD;						///< 今天日期
CString OneDayAgoYYYMMDD;					///< 一天前的日期
CString TwoDayAgoYYYMMDD;					///< 兩天前的日期
CString BedChangTime;						///< 滿床位起始時間 YYYYMMDDHHMM
CString UpdateTimeYYYMMDD;					///< 上傳資料的時間 YYYYMMDDHHMM

CTime OneDaysAgo;							///< 一天前的時間資料
CTime TwoDaysAgo;							///< 兩天前的時間資料
*/

void UpdateConnectState(void) {
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

	// 取得現在的日期字串 格式(民國) YYYMMDD
	CurrentTime = CTime::GetCurrentTime();
	// 取得更新的時間字串 格式 YYYYMMDDHHMM YYY-MM-DD HH:MM:00.000
	UpdateTimeYYYMMDD.Format(L"%04d-%02d-%02d %02d:%02d:00.000", CurrentTime.GetYear(), CurrentTime.GetMonth(), CurrentTime.GetDay(), CurrentTime.GetHour(), CurrentTime.GetMinute());

	CDatabase database;
	FILE * fptr = NULL;
	TRY{
		database.OpenEx(sConnectStringDsnLess);
		char CharW[1024];
		CString DeviceName, DeviceIP, ErrorTime;
		// 檢查每個設備
		for (int i = 0; i < DeviceList.size(); i++) {
			// 當連結失敗時
			if (DeviceList[i].m_StateConnect == false) {
				int cchWideChar;
				cchWideChar = MultiByteToWideChar(CP_ACP, 0, DeviceList[i].m_IP, -1, NULL, 0);
				PTCHAR ptszTextIP = NULL;
				ptszTextIP = new TCHAR[cchWideChar];
				MultiByteToWideChar(CP_ACP, 0, DeviceList[i].m_IP, -1, ptszTextIP, cchWideChar);

				cchWideChar = MultiByteToWideChar(CP_ACP, 0, DeviceList[i].m_name, -1, NULL, 0);
				PTCHAR ptszTextSwitchName = NULL;
				ptszTextSwitchName = new TCHAR[cchWideChar];
				MultiByteToWideChar(CP_ACP, 0, DeviceList[i].m_name, -1, ptszTextSwitchName, cchWideChar);

				sSqlString.Format(L"INSERT INTO [AFDBS].[dbo].[行政部門.資訊室.網路設備資產異常通報表]([設備名稱], [IP], [異常通報時間]) VALUES('%s', '%s', '%s')", ptszTextSwitchName, ptszTextIP, UpdateTimeYYYMMDD);
				database.ExecuteSQL(sSqlString);
				delete[] ptszTextIP;
				delete[] ptszTextSwitchName;

				fopen_s(&fptr, "ErrorLog.txt", "at");
				fprintf(fptr, "\n%04d-%02d-%02d %02d:%02d:%s %s", CurrentTime.GetYear(), CurrentTime.GetMonth(), CurrentTime.GetDay(), CurrentTime.GetHour(), CurrentTime.GetMinute(), DeviceList[i].m_IP, DeviceList[i].m_name);
				fclose(fptr);
			}
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

void UpdateRunStat(void) {
	// 資產代碼與服務代碼(重要)
	CString AssetCode = L"2019110801";
	CString ServiceCode = L"0001";
	
	// 取得現在的日期字串 格式(民國) YYYMMDD
	CurrentTime = CTime::GetCurrentTime();
	// 取得更新的時間字串 格式 YYYYMMDDHHMM YYY-MM-DD HH:MM:00.000
	UpdateTimeYYYMMDD.Format(L"%04d-%02d-%02d %02d:%02d:00.000", CurrentTime.GetYear(), CurrentTime.GetMonth(), CurrentTime.GetDay(), CurrentTime.GetHour(), CurrentTime.GetMinute());

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

		CString ReportStat = RunStat.Left(50);
		
		CRecordset recset(&database);
		recset.Open(CRecordset::forwardOnly, sSqlString, CRecordset::readOnly);

		// 檢查資料是否存在
		if (recset.IsEOF() == TRUE) {
			database.Close();
			database.OpenEx(sConnectStringDsnLess);
			// 當不存在該筆資料時插入一筆新的資料
			sSqlString.Format(	L"INSERT INTO [AFDBS].[dbo].[行政部門.資訊室.0250-4-A0-0101.資訊資產服務狀況回報表] "
								L"([資產代碼], [服務代碼], [最後異動時間], [最後異動_IP], [服務回報狀態]) "
								L"VALUES('%s', '%s', '%s', '%s', '%s')", AssetCode, ServiceCode, UpdateTimeYYYMMDD, LocalIP, ReportStat);
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
								UpdateTimeYYYMMDD, LocalIP, ReportStat, AssetCode, ServiceCode);
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

// 初始化環境 
// 開啟檔案讀取設備設定 並 更新本地端 IP
int Setup(void) {
	// 讀取設備設定檔
	FILE *fptr;
	fopen_s(&fptr, "DeviceList.txt", "rt");
	Device in;
	int i;
	do {
		fgets(in.m_IP, 17, fptr);
		i = strlen(in.m_IP);
		while ((in.m_IP[i - 1] == 0x09 || in.m_IP[i - 1] == 0x0a) && i-- > 0);
		in.m_IP[i] = '\0';

		fgets(in.m_name, 256, fptr);
		i = strlen(in.m_name);
		while ((in.m_name[i - 1] == 0x09 || in.m_name[i - 1] == 0x0a) && i-- > 0);
		in.m_name[i] = '\0';
		in.m_StateConnect = false;
		DeviceList.push_back(in);
	} while (!feof(fptr));
	fclose(fptr);

	// 取得本地端 IP
	char szBuffer[1024];
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 0);
	if (::WSAStartup(wVersionRequested, &wsaData) != 0)
		return 0;
	if (gethostname(szBuffer, sizeof(szBuffer)) == SOCKET_ERROR) {
		WSACleanup();
		return 0;
	}

	struct hostent *host = gethostbyname(szBuffer);
	if (host == NULL) {
		WSACleanup();
		return 0;
	}
	LocalIP = inet_ntoa(*(struct in_addr *)*host->h_addr_list);
	return 1;
}

// 測試指定編號的設備連線之狀態
int Ping(int index) {
	// 建立連線環境
	HANDLE hIcmpFile;
	unsigned long ipaddr = INADDR_NONE;
	DWORD dwRetVal = 0;
	char SendData[32] = "Data Buffer";
	LPVOID ReplyBuffer = NULL;
	DWORD ReplySize = 0;

	ipaddr = inet_addr(DeviceList[index].m_IP);
	if (ipaddr == INADDR_NONE) {
		printf("usage: %s IP address\n", DeviceList[index].m_IP);
		return 1;
	}

	hIcmpFile = IcmpCreateFile();
	if (hIcmpFile == INVALID_HANDLE_VALUE) {
		printf("\tUnable to open handle.\n");
		printf("IcmpCreatefile returned error: %ld\n", GetLastError());
		return 1;
	}

	ReplySize = sizeof(ICMP_ECHO_REPLY) + sizeof(SendData);
	ReplyBuffer = (VOID*)malloc(ReplySize);
	if (ReplyBuffer == NULL) {
		printf("\tUnable to allocate memory\n");
		return 1;
	}

	// 取得現在的日期字串 格式(民國) YYYMMDD
	CurrentTime = CTime::GetCurrentTime();
	// 取得更新的時間字串 格式 YYYYMMDDHHMM YYY-MM-DD HH:MM:00.000
	UpdateTimeYYYMMDD.Format(L"%04d-%02d-%02d %02d:%02d:00.000", CurrentTime.GetYear(), CurrentTime.GetMonth(), CurrentTime.GetDay(), CurrentTime.GetHour(), CurrentTime.GetMinute());

	// 執行線路測試
	dwRetVal = IcmpSendEcho(hIcmpFile, ipaddr, SendData, sizeof(SendData), NULL, ReplyBuffer, ReplySize, 1000);
	// 檢查連線狀態
	if (dwRetVal != 0) {
		// 連線成功 顯示訊息
		DeviceList[index].m_StateConnect = true;
		PICMP_ECHO_REPLY pEchoReply = (PICMP_ECHO_REPLY)ReplyBuffer;
		struct in_addr ReplyAddr;
		ReplyAddr.S_un.S_addr = pEchoReply->Address;
		printf("Sent %5s icmp message to %16s\n", DeviceList[index].m_name, DeviceList[index].m_IP);
		if (dwRetVal > 1) {
			printf("Received %ld icmp message responses ", dwRetVal);
			printf("Information from the first response:\n");
		} else {
			printf("Received %ld icmp message response ", dwRetVal);
			printf("Information from this response:\n");
		}
		printf("  Received from %s ", inet_ntoa(ReplyAddr));
		printf("Status = %ld ", pEchoReply->Status);
		printf("Roundtrip time = %ld milliseconds \n", pEchoReply->RoundTripTime);
	} else {
		// 連線失敗 更新錯誤訊息並顯示 等待五秒後繼續執行
		DeviceList[index].m_StateConnect = false;
		int cchWideChar;
		cchWideChar = MultiByteToWideChar(CP_ACP, 0, DeviceList[index].m_IP, -1, NULL, 0);
		PTCHAR ptszTextIP = NULL;
		ptszTextIP = new TCHAR[cchWideChar];
		MultiByteToWideChar(CP_ACP, 0, DeviceList[index].m_IP, -1, ptszTextIP, cchWideChar);

		cchWideChar = MultiByteToWideChar(CP_ACP, 0, DeviceList[index].m_name, -1, NULL, 0);
		PTCHAR ptszTextSwitchName = NULL;
		ptszTextSwitchName = new TCHAR[cchWideChar];
		MultiByteToWideChar(CP_ACP, 0, DeviceList[index].m_name, -1, ptszTextSwitchName, cchWideChar);

		CString ErrorString;
		printf("Call %s to IcmpSendEcho %s failed. ", DeviceList[index].m_name, DeviceList[index].m_IP);
		printf("IcmpSendEcho returned error: %ld\n", GetLastError());		
		ErrorString.Format(L"ping %s ip:%s fail. ", ptszTextSwitchName, ptszTextIP);
		RunStatNow += ErrorString;
		delete[] ptszTextIP;
		delete[] ptszTextSwitchName;
		free(ReplyBuffer);
		Sleep(5000);
		return 1;
	}
	free(ReplyBuffer);
	return 0;
}

// 唯一一個應用程式物件

CWinApp theApp;

using namespace std;

int main(int argc, char **argv)
{
    int nRetCode = 0;

    HMODULE hModule = ::GetModuleHandle(nullptr);

    if (hModule != nullptr)
    {
        // 將 MFC 初始化並於失敗時列印錯誤
        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            // TODO: 在此撰寫應用程式行為的程式碼。
            wprintf(L"嚴重錯誤: MFC 初始化失敗\n");
            nRetCode = 1;
		} else {
			// 程式起點
			// 初始化環境
			Setup();	
			// 無窮迴圈
			RunStat = L"無異常";
			int Count = 0;
			while (1) {
				// 根據設備清單輪巡各項設備是否回應 間隔 500ms 
				RunStatNow = L"";
				for (int i = 0; i < DeviceList.size(); i++) {
					// 測試指定編號的設備連線之狀態
					Ping(i);
					Sleep(500);
				}		
				// 當現在設備回應狀態  等於空字串 代表無異常 向資料庫更新執行狀態
				// 當現在設備回應狀態不等於空字串 代表異常狀態
				// 異常狀態時則檢查上一個執行狀態(RunStat) == 無異常 代表在發生邊界 向資料庫更新執行狀態
				// 異常狀態時則檢查上一個執行狀態(RunStat) != 無異常 代表已經向資料庫更新過數據 將放置直到系統逾期產生告警
				if (RunStatNow != L"") {
					if (RunStat == L"無異常") {
						RunStat = RunStatNow;
						UpdateRunStat();				
					} 
					if (Count++ > 50) {
						UpdateConnectState();
					}
				} else {
					RunStat = L"無異常";
					UpdateRunStat();
					Count = 0;
				}
			}
		}
    } else {
        // TODO: 配合您的需要變更錯誤碼
        wprintf(L"嚴重錯誤: GetModuleHandle 失敗\n");
        nRetCode = 1;
    }
    return nRetCode;
}
