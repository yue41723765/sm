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
	//, m_password(_T(""))
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
	DDX_Control(pDX, IDC_LOGIN, b_login);
}


BEGIN_MESSAGE_MAP(CLOGIN, CDialog)
	ON_BN_CLICKED(IDC_LOGIN, &CLOGIN::OnClickedLogin)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
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

//背景图片出错弹bug
BOOL CLOGIN::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	/*CBitmap bitmap; //创建位图
	bitmap.LoadBitmap(IDB_BITMAP1); //载入位图
	BITMAP bmp; //构造结构体
	bitmap.GetBitmap(&bmp); //用位图信息填充结构体
	CDC dcCompatible; //CDC类的兼容dc
	dcCompatible.CreateCompatibleDC(pDC);
	dcCompatible.SelectObject(&bitmap); //将位图选入设备描述表
	CRect rect;
	GetClientRect(&rect);
	//pDC->BitBlt(0,0,rect.Width(),rect.Height(),&dcCompatible,.0,0,SRCCOPY);//1:1显示
	pDC->StretchBlt(0,0,rect.Width(),rect.Height(),&dcCompatible,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);//放缩全屏显示
	*/
	return TRUE;//这一步不能忘记，不是默认的return
	//return CDialog::OnEraseBkgnd(pDC);
}


HBRUSH CLOGIN::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if(nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_BTN || nCtlColor ==  CTLCOLOR_STATIC)//设置窗口背景透明效果，字体颜色

	{   
		pDC->SetBkMode(TRANSPARENT);     //透明
		pDC->SetTextColor(RGB(0,0,0));//静态文本框字体颜色
// 		CFont * cFont=new CFont;  
// 		cFont->CreateFont(8,0,0,0,FW_SEMIBOLD,FALSE,FALSE,0,  ANSI_CHARSET,OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY, DEFAULT_PITCH&FF_SWISS,"Arial"); 
// 		pWnd->SetFont(cFont);

	}   
	// TODO: Return a different brush if the default is not desired

	
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
