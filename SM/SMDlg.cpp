
// SMDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SM.h"
#include "SMDlg.h"
#include "afxdialogex.h"
#include "MsgWindow.h"
#include "WininetHttp.h"
#include "WinUser.h"
#include <stdlib.h>
#include <stdio.h>
#include "LOGIN.h"
#include <json/json.h>
#include <fstream>
#pragma comment(lib, "Wininet.lib")
#include <tchar.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define WM_SHOWTASK  (WM_USER+001)

NOTIFYICONDATA NotifyIcon;
int sort_column; // 记录点击的列
bool method; // 记录比较方法
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
int m_minute = 0;//每个用户的提醒分钟
int time_type = 1;//间隔分类
int record=0;//记录上一次的时间
int nSize = 0;//记录推送条目数量




//比较函数
static int CALLBACK MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort) 
{
	//从参数中提取所需比较lc的两行数据
	int row1 =(int) lParam1;
	int row2 = (int)lParam2;

	CListCtrl*lc = (CListCtrl*)lParamSort;
	CString lp1 = lc->GetItemText(row1, sort_column);
	CString lp2 = lc->GetItemText(row2, sort_column);

	//比较不同的列，不同比较，注意记录前一次排序方向，下一次要相反排序
	if (sort_column < 2)
	{
		//int型比较
		if (method)
			return atoi(lp1) - atoi(lp2);
		else
			return atoi(lp1) - atoi(lp1);
	}
	else
	{
		if (method)
		{
			return lp1.CompareNoCase(lp2);

		}
		else
		{
			return lp2.CompareNoCase(lp1);
		}
	}
	return 0;
}
void CSMDlg::ClistDlg(NMHDR*pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO:在此添加控件通知处理程序代码
	sort_column = pNMLV->iSubItem;//点击的列
	/*
	POSITION pos = m_list_port.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		MessageBox(_T("请至少选择一项"), _T("tip"), MB_ICONEXCLAMATION);
		return;
	}
	*/
	int count = m_list_port.GetItemCount();

	for (int i = 0; i<count; i++)
		m_list_port.SetItemData(i, i); // 每行的比较关键字，此处为列序号（点击的列号），可以设置为其他比较函数的第一二个参数

	m_list_port.SortItems(MyCompareProc, (DWORD_PTR)&m_list_port);//排序第二个参数是比较函数的第三个参数

	*pResult = 0;
}


// CSMDlg 对话框




CSMDlg::CSMDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSMDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSMDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list_port);
	DDX_Control(pDX, IDC_COMBO1, m_chosecom);
	DDX_Control(pDX, IDOK, OKButton);
	DDX_Control(pDX, IDNO, CancelBtn);
}

BEGIN_MESSAGE_MAP(CSMDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CSMDlg::ClistDlg)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CSMDlg::OnSelchangeCombo1) 
	ON_MESSAGE(WM_SHOWTASK, OnShowTask) //关闭变为最小化
	ON_BN_CLICKED(IDNO, &CSMDlg::OnClickedIdno) //注销键
	ON_WM_TIMER() //定时器
END_MESSAGE_MAP()


// CSMDlg 消息处理程序
CString m_name;
string url;
string loginUrl;
BOOL CSMDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码


	
	//获取用户名
	CString str;
	CString password;
	GetProfileString("windows", "name", "", str.GetBuffer(254), 1024);
	GetProfileString("windows", "password", "", password.GetBuffer(254), 1024);
	string name = str.GetBuffer(0);
	url = "http://117.131.227.94:8045/bpmx/perfRemind/perfRemind/perfRemindinfo/getTaskForExe.ht?username=" +name+ "&password=" + password.GetBuffer(0);
	loginUrl= "http://117.131.227.94:8045/bpmx/login.ht?username="+name+ "&password="+password.GetBuffer(0);
	//这个是通知列表获取数据
	DWORD dwStyle = m_list_port.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;
	m_list_port.SetExtendedStyle(dwStyle);
	m_list_port.InsertColumn(0, _T("编号"), LVCFMT_LEFT, 50);
	m_list_port.InsertColumn(1, _T("标题"), LVCFMT_LEFT, 150);
	m_list_port.InsertColumn(2, _T("内容"), LVCFMT_LEFT, 366);
	m_list_port.InsertColumn(3, _T("类型"), LVCFMT_LEFT, 150);
	m_list_port.SetRowHeigt(35);               //设置行高度
	m_list_port.SetHeaderHeight(1.4);          //设置头部高度
	m_list_port.SetHeaderFontHW(16, 0);         //设置头部字体高度,和宽度,0表示缺省，自适应 
	m_list_port.SetHeaderTextColor(RGB(0, 0, 0)); //设置头部字体颜色
	m_list_port.SetHeaderBKColor(255, 255, 255, 0); //设置头部背景色
	m_list_port.SetFontHW(14, 0);                 //设置字体高度，和宽度,0表示缺省宽度
	initData();
	//截取账号最后一位数 做定时
	m_name = name.substr(name.length() - 1, name.length()).c_str();
	setData();
	//提醒时间
	m_chosecom.InsertString(0, "间隔一小时");
	m_chosecom.InsertString(1,"间隔两小时");
	m_chosecom.InsertString(2,"间隔四小时");
	m_chosecom.InsertString(3,"不提醒");
	//m_chosecom.InsertString(4, m_name);
	m_chosecom.SetCurSel(0);
	

	CSMDlg::SetTimer(1, 1000*60, NULL);


	
	COLORREF okUpColor = RGB(65, 105, 225);
	COLORREF okDownColor = RGB(70, 110, 225);
	COLORREF okTextColor = RGB(255, 255, 255);
	COLORREF celUpColor = RGB(220, 220, 220);
	COLORREF celDownColor = RGB(221, 221, 221);
	COLORREF celextColor = RGB(50, 50, 50);
	OKButton.Init(okTextColor, okUpColor, okDownColor, okDownColor, okDownColor,"关闭软件");
	CancelBtn.Init(celextColor, celUpColor, celDownColor,celDownColor, celDownColor,"注销软件");
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSMDlg::setData()
{
	if (m_name=="0")
	{
		m_minute = 0;
	}
	else if (m_name == "1")
	{
		m_minute = 6;
	}
	else if (m_name == "2")
	{
		m_minute = 12;
	}
	else if (m_name == "3")
	{
		m_minute = 18;
	}
	else if (m_name == "4")
	{
		m_minute = 24;
	}
	else if (m_name == "5")
	{
		m_minute = 30;
	}
	else if (m_name == "6")
	{
		m_minute = 36;
	}
	else if (m_name == "7")
	{
		m_minute = 42;
	}
	else if (m_name == "8")
	{
		m_minute = 48;
	}
	else if (m_name == "9")
	{
		m_minute = 54;
	}
	else
	{
		m_minute = 0;
	}

	
}

//解决中文转码问题  
wchar_t* changeString(string word)
{
	//解决中文转码问题  
	//LPCTSTR lpstr;
	int len = strlen(word.c_str()) + 1;
	char outch[MAX_PATH];
	WCHAR * wChar = new WCHAR[len];
	wChar[0] = 0;
	MultiByteToWideChar(CP_UTF8, 0, word.c_str(), len, wChar, len);
	WideCharToMultiByte(CP_ACP, 0, wChar, len, outch, len, 0, 0);
	delete[] wChar;
	char* pchar = (char*)outch;

	len = strlen(pchar) + 1;
	WCHAR outName[MAX_PATH];
	MultiByteToWideChar(CP_ACP, 0, pchar, len, outName, len);
	
	//USES_CONVERSION;
	//lpstr = W2CT(outName);
	return outName;
}
//这个是通知列表所有数据
void CSMDlg::initData()
{

	WininetHttp http;
	string strJsonInfo = http.RequestJsonInfo(url);
	string strMessInfo= http.ParseJsonMess(strJsonInfo);
	Json::Reader reader;                                    //解析json用Json::Reader
	Json::Value value;                                        //可以代表任意类型
	if (!reader.parse(strMessInfo, value))
	{
		return;
	}
	nSize = value.size();
	for (size_t i = 0; i<nSize; i++)
	{
		m_list_port.InsertItem(i, _T("yiyi"));
	}
	for (size_t i = 0; i < nSize; i++)
	{
		//文字转换 配合上边方法 部分代码写在上边的方法里会乱码
		CString number;
		number.Format(_T("%d"), i + 1);
		LPCTSTR titLp, conLp, takeLp;
		string tit = value[i]["title"].asString();
		string con = value[i]["content"].asString();
		string take = value[i]["takeType"].asString();
		if (i%2==0)
		{
			for (size_t j = 0; j< 4; j++)
			{
				m_list_port.SetItemColor(j, i, RGB(222, 240, 216));  //设置单元格字体颜色
			}
		}
		
		USES_CONVERSION;
		titLp = W2CT(changeString(tit));
		conLp = W2CT(changeString(con));
		takeLp = W2CT(changeString(take));
		//列表填充
		m_list_port.SetItemText(i, 0, number);
		m_list_port.SetItemText(i, 1, titLp);
		m_list_port.SetItemText(i, 2, conLp);
		m_list_port.SetItemText(i, 3, takeLp);
	}
}
//最小化三连杀 从托盘打开的后续工作
void CSMDlg::DeleteTray()
{
	NOTIFYICONDATA nid;
	nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	nid.hWnd = this->m_hWnd;
	nid.uID = IDR_MAINFRAME;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.uCallbackMessage = WM_SHOWTASK;//自定义的消息名称 
	nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	strcpy(nid.szTip, "通知");    //信息提示条为“计划任务提醒” 
	//Shell_NotifyIcon(NIM_DELETE, &nid);    //在托盘区删除图标 
}
//最小化三连杀 托盘的点击事件
 LRESULT CSMDlg::OnShowTask(WPARAM wParam, LPARAM lParam)
{
	 if (wParam != IDR_MAINFRAME)
		 return 1;
	 switch (lParam)
	 {
	 case WM_RBUTTONUP://右键起来时弹出快捷菜单，这里只有一个“关闭” 
	 {

		 LPPOINT lpoint = new tagPOINT;
		 ::GetCursorPos(lpoint);//得到鼠标位置 
		 CMenu menu;
		 menu.CreatePopupMenu();//声明一个弹出式菜单 
								//增加菜单项“关闭”，点击则发送消息WM_DESTROY给主窗口（已 
								//隐藏），将程序结束。 
		 menu.AppendMenu(MF_STRING, WM_DESTROY, "关闭");
		 //确定弹出式菜单的位置 
		 menu.TrackPopupMenu(TPM_LEFTALIGN, lpoint->x, lpoint->y, this);
		 //资源回收 
		 HMENU hmenu = menu.Detach();
		 menu.DestroyMenu();
		 delete lpoint;
	 }
	 break;
	 case WM_LBUTTONDBLCLK://双击左键的处理 
	 {
		 this->ShowWindow(SW_SHOW);//简单的显示主窗口完事儿 
		 DeleteTray();
	 }
	 break;
	 default:
		 break;
	 }
	 return 0;
}
 //最小化三连杀 隐藏到托盘
 void CSMDlg::ToTray()
 {
	 NOTIFYICONDATA nid;
	 nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	 nid.hWnd = this->m_hWnd;
	 nid.uID = IDR_MAINFRAME;
	 nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	 nid.uCallbackMessage = WM_SHOWTASK;//自定义的消息名称 
	 nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	 strcpy(nid.szTip, "通知");    //信息提示条 
	 Shell_NotifyIcon(NIM_ADD, &nid);    //在托盘区添加图标 
	 ShowWindow(SW_HIDE);    //隐藏主窗口
 }

void CSMDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		
	}
	else if (nID == SC_CLOSE)
	{
		//关闭键改为隐藏
		ToTray();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}
void  CSMDlg::ShowDialog()
{
	GetActiveWindow()->ShowWindow(SW_SHOW);//简单的显示主窗口完事儿 
	//DeleteTray();
}
// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSMDlg::OnPaint()
{
	//取消顶层窗口  
	//::SetWindowPos(this->m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSMDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//选择框处理数据 定时开始
void CSMDlg::OnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	int com_num = m_chosecom.GetCurSel();
	CString buf;
	buf.Format("%d", com_num);
	switch (com_num)
	{
	case 0: {
			time_type = 1;
			break;
	         }
	case 1:
		time_type = 2; break;
	case 2:
		//NewsWindow.Show();
		time_type = 4; break;
	case 3:
	{

		time_type = 0;
		break;
	}
		
	default: {break; }
		
	}
}


//注销按钮
void CSMDlg::OnClickedIdno()
{
	// TODO: 在此添加控件通知处理程序代码
	WriteProfileString("windows", "name", "");
	WriteProfileString("windows", "password", "");

	CSMDlg::KillTimer(1);
	CDialogEx::OnOK();
	CLOGIN Dlg3;           // 模态构造对话框类CTipDlg的实例   
	Dlg3.DoModal();
}


//定时器
void CSMDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case 1:
	{
	
		CTime time = CTime::GetCurrentTime();
		int  hour = time.GetHour();
		int  minute = time.GetMinute();
		if (time_type == 0)
		{
			return;
		}
		if (m_minute == minute)
		{
			if (hour - record >= time_type)
			{
				record = hour;
				initData();
				MsgWindow* p_MsgWindow = new MsgWindow;
				p_MsgWindow->SetSkin(MAKEINTRESOURCE(IDB_SKIN_XUNLEI));
				CString num;
				num.Format(_T("%d"), nSize);
				if (!p_MsgWindow->Create(m_hWnd, "通知"))
				{
					AfxMessageBox("推送失败，请打开窗口!"); return;
				}
				p_MsgWindow->SetMsg("您有新的任务", "共"+ num +"条", loginUrl.c_str());
				p_MsgWindow->Show();
			}
		}
		break;
	}
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

