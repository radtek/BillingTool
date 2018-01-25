// SmsSend.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "SmsSend.h"
#include "../../BillingTool/PluginInterface.h"
#include "../../BillingTool/ViewTree.h"
#include "../../BillingTool/ModuleContext.h"
#include "../../BillingTool/BillingTool.h"
#include <vector>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO:  ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE �����ӵ�
//		�ú�������ǰ�档
//
//		����: 
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ��  ����ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CSmsSendApp

BEGIN_MESSAGE_MAP(CSmsSendApp, CWinApp)
END_MESSAGE_MAP()


// CSmsSendApp ����

CSmsSendApp::CSmsSendApp()
{
	// TODO:  �ڴ˴����ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CSmsSendApp ����

//CSmsSendApp theApp;


// CSmsSendApp ��ʼ��

BOOL CSmsSendApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

static CString GetSysYMDTime()
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	CString result;
	result.Format(_TEXT("%04d%02d%02d"), st.wYear, st.wMonth, st.wDay);
	return result;
}


static CString GetSysTime()
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	CString result;
	result.Format(_TEXT("%04d%02d%02d%02d%02d%02d"), st.wYear, st.wMonth, st.wDay,st.wHour,st.wMinute,st.wSecond);
	return result;
}

static std::string CStringToString(const CString& src, UINT codepage)
{
	std::string dst;
	if (src.IsEmpty())
	{
		dst.clear();
		return "";
	}

	int length = ::WideCharToMultiByte(codepage, 0, src, src.GetLength(), NULL, 0, NULL, NULL);
	dst.resize(length);
	::WideCharToMultiByte(codepage, 0, src, src.GetLength(), &dst[0], (int)dst.size(), NULL, NULL);

	return dst;
}

static CString StringToCString(const std::string& src, UINT codepage)
{
	CString dst;
	if (src.empty())
	{
		return  dst;
	}
	int length = ::MultiByteToWideChar(codepage, 0, src.data(), (int)src.size(), NULL, 0);
	WCHAR* pBuffer = dst.GetBufferSetLength(length);
	::MultiByteToWideChar(codepage, 0, src.data(), (int)src.size(), pBuffer, length);

	return dst;
}

/*
#define BILL_URGING_PAY    7001 //�߷�����(��ֵ)
#define TRAFFIC_REMIND     7002 //��������->����ֵ(�ײ���)/����ֵ(�ײ���)/����ֵ(�ײ���)/����ֵ(ʹ������)
#define CONSUMPTION_REMIND 7003 //��������-->������ز������߼��жϣ�ֱ�Ӳ���ű�
#define DOUBLE_STOP        3110 //�ſ�ͣ�� -->Ŀǰ�ɷַ�����
#define SINGLE_STOP        3120 //�ſذ�ͣ�� -->�д���
#define TRAFFIC_CAP        3111 //�����ⶥ -->Ŀǰ��chengl�ַ��߼��������ɱ������Ͷ���
#define LAST_PAY_HASTEN    3200 //�󸶷Ѵ߷�-->�󸶷ѽ������ڴ���ʱ
*/
void TriggerSmsSendFile(ModuleContext *ctx, void *ptr);
void TriggerOneWayStopMsg(ModuleContext *ctx, void *ptr);
void TriggerDoubleStopMsg(ModuleContext *ctx, void *ptr);
void TriggerNotEnoughBalanceMsg(ModuleContext *ctx, void *ptr);
void TriggerDataRemindMsg(ModuleContext *ctx, void *ptr);
void TriggerDataTopMsg(ModuleContext *ctx, void *ptr);
void TriggerPostPaypMsg(ModuleContext *ctx, void *ptr);


void Initilize(CWnd *mainWnd, CViewTree *viewTree)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());


	HTREEITEM hRoot = viewTree->InsertItem(_T("��������"), 0, 0);
	viewTree->SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	HTREEITEM tmpItem = viewTree->InsertItem(_T("���ɶ����ļ�"), 1, 2, hRoot);
	viewTree->SetItemData(tmpItem, DWORD_PTR(TriggerSmsSendFile));

	tmpItem = viewTree->InsertItem(_T("������ͣ����"), 1, 2, hRoot);
	viewTree->SetItemData(tmpItem, DWORD_PTR(TriggerOneWayStopMsg));

	tmpItem = viewTree->InsertItem(_T("����˫ͣ����"), 1, 2, hRoot);
	viewTree->SetItemData(tmpItem, DWORD_PTR(TriggerDoubleStopMsg));

	tmpItem = viewTree->InsertItem(_T("�����������"), 1, 2, hRoot);
	viewTree->SetItemData(tmpItem, DWORD_PTR(TriggerNotEnoughBalanceMsg));
	
	tmpItem = viewTree->InsertItem(_T("��������ʹ�ö���"), 1, 2, hRoot);
	viewTree->SetItemData(tmpItem, DWORD_PTR(TriggerDataRemindMsg));

	tmpItem = viewTree->InsertItem(_T("���������ⶥ����"), 1, 2, hRoot);
	viewTree->SetItemData(tmpItem, DWORD_PTR(TriggerDataTopMsg));

	tmpItem = viewTree->InsertItem(_T("�����󸶷��û��߷Ѷ���"), 1, 2, hRoot);
	viewTree->SetItemData(tmpItem, DWORD_PTR(TriggerPostPaypMsg));
}

std::vector<std::string> BuildSmsContents(CString serialNumber, CString userId)
{
	/*
	17001810501|120150105227073|1|";"=>"\n"|�𾴵ķ���ͨ�ſͻ������ã�;��2016��9���˵�129.00Ԫ��������129.00Ԫ���Żݼ�����0.00Ԫ�������У�;�ײͼ��̶��ѣ�129.00Ԫ;-�����ײͷѣ�129.00Ԫ;�ɷ������10039.cc
	*/
	std::vector<std::string> result;
	std::string content = CStringToString(serialNumber, CP_ACP);
	content += "|";
	content += CStringToString(serialNumber, CP_ACP);
	content += "|";
	content += CStringToString(_TEXT("1|\"; \"=>\"\\n\"|�𾴵ķ���ͨ�ſͻ������ã�;��2016��9���˵�129.00Ԫ��������129.00Ԫ���Żݼ�����0.00Ԫ�������У�;�ײͼ��̶��ѣ�129.00Ԫ;-�����ײͷѣ�129.00Ԫ;�ɷ������10039.cc"), CP_UTF8);
	result.push_back(content);
	return result;

}

void TriggerSmsSendFile(ModuleContext *ctx, void *ptr)
{
	ListViewData resultViewData(ctx->m_funcGetProperty(_common, _TEXT("���Ժ���")), _TEXT("���ɶ����ļ�"));
	resultViewData.m_result = _TEXT("�����ɹ�.");

	std::string hostName = CStringToString(ctx->m_funcGetProperty(_common, _TEXT("IP��ַ")), CP_ACP);
	std::string userName = CStringToString(ctx->m_funcGetProperty(_common, _TEXT("�û���")), CP_ACP);
	std::string userPwd = CStringToString(ctx->m_funcGetProperty(_common, _TEXT("����")), CP_ACP);
	CString testNumber = ctx->m_funcGetProperty(_common, _TEXT("���Ժ���"));
	std::string tmpInFile = CStringToString(ctx->m_funcGetProperty(_sms_send, _TEXT("�ļ����")), CP_ACP) + "/tmp_smsSendFile";
	CString csInFile = ctx->m_funcGetProperty(_sms_send, _TEXT("�ļ����"))
							+ _TEXT("/smsSendFile") + GetSysYMDTime() + _TEXT(".dat");

	std::string inFile = CStringToString(csInFile, CP_ACP);

	UINT port = 22;

	bool result = true;
	do {
		result = (ctx->m_objSshCmdExecutor->*(ctx->m_funcSshConnectAndInit))(hostName, port, userName, userPwd);
		if (!result)
		{
			resultViewData.m_result = _TEXT("����ʧ��.");
			break;
		}
		result = (ctx->m_objSshCmdExecutor->*(ctx->m_funcSshExecuteCmd))("touch " + tmpInFile);
		if (!result)
		{
			resultViewData.m_result = _TEXT("����ʧ��.");
			break;
		}

		std::vector<std::string> contents = BuildSmsContents(testNumber, ctx->m_funcGetProperty(_common, _TEXT("�û�ID")));

		for (auto it : contents)
		{
			result = (ctx->m_objSshCmdExecutor->*(ctx->m_funcSshExecuteCmd))("echo '" + it + "' >>" + tmpInFile);
			if (!result)
			{
				resultViewData.m_result = _TEXT("����ʧ��.");
				break;
			}
		}

		result = (ctx->m_objSshCmdExecutor->*(ctx->m_funcSshExecuteCmd))("mv  " + tmpInFile + " " + inFile);
		if (!result)
		{
			resultViewData.m_result = _TEXT("����ʧ��.");
			break;
		}


	} while (false);
	(ctx->m_objSshCmdExecutor->*(ctx->m_funcSshDisconnectAndFree))();
	ctx->m_theApp->GetMainWnd()->SendMessage(MSG_WRITE_MSG2_LISTVIEW, 0, (LPARAM)&resultViewData);
}

bool BuildHastenNotice(ModuleContext *ctx, CString userId, CString tradeTypeCode, CString policyId, 
									CString leaveRealFee,CString realFee,CString creditValue)
{
	try {
		std::string sql = "insert into CDT_HASTEN_NOTICE(trade_type_code, "
			"	user_id, "
			"	trade_id, "
			"	main_tag, "
			"	product_id, "
			"	leave_real_fee, "
			"	real_fee, "
			"	credit_value, "
			"	hasten_policy_id, "
			"	in_time, "
			"	hasten_times, "
			"	already_hasten, "
			"	exec_time,  "
			"	last_exec_time, "
			"	deal_tag, "
			"	region_id, "
			"	remark, "
			"	over_owe) "
			"	values(:v1<char[20]>, "
			"		:v2<char[20]>, "
			"		:v3<char[20]>, "
			"		0,999999,"
			"		:v4<char[20]>,:v5<char[20]>,:v6<char[20]>,:v7<char[20]>,"
			"		sysdate,0,0,sysdate,sysdate,0,'test','',0)";



		otl_stream otlStm;
		otlStm.open(1, sql.c_str(), *(ctx->m_dbConn));
		otlStm.set_commit(0);

		otlStm << CStringToString(tradeTypeCode, CP_ACP).c_str()
			<< CStringToString(userId, CP_ACP).c_str()
			<< CStringToString(GetSysTime(), CP_ACP).c_str()
			<< CStringToString(leaveRealFee, CP_ACP).c_str()
			<< CStringToString(realFee, CP_ACP).c_str()
			<< CStringToString(creditValue, CP_ACP).c_str()
			<< CStringToString(policyId, CP_ACP).c_str();

		ctx->m_dbConn->commit();
	}
	catch (otl_exception &e)
	{
		char strExp[4096] = { 0 };
		sprintf_s(strExp, "code:%d,msg:%s,var_info:%s,stm_text:%s\n", e.code, e.msg, e.var_info, e.stm_text);
		CString exp = ctx->m_funcString2CString(strExp, CP_ACP);
		ctx->m_theApp->GetMainWnd()->SendMessage(MSG_WRITE_MSG2_STATUSBAR, 0, (LPARAM)exp.GetBuffer());
		exp.ReleaseBuffer();
		return false;
	}

	return true;
}


void TriggerOneWayStopMsg(ModuleContext *ctx, void *ptr)
{
	ListViewData resultViewData(ctx->m_funcGetProperty(_common, _TEXT("���Ժ���")), _TEXT("������ͣ����"));
	resultViewData.m_result = _TEXT("�����ɹ�.");

	/*
	ModuleContext *ctx, CString userId, CString tradeTypeCode, CString policyId, 
									CString leaveRealFee,CString realFee,CString creditValue
	*/
	if (!BuildHastenNotice(ctx, ctx->m_funcGetProperty(_common, _TEXT("�û�ID")),
		_TEXT("3120"),
		ctx->m_funcGetProperty(_sms_send, _TEXT("HASTEN_POLICY_ID")),
		ctx->m_funcGetProperty(_sms_send, _TEXT("LEAVE_REAL_FEE")),
		ctx->m_funcGetProperty(_sms_send, _TEXT("REAL_FEE")),
		ctx->m_funcGetProperty(_sms_send, _TEXT("CREDIT_VALUE"))))
	{
		resultViewData.m_result = _TEXT("����ʧ��.");
	}

	ctx->m_theApp->GetMainWnd()->SendMessage(MSG_WRITE_MSG2_LISTVIEW, 0, (LPARAM)&resultViewData);
}
void TriggerDoubleStopMsg(ModuleContext *ctx, void *ptr)
{
	ListViewData resultViewData(ctx->m_funcGetProperty(_common, _TEXT("���Ժ���")), _TEXT("����˫ͣ����"));
	resultViewData.m_result = _TEXT("�����ɹ�.");

	/*
	ModuleContext *ctx, CString userId, CString tradeTypeCode, CString policyId,
	CString leaveRealFee,CString realFee,CString creditValue
	*/
	if (!BuildHastenNotice(ctx, ctx->m_funcGetProperty(0, _TEXT("�û�ID")),
		_TEXT("3110"),
		ctx->m_funcGetProperty(_sms_send, _TEXT("HASTEN_POLICY_ID")),
		ctx->m_funcGetProperty(_sms_send, _TEXT("LEAVE_REAL_FEE")),
		ctx->m_funcGetProperty(_sms_send, _TEXT("REAL_FEE")),
		ctx->m_funcGetProperty(_sms_send, _TEXT("CREDIT_VALUE"))))
	{
		resultViewData.m_result = _TEXT("����ʧ��.");
	}

	ctx->m_theApp->GetMainWnd()->SendMessage(MSG_WRITE_MSG2_LISTVIEW, 0, (LPARAM)&resultViewData);
}

void TriggerNotEnoughBalanceMsg(ModuleContext *ctx, void *ptr)
{
	ListViewData resultViewData(ctx->m_funcGetProperty(_common, _TEXT("���Ժ���")), _TEXT("�����������"));
	resultViewData.m_result = _TEXT("�����ɹ�.");

	/*
	ModuleContext *ctx, CString userId, CString tradeTypeCode, CString policyId,
	CString leaveRealFee,CString realFee,CString creditValue
	*/
	if (!BuildHastenNotice(ctx, ctx->m_funcGetProperty(0, _TEXT("�û�ID")),
		_TEXT("7001"),
		ctx->m_funcGetProperty(_sms_send, _TEXT("HASTEN_POLICY_ID")),
		ctx->m_funcGetProperty(_sms_send, _TEXT("LEAVE_REAL_FEE")),
		ctx->m_funcGetProperty(_sms_send, _TEXT("REAL_FEE")),
		ctx->m_funcGetProperty(_sms_send, _TEXT("CREDIT_VALUE"))))
	{
		resultViewData.m_result = _TEXT("����ʧ��.");
	}

	ctx->m_theApp->GetMainWnd()->SendMessage(MSG_WRITE_MSG2_LISTVIEW, 0, (LPARAM)&resultViewData);
}

void TriggerDataRemindMsg(ModuleContext *ctx, void *ptr)
{
	ListViewData resultViewData(ctx->m_funcGetProperty(_common, _TEXT("���Ժ���")), _TEXT("��������ʹ�ö���"));
	resultViewData.m_result = _TEXT("�����ɹ�.");

	/*
	ModuleContext *ctx, CString userId, CString tradeTypeCode, CString policyId,
	CString leaveRealFee,CString realFee,CString creditValue
	*/
	if (!BuildHastenNotice(ctx, ctx->m_funcGetProperty(0, _TEXT("�û�ID")),
		_TEXT("7002"),
		ctx->m_funcGetProperty(_sms_send, _TEXT("HASTEN_POLICY_ID")),
		ctx->m_funcGetProperty(_sms_send, _TEXT("LEAVE_REAL_FEE")),
		ctx->m_funcGetProperty(_sms_send, _TEXT("REAL_FEE")),
		ctx->m_funcGetProperty(_sms_send, _TEXT("CREDIT_VALUE"))))
	{
		resultViewData.m_result = _TEXT("����ʧ��.");
	}

	ctx->m_theApp->GetMainWnd()->SendMessage(MSG_WRITE_MSG2_LISTVIEW, 0, (LPARAM)&resultViewData);
	
}
void TriggerDataTopMsg(ModuleContext *ctx, void *ptr)
{
	ListViewData resultViewData(ctx->m_funcGetProperty(_common, _TEXT("���Ժ���")), _TEXT("���������ⶥ����"));
	resultViewData.m_result = _TEXT("�����ɹ�.");

	/*
	ModuleContext *ctx, CString userId, CString tradeTypeCode, CString policyId,
	CString leaveRealFee,CString realFee,CString creditValue
	*/
	if (!BuildHastenNotice(ctx, ctx->m_funcGetProperty(0, _TEXT("�û�ID")),
		_TEXT("3111"),
		ctx->m_funcGetProperty(_sms_send, _TEXT("HASTEN_POLICY_ID")),
		ctx->m_funcGetProperty(_sms_send, _TEXT("LEAVE_REAL_FEE")),
		ctx->m_funcGetProperty(_sms_send, _TEXT("REAL_FEE")),
		ctx->m_funcGetProperty(_sms_send, _TEXT("CREDIT_VALUE"))))
	{
		resultViewData.m_result = _TEXT("����ʧ��.");
	}

	ctx->m_theApp->GetMainWnd()->SendMessage(MSG_WRITE_MSG2_LISTVIEW, 0, (LPARAM)&resultViewData);

}
void TriggerPostPaypMsg(ModuleContext *ctx, void *ptr)
{
	ListViewData resultViewData(ctx->m_funcGetProperty(_common, _TEXT("���Ժ���")), _TEXT("�����󸶷��û��߷Ѷ���"));
	resultViewData.m_result = _TEXT("�����ɹ�.");

	/*
	ModuleContext *ctx, CString userId, CString tradeTypeCode, CString policyId,
	CString leaveRealFee,CString realFee,CString creditValue
	*/
	if (!BuildHastenNotice(ctx, ctx->m_funcGetProperty(0, _TEXT("�û�ID")),
		_TEXT("3200"),
		ctx->m_funcGetProperty(_sms_send, _TEXT("HASTEN_POLICY_ID")),
		ctx->m_funcGetProperty(_sms_send, _TEXT("LEAVE_REAL_FEE")),
		ctx->m_funcGetProperty(_sms_send, _TEXT("REAL_FEE")),
		ctx->m_funcGetProperty(_sms_send, _TEXT("CREDIT_VALUE"))))
	{
		resultViewData.m_result = _TEXT("����ʧ��.");
	}

	ctx->m_theApp->GetMainWnd()->SendMessage(MSG_WRITE_MSG2_LISTVIEW, 0, (LPARAM)&resultViewData);

}