// CreditDegree.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "CreditDegree.h"
#include "../../BillingTool/PluginInterface.h"
#include "../../BillingTool/ViewTree.h"
#include "../../BillingTool/ModuleContext.h"
#include "../../BillingTool/BillingTool.h"
#include "../UtilDll/UtilDll.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO:  如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如: 
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。  这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CCreditDegreeApp

BEGIN_MESSAGE_MAP(CCreditDegreeApp, CWinApp)
END_MESSAGE_MAP()


// CCreditDegreeApp 构造

CCreditDegreeApp::CCreditDegreeApp()
{
	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CCreditDegreeApp 对象

//CCreditDegreeApp theApp;


// CCreditDegreeApp 初始化

BOOL CCreditDegreeApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
const CString VIPCLASS = _TEXT("VIP-CLASS");
const CString CREDIT_VALUE = _TEXT("信用度值");
const CString CREDIT_GRADE = _TEXT("信用度等级");

static std::map<CString, PropertyGrid> modulePropertys = {
	{ VIPCLASS,
		{ _TEXT("10"),nullptr , FALSE , }
	},
	{ CREDIT_VALUE,
		{ _TEXT("1000"),nullptr , FALSE , }
	},
	{ CREDIT_GRADE,
		{ _TEXT("A"),nullptr , FALSE , }
	}
};

void PropertyInitilize(std::map<int, std::map<CString, PropertyGrid> > &gProperty)
{
	gProperty.insert(std::make_pair(_credit_degree, modulePropertys));
}



void CreateInitCreditDegree(ModuleContext *ctx, void *ptr);
void ActiveTempCreditDegree(ModuleContext *ctx, void *ptr);
void ActiveDynamicCreditDegree(ModuleContext *ctx, void *ptr);
void ActiveInitCreditDegree(ModuleContext *ctx, void *ptr);


static _ItemCallBackDef moduleCallBackDef[] = {
			{ _credit_degree, CreateInitCreditDegree },
			{ _credit_degree, ActiveTempCreditDegree },
			{ _credit_degree, ActiveDynamicCreditDegree },
			{ _credit_degree, ActiveInitCreditDegree } };

void Initilize(CWnd *mainWnd, CViewTree *viewTree)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());


	HTREEITEM hRoot = viewTree->InsertItem(_T("信用度处理"), 0, 0);

	HTREEITEM tmpItem = viewTree->InsertItem(_T("初始信用度生成"), 1, 2, hRoot);
	viewTree->SetItemData(tmpItem, DWORD_PTR(&(moduleCallBackDef[0])));

	tmpItem = viewTree->InsertItem(_T("临时信用度生效"), 1, 2, hRoot);
	viewTree->SetItemData(tmpItem, DWORD_PTR(&(moduleCallBackDef[1])));

	tmpItem = viewTree->InsertItem(_T("动态信用度生效"), 1, 2, hRoot);
	viewTree->SetItemData(tmpItem, DWORD_PTR(&(moduleCallBackDef[2])));

	tmpItem = viewTree->InsertItem(_T("初始信用度生效"), 1, 2, hRoot);
	viewTree->SetItemData(tmpItem, DWORD_PTR(&(moduleCallBackDef[3])));
}



static bool BuildInitCredit(ModuleContext *ctx,CString serialNumber, CString userId, CString custId,CString vipClass)
{
	try {
		std::string sql = "insert into CS_CTOB_INITCREDIT("
			"	btoc_id, "
			"	serial_number, "
			"	user_id, "
			"	cust_id, "
			"	provider_code, "
			"	vip_class, "
			"	deal_tag, "
			"	update_time, "
			"	update_staff_id) "
			"	values (:v1<char[20]>,"
			"		:v2<char[20]>, "
			"		:v3<char[20]>, "
			"		:v4<char[20]>, "
			"		'CUCC', "
			"		:v5<char[20]>, "
			"		0,sysdate,'test') ";
			


		otl_stream otlStm;
		otlStm.open(1, sql.c_str(), *(ctx->m_dbConn));
		otlStm.set_commit(0);

		otlStm << CommonUtil::CStringToString(CommonUtil::GetSysTime(), CP_ACP).c_str()
			<< CommonUtil::CStringToString(serialNumber, CP_ACP).c_str()
			<< CommonUtil::CStringToString(userId, CP_ACP).c_str()
			<< CommonUtil::CStringToString(custId, CP_ACP).c_str()
			<< CommonUtil::CStringToString(vipClass, CP_ACP).c_str();

		ctx->m_dbConn->commit();
	}
	catch (otl_exception &e)
	{
		char strExp[4096] = { 0 };
		sprintf_s(strExp, "code:%d,msg:%s,var_info:%s,stm_text:%s\n", e.code, e.msg, e.var_info, e.stm_text);
		CString exp = CommonUtil::StringToCString(strExp, CP_ACP);
		ctx->m_theApp->GetMainWnd()->SendMessage(MSG_WRITE_MSG2_STATUSBAR, 0, (LPARAM)exp.GetBuffer());
		exp.ReleaseBuffer();
		return false;
	}

	return true;
}



static bool BuildUserCredit(ModuleContext *ctx, CString userId, CString creditValue,CString creditClass,CString creditType)
{
	try {
		std::string sql = "insert into CS_USER_CREDIT("
			"	user_id, "
			"	credit_value, "
			"	credit_class, "
			"	credit_type, "
			"	start_time, "
			"	end_time, "
			"	act_tag, "
			"	update_time, "
			"	update_staff_id) "
			"	values (:v1<char[20]>,"
			"		:v2<char[20]>, "
			"		:v3<char[6]>, "
			"		:v4<char[2]>, "
			"		sysdate,sysdate + 60,"
			"		0,sysdate,'test') ";



		otl_stream otlStm;
		otlStm.open(1, sql.c_str(), *(ctx->m_dbConn));
		otlStm.set_commit(0);

		otlStm << CommonUtil::CStringToString(userId, CP_ACP).c_str()
			<< CommonUtil::CStringToString(creditValue, CP_ACP).c_str()
			<< CommonUtil::CStringToString(creditClass, CP_ACP).c_str()
			<< CommonUtil::CStringToString(creditType, CP_ACP).c_str();

		ctx->m_dbConn->commit();
	}
	catch (otl_exception &e)
	{
		char strExp[4096] = { 0 };
		sprintf_s(strExp, "code:%d,msg:%s,var_info:%s,stm_text:%s\n", e.code, e.msg, e.var_info, e.stm_text);
		CString exp = CommonUtil::StringToCString(strExp, CP_ACP);
		ctx->m_theApp->GetMainWnd()->SendMessage(MSG_WRITE_MSG2_STATUSBAR, 0, (LPARAM)exp.GetBuffer());
		exp.ReleaseBuffer();
		return false;
	}

	return true;
}

void CreateInitCreditDegree(ModuleContext *ctx, void *ptr)
{
	ListViewData resultViewData(ctx->m_funcGetProperty(_common, TEST_NUMBER), _TEXT("初始信用度评估"));
	if (!BuildInitCredit(ctx, ctx->m_funcGetProperty(_common, TEST_NUMBER),
		ctx->m_funcGetProperty(_common, USER_ID),
		ctx->m_funcGetProperty(_common, CUST_ID),
		ctx->m_funcGetProperty(_credit_degree, VIPCLASS)))
	{
		resultViewData.PushMsg(_TEXT("触发失败."));
	}
	else
	{
		resultViewData.PushMsg(_TEXT("触发成功."));
	}

	ctx->m_theApp->GetMainWnd()->SendMessage(MSG_WRITE_MSG2_LISTVIEW, 0, (LPARAM)&resultViewData);

}
void ActiveTempCreditDegree(ModuleContext *ctx, void *ptr)
{
	ListViewData resultViewData(ctx->m_funcGetProperty(_common, TEST_NUMBER), _TEXT("临时信用度生效"));
	if (!BuildUserCredit(ctx, ctx->m_funcGetProperty(_common, USER_ID),
		ctx->m_funcGetProperty(_credit_degree, CREDIT_VALUE),
		ctx->m_funcGetProperty(_credit_degree, CREDIT_GRADE),
		_TEXT("3")))
	{
		resultViewData.PushMsg(_TEXT("触发失败."));
	}
	else
	{
		resultViewData.PushMsg(_TEXT("触发成功."));
	}

	ctx->m_theApp->GetMainWnd()->SendMessage(MSG_WRITE_MSG2_LISTVIEW, 0, (LPARAM)&resultViewData);

}
void ActiveDynamicCreditDegree(ModuleContext *ctx, void *ptr)
{
	ListViewData resultViewData(ctx->m_funcGetProperty(_common, TEST_NUMBER), _TEXT("动态信用度生效"));
	if (!BuildUserCredit(ctx, ctx->m_funcGetProperty(_common, USER_ID),
		ctx->m_funcGetProperty(_credit_degree, CREDIT_VALUE),
		ctx->m_funcGetProperty(_credit_degree, CREDIT_GRADE),
		_TEXT("2")))
	{
		resultViewData.PushMsg(_TEXT("触发失败."));
	}
	else
	{
		resultViewData.PushMsg(_TEXT("触发成功."));
	}
	ctx->m_theApp->GetMainWnd()->SendMessage(MSG_WRITE_MSG2_LISTVIEW, 0, (LPARAM)&resultViewData);

}

void ActiveInitCreditDegree(ModuleContext *ctx, void *ptr)
{
	ListViewData resultViewData(ctx->m_funcGetProperty(_common, TEST_NUMBER), _TEXT("初始信用度生效"));
	if (!BuildUserCredit(ctx, ctx->m_funcGetProperty(_common, USER_ID),
		ctx->m_funcGetProperty(_credit_degree, CREDIT_VALUE),
		ctx->m_funcGetProperty(_credit_degree, CREDIT_GRADE),
		_TEXT("1")))
	{
		resultViewData.PushMsg(_TEXT("触发失败."));
	}
	else
	{
		resultViewData.PushMsg(_TEXT("触发成功."));
	}

	ctx->m_theApp->GetMainWnd()->SendMessage(MSG_WRITE_MSG2_LISTVIEW, 0, (LPARAM)&resultViewData);

}