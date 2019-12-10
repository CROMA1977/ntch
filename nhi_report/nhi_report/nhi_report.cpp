// nhi_report.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//

#include "pch.h"
#include "nhi_report.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

char * ItemName[] = {
	"報告類別",
	"醫事機構代碼",
	"醫事類別",
	"執行年月",
	"健保卡過卡日期時間",
	"就醫類別",
	"就醫序號",
	"補卡註記",
	"身分證統一編號",
	"出生日期",
	"就醫日期",
	"治療結束日期",
	"入院年月日",
	"出院年月日",
	"醫令代碼",
	"檢驗(查)結果上傳日期時間",
	"醫囑醫師身分證號",
	"報告/病理醫師身分證號",

	"醫囑日期時間",
	"採檢/實際檢查/手術時間",
	"醫師確認報告時間",
	"檢體採檢方法/來源/類別",
	"報告序號",
	"檢驗項目名稱",

	"檢驗方法",
	"檢驗報告結果值",
	"單位",
	"參考值",
	"報告結果",
	"病理發現及診斷",

	"報告或病理發現(一)",
	"報告或病理發現(二)",
	"醫事人員身分證號",
	"報告日期時間",
	"備註",
	"檢驗(查)結果值註記",

	"依附就醫新生兒出生日期",
	"依附就醫新生兒胞胎註記",
	"參考值下限",
	"參考值上限",
	"輻射劑量",
	"報告或病理發現(三)",
	"報告或病理發現(四)",
	"報告或病理發現(五)",
	"報告或病理發現(六)",
	"報告或病理發現(七)",
	"報告或病理發現(八)",
	"實際收到報告日期時間",
	"錯誤代碼"
};
char * ErrorString[] = { "01 - 資料長度不正確",
						"02 - 資料類別錯誤",
						"03 - 代碼類別錯誤",
						"04 - 日期格式錯誤",
						"05 - 數值格式錯誤",
						"06 - 數值必須大於0",
						"07 - 必為空白",
						"08 - 數值區間錯誤",
						"09 - 小數點位數錯誤",
						"10 - 與執行年月之年月不一致",
						"11 - 新生兒胞胎註記有值，新生兒出生日期為必填",
						"12 - 新生兒出生日期有值，新生兒胞胎註記為必填",
						"13 - 本件不符合新生兒依附就醫日數規定",
						"14 - 參考值下限、參考值上限至少需擇一填寫",
						"21 - 就醫類別錯誤",
						"22 - 上傳檔名與檔案內容不符",
						"23 - 就醫序號錯誤",
						"24 - 補卡註記錯誤",
						"25 - 資料重覆",
						"26 - 與原送核補報清單資料勾稽查無該筆案件資料",
						"27 - 流水號重覆",
						"28 - 醫事機構主檔無此代碼",
						"30 - 醫令代碼報告類別錯誤",
						"31 - 採檢 / 實際檢查 / 手術日期時間與醫令執行時間起不符",
						"37 - 代碼檔中無此代碼或未於生效起迄日內",
						"A1 - 條碼長度錯誤",
						"A2 - 條碼數值格式錯誤",
						"A3 - 條碼英數字格式錯誤",
						"A4 - 條碼日期格式錯誤",
						"A5 - 條碼日期小於費用年月",
						"A6 - GS1條碼含符號",
						"B1 - 條碼長度錯誤",
						"B2 - 條碼數值格式錯誤",
						"B3 - 條碼英數字格式錯誤",
						"B4 - 條碼日期格式錯誤",
						"B5 - 條碼日期小於費用年月",
						"B6 - 無效期條碼",
						"B7 - HIBCC編碼錯誤",
						"C1 - 條碼長度錯誤",
						"C2 - 條碼英數字格式錯誤",
						"C3 - 請確認資料含REF、效期、LOT。舉例REF「06 - 2805」、效期「2019 - 11」、LOT「MNPYX2」，則上傳資訊為06 - 28051911MNPYX2",
						"D1 - 非GS1 / HIBSS / 廠商編碼",
						"D2 - 品項數超出20個" };
// 唯一一個應用程式物件

CWinApp theApp;

using namespace std;
// 讀取單行資料並去除掉換行符號
int GetDataLine(FILE * fptr, char * Buffer) {
	fgets(Buffer, 10240, fptr);
	int len = strlen(Buffer);
	if (len > 0) {
		len--;
		if (Buffer[len] == 10)
			Buffer[len] = NULL;
	}
	return len;
}

int LineDataNumber = 0;
int DataRange[128][2];
// 計算資料行的區域
void Pass1(void) {
	FILE * fptrR = NULL;
	fopen_s(&fptrR, "0131020016-1081122-5-1081122432351-7.assaychk.txt", "rt");
	int len;	
	char Buffer[10240], *ptr;
	while (!feof(fptrR)) {
		// 取出單行資料
		len = GetDataLine(fptrR, Buffer);
		// 檢查是否為資料區起點
		if ((ptr = strstr(Buffer, "  ==")) != NULL) {
			ptr += 3;
			if (strstr(ptr, " ") == NULL)
				continue;
			bool IsDataRange = false;
			int index = 0;
			// 檢查第一筆是否為起點
			if (Buffer[0] == '=') {
				IsDataRange = true;
				DataRange[index][0] = 0;
			}
			// 尋找每一個區段的位置
			for (int i = 1; i < len; i++) {
				if (IsDataRange == true) {
					if (Buffer[i] != '=') {
						IsDataRange = false;
						DataRange[index++][1] = i - 1;
					}
				}
				if (IsDataRange == false) {
					if (Buffer[i] == '=') {
						IsDataRange = true;
						DataRange[index][0] = i;
					}
				}
			}
			LineDataNumber = index;
			return;
		}
	}
}
void Pass2(void) {
	FILE * fptrR = NULL;
	fopen_s(&fptrR, "0131020016-1081122-5-1081122432351-7.assaychk.txt", "rt");
	FILE * fptrW = NULL;
	fopen_s(&fptrW, "PASS2.txt", "wt");

	int i = 0;
	bool begin = false;
	char Buffer[10240];

	while (!feof(fptrR)) {
		fgets(Buffer, 10240, fptrR);
		if (begin == false) {
			if (strstr(Buffer, "  程式代號:") == NULL) 
				continue;
			begin = true;
		}
		if (strstr(Buffer, "  程式代號:") != NULL) {
			while (!feof(fptrR)) {
				fgets(Buffer, 10240, fptrR);
				if (strstr(Buffer, "  =================== ") != NULL) {
					fgets(Buffer, 10240, fptrR);
					break;
				}
			}
		}
		if (Buffer[0] == 12) {
			fgets(Buffer, 10240, fptrR);
			if ( Buffer[0] == 10) {
				continue;
			}
		}

		fputs(Buffer, fptrW);
	};
	fclose(fptrR);
	fclose(fptrW);
}
void Pass3(void) {
	FILE * fptrR = NULL;
	fopen_s(&fptrR, "PASS2.txt", "rt");
	FILE * fptrW0 = NULL;
	fopen_s(&fptrW0, "PASS3.txt", "wt");
	FILE * fptrW1 = NULL;
	fopen_s(&fptrW1, "PASS3-Error.txt", "wt");

	int index = 0;
	char Buffer[20][10240];
	while (!feof(fptrR)) {
		GetDataLine(fptrR, Buffer[index]);
		// 當讀取到換行符號
		if (strcmp(Buffer[index], "-------------------------------------------------------------------------------------------------------------------------------------") == NULL) {
			// 如果不等於 9 筆資料則輸入到錯誤資料表
			if (index != 9) {
				for (int i = 0; i <= index; i++) {
					fprintf(fptrW1, "%s\n", Buffer[i]);
				}
			}
			else 
			{
				for (int i = 0; i <= index; i++) {
					fprintf(fptrW0, "%s\n", Buffer[i]);
				}
			}
			index = 0;
			continue;
		}
		index++;
		if (index == 20) {
			fclose(fptrR);
			fclose(fptrW0);
			fclose(fptrW1);

			return;
		}
	}
	fclose(fptrR);
	fclose(fptrW0);
	fclose(fptrW1);
}

class DataSet {
public:
	bool m_FlagError;
	char m_DataSet[32];

	DataSet() {
		m_FlagError = false;
		m_DataSet[0] = 0;
	}
	void SetData(char * Buffer, int start, int end) {
		if (Buffer[start] == '*') {
			m_FlagError = true;
		}
		else 
		{
			m_FlagError = false;
		}
		char * ptr = m_DataSet;
		for (int i = start + 1; i <= end; i++) {
			if (Buffer[i] == ' ')
				continue;
			*ptr++ = Buffer[i];
		}
		*ptr = 0;
	}
};
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
		//wprintf(L"[%s][Write Info To SQL Server Database error:%s]", UpdateTimeYYYMMDD.GetBuffer(), (e->m_strError.GetBuffer()));
	}
	END_CATCH;
}
void Pass4(void) {
	
	DataSet Data[55];
	
	char Buffer[10][1024];

	FILE * fptrR = NULL;
	fopen_s(&fptrR, "PASS3.txt", "rt");

	CString sSqlString;
	char ErrorMessage[1024] = "";

	while (!feof(fptrR)) {
		GetDataLine(fptrR, Buffer[0]);
		GetDataLine(fptrR, Buffer[1]);
		GetDataLine(fptrR, Buffer[2]);
		GetDataLine(fptrR, Buffer[3]);
		GetDataLine(fptrR, Buffer[4]);
		GetDataLine(fptrR, Buffer[5]);
		GetDataLine(fptrR, Buffer[6]);
		GetDataLine(fptrR, Buffer[7]);
		GetDataLine(fptrR, Buffer[8]);
		GetDataLine(fptrR, Buffer[9]);

		int index = 0;
		int ErrorIndex = -1;
		// 取出資料並記錄錯誤的位置
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < LineDataNumber; j++) {
				Data[index].SetData(Buffer[i], DataRange[j][0], DataRange[j][1]);
				if (Data[index].m_FlagError == true) {
					ErrorIndex = index;
				}
				index++;
			}
		}
		for (int i = 0; i < 43; i++) {
			if (Data[48].m_DataSet[0] == ErrorString[i][0] && Data[48].m_DataSet[1] == ErrorString[i][1]) {
				sprintf_s(ErrorMessage, "%s 發生錯誤，%s", ItemName[ErrorIndex], ErrorString[i]);
				break;
			}
		}
		// 
		Data[19].m_DataSet[11] = 0;
		
		sSqlString.Format("INSERT INTO [AFDBS].[dbo].[行政部門.資訊室.特約醫事服務機構每日上傳檢驗檢查.檢核錯誤明細表]	([h1_報告類別], [h2_醫事機構代碼], [h3_醫事類別], [h4_執行年月], [h5_健保卡過卡日期時間], [h6_就醫類別], [h7_就醫序號], [h8_補卡註記], [h9_身分證統一編號], [h10_出生日期], [h11_就醫日期], [h12_治療結束日期], [h13_入院年月日], [h14_出院年月日], [h15_醫令代碼], [h16_檢驗(查)結果上傳日期時間], [h17_醫囑醫師身分證號], [h18_報告/病理醫師身分證號], [h19_醫囑日期時間], [h20_採檢/實際檢查/手術時間], [h21_醫師確認報告時間], [h22_檢體採檢方法/來源/類別], [r1_報告序號], [r2_檢驗項目名稱], [r3_檢驗方法], [r4_檢驗報告結果值], [r5_單位], [r6_參考值], [r7_報告結果], [r8-1_病理發現及診斷], [r8-2_報告或病理發現(一)], [r8-3_報告或病理發現(二)], [r9_醫事人員身分證號], [r10_報告日期時間], [r11_備註], [r12_檢驗(查)結果值註記], [h23_依附就醫新生兒出生日期], [h24_依附就醫新生兒胞胎註記], [r6-1_參考值下限], [r6-2_參考值上限], [r13_輻射劑量], [r8-4_報告或病理發現(三)], [r8-5_報告或病理發現(四)], [r8-6_報告或病理發現(五)], [r8-7_報告或病理發現(六)], [r8-8_報告或病理發現(七)], [r8-9_報告或病理發現(八)], [r14_實際收到報告日期時間], [錯誤代碼], [錯誤訊息]) VALUES('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')",
			Data[0].m_DataSet, Data[1].m_DataSet, Data[2].m_DataSet, Data[3].m_DataSet, Data[4].m_DataSet, 
			Data[5].m_DataSet, Data[6].m_DataSet, Data[7].m_DataSet, Data[8].m_DataSet, Data[9].m_DataSet, 
			Data[10].m_DataSet, Data[11].m_DataSet, Data[12].m_DataSet, Data[13].m_DataSet, Data[14].m_DataSet, 
			Data[15].m_DataSet, Data[16].m_DataSet, Data[17].m_DataSet, Data[18].m_DataSet, Data[19].m_DataSet, 
			Data[20].m_DataSet, Data[21].m_DataSet, Data[22].m_DataSet, Data[23].m_DataSet, Data[24].m_DataSet, 
			Data[25].m_DataSet, Data[26].m_DataSet, Data[27].m_DataSet, Data[28].m_DataSet, Data[29].m_DataSet, 
			Data[30].m_DataSet, Data[31].m_DataSet, Data[32].m_DataSet, Data[33].m_DataSet, Data[34].m_DataSet, 
			Data[35].m_DataSet, Data[36].m_DataSet, Data[37].m_DataSet, Data[38].m_DataSet, Data[39].m_DataSet, 
			Data[40].m_DataSet, Data[41].m_DataSet, Data[42].m_DataSet, Data[43].m_DataSet, Data[44].m_DataSet, 
			Data[45].m_DataSet, Data[46].m_DataSet, Data[47].m_DataSet, Data[48].m_DataSet, ErrorMessage );
		SendToDatabase(sSqlString);
	}
	
}
void Pass5(void) {
	DataSet Data[55];

	char Buffer[16][1024];

	FILE * fptrR = NULL;
	fopen_s(&fptrR, "PASS3-Error.txt", "rt");

	FILE * fptrW0 = NULL;
	fopen_s(&fptrW0, "PASS4.txt", "wt");
	


	CString sSqlString;
	char ErrorMessage[1024] = "";

	while (!feof(fptrR)) {
		int LineNumber = -1;
		for (int i = 0; i < 32; i++) {
			GetDataLine(fptrR, Buffer[i]);
			if (strcmp(Buffer[i], "-------------------------------------------------------------------------------------------------------------------------------------") == NULL) {
				LineNumber = i;
				break;
			}
		}
		if (LineNumber == -1)
			continue;
		for (int i = 0; i < 55; i++) {
			Data[i].m_FlagError = false;
			strcpy(Data[i].m_DataSet, "無法讀取的訊息");
		}

		int index = 0;
		int ErrorIndex = -1;
		// 取出資料並記錄錯誤的位置
		for (int i = 0; i <= 9; i++) {
			for (int j = 0; j < LineDataNumber; j++) {
				Data[index].SetData(Buffer[i], DataRange[j][0], DataRange[j][1]);
				if (Data[index].m_FlagError == true) {
					ErrorIndex = index;
					goto point;
				}
				index++;
			}
		}
point:
		Data[48].SetData(Buffer[LineNumber - 1], DataRange[0][0], DataRange[0][1]);
		for (int i = 0; i < 43; i++) {
			if (Data[48].m_DataSet[0] == ErrorString[i][0] && Data[48].m_DataSet[1] == ErrorString[i][1]) {
				sprintf_s(ErrorMessage, "%s 發生錯誤，%s", ItemName[ErrorIndex], ErrorString[i]);
				break;
			}
		}
		Data[19].m_DataSet[11] = 0;

		sSqlString.Format("INSERT INTO [AFDBS].[dbo].[行政部門.資訊室.特約醫事服務機構每日上傳檢驗檢查.檢核錯誤明細表]	([h1_報告類別], [h2_醫事機構代碼], [h3_醫事類別], [h4_執行年月], [h5_健保卡過卡日期時間], [h6_就醫類別], [h7_就醫序號], [h8_補卡註記], [h9_身分證統一編號], [h10_出生日期], [h11_就醫日期], [h12_治療結束日期], [h13_入院年月日], [h14_出院年月日], [h15_醫令代碼], [h16_檢驗(查)結果上傳日期時間], [h17_醫囑醫師身分證號], [h18_報告/病理醫師身分證號], [h19_醫囑日期時間], [h20_採檢/實際檢查/手術時間], [h21_醫師確認報告時間], [h22_檢體採檢方法/來源/類別], [r1_報告序號], [r2_檢驗項目名稱], [r3_檢驗方法], [r4_檢驗報告結果值], [r5_單位], [r6_參考值], [r7_報告結果], [r8-1_病理發現及診斷], [r8-2_報告或病理發現(一)], [r8-3_報告或病理發現(二)], [r9_醫事人員身分證號], [r10_報告日期時間], [r11_備註], [r12_檢驗(查)結果值註記], [h23_依附就醫新生兒出生日期], [h24_依附就醫新生兒胞胎註記], [r6-1_參考值下限], [r6-2_參考值上限], [r13_輻射劑量], [r8-4_報告或病理發現(三)], [r8-5_報告或病理發現(四)], [r8-6_報告或病理發現(五)], [r8-7_報告或病理發現(六)], [r8-8_報告或病理發現(七)], [r8-9_報告或病理發現(八)], [r14_實際收到報告日期時間], [錯誤代碼], [錯誤訊息]) VALUES('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')",
			Data[0].m_DataSet, Data[1].m_DataSet, Data[2].m_DataSet, Data[3].m_DataSet, Data[4].m_DataSet,
			Data[5].m_DataSet, Data[6].m_DataSet, Data[7].m_DataSet, Data[8].m_DataSet, Data[9].m_DataSet,
			Data[10].m_DataSet, Data[11].m_DataSet, Data[12].m_DataSet, Data[13].m_DataSet, Data[14].m_DataSet,
			Data[15].m_DataSet, Data[16].m_DataSet, Data[17].m_DataSet, Data[18].m_DataSet, Data[19].m_DataSet,
			Data[20].m_DataSet, Data[21].m_DataSet, Data[22].m_DataSet, Data[23].m_DataSet, Data[24].m_DataSet,
			Data[25].m_DataSet, Data[26].m_DataSet, Data[27].m_DataSet, Data[28].m_DataSet, Data[29].m_DataSet,
			Data[30].m_DataSet, Data[31].m_DataSet, Data[32].m_DataSet, Data[33].m_DataSet, Data[34].m_DataSet,
			Data[35].m_DataSet, Data[36].m_DataSet, Data[37].m_DataSet, Data[38].m_DataSet, Data[39].m_DataSet,
			Data[40].m_DataSet, Data[41].m_DataSet, Data[42].m_DataSet, Data[43].m_DataSet, Data[44].m_DataSet,
			Data[45].m_DataSet, Data[46].m_DataSet, Data[47].m_DataSet, Data[48].m_DataSet, ErrorMessage);

		SendToDatabase(sSqlString);
	}
	fclose(fptrR);
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
			Pass1();
			Pass2();
			Pass3();
			Pass4();
			Pass5();
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
