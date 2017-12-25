// LOGIN.cpp : 实现文件
//

#include "stdafx.h"
#include "SM.h"
#include "LOGIN.h"
#include "afxdialogex.h"
#include "WininetHttp.h"
#include <Winbase.h> 
#include "SMDlg.h"

// CLOGIN 对话框

IMPLEMENT_DYNAMIC(CLOGIN, CDialog)
WininetHttp http;
CLOGIN::CLOGIN(CWnd* pParent /*=NULL*/)
	: CDialog(CLOGIN::IDD, pParent)
{

	//对话框里显示的东西
	m_username = _T("");
	m_password = _T("");

}

CLOGIN::~CLOGIN()
{
	
}

void CLOGIN::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_USERNAME, m_username);
	DDX_Text(pDX, IDC_PASSWORD, m_password);
	//DDX_Control(pDX, IDC_LOGIN, b_login);
	GetDlgItem(IDC_LOGIN)->SetWindowTextA(_T("强制更改"));
	DDX_Control(pDX, IDC_LOGIN, b_login);
	DDX_Control(pDX, IDC_USERNAME, m_user);
	DDX_Control(pDX, IDC_PASSWORD, m_pass);
}


BEGIN_MESSAGE_MAP(CLOGIN, CDialog)
	ON_BN_CLICKED(IDC_LOGIN, &CLOGIN::OnClickedLogin)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_DRAWITEM()
	ON_EN_CHANGE(IDC_PASSWORD, &CLOGIN::OnEnChangePassword)
END_MESSAGE_MAP()


// CLOGIN 消息处理程序


void CLOGIN::OnClickedLogin()
{
	// TODO: 在此添加控件通知处理程序代码
	//m_username m_password
	UpdateData(TRUE);
	string url = "http://117.131.227.94:8045/bpmx/perfRemind/perfRemind/perfRemindinfo/getTaskForExe.ht?username="+ m_username +"&password="+ m_password;
	string res=http.RequestJsonInfo(url);
	int	ress = http.ParseJsonInfo(res);
	CString t;
	t.Format(_T("%d"), ress);
	if (ress == 1)
	{

		//CLOGIN::DestroyWindow();
		WriteProfileString("windows", "name", m_username);
		WriteProfileString("windows", "password", m_password);


		CDialog::OnOK();
		CSMDlg Dlg3;           // 模态构造对话框类CTipDlg的实例   
		Dlg3.DoModal();
	}
	if (ress == 0)
	{
		MessageBox("用户名或密码错误");
	}

}

//背景图片
BOOL CLOGIN::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CBitmap bitmap; //创建位图
	bitmap.LoadBitmap(IDB_BITMAP2); //载入位图
	BITMAP bmp; //构造结构体
	bitmap.GetBitmap(&bmp); //用位图信息填充结构体
	CDC dcCompatible; //CDC类的兼容dc
	dcCompatible.CreateCompatibleDC(pDC);
	dcCompatible.SelectObject(&bitmap); //将位图选入设备描述表
	CRect rect;
	GetClientRect(&rect);
	//pDC->BitBlt(0,0,rect.Width(),rect.Height(),&dcCompatible,.0,0,SRCCOPY);//1:1显示
	pDC->StretchBlt(0,0,rect.Width(),rect.Height(),&dcCompatible,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);//放缩全屏显示
	return TRUE;//这一步不能忘记，不是默认的return
	//return CDialog::OnEraseBkgnd(pDC);
}

//输入框背景颜色 字体颜色
HBRUSH CLOGIN::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{

	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if ((pWnd->GetDlgCtrlID() == IDC_USERNAME) && (nCtlColor == CTLCOLOR_STATIC))
	{
		pDC->SetBkColor(m_TextColor);
		pDC->SetTextColor(m_EditColor);
		//hbr = (HBRUSH)m_brMine;
		//return m_brMine;
	}
	else
	{
		
		hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		return hbr;
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return   HBRUSH(GetStockObject(HOLLOW_BRUSH));
	//return hbr;
}


void CLOGIN::OnEnChangePassword()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


//回车变登录
BOOL CLOGIN::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:    // 屏蔽回车
			CLOGIN::OnClickedLogin();
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
//填充默认数据
BOOL CLOGIN::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//获得EDIT
	LPCSTR str = "TestStr";
	USES_CONVERSION;
	LPCWSTR pwcStr = A2CW(str);
	m_user.SetCueBanner(pwcStr,TRUE);
	m_pass.SetCueBanner(pwcStr, FALSE);

	//颜色赋值
	m_DownColor = RGB(122, 103, 238);//按钮色
	m_UpColor = RGB(106, 90, 238);   //浅紫色
	m_TextColor = RGB(248, 248, 255);//白色
	m_EditColor = RGB(10, 10, 10);   //输入文字
	m_brMine.CreateSolidBrush(m_TextColor); //白底黑字
	b_login.Init(m_TextColor, m_UpColor, m_DownColor, m_DownColor, m_DownColor, "登   录");

	m_user.Enable(true);
	m_user.SetClrBorder(m_DownColor, m_TextColor);
	m_user.SetClrText(RGB(10, 10, 10));  //设置字体颜色
	//m_user.SetText("请输入账号密码");
	m_pass.Enable(true);
	m_pass.SetClrBorder(m_DownColor, m_TextColor);
	m_pass.SetClrText(RGB(10, 10, 10));  //设置字体颜色


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
