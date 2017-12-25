
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




//�ȽϺ���
static int CALLBACK MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort) 
{
	//�Ӳ�������ȡ����Ƚ�lc����������
	int row1 =(int) lParam1;
	int row2 = (int)lParam2;

	CListCtrl*lc = (CListCtrl*)lParamSort;
	CString lp1 = lc->GetItemText(row1, sort_column);
	CString lp2 = lc->GetItemText(row2, sort_column);

	//�Ƚϲ�ͬ���У���ͬ�Ƚϣ�ע���¼ǰһ����������һ��Ҫ�෴����
	if (sort_column < 2)
	{
		//int�ͱȽ�
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

	m_list_port.SortItems(MyCompareProc, (DWORD_PTR)&m_list_port);//����ڶ��������ǱȽϺ����ĵ���������

	*pResult = 0;
}


// CSMDlg �Ի���




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
	ON_MESSAGE(WM_SHOWTASK, OnShowTask) //�رձ�Ϊ��С��
	ON_BN_CLICKED(IDNO, &CSMDlg::OnClickedIdno) //ע����
	ON_WM_TIMER() //��ʱ��
END_MESSAGE_MAP()


// CSMDlg ��Ϣ�������
CString m_name;
string url;
string loginUrl;
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


	
	//��ȡ�û���
	CString str;
	CString password;
	GetProfileString("windows", "name", "", str.GetBuffer(254), 1024);
	GetProfileString("windows", "password", "", password.GetBuffer(254), 1024);
	string name = str.GetBuffer(0);
	url = "http://117.131.227.94:8045/bpmx/perfRemind/perfRemind/perfRemindinfo/getRemindsJsonArr.ht?username=" +name+ "&password=" + password.GetBuffer(0);
	loginUrl= "http://117.131.227.94:8045/bpmx/login.ht?username="+name+ "&password="+password.GetBuffer(0);
	//�����֪ͨ�б��ȡ����
	DWORD dwStyle = m_list_port.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;
	m_list_port.SetExtendedStyle(dwStyle);
	m_list_port.InsertColumn(0, _T("���"), LVCFMT_LEFT, 50);
	m_list_port.InsertColumn(1, _T("����"), LVCFMT_LEFT, 150);
	m_list_port.InsertColumn(2, _T("����"), LVCFMT_LEFT, 450);
	m_list_port.InsertColumn(3, _T("����"), LVCFMT_LEFT, 160);
	m_list_port.InsertColumn(3, _T("�û�"), LVCFMT_LEFT, 148);
	m_list_port.SetRowHeigt(35);               //�����и߶�
	m_list_port.SetHeaderHeight(1.6);          //����ͷ���߶�
	m_list_port.SetHeaderFontHW(16, 0);         //����ͷ������߶�,�Ϳ��,0��ʾȱʡ������Ӧ 
	m_list_port.SetHeaderTextColor(RGB(0, 0, 0)); //����ͷ��������ɫ
	m_list_port.SetHeaderBKColor(255, 255, 255, 0); //����ͷ������ɫ
	m_list_port.SetFontHW(14, 0);                 //��������߶ȣ��Ϳ��,0��ʾȱʡ���
	initData();
	//��ȡ�˺����һλ�� ����ʱ
	m_name = name.substr(name.length() - 1, name.length()).c_str();
	setData(m_name);
	//����ʱ��
	m_chosecom.InsertString(0,"���һСʱ");
	m_chosecom.InsertString(1,"�����Сʱ");
	m_chosecom.InsertString(2,"�����Сʱ");
	m_chosecom.InsertString(3,"������");
	//m_chosecom.InsertString(4, m_name);
	m_chosecom.SetCurSel(0);
	

	CSMDlg::SetTimer(1, 1000*60, NULL);


	
	//COLORREF okUpColor = RGB(65, 105, 225);
	//COLORREF okDownColor = RGB(70, 110, 225);
	COLORREF okTextColor = RGB(255, 255, 255);
	//COLORREF celUpColor = RGB(220, 220, 220);
	//COLORREF celDownColor = RGB(221, 221, 221);
	COLORREF celextColor = RGB(50, 50, 50);
	//OKButton.Init(okTextColor, okUpColor, okDownColor, okDownColor, okDownColor,"�ر����");
	//CancelBtn.Init(celextColor, celUpColor, celDownColor,celDownColor, celDownColor,"ע�����");
	OKButton.SetBitmapId(IDB_BULE_OFF, IDB_BULE_OFF, IDB_BULE_ON, IDB_BULE_OFF, "�ر����", okTextColor);
	CancelBtn.SetBitmapId(IDB_WHITE_OFF, IDB_WHITE_OFF, IDB_WHITE_ON, IDB_WHITE_OFF, "ע�����", celextColor);

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
	for (size_t i = 0; i<nSize; i++)
	{
		m_list_port.InsertItem(i, _T(""));
	}
	for (size_t i = 0; i < nSize; i++)
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
		m_list_port.SetItemText(i, 0, " " + number);
		m_list_port.SetItemText(i, 1, " "+titLp);
		m_list_port.SetItemText(i, 2, " " + conLp);
		m_list_port.SetItemText(i, 3, " " + takeLp);
		m_list_port.SetItemText(i, 4, " " + nameLp);
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
	strcpy(nid.szTip, "֪ͨ");    //��Ϣ��ʾ��Ϊ���ƻ��������ѡ� 
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
		 menu.AppendMenu(MF_STRING, WM_DESTROY, "�ر�");
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
	 strcpy(nid.szTip, "֪ͨ");    //��Ϣ��ʾ�� 
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

//ѡ��������� ��ʱ��ʼ
void CSMDlg::OnSelchangeCombo1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		break;
	}
		
	default: {break; }
		
	}
}


//ע����ť
void CSMDlg::OnClickedIdno()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	WriteProfileString("windows", "name", "");
	WriteProfileString("windows", "password", "");

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
				if (!p_MsgWindow->Create(m_hWnd, "֪ͨ"))
				{
					AfxMessageBox("����ʧ�ܣ���򿪴���!"); return;
				}
				p_MsgWindow->SetMsg("�����µ�����", "��"+ num +"��", loginUrl.c_str());
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

