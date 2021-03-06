// SettSecPrice.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "SettSecPrice.h"
#include "../../BillingTool/PluginInterface.h"
#include "../../BillingTool/ViewTree.h"
#include "../../BillingTool/ModuleContext.h"
#include "../../BillingTool/BillingTool.h"
#include <vector>
#include "../UtilDll/UtilDll.h"
#include "../UtilDll/SshCmdExecutor.h"


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

// CSettSecPriceApp

BEGIN_MESSAGE_MAP(CSettSecPriceApp, CWinApp)
END_MESSAGE_MAP()


// CSettSecPriceApp 构造

CSettSecPriceApp::CSettSecPriceApp()
{
	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CSettSecPriceApp 对象

//CSettSecPriceApp theApp;


// CSettSecPriceApp 初始化

BOOL CSettSecPriceApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}


const CString FILE_IN = _TEXT("文件入口");
const CString PEER_NUMBER = _TEXT("对端号码");
const CString DURATION = _TEXT("时长");
const CString DOWNLOAD_VALUE = _TEXT("下行流量");
const CString UPLOAD_VALUE = _TEXT("上行流量");

static std::map<CString, PropertyGrid> modulePropertys = {
	{ FILE_IN,
		{ _TEXT("/home/chengl/src/BillRate/data/in"),nullptr , FALSE , }
	},
	{ PEER_NUMBER,
		{ _TEXT("18645005420"),nullptr , FALSE , }
	},
	{ DURATION,
		{ _TEXT("1000"),nullptr , FALSE , }
	},
	{ DOWNLOAD_VALUE,
		{ _TEXT("102400"),nullptr , FALSE , }
	},
	{ UPLOAD_VALUE,
		{ _TEXT("102400"),nullptr , FALSE , }
	}
};


void PropertyInitilize(std::map<int, std::map<CString, PropertyGrid> > &gProperty)
{
	gProperty.insert(std::make_pair(_sett_sec_price, modulePropertys));
}

void TriggerVcLocalCdr(ModuleContext *ctx, void *ptr);
void TriggerVcLongCdr(ModuleContext *ctx, void *ptr);
void TriggerVcRoamInProviceCdr(ModuleContext *ctx, void *ptr);
void TriggerVcRoamProviceCdr(ModuleContext *ctx, void *ptr);
void TriggerVcLongInterCdr(ModuleContext *ctx, void *ptr);
void TriggerVcRoamInterCdr(ModuleContext *ctx, void *ptr);
void TriggerVcIpCallCdr(ModuleContext *ctx, void *ptr);
void TriggerVcDivertCdr(ModuleContext *ctx, void *ptr);
void TriggerVcCalledCdr(ModuleContext *ctx, void *ptr);


void TriggerSmsChnCdr(ModuleContext *ctx, void *ptr);
void TriggerSmsInterCdr(ModuleContext *ctx, void *ptr);
void TriggerSmsRoamCdr(ModuleContext *ctx, void *ptr);
void TriggerSmsCalledCdr(ModuleContext *ctx, void *ptr);



void TriggerDataInProvinceCdr(ModuleContext *ctx, void *ptr);
void TriggerDataProvinceCdr(ModuleContext *ctx, void *ptr);
void TriggerDataInterCdr(ModuleContext *ctx, void *ptr);

static _ItemCallBackDef moduleCallBackDef[] = {
	{ _sett_sec_price, TriggerVcLocalCdr },
	{ _sett_sec_price, TriggerVcLongCdr },
	{ _sett_sec_price, TriggerVcRoamInProviceCdr },
	{ _sett_sec_price, TriggerVcRoamProviceCdr },
	{ _sett_sec_price, TriggerVcLongInterCdr },
	{ _sett_sec_price, TriggerVcRoamInterCdr },
	{ _sett_sec_price, TriggerVcIpCallCdr },
	{ _sett_sec_price, TriggerVcDivertCdr },
	{ _sett_sec_price, TriggerVcCalledCdr },
	{ _sett_sec_price, TriggerSmsChnCdr },
	{ _sett_sec_price, TriggerSmsInterCdr },
	{ _sett_sec_price, TriggerSmsRoamCdr },
	{ _sett_sec_price, TriggerSmsCalledCdr },
	{ _sett_sec_price, TriggerDataInProvinceCdr },
	{ _sett_sec_price, TriggerDataProvinceCdr },
	{ _sett_sec_price, TriggerDataInterCdr } };

void Initilize(CWnd *mainWnd, CViewTree *viewTree)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());


	HTREEITEM hRoot = viewTree->InsertItem(_T("二次批价(结算)"), 0, 0);

	HTREEITEM secItem = viewTree->InsertItem(_T("语音"), 0, 0, hRoot);

	HTREEITEM tmpItem = viewTree->InsertItem(_T("市话"), 1, 2, secItem);
	viewTree->SetItemData(tmpItem, DWORD_PTR(&(moduleCallBackDef[0])));

	tmpItem = viewTree->InsertItem(_T("长途"), 1, 2, secItem);
	viewTree->SetItemData(tmpItem, DWORD_PTR(&(moduleCallBackDef[1])));

	tmpItem = viewTree->InsertItem(_T("省内漫游"), 1, 2, secItem);
	viewTree->SetItemData(tmpItem, DWORD_PTR(&(moduleCallBackDef[2])));

	tmpItem = viewTree->InsertItem(_T("省际漫游"), 1, 2, secItem);
	viewTree->SetItemData(tmpItem, DWORD_PTR(&(moduleCallBackDef[3])));

	tmpItem = viewTree->InsertItem(_T("国际长途"), 1, 2, secItem);
	viewTree->SetItemData(tmpItem, DWORD_PTR(&(moduleCallBackDef[4])));

	tmpItem = viewTree->InsertItem(_T("国际漫游"), 1, 2, secItem);
	viewTree->SetItemData(tmpItem, DWORD_PTR(&(moduleCallBackDef[5])));

	tmpItem = viewTree->InsertItem(_T("IP通话"), 1, 2, secItem);
	viewTree->SetItemData(tmpItem, DWORD_PTR(&(moduleCallBackDef[6])));

	tmpItem = viewTree->InsertItem(_T("呼转通话"), 1, 2, secItem);
	viewTree->SetItemData(tmpItem, DWORD_PTR(&(moduleCallBackDef[7])));

	tmpItem = viewTree->InsertItem(_T("国内被叫"), 1, 2, secItem);
	viewTree->SetItemData(tmpItem, DWORD_PTR(&(moduleCallBackDef[8])));

	secItem = viewTree->InsertItem(_T("短信"), 0, 0, hRoot);

	tmpItem = viewTree->InsertItem(_T("国内短信"), 1, 2, secItem);
	viewTree->SetItemData(tmpItem, DWORD_PTR(&(moduleCallBackDef[9])));

	tmpItem = viewTree->InsertItem(_T("国际短信"), 1, 2, secItem);
	viewTree->SetItemData(tmpItem, DWORD_PTR(&(moduleCallBackDef[10])));

	tmpItem = viewTree->InsertItem(_T("国际漫游短信"), 1, 2, secItem);
	viewTree->SetItemData(tmpItem, DWORD_PTR(&(moduleCallBackDef[11])));

	tmpItem = viewTree->InsertItem(_T("短信被叫"), 1, 2, secItem);
	viewTree->SetItemData(tmpItem, DWORD_PTR(&(moduleCallBackDef[12])));


	secItem = viewTree->InsertItem(_T("流量"), 0, 0, hRoot);

	tmpItem = viewTree->InsertItem(_T("省内流量"), 1, 2, secItem);
	viewTree->SetItemData(tmpItem, DWORD_PTR(&(moduleCallBackDef[13])));

	tmpItem = viewTree->InsertItem(_T("省际流量"), 1, 2, secItem);
	viewTree->SetItemData(tmpItem, DWORD_PTR(&(moduleCallBackDef[14])));

	tmpItem = viewTree->InsertItem(_T("国际流量"), 1, 2, secItem);
	viewTree->SetItemData(tmpItem, DWORD_PTR(&(moduleCallBackDef[15])));

}





std::vector<std::string> BuildCdrContent(CString cdrTag, CString callType, CString serialNumber, CString providerCode,
	CString peerNumber, CString duration, CString downVolume, CString upVolume, CString roamType, CString longType,
	BOOL ipCall, CString roamNation, CString firstPriceFee, CString imsi, CString imei, CString phoneRegion)
{
	std::vector<std::string> result;
	std::string content;
	////
	content = "THEAD,";
	content += CommonUtil::CStringToString(CommonUtil::GetSysYMTime(), CP_ACP);
	content += ",v1,STDBILL_CDR";
	result.push_back(content);

	///
	content = CommonUtil::CStringToString(CommonUtil::GetSysUtcTime(), CP_ACP);
	content += ",";

	content += CommonUtil::CStringToString(cdrTag, CP_ACP);
	content += ",";

	content += "FX_XXXXXXXXXXXXXX.cdr";
	content += ",";

	content += CommonUtil::CStringToString(CommonUtil::GetSysYMTime(), CP_ACP);
	content += ",";

	content += CommonUtil::CStringToString(providerCode, CP_ACP);
	content += ",";

	content += CommonUtil::CStringToString(serialNumber, CP_ACP);
	content += ",,,";

	content += CommonUtil::CStringToString(peerNumber, CP_ACP);
	content += ",,,,,";

	content += CommonUtil::CStringToString(callType, CP_ACP);
	content += ",0,";

	content += CommonUtil::CStringToString(longType, CP_ACP);
	content += ",USA,";
	
	content += CommonUtil::CStringToString(roamType, CP_ACP);
	content += ",USA,";

	content += CommonUtil::CStringToString(CommonUtil::GetSysTime(), CP_ACP);
	content += ",";

	content += CommonUtil::CStringToString(CommonUtil::GetSysTime(), CP_ACP);
	content += ",";

	content += CommonUtil::CStringToString(duration, CP_ACP);
	content += ",";

	content += ",,,";

	if (TRUE == ipCall)
	{
		content += "IPCALL";
	}
	content += ",,,,";

	content += CommonUtil::CStringToString(upVolume, CP_ACP);
	content += ",";

	content += CommonUtil::CStringToString(downVolume, CP_ACP);
	content += ",";



	__int64 totalVolume = _wtoi64(downVolume) + _wtoi64(upVolume);
	TCHAR strTotalVolume[60];
	_i64tow_s(totalVolume, strTotalVolume, 60, 10);

	content += CommonUtil::CStringToString(CString(strTotalVolume), CP_ACP);
	content += ",";

	content += "0:1000,1000,";

	content += CommonUtil::CStringToString(CommonUtil::GetSysTime(), CP_ACP);
	content += ",";

	content += ",,,,0,,,,,60000001,,,,,,,,,";


	result.push_back(content);

	///
	////
	content = "TEND,1,0,0,0,";
	result.push_back(content);

	return result;
}


std::vector<std::string> GetFiles(ModuleContext *ctx, CString testNumber, CString providerCode)
{
	std::vector<std::string> result;
	CString inPath = ctx->m_funcGetProperty(_sett_sec_price, FILE_IN);
	result.push_back(CommonUtil::CStringToString(inPath, CP_ACP) + "/../tmpSET_STAND_CDR_.dat");
	CString csInFile = inPath + _TEXT("/SET_STAND_CDR_") + CommonUtil::GetSysYMDTime() + _TEXT("_") + testNumber.Left(7) + "_" + providerCode
		+ _TEXT("_") + CommonUtil::GetSysYMTime() + _TEXT("_") + CommonUtil::GetSerial() + _TEXT(".dat");
	result.push_back(CommonUtil::CStringToString(csInFile, CP_ACP));
	return result;
}



void TriggerVcLocalCdr(ModuleContext *ctx, void *ptr)
{
	CString testNumber = ctx->m_funcGetProperty(_common, TEST_NUMBER);
	std::string hostName = CommonUtil::CStringToString(ctx->m_funcGetProperty(_common, IP_ADDR), CP_ACP);
	std::string userName = CommonUtil::CStringToString(ctx->m_funcGetProperty(_common, HOST_USERNAME), CP_ACP);
	std::string userPwd = CommonUtil::CStringToString(ctx->m_funcGetProperty(_common, HOST_USERPWD), CP_ACP);
	CString providerCode = ctx->m_funcGetProperty(_common, USER_PROVIDER_CODE);
	UINT port = 22;

	std::vector < std::string> files = GetFiles(ctx , testNumber, providerCode);
	std::string  tmpInFile = files.at(0);
	std::string  inFile = files.at(1);

	ListViewData resultViewData(testNumber, _TEXT("二批-本地市话【结算】"));

	bool result = true;
	do {
		result = ctx->m_objSshCmdExecutor->ConnectAndInit(hostName, port, userName, userPwd);
		if (!result)
		{
			resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
			resultViewData.PushMsg( _TEXT("触发失败."));
			break;
		}
		result = ctx->m_objSshCmdExecutor->ExecuteCmd("touch " + tmpInFile);
		if (!result)
		{
			resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
			resultViewData.PushMsg(_TEXT("触发失败."));
			break;
		}
		
		std::vector<std::string> contents = BuildCdrContent(_TEXT("CDR_CALL"), _TEXT("CALLING"),
			testNumber, providerCode,
			ctx->m_funcGetProperty(_sett_sec_price, PEER_NUMBER),
			ctx->m_funcGetProperty(_sett_sec_price, DURATION),
			ctx->m_funcGetProperty(_sett_sec_price, DOWNLOAD_VALUE),
			ctx->m_funcGetProperty(_sett_sec_price, UPLOAD_VALUE),
			_TEXT("R_LOCAL"), _TEXT("T_LOCAL"), FALSE, _TEXT("CHN"), _TEXT("40"),
			ctx->m_funcGetProperty(_common, USER_IMSI),
			ctx->m_funcGetProperty(_common, USER_IMEI),
			ctx->m_funcGetProperty(_common, USER_HOME_CODE)
		);

		for (auto it : contents)
		{
			result = ctx->m_objSshCmdExecutor->ExecuteCmd("echo '" + it + "' >>" + tmpInFile);
			if (!result)
			{
				resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
				resultViewData.PushMsg(_TEXT("触发失败."));
				break;
			}
		}

		result = ctx->m_objSshCmdExecutor->ExecuteCmd("mv  " + tmpInFile + " " + inFile);
		if (!result)
		{
			resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
			resultViewData.PushMsg(_TEXT("触发失败."));
			break;
		}

		resultViewData.PushMsg(_TEXT("触发成功."));
	} while (false);
	ctx->m_objSshCmdExecutor->DisconnectAndFree();
	ctx->m_theApp->GetMainWnd()->SendMessage(MSG_WRITE_MSG2_LISTVIEW, 0, (LPARAM)&resultViewData);
}
void TriggerVcLongCdr(ModuleContext *ctx, void *ptr)
{
	CString testNumber = ctx->m_funcGetProperty(_common, TEST_NUMBER);
	std::string hostName = CommonUtil::CStringToString(ctx->m_funcGetProperty(_common, IP_ADDR), CP_ACP);
	std::string userName = CommonUtil::CStringToString(ctx->m_funcGetProperty(_common, HOST_USERNAME), CP_ACP);
	std::string userPwd = CommonUtil::CStringToString(ctx->m_funcGetProperty(_common, HOST_USERPWD), CP_ACP);
	CString providerCode = ctx->m_funcGetProperty(_common, USER_PROVIDER_CODE);
	UINT port = 22;

	std::vector < std::string> files = GetFiles(ctx, testNumber, providerCode);
	std::string  tmpInFile = files.at(0);
	std::string  inFile = files.at(1);

	ListViewData resultViewData(testNumber, _TEXT("二批-国内长途【结算】"));

	bool result = true;
	do {
		result = ctx->m_objSshCmdExecutor->ConnectAndInit(hostName, port, userName, userPwd);
		if (!result)
		{
			resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
			resultViewData.PushMsg(_TEXT("触发失败."));
			break;
		}
		result = ctx->m_objSshCmdExecutor->ExecuteCmd("touch " + tmpInFile);
		if (!result)
		{
			resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
			resultViewData.PushMsg(_TEXT("触发失败."));
			break;
		}
		
		std::vector<std::string> contents = BuildCdrContent(_TEXT("CDR_CALL"), _TEXT("CALLING"),
			testNumber, providerCode,
			ctx->m_funcGetProperty(_sett_sec_price, PEER_NUMBER),
			ctx->m_funcGetProperty(_sett_sec_price, DURATION),
			ctx->m_funcGetProperty(_sett_sec_price, DOWNLOAD_VALUE),
			ctx->m_funcGetProperty(_sett_sec_price, UPLOAD_VALUE),
			_TEXT("R_LOCAL"), _TEXT("T_BT_PROVINCE"), FALSE, _TEXT("CHN"), _TEXT("40"),
			ctx->m_funcGetProperty(_common, USER_IMSI),
			ctx->m_funcGetProperty(_common, USER_IMEI),
			ctx->m_funcGetProperty(_common, USER_HOME_CODE)
		);

		for (auto it : contents)
		{
			result = ctx->m_objSshCmdExecutor->ExecuteCmd("echo '" + it + "' >>" + tmpInFile);
			if (!result)
			{
				resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
				resultViewData.PushMsg(_TEXT("触发失败."));
				break;
			}
		}

		result = ctx->m_objSshCmdExecutor->ExecuteCmd("mv  " + tmpInFile + " " + inFile);
		if (!result)
		{
			resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
			resultViewData.PushMsg(_TEXT("触发失败."));			break;
		}

		resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
		resultViewData.PushMsg(_TEXT("触发成功."));
	} while (false);
	ctx->m_objSshCmdExecutor->DisconnectAndFree();
	ctx->m_theApp->GetMainWnd()->SendMessage(MSG_WRITE_MSG2_LISTVIEW, 0, (LPARAM)&resultViewData);
}
void TriggerVcRoamInProviceCdr(ModuleContext *ctx, void *ptr)
{
	CString testNumber = ctx->m_funcGetProperty(_common, TEST_NUMBER);
	std::string hostName = CommonUtil::CStringToString(ctx->m_funcGetProperty(_common, IP_ADDR), CP_ACP);
	std::string userName = CommonUtil::CStringToString(ctx->m_funcGetProperty(_common, HOST_USERNAME), CP_ACP);
	std::string userPwd = CommonUtil::CStringToString(ctx->m_funcGetProperty(_common, HOST_USERPWD), CP_ACP);
	CString providerCode = ctx->m_funcGetProperty(_common, USER_PROVIDER_CODE);
	UINT port = 22;

	std::vector < std::string> files = GetFiles(ctx, testNumber, providerCode);
	std::string  tmpInFile = files.at(0);
	std::string  inFile = files.at(1);

	ListViewData resultViewData(testNumber, _TEXT("二批-省内漫游【结算】"));

	bool result = true;
	do {
		result = ctx->m_objSshCmdExecutor->ConnectAndInit(hostName, port, userName, userPwd);
		if (!result)
		{
			resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
			resultViewData.PushMsg(_TEXT("触发失败."));			
			break;
		}
		result = ctx->m_objSshCmdExecutor->ExecuteCmd("touch " + tmpInFile);
		if (!result)
		{
			resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
			resultViewData.PushMsg(_TEXT("触发失败.")); 
			break;
		}
		
		std::vector<std::string> contents = BuildCdrContent(_TEXT("CDR_CALL"), _TEXT("CALLING"),
			testNumber, providerCode,
			ctx->m_funcGetProperty(_sett_sec_price, PEER_NUMBER),
			ctx->m_funcGetProperty(_sett_sec_price, DURATION),
			ctx->m_funcGetProperty(_sett_sec_price, DOWNLOAD_VALUE),
			ctx->m_funcGetProperty(_sett_sec_price, UPLOAD_VALUE),
			_TEXT("R_IN_PROVINCE"), _TEXT("T_BT_PROVINCE"), FALSE, _TEXT("CHN"), _TEXT("40"),
			ctx->m_funcGetProperty(_common, USER_IMSI),
			ctx->m_funcGetProperty(_common, USER_IMEI),
			ctx->m_funcGetProperty(_common, USER_HOME_CODE)
		);

		for (auto it : contents)
		{
			result = ctx->m_objSshCmdExecutor->ExecuteCmd("echo '" + it + "' >>" + tmpInFile);
			if (!result)
			{
				resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
				resultViewData.PushMsg(_TEXT("触发失败."));
				break;
			}
		}

		result = ctx->m_objSshCmdExecutor->ExecuteCmd("mv  " + tmpInFile + " " + inFile);
		if (!result)
		{
			resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
			resultViewData.PushMsg(_TEXT("触发失败."));	
			break;
		}

		resultViewData.PushMsg(_TEXT("触发成功."));
	} while (false);
	ctx->m_objSshCmdExecutor->DisconnectAndFree();
	ctx->m_theApp->GetMainWnd()->SendMessage(MSG_WRITE_MSG2_LISTVIEW, 0, (LPARAM)&resultViewData);
}
void TriggerVcRoamProviceCdr(ModuleContext *ctx, void *ptr)
{
	CString testNumber = ctx->m_funcGetProperty(_common, TEST_NUMBER);
	std::string hostName = CommonUtil::CStringToString(ctx->m_funcGetProperty(_common, IP_ADDR), CP_ACP);
	std::string userName = CommonUtil::CStringToString(ctx->m_funcGetProperty(_common, HOST_USERNAME), CP_ACP);
	std::string userPwd = CommonUtil::CStringToString(ctx->m_funcGetProperty(_common, HOST_USERPWD), CP_ACP);
	CString providerCode = ctx->m_funcGetProperty(_common, USER_PROVIDER_CODE);
	UINT port = 22;

	std::vector < std::string> files = GetFiles(ctx, testNumber, providerCode);
	std::string  tmpInFile = files.at(0);
	std::string  inFile = files.at(1);

	ListViewData resultViewData(testNumber, _TEXT("二批-省际漫游【结算】"));

	bool result = true;
	do {
		result = ctx->m_objSshCmdExecutor->ConnectAndInit(hostName, port, userName, userPwd);
		if (!result)
		{
			resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
			resultViewData.PushMsg(_TEXT("触发失败."));
			break;
		}
		result = ctx->m_objSshCmdExecutor->ExecuteCmd("touch " + tmpInFile);
		if (!result)
		{
			resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
			resultViewData.PushMsg(_TEXT("触发失败."));
			break;
		}
		
		std::vector<std::string> contents = BuildCdrContent(_TEXT("CDR_CALL"), _TEXT("CALLING"),
			testNumber, providerCode,
			ctx->m_funcGetProperty(_sett_sec_price, PEER_NUMBER),
			ctx->m_funcGetProperty(_sett_sec_price, DURATION),
			ctx->m_funcGetProperty(_sett_sec_price, DOWNLOAD_VALUE),
			ctx->m_funcGetProperty(_sett_sec_price, UPLOAD_VALUE),
			_TEXT("R_PROVINCE"), _TEXT("T_BT_PROVINCE"), FALSE, _TEXT("CHN"), _TEXT("40"),
			ctx->m_funcGetProperty(_common, USER_IMSI),
			ctx->m_funcGetProperty(_common, USER_IMEI),
			ctx->m_funcGetProperty(_common, USER_HOME_CODE)
		);

		for (auto it : contents)
		{
			result = ctx->m_objSshCmdExecutor->ExecuteCmd("echo '" + it + "' >>" + tmpInFile);
			if (!result)
			{
				resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
				resultViewData.PushMsg(_TEXT("触发失败."));
				break;
			}
		}

		result = ctx->m_objSshCmdExecutor->ExecuteCmd("mv  " + tmpInFile + " " + inFile);
		if (!result)
		{
			resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
			resultViewData.PushMsg(_TEXT("触发失败."));
			break;
		}


	} while (false);
	ctx->m_objSshCmdExecutor->DisconnectAndFree();
	ctx->m_theApp->GetMainWnd()->SendMessage(MSG_WRITE_MSG2_LISTVIEW, 0, (LPARAM)&resultViewData);
}
void TriggerVcLongInterCdr(ModuleContext *ctx, void *ptr)
{
	CString testNumber = ctx->m_funcGetProperty(_common, TEST_NUMBER);
	std::string hostName = CommonUtil::CStringToString(ctx->m_funcGetProperty(_common, IP_ADDR), CP_ACP);
	std::string userName = CommonUtil::CStringToString(ctx->m_funcGetProperty(_common, HOST_USERNAME), CP_ACP);
	std::string userPwd = CommonUtil::CStringToString(ctx->m_funcGetProperty(_common, HOST_USERPWD), CP_ACP);
	CString providerCode = ctx->m_funcGetProperty(_common, USER_PROVIDER_CODE);
	UINT port = 22;

	std::vector < std::string> files = GetFiles(ctx, testNumber, providerCode);
	std::string  tmpInFile = files.at(0);
	std::string  inFile = files.at(1);

	ListViewData resultViewData(testNumber, _TEXT("二批-国际长途【结算】"));

	bool result = true;
	do {
		result = ctx->m_objSshCmdExecutor->ConnectAndInit(hostName, port, userName, userPwd);
		if (!result)
		{
			resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
			resultViewData.PushMsg(_TEXT("触发失败."));
			break;
		}
		result = ctx->m_objSshCmdExecutor->ExecuteCmd("touch " + tmpInFile);
		if (!result)
		{
			resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
			resultViewData.PushMsg(_TEXT("触发失败."));
			break;
		}
		
		std::vector<std::string> contents = BuildCdrContent(_TEXT("CDR_CALL"), _TEXT("CALLING"),
			testNumber, providerCode,
			ctx->m_funcGetProperty(_sett_sec_price, PEER_NUMBER),
			ctx->m_funcGetProperty(_sett_sec_price, DURATION),
			ctx->m_funcGetProperty(_sett_sec_price, DOWNLOAD_VALUE),
			ctx->m_funcGetProperty(_sett_sec_price, UPLOAD_VALUE),
			_TEXT("R_LOCAL"), _TEXT("T_INTERNATIONAL"), FALSE, _TEXT("CHN"), _TEXT("40"),
			ctx->m_funcGetProperty(_common, USER_IMSI),
			ctx->m_funcGetProperty(_common, USER_IMEI),
			ctx->m_funcGetProperty(_common, USER_HOME_CODE)
		);

		for (auto it : contents)
		{
			result = ctx->m_objSshCmdExecutor->ExecuteCmd("echo '" + it + "' >>" + tmpInFile);
			if (!result)
			{
				resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
				resultViewData.PushMsg(_TEXT("触发失败."));
				break;
			}
		}

		result = ctx->m_objSshCmdExecutor->ExecuteCmd("mv  " + tmpInFile + " " + inFile);
		if (!result)
		{
			resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
			resultViewData.PushMsg(_TEXT("触发失败."));
			break;
		}

		resultViewData.PushMsg(_TEXT("触发成功."));
	} while (false);
	ctx->m_objSshCmdExecutor->DisconnectAndFree();
	ctx->m_theApp->GetMainWnd()->SendMessage(MSG_WRITE_MSG2_LISTVIEW, 0, (LPARAM)&resultViewData);
}
void TriggerVcRoamInterCdr(ModuleContext *ctx, void *ptr)
{
	CString testNumber = ctx->m_funcGetProperty(_common, TEST_NUMBER);
	std::string hostName = CommonUtil::CStringToString(ctx->m_funcGetProperty(_common, IP_ADDR), CP_ACP);
	std::string userName = CommonUtil::CStringToString(ctx->m_funcGetProperty(_common, HOST_USERNAME), CP_ACP);
	std::string userPwd = CommonUtil::CStringToString(ctx->m_funcGetProperty(_common, HOST_USERPWD), CP_ACP);
	CString providerCode = ctx->m_funcGetProperty(_common, USER_PROVIDER_CODE);
	UINT port = 22;

	std::vector < std::string> files = GetFiles(ctx, testNumber, providerCode);
	std::string  tmpInFile = files.at(0);
	std::string  inFile = files.at(1);

	ListViewData resultViewData(testNumber, _TEXT("二批-国际漫游【结算】"));

	bool result = true;
	do {
		result = ctx->m_objSshCmdExecutor->ConnectAndInit(hostName, port, userName, userPwd);
		if (!result)
		{
			resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
			resultViewData.PushMsg(_TEXT("触发失败."));
			break;
		}
		result = ctx->m_objSshCmdExecutor->ExecuteCmd("touch " + tmpInFile);
		if (!result)
		{
			resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
			resultViewData.PushMsg(_TEXT("触发失败."));
			break;
		}
		
		std::vector<std::string> contents = BuildCdrContent(_TEXT("CDR_CALL"), _TEXT("CALLING"),
			testNumber, providerCode,
			ctx->m_funcGetProperty(_sett_sec_price, PEER_NUMBER),
			ctx->m_funcGetProperty(_sett_sec_price, DURATION),
			ctx->m_funcGetProperty(_sett_sec_price, DOWNLOAD_VALUE),
			ctx->m_funcGetProperty(_sett_sec_price, UPLOAD_VALUE),
			_TEXT("R_OUT_INTERNATIONAL"), _TEXT("T_LOCAL"), FALSE, _TEXT("CHN"), _TEXT("40"),
			ctx->m_funcGetProperty(_common, USER_IMSI),
			ctx->m_funcGetProperty(_common, USER_IMEI),
			ctx->m_funcGetProperty(_common, USER_HOME_CODE)
		);

		for (auto it : contents)
		{
			result = ctx->m_objSshCmdExecutor->ExecuteCmd("echo '" + it + "' >>" + tmpInFile);
			if (!result)
			{
				resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
				resultViewData.PushMsg(_TEXT("触发失败."));
				break;
			}
		}

		result = ctx->m_objSshCmdExecutor->ExecuteCmd("mv  " + tmpInFile + " " + inFile);
		if (!result)
		{
			resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
			resultViewData.PushMsg(_TEXT("触发失败."));
			break;
		}

		resultViewData.PushMsg(_TEXT("触发成功."));
	} while (false);
	ctx->m_objSshCmdExecutor->DisconnectAndFree();
	ctx->m_theApp->GetMainWnd()->SendMessage(MSG_WRITE_MSG2_LISTVIEW, 0, (LPARAM)&resultViewData);
}
void TriggerVcIpCallCdr(ModuleContext *ctx, void *ptr)
{
	CString testNumber = ctx->m_funcGetProperty(_common, TEST_NUMBER);
	std::string hostName = CommonUtil::CStringToString(ctx->m_funcGetProperty(_common, IP_ADDR), CP_ACP);
	std::string userName = CommonUtil::CStringToString(ctx->m_funcGetProperty(_common, HOST_USERNAME), CP_ACP);
	std::string userPwd = CommonUtil::CStringToString(ctx->m_funcGetProperty(_common, HOST_USERPWD), CP_ACP);
	CString providerCode = ctx->m_funcGetProperty(_common, USER_PROVIDER_CODE);
	UINT port = 22;

	std::vector < std::string> files = GetFiles(ctx, testNumber, providerCode);
	std::string  tmpInFile = files.at(0);
	std::string  inFile = files.at(1);

	ListViewData resultViewData(testNumber, _TEXT("二批-IP通话【结算】"));

	bool result = true;
	do {
		result = ctx->m_objSshCmdExecutor->ConnectAndInit(hostName, port, userName, userPwd);
		if (!result)
		{
			resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
			resultViewData.PushMsg(_TEXT("触发失败."));
			break;
		}
		result = ctx->m_objSshCmdExecutor->ExecuteCmd("touch " + tmpInFile);
		if (!result)
		{
			resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
			resultViewData.PushMsg(_TEXT("触发失败."));
			break;
		}
		
		std::vector<std::string> contents = BuildCdrContent(_TEXT("CDR_CALL"), _TEXT("CALLING"),
			testNumber, providerCode,
			ctx->m_funcGetProperty(_sett_sec_price, PEER_NUMBER),
			ctx->m_funcGetProperty(_sett_sec_price, DURATION),
			ctx->m_funcGetProperty(_sett_sec_price, DOWNLOAD_VALUE),
			ctx->m_funcGetProperty(_sett_sec_price, UPLOAD_VALUE),
			_TEXT("R_LOCAL"), _TEXT("T_LOCAL"), TRUE, _TEXT("CHN"), _TEXT("40"),
			ctx->m_funcGetProperty(_common, USER_IMSI),
			ctx->m_funcGetProperty(_common, USER_IMEI),
			ctx->m_funcGetProperty(_common, USER_HOME_CODE)
		);

		for (auto it : contents)
		{
			result = ctx->m_objSshCmdExecutor->ExecuteCmd("echo '" + it + "' >>" + tmpInFile);
			if (!result)
			{
				resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
				resultViewData.PushMsg(_TEXT("触发失败."));
				break;
			}
		}

		result = ctx->m_objSshCmdExecutor->ExecuteCmd("mv  " + tmpInFile + " " + inFile);
		if (!result)
		{
			resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
			resultViewData.PushMsg(_TEXT("触发失败."));
			break;
		}

		resultViewData.PushMsg(_TEXT("触发成功."));
	} while (false);
	ctx->m_objSshCmdExecutor->DisconnectAndFree();
	ctx->m_theApp->GetMainWnd()->SendMessage(MSG_WRITE_MSG2_LISTVIEW, 0, (LPARAM)&resultViewData);
}
void TriggerVcDivertCdr(ModuleContext *ctx, void *ptr)
{
	CString testNumber = ctx->m_funcGetProperty(_common, TEST_NUMBER);
	std::string hostName = CommonUtil::CStringToString(ctx->m_funcGetProperty(_common, IP_ADDR), CP_ACP);
	std::string userName = CommonUtil::CStringToString(ctx->m_funcGetProperty(_common, HOST_USERNAME), CP_ACP);
	std::string userPwd = CommonUtil::CStringToString(ctx->m_funcGetProperty(_common, HOST_USERPWD), CP_ACP);
	CString providerCode = ctx->m_funcGetProperty(_common, USER_PROVIDER_CODE);
	UINT port = 22;

	std::vector < std::string> files = GetFiles(ctx, testNumber, providerCode);
	std::string  tmpInFile = files.at(0);
	std::string  inFile = files.at(1);

	ListViewData resultViewData(testNumber, _TEXT("二批-呼转通话【结算】"));

	bool result = true;
	do {
		result = ctx->m_objSshCmdExecutor->ConnectAndInit(hostName, port, userName, userPwd);
		if (!result)
		{
			resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
			resultViewData.PushMsg(_TEXT("触发失败."));
			break;
		}
		result = ctx->m_objSshCmdExecutor->ExecuteCmd("touch " + tmpInFile);
		if (!result)
		{
			resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
			resultViewData.PushMsg(_TEXT("触发失败."));
			break;
		}
		
		std::vector<std::string> contents = BuildCdrContent(_TEXT("CDR_CALL"), _TEXT("DIVERTING"),
			testNumber, providerCode,
			ctx->m_funcGetProperty(_sett_sec_price, PEER_NUMBER),
			ctx->m_funcGetProperty(_sett_sec_price, DURATION),
			ctx->m_funcGetProperty(_sett_sec_price, DOWNLOAD_VALUE),
			ctx->m_funcGetProperty(_sett_sec_price, UPLOAD_VALUE),
			_TEXT("R_LOCAL"), _TEXT("T_LOCAL"), TRUE, _TEXT("CHN"), _TEXT("40"),
			ctx->m_funcGetProperty(_common, USER_IMSI),
			ctx->m_funcGetProperty(_common, USER_IMEI),
			ctx->m_funcGetProperty(_common, USER_HOME_CODE)
		);

		for (auto it : contents)
		{
			result = ctx->m_objSshCmdExecutor->ExecuteCmd("echo '" + it + "' >>" + tmpInFile);
			if (!result)
			{
				resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
				resultViewData.PushMsg(_TEXT("触发失败."));
				break;
			}
		}

		result = ctx->m_objSshCmdExecutor->ExecuteCmd("mv  " + tmpInFile + " " + inFile);
		if (!result)
		{
			resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
			resultViewData.PushMsg(_TEXT("触发失败."));
			break;
		}

		resultViewData.PushMsg(_TEXT("触发成功."));
	} while (false);
	ctx->m_objSshCmdExecutor->DisconnectAndFree();
	ctx->m_theApp->GetMainWnd()->SendMessage(MSG_WRITE_MSG2_LISTVIEW, 0, (LPARAM)&resultViewData);
}

void TriggerVcCalledCdr(ModuleContext *ctx, void *ptr)
{
	CString testNumber = ctx->m_funcGetProperty(_common, TEST_NUMBER);
	std::string hostName = CommonUtil::CStringToString(ctx->m_funcGetProperty(_common, IP_ADDR), CP_ACP);
	std::string userName = CommonUtil::CStringToString(ctx->m_funcGetProperty(_common, HOST_USERNAME), CP_ACP);
	std::string userPwd = CommonUtil::CStringToString(ctx->m_funcGetProperty(_common, HOST_USERPWD), CP_ACP);
	CString providerCode = ctx->m_funcGetProperty(_common, USER_PROVIDER_CODE);
	UINT port = 22;

	std::vector < std::string> files = GetFiles(ctx, testNumber, providerCode);
	std::string  tmpInFile = files.at(0);
	std::string  inFile = files.at(1);

	ListViewData resultViewData(testNumber, _TEXT("二批-国内被叫【结算】"));

	bool result = true;
	do {
		result = ctx->m_objSshCmdExecutor->ConnectAndInit(hostName, port, userName, userPwd);
		if (!result)
		{
			resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
			resultViewData.PushMsg(_TEXT("触发失败."));
			break;
		}
		result = ctx->m_objSshCmdExecutor->ExecuteCmd("touch " + tmpInFile);
		if (!result)
		{
			resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
			resultViewData.PushMsg(_TEXT("触发失败."));
			break;
		}
		
		std::vector<std::string> contents = BuildCdrContent(_TEXT("CDR_CALL"), _TEXT("CALLED"),
			testNumber, providerCode,
			ctx->m_funcGetProperty(_sett_sec_price, PEER_NUMBER),
			ctx->m_funcGetProperty(_sett_sec_price, DURATION),
			ctx->m_funcGetProperty(_sett_sec_price, DOWNLOAD_VALUE),
			ctx->m_funcGetProperty(_sett_sec_price, UPLOAD_VALUE),
			_TEXT("R_LOCAL"), _TEXT("T_LOCAL"), TRUE, _TEXT("CHN"), _TEXT("40"),
			ctx->m_funcGetProperty(_common, USER_IMSI),
			ctx->m_funcGetProperty(_common, USER_IMEI),
			ctx->m_funcGetProperty(_common, USER_HOME_CODE)
		);

		for (auto it : contents)
		{
			result = ctx->m_objSshCmdExecutor->ExecuteCmd("echo '" + it + "' >>" + tmpInFile);
			if (!result)
			{
				resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
				resultViewData.PushMsg(_TEXT("触发失败."));
				break;
			}
		}

		result = ctx->m_objSshCmdExecutor->ExecuteCmd("mv  " + tmpInFile + " " + inFile);
		if (!result)
		{
			resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
			resultViewData.PushMsg(_TEXT("触发失败."));
			break;
		}

		resultViewData.PushMsg(_TEXT("触发成功."));
	} while (false);
	ctx->m_objSshCmdExecutor->DisconnectAndFree();
	ctx->m_theApp->GetMainWnd()->SendMessage(MSG_WRITE_MSG2_LISTVIEW, 0, (LPARAM)&resultViewData);
}

void TriggerSmsChnCdr(ModuleContext *ctx, void *ptr)
{
	CString testNumber = ctx->m_funcGetProperty(_common, TEST_NUMBER);
	std::string hostName = CommonUtil::CStringToString(ctx->m_funcGetProperty(_common, IP_ADDR), CP_ACP);
	std::string userName = CommonUtil::CStringToString(ctx->m_funcGetProperty(_common, HOST_USERNAME), CP_ACP);
	std::string userPwd = CommonUtil::CStringToString(ctx->m_funcGetProperty(_common, HOST_USERPWD), CP_ACP);
	CString providerCode = ctx->m_funcGetProperty(_common, USER_PROVIDER_CODE);
	UINT port = 22;

	std::vector < std::string> files = GetFiles(ctx, testNumber, providerCode);
	std::string  tmpInFile = files.at(0);
	std::string  inFile = files.at(1);

	ListViewData resultViewData(testNumber, _TEXT("二批-国内短信【结算】"));

	bool result = true;
	do {
		result = ctx->m_objSshCmdExecutor->ConnectAndInit(hostName, port, userName, userPwd);
		if (!result)
		{
			resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
			resultViewData.PushMsg(_TEXT("触发失败."));
			break;
		}
		result = ctx->m_objSshCmdExecutor->ExecuteCmd("touch " + tmpInFile);
		if (!result)
		{
			resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
			resultViewData.PushMsg(_TEXT("触发失败."));
			break;
		}
	
		std::vector<std::string> contents = BuildCdrContent(_TEXT("CDR_SMS"), _TEXT("CALLING"),
			testNumber, providerCode,
			ctx->m_funcGetProperty(_sett_sec_price, PEER_NUMBER),
			ctx->m_funcGetProperty(_sett_sec_price, DURATION),
			ctx->m_funcGetProperty(_sett_sec_price, DOWNLOAD_VALUE),
			ctx->m_funcGetProperty(_sett_sec_price, UPLOAD_VALUE),
			_TEXT("R_LOCAL"), _TEXT("T_LOCAL"), FALSE, _TEXT("CHN"), _TEXT("40"),
			ctx->m_funcGetProperty(_common, USER_IMSI),
			ctx->m_funcGetProperty(_common, USER_IMEI),
			ctx->m_funcGetProperty(_common, USER_HOME_CODE)
		);

		for (auto it : contents)
		{
			result = ctx->m_objSshCmdExecutor->ExecuteCmd("echo '" + it + "' >>" + tmpInFile);
			if (!result)
			{
				resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
				resultViewData.PushMsg(_TEXT("触发失败."));
				break;
			}
		}

		result = ctx->m_objSshCmdExecutor->ExecuteCmd("mv  " + tmpInFile + " " + inFile);
		if (!result)
		{
			resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
			resultViewData.PushMsg(_TEXT("触发失败."));
			break;
		}

		resultViewData.PushMsg(_TEXT("触发成功."));
	} while (false);
	ctx->m_objSshCmdExecutor->DisconnectAndFree();
	ctx->m_theApp->GetMainWnd()->SendMessage(MSG_WRITE_MSG2_LISTVIEW, 0, (LPARAM)&resultViewData);
}
void TriggerSmsInterCdr(ModuleContext *ctx, void *ptr)
{
	CString testNumber = ctx->m_funcGetProperty(_common, TEST_NUMBER);
	std::string hostName = CommonUtil::CStringToString(ctx->m_funcGetProperty(_common, IP_ADDR), CP_ACP);
	std::string userName = CommonUtil::CStringToString(ctx->m_funcGetProperty(_common, HOST_USERNAME), CP_ACP);
	std::string userPwd = CommonUtil::CStringToString(ctx->m_funcGetProperty(_common, HOST_USERPWD), CP_ACP);
	CString providerCode = ctx->m_funcGetProperty(_common, USER_PROVIDER_CODE);
	UINT port = 22;

	std::vector < std::string> files = GetFiles(ctx, testNumber, providerCode);
	std::string  tmpInFile = files.at(0);
	std::string  inFile = files.at(1);

	ListViewData resultViewData(testNumber, _TEXT("二批-国际短信【结算】"));

	bool result = true;
	do {
		result = ctx->m_objSshCmdExecutor->ConnectAndInit(hostName, port, userName, userPwd);
		if (!result)
		{
			resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
			resultViewData.PushMsg(_TEXT("触发失败."));
			break;
		}
		result = ctx->m_objSshCmdExecutor->ExecuteCmd("touch " + tmpInFile);
		if (!result)
		{
			resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
			resultViewData.PushMsg(_TEXT("触发失败."));
			break;
		}
		
		std::vector<std::string> contents = BuildCdrContent(_TEXT("CDR_SMS"), _TEXT("CALLING"),
			testNumber, providerCode,
			ctx->m_funcGetProperty(_sett_sec_price, PEER_NUMBER),
			ctx->m_funcGetProperty(_sett_sec_price, DURATION),
			ctx->m_funcGetProperty(_sett_sec_price, DOWNLOAD_VALUE),
			ctx->m_funcGetProperty(_sett_sec_price, UPLOAD_VALUE),
			_TEXT("R_LOCAL"), _TEXT("T_INTERNATIONAL"), FALSE, _TEXT("CHN"), _TEXT("40"),
			ctx->m_funcGetProperty(_common, USER_IMSI),
			ctx->m_funcGetProperty(_common, USER_IMEI),
			ctx->m_funcGetProperty(_common, USER_HOME_CODE)
		);

		for (auto it : contents)
		{
			result = ctx->m_objSshCmdExecutor->ExecuteCmd("echo '" + it + "' >>" + tmpInFile);
			if (!result)
			{
				resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
				resultViewData.PushMsg(_TEXT("触发失败."));
				break;
			}
		}

		result = ctx->m_objSshCmdExecutor->ExecuteCmd("mv  " + tmpInFile + " " + inFile);
		if (!result)
		{
			resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
			resultViewData.PushMsg(_TEXT("触发失败."));
			break;
		}

		resultViewData.PushMsg(_TEXT("触发成功."));
	} while (false);
	ctx->m_objSshCmdExecutor->DisconnectAndFree();
	ctx->m_theApp->GetMainWnd()->SendMessage(MSG_WRITE_MSG2_LISTVIEW, 0, (LPARAM)&resultViewData);
}

void TriggerSmsRoamCdr(ModuleContext *ctx, void *ptr)
{
	CString testNumber = ctx->m_funcGetProperty(_common, TEST_NUMBER);
	std::string hostName = CommonUtil::CStringToString(ctx->m_funcGetProperty(_common, IP_ADDR), CP_ACP);
	std::string userName = CommonUtil::CStringToString(ctx->m_funcGetProperty(_common, HOST_USERNAME), CP_ACP);
	std::string userPwd = CommonUtil::CStringToString(ctx->m_funcGetProperty(_common, HOST_USERPWD), CP_ACP);
	CString providerCode = ctx->m_funcGetProperty(_common, USER_PROVIDER_CODE);
	UINT port = 22;

	std::vector < std::string> files = GetFiles(ctx, testNumber, providerCode);
	std::string  tmpInFile = files.at(0);
	std::string  inFile = files.at(1);

	ListViewData resultViewData(testNumber, _TEXT("二批-国际漫游短信【结算】"));

	bool result = true;
	do {
		result = ctx->m_objSshCmdExecutor->ConnectAndInit(hostName, port, userName, userPwd);
		if (!result)
		{
			resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
			resultViewData.PushMsg(_TEXT("触发失败."));
			break;
		}
		result = ctx->m_objSshCmdExecutor->ExecuteCmd("touch " + tmpInFile);
		if (!result)
		{
			resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
			resultViewData.PushMsg(_TEXT("触发失败."));
			break;
		}
		
		std::vector<std::string> contents = BuildCdrContent(_TEXT("CDR_SMS"), _TEXT("CALLING"),
			testNumber, providerCode,
			ctx->m_funcGetProperty(_sett_sec_price, PEER_NUMBER),
			ctx->m_funcGetProperty(_sett_sec_price, DURATION),
			ctx->m_funcGetProperty(_sett_sec_price, DOWNLOAD_VALUE),
			ctx->m_funcGetProperty(_sett_sec_price, UPLOAD_VALUE),
			_TEXT("R_OUT_INTERNATIONAL"), _TEXT("T_LOCAL"), FALSE, _TEXT("CHN"), _TEXT("40"),
			ctx->m_funcGetProperty(_common, USER_IMSI),
			ctx->m_funcGetProperty(_common, USER_IMEI),
			ctx->m_funcGetProperty(_common, USER_HOME_CODE)
		);

		for (auto it : contents)
		{
			result = ctx->m_objSshCmdExecutor->ExecuteCmd("echo '" + it + "' >>" + tmpInFile);
			if (!result)
			{
				resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
				resultViewData.PushMsg(_TEXT("触发失败."));
				break;
			}
		}

		result = ctx->m_objSshCmdExecutor->ExecuteCmd("mv  " + tmpInFile + " " + inFile);
		if (!result)
		{
			resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
			resultViewData.PushMsg(_TEXT("触发失败."));
			break;
		}

		resultViewData.PushMsg(_TEXT("触发成功."));
	} while (false);
	ctx->m_objSshCmdExecutor->DisconnectAndFree();
	ctx->m_theApp->GetMainWnd()->SendMessage(MSG_WRITE_MSG2_LISTVIEW, 0, (LPARAM)&resultViewData);
}

void TriggerSmsCalledCdr(ModuleContext *ctx, void *ptr)
{
	CString testNumber = ctx->m_funcGetProperty(_common, TEST_NUMBER);
	std::string hostName = CommonUtil::CStringToString(ctx->m_funcGetProperty(_common, IP_ADDR), CP_ACP);
	std::string userName = CommonUtil::CStringToString(ctx->m_funcGetProperty(_common, HOST_USERNAME), CP_ACP);
	std::string userPwd = CommonUtil::CStringToString(ctx->m_funcGetProperty(_common, HOST_USERPWD), CP_ACP);
	CString providerCode = ctx->m_funcGetProperty(_common, USER_PROVIDER_CODE);
	UINT port = 22;

	std::vector < std::string> files = GetFiles(ctx, testNumber, providerCode);
	std::string  tmpInFile = files.at(0);
	std::string  inFile = files.at(1);

	ListViewData resultViewData(testNumber, _TEXT("二批-国内被叫短信【结算】"));

	bool result = true;
	do {
		result = ctx->m_objSshCmdExecutor->ConnectAndInit(hostName, port, userName, userPwd);
		if (!result)
		{
			resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
			resultViewData.PushMsg(_TEXT("触发失败."));
			break;
		}
		result = ctx->m_objSshCmdExecutor->ExecuteCmd("touch " + tmpInFile);
		if (!result)
		{
			resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
			resultViewData.PushMsg(_TEXT("触发失败."));
			break;
		}
		
		std::vector<std::string> contents = BuildCdrContent(_TEXT("CDR_SMS"), _TEXT("CALLED"),
			testNumber, providerCode,
			ctx->m_funcGetProperty(_sett_sec_price, PEER_NUMBER),
			ctx->m_funcGetProperty(_sett_sec_price, DURATION),
			ctx->m_funcGetProperty(_sett_sec_price, DOWNLOAD_VALUE),
			ctx->m_funcGetProperty(_sett_sec_price, UPLOAD_VALUE),
			_TEXT("R_LOCAL"), _TEXT("T_LOCAL"), FALSE, _TEXT("CHN"), _TEXT("40"),
			ctx->m_funcGetProperty(_common, USER_IMSI),
			ctx->m_funcGetProperty(_common, USER_IMEI),
			ctx->m_funcGetProperty(_common, USER_HOME_CODE)
		);

		for (auto it : contents)
		{
			result = ctx->m_objSshCmdExecutor->ExecuteCmd("echo '" + it + "' >>" + tmpInFile);
			if (!result)
			{
				resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
				resultViewData.PushMsg(_TEXT("触发失败."));
				break;
			}
		}

		result = ctx->m_objSshCmdExecutor->ExecuteCmd("mv  " + tmpInFile + " " + inFile);
		if (!result)
		{
			resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
			resultViewData.PushMsg(_TEXT("触发失败."));
			break;
		}

		resultViewData.PushMsg(_TEXT("触发成功."));
	} while (false);
	ctx->m_objSshCmdExecutor->DisconnectAndFree();
	ctx->m_theApp->GetMainWnd()->SendMessage(MSG_WRITE_MSG2_LISTVIEW, 0, (LPARAM)&resultViewData);
}

void TriggerDataInProvinceCdr(ModuleContext *ctx, void *ptr)
{
	CString testNumber = ctx->m_funcGetProperty(_common, TEST_NUMBER);
	std::string hostName = CommonUtil::CStringToString(ctx->m_funcGetProperty(_common, IP_ADDR), CP_ACP);
	std::string userName = CommonUtil::CStringToString(ctx->m_funcGetProperty(_common, HOST_USERNAME), CP_ACP);
	std::string userPwd = CommonUtil::CStringToString(ctx->m_funcGetProperty(_common, HOST_USERPWD), CP_ACP);
	CString providerCode = ctx->m_funcGetProperty(_common, USER_PROVIDER_CODE);
	UINT port = 22;

	std::vector < std::string> files = GetFiles(ctx, testNumber, providerCode);
	std::string  tmpInFile = files.at(0);
	std::string  inFile = files.at(1);

	ListViewData resultViewData(testNumber, _TEXT("二批-省内流量【结算】"));

	bool result = true;
	do {
		result = ctx->m_objSshCmdExecutor->ConnectAndInit(hostName, port, userName, userPwd);
		if (!result)
		{
			resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
			resultViewData.PushMsg(_TEXT("触发失败."));
			break;
		}
		result = ctx->m_objSshCmdExecutor->ExecuteCmd("touch " + tmpInFile);
		if (!result)
		{
			resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
			resultViewData.PushMsg(_TEXT("触发失败."));
			break;
		}
		
		std::vector<std::string> contents = BuildCdrContent(_TEXT("CDR_NET"), _TEXT("CALLING"),
			testNumber, providerCode,
			ctx->m_funcGetProperty(_sett_sec_price, PEER_NUMBER),
			ctx->m_funcGetProperty(_sett_sec_price, DURATION),
			ctx->m_funcGetProperty(_sett_sec_price, DOWNLOAD_VALUE),
			ctx->m_funcGetProperty(_sett_sec_price, UPLOAD_VALUE),
			_TEXT("R_LOCAL"), _TEXT("T_LOCAL"), FALSE, _TEXT("CHN"), _TEXT("40"),
			ctx->m_funcGetProperty(_common, USER_IMSI),
			ctx->m_funcGetProperty(_common, USER_IMEI),
			ctx->m_funcGetProperty(_common, USER_HOME_CODE)
		);

		for (auto it : contents)
		{
			result = ctx->m_objSshCmdExecutor->ExecuteCmd("echo '" + it + "' >>" + tmpInFile);
			if (!result)
			{
				resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
				resultViewData.PushMsg(_TEXT("触发失败."));
				break;
			}
		}

		result = ctx->m_objSshCmdExecutor->ExecuteCmd("mv  " + tmpInFile + " " + inFile);
		if (!result)
		{
			resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
			resultViewData.PushMsg(_TEXT("触发失败."));
			break;
		}

		resultViewData.PushMsg(_TEXT("触发成功."));
	} while (false);
	ctx->m_objSshCmdExecutor->DisconnectAndFree();
	ctx->m_theApp->GetMainWnd()->SendMessage(MSG_WRITE_MSG2_LISTVIEW, 0, (LPARAM)&resultViewData);
}
void TriggerDataProvinceCdr(ModuleContext *ctx, void *ptr)
{
	CString testNumber = ctx->m_funcGetProperty(_common, TEST_NUMBER);
	std::string hostName = CommonUtil::CStringToString(ctx->m_funcGetProperty(_common, IP_ADDR), CP_ACP);
	std::string userName = CommonUtil::CStringToString(ctx->m_funcGetProperty(_common, HOST_USERNAME), CP_ACP);
	std::string userPwd = CommonUtil::CStringToString(ctx->m_funcGetProperty(_common, HOST_USERPWD), CP_ACP);
	CString providerCode = ctx->m_funcGetProperty(_common, USER_PROVIDER_CODE);
	UINT port = 22;

	std::vector < std::string> files = GetFiles(ctx, testNumber, providerCode);
	std::string  tmpInFile = files.at(0);
	std::string  inFile = files.at(1);

	ListViewData resultViewData(testNumber, _TEXT("二批-省际流量【结算】"));

	bool result = true;
	do {
		result = ctx->m_objSshCmdExecutor->ConnectAndInit(hostName, port, userName, userPwd);
		if (!result)
		{
			resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
			resultViewData.PushMsg(_TEXT("触发失败."));
			break;
		}
		result = ctx->m_objSshCmdExecutor->ExecuteCmd("touch " + tmpInFile);
		if (!result)
		{
			resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
			resultViewData.PushMsg(_TEXT("触发失败."));
			break;
		}
		
		std::vector<std::string> contents = BuildCdrContent(_TEXT("CDR_NET"), _TEXT("CALLING"),
			testNumber, providerCode,
			ctx->m_funcGetProperty(_sett_sec_price, PEER_NUMBER),
			ctx->m_funcGetProperty(_sett_sec_price, DURATION),
			ctx->m_funcGetProperty(_sett_sec_price, DOWNLOAD_VALUE),
			ctx->m_funcGetProperty(_sett_sec_price, UPLOAD_VALUE),
			_TEXT("R_PROVINCE"), _TEXT("T_LOCAL"), FALSE, _TEXT("CHN"), _TEXT("40"),
			ctx->m_funcGetProperty(_common, USER_IMSI),
			ctx->m_funcGetProperty(_common, USER_IMEI),
			ctx->m_funcGetProperty(_common, USER_HOME_CODE)
		);

		for (auto it : contents)
		{
			result = ctx->m_objSshCmdExecutor->ExecuteCmd("echo '" + it + "' >>" + tmpInFile);
			if (!result)
			{
				resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
				resultViewData.PushMsg(_TEXT("触发失败."));
				break;
			}
		}

		result = ctx->m_objSshCmdExecutor->ExecuteCmd("mv  " + tmpInFile + " " + inFile);
		if (!result)
		{
			resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
			resultViewData.PushMsg(_TEXT("触发失败."));
			break;
		}
		resultViewData.PushMsg(_TEXT("触发成功."));

	} while (false);
	ctx->m_objSshCmdExecutor->DisconnectAndFree();
	ctx->m_theApp->GetMainWnd()->SendMessage(MSG_WRITE_MSG2_LISTVIEW, 0, (LPARAM)&resultViewData);
}
void TriggerDataInterCdr(ModuleContext *ctx, void *ptr)
{

	CString testNumber = ctx->m_funcGetProperty(_common, TEST_NUMBER);
	std::string hostName = CommonUtil::CStringToString(ctx->m_funcGetProperty(_common, IP_ADDR), CP_ACP);
	std::string userName = CommonUtil::CStringToString(ctx->m_funcGetProperty(_common, HOST_USERNAME), CP_ACP);
	std::string userPwd = CommonUtil::CStringToString(ctx->m_funcGetProperty(_common, HOST_USERPWD), CP_ACP);
	CString providerCode = ctx->m_funcGetProperty(_common, USER_PROVIDER_CODE);
	UINT port = 22;

	std::vector < std::string> files = GetFiles(ctx, testNumber, providerCode);
	std::string  tmpInFile = files.at(0);
	std::string  inFile = files.at(1);

	ListViewData resultViewData(testNumber, _TEXT("二批-国际漫游流量【结算】"));

	bool result = true;
	do {
		result = ctx->m_objSshCmdExecutor->ConnectAndInit(hostName, port, userName, userPwd);
		if (!result)
		{
			resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
			resultViewData.PushMsg(_TEXT("触发失败."));
			break;
		}
		result = ctx->m_objSshCmdExecutor->ExecuteCmd("touch " + tmpInFile);
		if (!result)
		{
			resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
			resultViewData.PushMsg(_TEXT("触发失败."));
			break;
		}
		
		std::vector<std::string> contents = BuildCdrContent(_TEXT("CDR_NET"), _TEXT("CALLING"),
			testNumber, providerCode,
			ctx->m_funcGetProperty(_sett_sec_price, PEER_NUMBER),
			ctx->m_funcGetProperty(_sett_sec_price, DURATION),
			ctx->m_funcGetProperty(_sett_sec_price, DOWNLOAD_VALUE),
			ctx->m_funcGetProperty(_sett_sec_price, UPLOAD_VALUE),
			_TEXT("R_OUT_INTERNATIONAL"), _TEXT("T_LOCAL"), FALSE, _TEXT("CHN"), _TEXT("40"),
			ctx->m_funcGetProperty(_common, USER_IMSI),
			ctx->m_funcGetProperty(_common, USER_IMEI),
			ctx->m_funcGetProperty(_common, USER_HOME_CODE)
		);

		for (auto it : contents)
		{
			result = ctx->m_objSshCmdExecutor->ExecuteCmd("echo '" + it + "' >>" + tmpInFile);
			if (!result)
			{
				resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
				resultViewData.PushMsg(_TEXT("触发失败."));
				break;
			}
		}

		result = ctx->m_objSshCmdExecutor->ExecuteCmd("mv  " + tmpInFile + " " + inFile);
		if (!result)
		{
			resultViewData.PushMsg(ctx->m_objSshCmdExecutor->GetErrMsg());
			resultViewData.PushMsg(_TEXT("触发失败."));
			break;
		}

		resultViewData.PushMsg(_TEXT("触发成功."));
	} while (false);
	ctx->m_objSshCmdExecutor->DisconnectAndFree();
	ctx->m_theApp->GetMainWnd()->SendMessage(MSG_WRITE_MSG2_LISTVIEW, 0, (LPARAM)&resultViewData);
}
