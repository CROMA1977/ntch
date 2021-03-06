// NtchLib.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//

#include "pch.h"
#include "NtchLib.h"
#include "CDatabaseNtch.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class CTestSql : public CSqlOrder {
public:
	CTestSql();
	virtual ~CTestSql();
	virtual CString GetSqlOrder() {
		CString sSqlString;
		CTimeNtch NowTime;
		NowTime.SetCurrentTime();
		CString TimeString = NowTime.GetDateStringC3YMMDD();

		sSqlString.Format(L"select b.area, count(*) "
			L"from "
			L"fpr005k a, fth007k b "
			L"where a.sdate >= '%s' "
			L"and a.dept = b.dept "
			L"and (a.status <> '3' or a.status is null) "
			L"and a.source = '2' "
			L"and (a.see_doct = '' or a.see_doct is null) group by b.area", TimeString);
		return sSqlString;
	}
	virtual CString SqlOrderProcFunc(CRecordset & Recordset) {
		// 取出各欄位的資訊
		CString sArea, sCount;
		// Loop through each record
		while (!Recordset.IsEOF()) {
			Recordset.GetFieldValue(L"AREA", sArea);
			Recordset.GetFieldValue(L"COUNT(*)", sCount);
			Recordset.MoveNext();
		}
		return L"A";
	}
};


// 唯一一個應用程式物件

CWinApp theApp;

using namespace std;

int main()
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
        }
		else
		{
			// TODO: 在此撰寫應用程式行為的程式碼。
			CTimeNtch NowTime;
			NowTime.SetCurrentTime();
			CString TimeString = NowTime.GetDateStringC3YMMDD();
			CString sSqlString;
			CDatabaseNtchDsnLess Database;

			Database.Assign(DT_Oracle, L"health", L"q0004", L"q0004", L"10.35.250.150");			
			//Database.AssignOracle();
			Database.OpenDatabase();
			CTestSql a;
			Database.ExecuteSQL(a);
        }	
    }
    else
    {
        // TODO: 配合您的需要變更錯誤碼
        wprintf(L"嚴重錯誤: GetModuleHandle 失敗\n");
        nRetCode = 1;
    }

    return nRetCode;
}
