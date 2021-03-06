#include "stdafx.h"
#include "ModuleContext.h"
#include "BillingTool.h"
#include "BillingToolView.h"
#include "MainFrm.h"
#include "PubFunc.h"
#include "../Plugins/UtilDll/SshCmdExecutor.h"
#include "../Plugins/UtilDll/UtilDll.h"

ModuleContext::ModuleContext(CBillingToolApp *app)
	:m_dbConn(&gDbConn),
	m_theApp(app),
	m_funcGetProperty(GetProperty),
	m_objSshCmdExecutor(new SshCmdExecutor())
{
}
ModuleContext::~ModuleContext()
{
	try
	{
		m_dbConn->rollback();
		m_dbConn->logoff();
	}
	catch (otl_exception &e) 
	{
		char errMsg[4096] = { 0 };
		sprintf_s(errMsg, "code:%d,msg:%s,var_info:%s,stm_text:%s", e.code, e.msg, e.var_info, e.stm_text);
		CString text = CommonUtil::StringToCString(errMsg, CP_ACP);
		theApp.GetMainWnd()->SendMessage(MSG_WRITE_MSG2_STATUSBAR, 0, (LPARAM)text.GetBuffer());
		text.ReleaseBuffer();

	}
	
}


int ModuleContext::ConnectDb(std::string dbString)
{
	if (m_dbConn->connected)
		return SUCCESS;

	try
	{ 
		m_dbConn->rlogon(dbString.c_str(), false);
	}
	catch (otl_exception &e)
	{
		char errMsg[4096] = { 0 };
		sprintf_s(errMsg , "connect string:%s,code:%d,msg:%s,var_info:%s,stm_text:%s", dbString.c_str(), e.code, e.msg, e.var_info, e.stm_text);
		CString text = CommonUtil::StringToCString(errMsg, CP_ACP);
		theApp.GetMainWnd()->SendMessage(MSG_WRITE_MSG2_STATUSBAR, 0, (LPARAM)text.GetBuffer());
		text.ReleaseBuffer();

		return FAILURE;
	}

	return SUCCESS;
}
