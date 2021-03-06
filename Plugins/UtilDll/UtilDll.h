// UtilDll.h: UtilDll DLL 的主标头文件
//

#pragma once
#include "stdafx.h"
#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include <string>

// CUtilDllApp
// 有关此类实现的信息，请参阅 UtilDll.cpp
//

class CUtilDllApp : public CWinApp
{
public:
	CUtilDllApp();

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};


class _declspec(dllexport) CommonUtil
{
public:
	/*
	* 获取系统时间 yyyymmddHHMISS
	*/
	static CString GetSysTime();
	/*
	* 获取系统时间 yyyymm
	*/
	static CString GetSysYMTime();
	/*
	* 获取系统时间 yyyymmdd
	*/
	static CString GetSysYMDTime();
	/*
	* 获取系统时间
	*/
	static CString GetSysUtcTime();

	/*
	* 获取当前日期
	*/
	static int CommonUtil::GetCurDay();

	/*
	* 获取系统时间： yyyy-mm-dd HH:MI:SS
	*/
	static CString GetFormtSysTime();

	/*
	* 获取序号
	*/
	static CString GetSerial();
	/*
		CString转换std::string
	*/
	static std::string CStringToString(const CString& src, UINT codepage);
	/*
		std::string转换CString
	*/
	static CString StringToCString(const std::string& src, UINT codepage);

private:
	CommonUtil();
	CommonUtil(const CommonUtil &cu) = delete;
	CommonUtil& operator =(const CommonUtil &cu) = delete;
	CommonUtil(CommonUtil &&cu) = delete;
	CommonUtil& operator =(CommonUtil &&cu) = delete;

};