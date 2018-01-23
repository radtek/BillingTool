#include "stdafx.h"

#include "PropertiesWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "BillingTool.h"
#include <map>
#include "PubFunc.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
static void cbTestPhoneChg(CPropertiesWnd *propWnd,std::map<CString, PropertyInfo> &modProp, CString newValue)
{
	if (!gDbConn.connected)
	{
		CString text;
		text.Format(_TEXT("数据库未连接"));
		theApp.GetMainWnd()->SendMessage(MSG_WRITE_MSG2_STATUSBAR, 0, (LPARAM)text.GetBuffer());
		text.ReleaseBuffer();
		return;
	}
	newValue.Trim();

	char strUserId[30] = {0}, strAcctId[30] = { 0 };
	try {
		std::string sql = "select to_char(t1.user_id), to_char(t2.acct_id) "
						" from cs_user_info t1, cs_user_payrelation t2 "
						" where t1.user_id = t2.user_id "
						"	and t2.default_tag = '1' "
						"	and t2.state = '0' "
						"	and sysdate between t2.start_date and t2.end_date "
						"	and serial_number = :v1<char[32]>";


		otl_stream otlStm;
		otlStm.open(1, sql.c_str(), gDbConn);
		otlStm.set_commit(0);

		otlStm << CStringToString(newValue, CP_ACP).c_str();

		otlStm >> strUserId >> strAcctId;

		modProp.at(_TEXT("账户ID")).propertyValue = StringToCString(strAcctId,CP_ACP);
		modProp.at(_TEXT("用户ID")).propertyValue = StringToCString(strUserId, CP_ACP);
	}
	catch (otl_exception &e)
	{
		char strExp[4096] = { 0 };
		sprintf_s(strExp, "code:%d,msg:%s,var_info:%s,stm_text:%s\n", e.code, e.msg, e.var_info, e.stm_text);
		CString exp = StringToCString(strExp, CP_ACP);
		theApp.GetMainWnd()->SendMessage(MSG_WRITE_MSG2_STATUSBAR, 0, (LPARAM)exp.GetBuffer());
		exp.ReleaseBuffer();
	}


	
	propWnd->OnCbnSelChanged();
}

// CResourceViewBar
std::map<int , std::map<CString, PropertyInfo> > gProperties = {
	{ 0/*公共属性*/,	
			{ 
				{_TEXT("IP地址"),{_TEXT("192.168.88.150"),nullptr} },
				{ _TEXT("端口号"),{_TEXT("4444"),nullptr} },
				{ _TEXT("测试号码"),{_TEXT("17001818555"),cbTestPhoneChg } },
				{ _TEXT("账户ID"),{_TEXT("320150105718968"),nullptr} },
				{ _TEXT("用户ID"),{_TEXT("120150105199770"),nullptr} },
				{ _TEXT("数据库连接串"),{_TEXT("cmcc/CMCC@fxzn"),nullptr} },
				{ _TEXT("用户名"),{_TEXT("chengl"),nullptr} },
				{ _TEXT("密码"),{_TEXT("chengl123"),nullptr} }
				
			} 
	},

	{ 1/*信控分发属性*/,
			{ 
				{ _TEXT("触发停机服务地址"),{_TEXT("/creditTrigger"),nullptr} },
				{ _TEXT("话单费用"), {_TEXT("1000"),nullptr} },
				{ _TEXT("话单使用量"), {_TEXT("10"),nullptr} },
				{ _TEXT("截止本条话单前的总是用量"), {_TEXT("100"),nullptr} },
				{ _TEXT("用户总的基础量"), {_TEXT("200"),nullptr} },
				{ _TEXT("信控分发文件入口"), {_TEXT("/home/chengl/src/soCreditDispatch/data/in"),nullptr} },
			} 
	} ,
	{ 2/*帐前调账*/,
		{
			{ _TEXT("调账金额/比例"),{_TEXT("1000"),nullptr} },
			{ _TEXT("账目编码"),{_TEXT("110000"),nullptr} },
			{ _TEXT("生效标识"),{_TEXT("0"),nullptr} }
		}
	},
	{ 3/*帐后调账*/,
		{
			{ _TEXT("账单ID"),{_TEXT("1111111000"),nullptr} },
			{ _TEXT("调账金额/比例"),{_TEXT("1000"),nullptr} },
			{ _TEXT("账目编码"),{_TEXT("110000"),nullptr} },
			{ _TEXT("账单费用"),{_TEXT("10000"),nullptr} },
			{ _TEXT("账单余额"),{_TEXT("100"),nullptr} },
			{ _TEXT("调减余额处理方式"),{_TEXT("0"),nullptr} }
		}
	}
};

CPropertiesWnd::CPropertiesWnd()
{
	m_nComboHeight = 0;
}

CPropertiesWnd::~CPropertiesWnd()
{
}

BEGIN_MESSAGE_MAP(CPropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
	ON_CBN_SELCHANGE(1 , OnCbnSelChanged)
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar 消息处理程序

void CPropertiesWnd::AdjustLayout()
{
	if (GetSafeHwnd () == NULL || (AfxGetMainWnd() != NULL && AfxGetMainWnd()->IsIconic()))
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	m_wndObjectCombo.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), m_nComboHeight, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top + m_nComboHeight , rectClient.Width(), rectClient.Height() -(m_nComboHeight), SWP_NOACTIVATE | SWP_NOZORDER);
}

int CPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 创建组合: 
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndObjectCombo.Create(dwViewStyle, rectDummy, this, 1))
	{
		TRACE0("未能创建属性组合 \n");
		return -1;      // 未能创建
	}
	int idx = 0;
	idx = m_wndObjectCombo.AddString(_T("公共属性"));
	m_wndObjectCombo.SetItemData(idx, 0);
	idx = m_wndObjectCombo.AddString(_T("信控分发"));
	m_wndObjectCombo.SetItemData(idx, 1);
	idx = m_wndObjectCombo.AddString(_T("帐前调账"));
	m_wndObjectCombo.SetItemData(idx, 2);
	idx = m_wndObjectCombo.AddString(_T("帐后调账"));
	m_wndObjectCombo.SetItemData(idx, 3);
	m_wndObjectCombo.SetCurSel(0);

	CRect rectCombo;
	m_wndObjectCombo.GetClientRect (&rectCombo);

	m_nComboHeight = rectCombo.Height();

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("未能创建属性网格\n");
		return -1;      // 未能创建
	}

	InitCommonPropList();

	AdjustLayout();
	return 0;
}

void CPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CPropertiesWnd::OnCbnSelChanged()
{
	DWORD_PTR val =  m_wndObjectCombo.GetItemData(m_wndObjectCombo.GetCurSel());
	m_wndPropList.RemoveAll();
	switch (val)
	{
	case 0://公共属性
		return InitCommonPropList();
	case 1://信控分发属性
		return InitCreditDispatchPropList();
	case 2://帐前调账
		return InitAdjustPropList();
	case 3://账后调账
		return InitAftAdjustPropList();
	}

}


void CPropertiesWnd::InitCommonPropList()
{
	
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();
	CMFCPropertyGridProperty *pCommonGroup = new CMFCPropertyGridProperty(_T("公共属性"));
	for (auto comm : gProperties.at(0))
	{
		pCommonGroup->AddSubItem(new CMFCPropertyGridProperty(comm.first, comm.second.propertyValue, comm.first));
	
	}
	
	
	m_wndPropList.AddProperty(pCommonGroup);

	/*CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(_T("边框"), _T("对话框外框"), _T("其中之一: “无”、“细”、“可调整大小”或“对话框外框”"));
	pProp->AddOption(_T("无"));
	pProp->AddOption(_T("细"));
	pProp->AddOption(_T("可调整大小"));
	pProp->AddOption(_T("对话框外框"));
	pProp->AllowEdit(FALSE);

	pGroup1->AddSubItem(pProp);
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("标题"), (_variant_t) _T("关于"), _T("指定窗口标题栏中显示的文本")));

	

	CMFCPropertyGridProperty* pSize = new CMFCPropertyGridProperty(_T("窗口大小"), 0, TRUE);

	pProp = new CMFCPropertyGridProperty(_T("高度"), (_variant_t) 250l, _T("指定窗口的高度"));
	pProp->EnableSpinControl(TRUE, 50, 300);
	pSize->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty( _T("宽度"), (_variant_t) 150l, _T("指定窗口的宽度"));
	pProp->EnableSpinControl(TRUE, 50, 200);
	pSize->AddSubItem(pProp);

	m_wndPropList.AddProperty(pSize);

	CMFCPropertyGridProperty* pGroup2 = new CMFCPropertyGridProperty(_T("字体"));

	LOGFONT lf;
	CFont* font = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	font->GetLogFont(&lf);

	_tcscpy_s(lf.lfFaceName, _T("宋体, Arial"));

	pGroup2->AddSubItem(new CMFCPropertyGridFontProperty(_T("字体"), lf, CF_EFFECTS | CF_SCREENFONTS, _T("指定窗口的默认字体")));
	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("使用系统字体"), (_variant_t) true, _T("指定窗口使用“MS Shell Dlg”字体")));

	m_wndPropList.AddProperty(pGroup2);

	CMFCPropertyGridProperty* pGroup3 = new CMFCPropertyGridProperty(_T("杂项"));
	pProp = new CMFCPropertyGridProperty(_T("(名称)"), _T("应用程序"));
	pProp->Enable(FALSE);
	pGroup3->AddSubItem(pProp);

	CMFCPropertyGridColorProperty* pColorProp = new CMFCPropertyGridColorProperty(_T("窗口颜色"), RGB(210, 192, 254), NULL, _T("指定默认的窗口颜色"));
	pColorProp->EnableOtherButton(_T("其他..."));
	pColorProp->EnableAutomaticButton(_T("默认"), ::GetSysColor(COLOR_3DFACE));
	pGroup3->AddSubItem(pColorProp);

	static const TCHAR szFilter[] = _T("图标文件(*.ico)|*.ico|所有文件(*.*)|*.*||");
	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("图标"), TRUE, _T(""), _T("ico"), 0, szFilter, _T("指定窗口图标")));

	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("文件夹"), _T("c:\\")));

	m_wndPropList.AddProperty(pGroup3);
	*/
}



void CPropertiesWnd::InitCreditDispatchPropList()
{
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	CMFCPropertyGridProperty *pCreditDispatchGroup = new CMFCPropertyGridProperty(_T("信控分发属性"));
	for (auto comm : gProperties.at(1))
	{
		pCreditDispatchGroup->AddSubItem(new CMFCPropertyGridProperty(comm.first, comm.second.propertyValue, comm.first));

	}

	m_wndPropList.AddProperty(pCreditDispatchGroup);

}

void CPropertiesWnd::InitAdjustPropList()
{
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	CMFCPropertyGridProperty *pCreditDispatchGroup = new CMFCPropertyGridProperty(_T("帐前调账属性"));
	for (auto comm : gProperties.at(2))
	{
		pCreditDispatchGroup->AddSubItem(new CMFCPropertyGridProperty(comm.first, comm.second.propertyValue, comm.first));

	}

	m_wndPropList.AddProperty(pCreditDispatchGroup);
}

void CPropertiesWnd::InitAftAdjustPropList()
{
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	CMFCPropertyGridProperty *pCreditDispatchGroup = new CMFCPropertyGridProperty(_T("账后调账属性"));
	for (auto comm : gProperties.at(3))
	{
		pCreditDispatchGroup->AddSubItem(new CMFCPropertyGridProperty(comm.first, comm.second.propertyValue, comm.first));

	}

	m_wndPropList.AddProperty(pCreditDispatchGroup);
}



void CPropertiesWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus();
}

void CPropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetPropListFont();
}

void CPropertiesWnd::SetPropListFont()
{
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_wndPropList.SetFont(&m_fntPropList);
	m_wndObjectCombo.SetFont(&m_fntPropList);
}


LRESULT CPropertiesWnd::OnPropertyChanged(WPARAM wParam, LPARAM lParam)
{
	CMFCPropertyGridProperty* pProp = (CMFCPropertyGridProperty*)lParam;

	std::map<CString, PropertyInfo> &properties = gProperties.at(m_wndObjectCombo.GetItemData(m_wndObjectCombo.GetCurSel()));
	properties[pProp->GetName()].propertyValue = pProp->GetValue();
	PropertyInfo &tmpProp = properties[pProp->GetName()];
	if (tmpProp.callBack != nullptr)
	{
		tmpProp.callBack(this,properties,pProp->GetValue());
	}
	


	return 0;
}
