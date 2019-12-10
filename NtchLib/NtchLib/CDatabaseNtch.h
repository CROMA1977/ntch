#pragma once
#include <afxdb.h>
#include <atlconv.h>
#include <odbcinst.h>

typedef CString(* SqlSelectProcFunc)(CRecordset &);							///< �w�q Select SQL �T���B�z��ƪ����A
//CRecordset recset(&Database);
//recset.Open(CRecordset::forwardOnly, sSqlString, CRecordset::readOnly);

class CSqlOrder {
public:
																			 CSqlOrder() {};
	virtual																	~CSqlOrder() {};

	virtual CString															 GetSqlOrder(void) = 0;
	virtual	CString															 SqlOrderProcFunc(CRecordset & Recordset) = 0;
};

class CDatabaseNtchDsn : public CDatabase {
public:
			bool															 m_StandbySetup;
			CString															 m_DSN;
			CString															 m_UID;
			CString															 m_PWD;

																			 CDatabaseNtchDsn();
	virtual																	~CDatabaseNtchDsn();

			void															 Assign(CString DSN, CString UID, CString PWD);
			void															 AssignOracle(void);
			void															 AssignMsServer(void);
			void 															 AssignSybase(void);

	virtual BOOL															 OpenDatabase(void);
	static	void															 DisplayErrorMsg(CDatabaseNtchDsn * Ptr, CString Msg);
};

enum DatabaseType {DT_Oracle, DT_MS_Server, DT_Sybase};
class CDatabaseNtchDsnLess : public CDatabase {
public:
			bool															 m_StandbySetup;
			DatabaseType													 m_DatabaseType;
			CString															 m_IP;
			CString															 m_Port;
			CString															 m_DatabaseName;
			CString															 m_UID;
			CString															 m_PWD;

	typedef CString															 (*GetConnectStringFunc)(CDatabaseNtchDsnLess *);	 ///< �w�q��ƫ��Ъ����A
			GetConnectStringFunc											 m_Func;											 ///< �ŧi��ƫ���

			CDatabaseNtchDsnLess();
	virtual																	~CDatabaseNtchDsnLess();

			void															 Assign(DatabaseType Type, CString DatabaseName, CString UID, CString PWD, CString IP, CString Port = L"");
			void															 AssignOracle(void);
			void															 AssignMsServer(void);
			void 															 AssignSybase(void);


	virtual BOOL															 OpenDatabase(void);
			CString															 ExecuteSQL(CSqlOrder & Order);

	static  CString															 GetConnectStringForOracle(CDatabaseNtchDsnLess * Ptr);
	static  CString															 GetConnectStringForMsServer(CDatabaseNtchDsnLess * Ptr);
	static  CString															 GetConnectStringForSybase(CDatabaseNtchDsnLess * Ptr);
	static	void															 DisplayErrorMsg(CDatabaseNtchDsnLess * Ptr, CString Msg);

};

class CTimeNtch : public CTime {
public:
																			 CTimeNtch();
	//																		 CTimeNtch(const CTime & Obj);
																			 CTimeNtch(const CTimeNtch & Obj);
	virtual																	~CTimeNtch();

			void															 SetCurrentTime(void);
			void															 SetTimeC3YMMDD(CString TimeString);			///< ���� YYY  �~�r��
			void															 SetTimeC4YMMDD(CString TimeString);			///< ���� YYYY �~�r�� �Ĥ@�X�� 1 �N�����e
			CString															 GetDateStringC3YMMDD(void);
			CString															 GetDateStringC4YMMDD(void);
			CTimeNtch &														 operator = (const CTimeNtch &);
};