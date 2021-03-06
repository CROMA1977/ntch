#pragma once

#include "resource.h"
#include <Windns.h>
#include <atlconv.h>

void  UpdateRunTimeInfo(void);					///< 更新執行階段的資訊
void  ReadInfoForOracleDatabase(void);			///< 讀取 Oracle 資料庫的資訊
void  ReadInfoForSqlServer(void);				///< 讀取 Sql Server 資訊
void  WriteInfoToSqlServer(void);				///< 寫入 Sql Server 的資訊
void  UpdateRunStat(void);
CTime ConversionTimeFormat(CString & sTime);	///< 將時間字串轉換時間物件

CString UpdateTime;							///< 上傳資料的時間
CString TodayYYYMMDD;						///< 今天日期
CString OneDayAgoYYYMMDD;					///< 一天前的日期
CString TwoDayAgoYYYMMDD;					///< 兩天前的日期
CString BedChangTime;						///< 滿床位起始時間 YYYYMMDDHHMM
CString UpdateTimeYYYMMDD;					///< 上傳資料的時間 YYYYMMDDHHMM
CString LocalIP;							///< 本機 IP
CString RunStat;							///< 執行狀態

CTime CurrentTime;							///< 現在時間資料
CTime OneDaysAgo;							///< 一天前的時間資料
CTime TwoDaysAgo;							///< 兩天前的時間資料



//CString CurrentDirectory;					///< 執行階段目錄
//CString ConfigIniPathName;					///< 設定檔的檔案路徑全名
