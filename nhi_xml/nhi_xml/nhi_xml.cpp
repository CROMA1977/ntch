// nhi_xml.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//

#include "pch.h"
#include "nhi_xml.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 唯一一個應用程式物件

CWinApp theApp;

using namespace std;

void SendToDatabase(CString & sSqlString) {

	// 進行資料庫資料更新作業
	CString sConnectStringDsn, sConnectStringDsnLess;
	// 設定資料庫連線的相關參數
	CString sDriver = "SQL Server";
	CString sServer = "192.168.255.15";
	CString sDsn = "WebPageUpdater";
	CString sUID = "sa";
	CString sPWD = "Pa55w0rd";

	// 產生資料庫的連線字串 DSN or DSN LESS
	sConnectStringDsn.Format("DSN=%s;UID=%s;PWD=%s", sDsn, sUID, sPWD);
	sConnectStringDsnLess.Format("DRIVER={%s};SERVER=%s;UID=%s;PWD=%s;", sDriver, sServer, sUID, sPWD);

	CDatabase database;
	TRY{
		// 連線資料庫 DSN or DSN LESS
		database.OpenEx(sConnectStringDsnLess);
		database.ExecuteSQL(sSqlString);
		// 關閉資料庫連線
		database.Close();
	}
		CATCH(CDBException, e) {
		database.Close();

	}
	END_CATCH;
}

MSXML2::IXMLDOMDocument2Ptr	m_plDomDocument;
MSXML2::IXMLDOMElementPtr	m_pDocRoot;

MSXML2::IXMLDOMNodePtr findChild(MSXML2::IXMLDOMNodePtr Node, CString nodeName) {
	for (MSXML2::IXMLDOMNodePtr pChild = Node->firstChild; pChild != NULL; pChild = pChild->nextSibling) {
		if( nodeName == (LPCTSTR)pChild->nodeName)
			return pChild;
	}
	return NULL;
}

CString ReadHdata(MSXML2::IXMLDOMNodePtr Node) {
	CString strElement = ((LPCTSTR)Node->nodeName);

	MSXML2::IXMLDOMNodePtr ObjHdata[25];	
	ObjHdata[1] = findChild(Node, "h1");
	ObjHdata[2] = findChild(Node, "h2");
	ObjHdata[3] = findChild(Node, "h3");
	ObjHdata[4] = findChild(Node, "h4");
	ObjHdata[5] = findChild(Node, "h5");
	ObjHdata[6] = findChild(Node, "h6");
	ObjHdata[7] = findChild(Node, "h7");
	ObjHdata[8] = findChild(Node, "h8");
	ObjHdata[9] = findChild(Node, "h9");
	ObjHdata[10] = findChild(Node, "h10");
	ObjHdata[11] = findChild(Node, "h11");
	ObjHdata[12] = findChild(Node, "h12");
	ObjHdata[13] = findChild(Node, "h13");
	ObjHdata[14] = findChild(Node, "h14");
	ObjHdata[15] = findChild(Node, "h15");
	ObjHdata[16] = findChild(Node, "h16");
	ObjHdata[17] = findChild(Node, "h17");
	ObjHdata[18] = findChild(Node, "h18");
	ObjHdata[19] = findChild(Node, "h19");
	ObjHdata[20] = findChild(Node, "h20");
	ObjHdata[21] = findChild(Node, "h21");
	ObjHdata[22] = findChild(Node, "h22");
	ObjHdata[23] = findChild(Node, "h23");
	ObjHdata[24] = findChild(Node, "h24");

	CString StringHdata[25];
	for (int i = 1; i < 25; i++) {
		if (ObjHdata[i] == NULL) {
			StringHdata[i] = "無此項資料";
		}
		else 
		{
			StringHdata[i] = ((LPCTSTR)ObjHdata[i]->text);
		}
	}

	MSXML2::IXMLDOMNodePtr ObjRawdata = findChild(Node, "rdata");
	MSXML2::IXMLDOMNodePtr ObjRawdataArray[15];
	ObjRawdataArray[1] = findChild(ObjRawdata, "r1");
	ObjRawdataArray[2] = findChild(ObjRawdata, "r2");
	ObjRawdataArray[3] = findChild(ObjRawdata, "r3");
	ObjRawdataArray[4] = findChild(ObjRawdata, "r4");
	ObjRawdataArray[5] = findChild(ObjRawdata, "r5");
	ObjRawdataArray[6] = findChild(ObjRawdata, "r6");
	ObjRawdataArray[7] = findChild(ObjRawdata, "r7");
	ObjRawdataArray[8] = findChild(ObjRawdata, "r8");
	ObjRawdataArray[9] = findChild(ObjRawdata, "r9");
	ObjRawdataArray[10] = findChild(ObjRawdata, "r10");
	ObjRawdataArray[11] = findChild(ObjRawdata, "r11");
	ObjRawdataArray[12] = findChild(ObjRawdata, "r12");
	ObjRawdataArray[13] = findChild(ObjRawdata, "r13");
	ObjRawdataArray[14] = findChild(ObjRawdata, "r14");

	CString StringRawdata[15];
	for (int i = 1; i < 15; i++) {
		if (ObjRawdataArray[i] == NULL) {
			StringRawdata[i] = "無此項資料";
		}
		else
		{
			StringRawdata[i] = ((LPCTSTR)ObjRawdataArray[i]->text);
		}
	}

	MSXML2::IXMLDOMNodePtr ObjRawdataR6[3];
	ObjRawdataR6[1] = findChild(ObjRawdata, "r6-1");
	ObjRawdataR6[2] = findChild(ObjRawdata, "r6-2");

	CString StringRawdataR6[3];
	for (int i = 1; i < 3; i++) {
		if (ObjRawdataR6[i] == NULL) {
			StringRawdataR6[i] = "無此項資料";
		}
		else
		{
			StringRawdataR6[i] = ((LPCTSTR)ObjRawdataR6[i]->text);
		}
	}

	MSXML2::IXMLDOMNodePtr ObjRawdataR8[10];
	ObjRawdataR8[1] = findChild(ObjRawdata, "r8-1");
	ObjRawdataR8[2] = findChild(ObjRawdata, "r8-2");
	ObjRawdataR8[3] = findChild(ObjRawdata, "r8-3");
	ObjRawdataR8[4] = findChild(ObjRawdata, "r8-4");
	ObjRawdataR8[5] = findChild(ObjRawdata, "r8-5");
	ObjRawdataR8[6] = findChild(ObjRawdata, "r8-6");
	ObjRawdataR8[7] = findChild(ObjRawdata, "r8-7");
	ObjRawdataR8[8] = findChild(ObjRawdata, "r8-8");
	ObjRawdataR8[9] = findChild(ObjRawdata, "r8-9");

	CString StringRawdataR8[10];
	for (int i = 1; i < 10; i++) {
		if (ObjRawdataR8[i] == NULL) {
			StringRawdataR8[i] = "無此項資料";
		}
		else
		{
			StringRawdataR8[i] = ((LPCTSTR)ObjRawdataR8[i]->text);
		}
	}

	CString sSqlString;
	sSqlString.Format("INSERT INTO [AFDBS].[dbo].[行政部門.資訊室.特約醫事服務機構每日上傳檢驗檢查.上傳資料清單]	([h1_報告類別], [h2_醫事機構代碼], [h3_醫事類別], [h4_執行年月], [h5_健保卡過卡日期時間], [h6_就醫類別], [h7_就醫序號], [h8_補卡註記], [h9_身分證統一編號], [h10_出生日期], [h11_就醫日期], [h12_治療結束日期], [h13_入院年月日], [h14_出院年月日], [h15_醫令代碼], [h16_檢驗(查)結果上傳日期時間], [h17_醫囑醫師身分證號], [h18_報告/病理醫師身分證號], [h19_醫囑日期時間], [h20_採檢/實際檢查/手術時間], [h21_醫師確認報告時間], [h22_檢體採檢方法/來源/類別], [r1_報告序號], [r2_檢驗項目名稱], [r3_檢驗方法], [r4_檢驗報告結果值], [r5_單位], [r6_參考值], [r7_報告結果], [r8-1_病理發現及診斷], [r8-2_報告或病理發現(一)], [r8-3_報告或病理發現(二)], [r9_醫事人員身分證號], [r10_報告日期時間], [r11_備註], [r12_檢驗(查)結果值註記], [h23_依附就醫新生兒出生日期], [h24_依附就醫新生兒胞胎註記], [r6-1_參考值下限], [r6-2_參考值上限], [r13_輻射劑量], [r8-4_報告或病理發現(三)], [r8-5_報告或病理發現(四)], [r8-6_報告或病理發現(五)], [r8-7_報告或病理發現(六)], [r8-8_報告或病理發現(七)], [r8-9_報告或病理發現(八)], [r14_實際收到報告日期時間]) VALUES('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')",
		StringHdata[1].GetBuffer(), StringHdata[2].GetBuffer(), StringHdata[3].GetBuffer(), StringHdata[4].GetBuffer(), StringHdata[5].GetBuffer(),
		StringHdata[6].GetBuffer(), StringHdata[7].GetBuffer(), StringHdata[8].GetBuffer(), StringHdata[9].GetBuffer(), StringHdata[10].GetBuffer(),
		StringHdata[11].GetBuffer(), StringHdata[12].GetBuffer(), StringHdata[13].GetBuffer(), StringHdata[14].GetBuffer(), StringHdata[15].GetBuffer(),
		StringHdata[16].GetBuffer(), StringHdata[17].GetBuffer(), StringHdata[18].GetBuffer(), StringHdata[19].GetBuffer(), StringHdata[20].GetBuffer(),
		StringHdata[21].GetBuffer(), StringHdata[22].GetBuffer(),

		StringRawdata[1].GetBuffer(), StringRawdata[2].GetBuffer(), StringRawdata[3].GetBuffer(), StringRawdata[4].GetBuffer(), StringRawdata[5].GetBuffer(),
		StringRawdata[6].GetBuffer(), StringRawdata[7].GetBuffer(),

		StringRawdataR8[1].GetBuffer(), StringRawdataR8[2].GetBuffer(), StringRawdataR8[3].GetBuffer(),

		StringRawdata[9].GetBuffer(), StringRawdata[10].GetBuffer(), StringRawdata[11].GetBuffer(), StringRawdata[12].GetBuffer(),

		StringHdata[23].GetBuffer(), StringHdata[24].GetBuffer(),

		StringRawdataR6[1].GetBuffer(), StringRawdataR6[2].GetBuffer(),

		StringRawdata[13].GetBuffer(),

		StringRawdataR8[4].GetBuffer(), StringRawdataR8[5].GetBuffer(), StringRawdataR8[6].GetBuffer(), StringRawdataR8[7].GetBuffer(), StringRawdataR8[8].GetBuffer(),
		StringRawdataR8[9].GetBuffer(),
		StringRawdata[14].GetBuffer()
	);
	SendToDatabase(sSqlString);


	return strElement;
}

void ReadXML(CString FileName) {
	_bstr_t bstrFileName;
	bstrFileName = FileName.AllocSysString();
	variant_t vResult;
	vResult = m_plDomDocument->load(bstrFileName);
	if (((bool)vResult) == TRUE) 
	{			
		MSXML2::IXMLDOMNodePtr m_pXMLRoot = m_plDomDocument->documentElement;  
		for(MSXML2::IXMLDOMNodePtr pChild = m_pXMLRoot->firstChild; pChild != NULL; pChild = pChild->nextSibling)			
		{				
			ReadHdata(pChild); 			
		}		
	}
}
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
			// intianlize XML Parser COM    
			::CoInitialize(NULL);
			HRESULT hr = m_plDomDocument.CreateInstance(MSXML2::CLSID_DOMDocument);
			if (FAILED(hr)) {
				_com_error er(hr);
				AfxMessageBox(er.ErrorMessage());
			}
			else {
				ReadXML("01310200161081122014.xml");
			}

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
