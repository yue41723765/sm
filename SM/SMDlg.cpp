
// SMDlg.cpp : ʵ���ļ�
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
int sort_column; // ��¼�������
bool method; // ��¼�ȽϷ���
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
int m_minute = 0;//ÿ���û������ѷ���
int time_type = 1;//�������
int record=0;//��¼��һ�ε�ʱ��
int nSize = 0;//��¼������Ŀ����




void CSMDlg::ClistDlg(NMHDR*pNMHDR, LRESULT *pResult)
{

	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO:�ڴ���ӿؼ�֪ͨ����������
	sort_column = pNMLV->iSubItem;//�������
	/*
	POSITION pos = m_list_port.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		MessageBox(_T("������ѡ��һ��"), _T("tip"), MB_ICONEXCLAMATION);
		return;
	}
	*/
	int count = m_list_port.GetItemCount();

	for (int i = 0; i<count; i++)
		m_list_port.SetItemData(i, i); // ÿ�еıȽϹؼ��֣��˴�Ϊ����ţ�������кţ�����������Ϊ�����ȽϺ����ĵ�һ��������


	*pResult = 0;
}


// CSMDlg �Ի���




CSMDlg::CSMDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSMDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	pageSize = 10;
	pageNo = 0;
}

void CSMDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list_port);
	DDX_Control(pDX, IDC_COMBO1, m_chosecom);
	DDX_Control(pDX, IDOK, OKButton);
	DDX_Control(pDX, IDNO, CancelBtn);

	DDX_Control(pDX, IDC_NEXT, btn_next);
	DDX_Control(pDX, IDC_PREVICOUS, btn_previcous);
}

BEGIN_MESSAGE_MAP(CSMDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CSMDlg::ClistDlg)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CSMDlg::OnSelchangeCombo) //ʧЧ��
	ON_MESSAGE(WM_SHOWTASK, OnShowTask) //�رձ�Ϊ��С��
	ON_BN_CLICKED(IDNO, &CSMDlg::OnClickedIdno) //ע����
	ON_WM_TIMER() //��ʱ��
	ON_BN_CLICKED(IDC_PREVICOUS, &CSMDlg::OnBtnPageup)
	ON_BN_CLICKED(IDC_NEXT, &CSMDlg::OnBtnPagedown)
END_MESSAGE_MAP()


// CSMDlg ��Ϣ�������
CString m_name;
string url;
CString loginUrl;
CString m_username;
BOOL CSMDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	pageNo = 1;

	
	//��ȡ�û���
	CString str;
	CString password;
	GetProfileString(_T("windows"), _T("name"), _T(""), str.GetBuffer(254), 1024);
	GetProfileString(_T("windows"), _T("password"), _T(""), password.GetBuffer(254), 1024);
	USES_CONVERSION;
	string name = str;
	m_username = str.GetBuffer(0);
	url = _T("http://117.131.227.94:8045/bpmx/perfRemind/perfRemind/perfRemindinfo/getTaskForExe.ht?username="+ m_username) + _T("&password=" + password.GetBuffer(0)) + password;
	loginUrl= _T("http://117.131.227.94:8045/bpmx/login.ht?username="+ m_username) + _T("&password="+password.GetBuffer(0));
	//�����֪ͨ�б��ȡ����
	DWORD dwStyle = m_list_port.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;
	dwStyle |= LVS_EX_TRACKSELECT;
	m_list_port.SetExtendedStyle(dwStyle);
	m_list_port.InsertColumn(0, _T("���"), LVCFMT_CENTER, 50);
	m_list_port.InsertColumn(1, _T("����"), LVCFMT_LEFT, 150);
	m_list_port.InsertColumn(2, _T("����"), LVCFMT_CENTER, 450);
	m_list_port.InsertColumn(3, _T("����"), LVCFMT_CENTER, 160);
	m_list_port.InsertColumn(3, _T("�û�"), LVCFMT_CENTER, 148);
	m_list_port.SetRowHeigt(35);               //�����и߶�
	m_list_port.SetHeaderHeight(1.6f);          //����ͷ���߶�
	m_list_port.SetHeaderFontHW(16, 0);         //����ͷ������߶�,�Ϳ��,0��ʾȱʡ������Ӧ 
	m_list_port.SetHeaderTextColor(RGB(0, 0, 0)); //����ͷ��������ɫ
	m_list_port.SetHeaderBKColor(255, 255, 255, 0); //����ͷ������ɫ
	m_list_port.SetFontHW(14, 0);                 //��������߶ȣ��Ϳ��,0��ʾȱʡ���

	initData();
	ListShow();
	//��ȡ�˺����һλ�� ����ʱ
	m_name = name.substr(name.length() - 1, name.length()).c_str();
	setData(m_name);
	//����ʱ��
	m_chosecom.InsertString(0,_T("���һСʱ"));
	m_chosecom.InsertString(1, _T("�����Сʱ"));
	m_chosecom.InsertString(2, _T("�����Сʱ"));
	m_chosecom.InsertString(3, _T("������"));
	m_chosecom.SetCurSel(0);

	CSMDlg::SetTimer(1, 1000*60, NULL);

	COLORREF okTextColor = RGB(255, 255, 255);
	COLORREF celextColor = RGB(50, 50, 50);
	OKButton.SetBitmapId(IDB_BULE_OFF, IDB_BULE_OFF, IDB_BULE_ON, IDB_BULE_OFF, _T("�ر����"), okTextColor);
	CancelBtn.SetBitmapId(IDB_WHITE_OFF, IDB_WHITE_OFF, IDB_WHITE_ON, IDB_WHITE_OFF, _T("ע�����"), celextColor);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}
//�����û��ó�������
void CSMDlg::setData(CString m_name)
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

//�������ת������  
wchar_t* changeString(string word)
{
	//�������ת������  
	LPCTSTR lpstr;
	/**/
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
//�����֪ͨ�б���������
void CSMDlg::initData()
{

	m_list_port.DeleteAllItems();
	WininetHttp http;
	string strJsonInfo = http.RequestJsonInfo(url);
	string strMessInfo= http.ParseJsonMess(strJsonInfo);
	Json::Reader reader;                                    //����json��Json::Reader
	Json::Value value;                                        //���Դ�����������
	if (!reader.parse(strMessInfo, value))
	{
		return;
	}
	nSize = value.size();
	for (int i = 0; i < nSize; i++)
	{
		m_list_port.InsertItem(i, _T(""));
	}
	for (int i = 0; i < nSize; i++)
	{
		//����ת�� ����ϱ߷��� ���ִ���д���ϱߵķ����������
		CString number;
		number.Format(_T("%d"), i + 1);
		CString titLp, conLp, takeLp,nameLp;
		string tit = value[i]["title"].asString();
		string con = value[i]["content"].asString();
		string take = value[i]["takeType"].asString();
		string uName = value[i]["username"].asString();
		if (i%2==0)
		{
			for (size_t j = 0; j< 5; j++)
			{
				m_list_port.SetItemColor(j, i, RGB(222, 240, 216));  //���õ�Ԫ��������ɫ
			}
		}
		
		USES_CONVERSION;
		titLp = W2CT(changeString(tit));
		conLp = W2CT(changeString(con));
		takeLp = W2CT(changeString(take));
		nameLp = W2CT(changeString(uName));
		//�б����
		m_list_port.SetItemText(i, 0, number);
		m_list_port.SetItemText(i, 1, titLp);
		m_list_port.SetItemText(i, 2, _T(" ") + conLp);
		m_list_port.SetItemText(i, 3, takeLp);
		m_list_port.SetItemText(i, 4,  nameLp);
	}
}
//��С������ɱ �����̴򿪵ĺ�������
void CSMDlg::DeleteTray()
{
	NOTIFYICONDATA nid;
	nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	nid.hWnd = this->m_hWnd;
	nid.uID = IDR_MAINFRAME;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.uCallbackMessage = WM_SHOWTASK;//�Զ������Ϣ���� 
	nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	strcpy(nid.szTip, _T("֪ͨ"));    //��Ϣ��ʾ��
	//Shell_NotifyIcon(NIM_DELETE, &nid);    //��������ɾ��ͼ�� 
}
//��С������ɱ ���̵ĵ���¼�
 LRESULT CSMDlg::OnShowTask(WPARAM wParam, LPARAM lParam)
{
	 if (wParam != IDR_MAINFRAME)
		 return 1;
	 switch (lParam)
	 {
	 case WM_RBUTTONUP://�Ҽ�����ʱ������ݲ˵�������ֻ��һ�����رա� 
	 {

		 LPPOINT lpoint = new tagPOINT;
		 ::GetCursorPos(lpoint);//�õ����λ�� 
		 CMenu menu;
		 menu.CreatePopupMenu();//����һ������ʽ�˵� 
								//���Ӳ˵���رա������������ϢWM_DESTROY�������ڣ��� 
								//���أ�������������� 
		 menu.AppendMenu(MF_STRING, WM_DESTROY, _T("�ر�"));
		 //ȷ������ʽ�˵���λ�� 
		 menu.TrackPopupMenu(TPM_LEFTALIGN, lpoint->x, lpoint->y, this);
		 //��Դ���� 
		 HMENU hmenu = menu.Detach();
		 menu.DestroyMenu();
		 delete lpoint;
	 }
	 break;
	 case WM_LBUTTONDBLCLK://˫������Ĵ��� 
	 {
		 this->ShowWindow(SW_SHOW);//�򵥵���ʾ���������¶� 
		 DeleteTray();
	 }
	 break;
	 default:
		 break;
	 }
	 return 0;
}
 //��С������ɱ ���ص�����
 void CSMDlg::ToTray()
 {
	 NOTIFYICONDATA nid;
	 nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	 nid.hWnd = this->m_hWnd;
	 nid.uID = IDR_MAINFRAME;
	 nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	 nid.uCallbackMessage = WM_SHOWTASK;//�Զ������Ϣ���� 
	 nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	 strcpy(nid.szTip, _T("֪ͨ"));    //��Ϣ��ʾ�� 
	 Shell_NotifyIcon(NIM_ADD, &nid);    //�����������ͼ�� 
	 ShowWindow(SW_HIDE);    //����������
 }

void CSMDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		
	}
	else if (nID == SC_CLOSE)
	{
		//�رռ���Ϊ����
		ToTray();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}
void  CSMDlg::ShowDialog()
{
	GetActiveWindow()->ShowWindow(SW_SHOW);//�򵥵���ʾ���������¶� 
	//DeleteTray();
}
// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSMDlg::OnPaint()
{
	//ȡ�����㴰��  
	//::SetWindowPos(this->m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù�� ��ʾ
HCURSOR CSMDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//combox����
void CSMDlg::OnSelchangeCombo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int com_num = m_chosecom.GetCurSel();
	switch (com_num)
	{
	case 0: {
			time_type = 1;
			break;
	         }
	case 1:
		time_type = 2; break;
	case 2:
		time_type = 4; break;
	case 3:
	{
		time_type = 0;
		break;
	}
	default: {break; }
		
	}
}


//ע����ť
void CSMDlg::OnClickedIdno()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	WriteProfileString(_T("windows"), _T("name"), _T(""));
	WriteProfileString(_T("windows"),_T( "password"), _T(""));

	CSMDlg::KillTimer(1);
	CDialogEx::OnOK();
	CLOGIN Dlg3;           // ģ̬����Ի�����CTipDlg��ʵ��   
	Dlg3.DoModal();
}


//��ʱ��
void CSMDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
				if (!p_MsgWindow->Create(m_hWnd, _T("֪ͨ")))
				{
					AfxMessageBox(_T("����ʧ�ܣ���򿪴���!")); return;
				}
				p_MsgWindow->SetMsg(_T("�����µ�����"), _T("��"+ num +"��"),loginUrl);
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


//��һҳ
void CSMDlg::OnBtnPageup()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (pageNo == 1)
	{
		AfxMessageBox(_T("���ǵ�һҳ"));
	}
	if (pageNo>1)
	{
		pageNo--;
		m_list_port.DeleteAllItems();
		ListShow();
	}
}

//��һҳ
void CSMDlg::OnBtnPagedown()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	pageNo++;
	if (pageNo > max_page)
	{
		pageNo--;
		AfxMessageBox(_T("�������һҳ"));
		return;
	}
	m_list_port.DeleteAllItems();
	ListShow();
}
//�б�����
void CSMDlg::ListShow() 
{
	COLORREF okTextColor = RGB(255, 255, 255);
	COLORREF celUpColor = RGB(220, 220, 220);
	COLORREF celDownColor = RGB(221, 221, 221);
	COLORREF celextColor = RGB(212, 230, 206);
	//btn_next.Init(okTextColor, celextColor, celDownColor, celDownColor, celDownColor,_T("��һҳ"));
	//btn_previcous.Init(okTextColor, celextColor, celDownColor,celDownColor, celDownColor,_T("��һҳ"));
}
