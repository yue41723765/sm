// LOGIN.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SM.h"
#include "LOGIN.h"
#include "afxdialogex.h"
#include "WininetHttp.h"
#include <Winbase.h> 
#include "SMDlg.h"

// CLOGIN �Ի���

IMPLEMENT_DYNAMIC(CLOGIN, CDialog)
WininetHttp http;
CLOGIN::CLOGIN(CWnd* pParent /*=NULL*/)
	: CDialog(CLOGIN::IDD, pParent)
{

	//�Ի�������ʾ�Ķ���
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
	DDX_Control(pDX, IDC_USERNAME, m_user);
	DDX_Control(pDX, IDC_PASSWORD, m_pass);
	
}


BEGIN_MESSAGE_MAP(CLOGIN, CDialog)
	ON_BN_CLICKED(IDC_LOGIN, &CLOGIN::OnClickedLogin)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_DRAWITEM()
	ON_EN_CHANGE(IDC_PASSWORD, &CLOGIN::OnEnChangePassword)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CLOGIN ��Ϣ�������


void CLOGIN::OnClickedLogin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//m_username m_password
	UpdateData(TRUE);
	string url = _T("http://117.131.227.94:8045/bpmx/perfRemind/perfRemind/perfRemindinfo/getTaskForExe.ht?username=")+ m_username +_T("&password=")+ m_password;

	string res=http.RequestJsonInfo(url);
	int	ress = http.ParseJsonInfo(res);
	CString t;
	t.Format(_T("%d"), ress);
	if (ress == 1)
	{

		//CLOGIN::DestroyWindow();
		WriteProfileString(_T("windows"), _T("name"), m_username);
		WriteProfileString(_T("windows"),_T( "password"), m_password);


		CDialog::OnOK();
		CSMDlg Dlg3;           // ģ̬����Ի�����CTipDlg��ʵ��   
		Dlg3.DoModal();
	}
	if (ress == 0)
	{
		MessageBox(_T("�û������������"));
	}

}

//����ͼƬ
BOOL CLOGIN::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CBitmap bitmap; //����λͼ
	bitmap.LoadBitmap(IDB_BITMAP2); //����λͼ
	BITMAP bmp; //����ṹ��
	bitmap.GetBitmap(&bmp); //��λͼ��Ϣ���ṹ��
	CDC dcCompatible; //CDC��ļ���dc
	dcCompatible.CreateCompatibleDC(pDC);
	dcCompatible.SelectObject(&bitmap); //��λͼѡ���豸������
	CRect rect;
	GetClientRect(&rect);
	pDC->BitBlt(0,0,rect.Width(),rect.Height(),&dcCompatible,0,0,SRCCOPY);//1:1��ʾ
	//pDC->StretchBlt(0,0,rect.Width(),rect.Height(),&dcCompatible,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);//����ȫ����ʾ
	return TRUE;//��һ���������ǣ�����Ĭ�ϵ�return
	//return CDialog::OnEraseBkgnd(pDC);
}

//����򱳾���ɫ ������ɫ
HBRUSH CLOGIN::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����  
	CBrush   m_secbg;
	m_secbg.DeleteObject();
	m_secbg.m_hObject = NULL;
	m_secbg.CreateSolidBrush(RGB(122, 122, 122));
	switch (nCtlColor)
	{
	case CTLCOLOR_STATIC: //�����о�̬�ı��ؼ�������  
	{
		pDC->SetTextColor(RGB(255, 255, 255)); //����������ɫ  
		//pDC->SetBkColor(RGB(0,0,0));  
		pDC->SetBkMode(TRANSPARENT); //���ñ���Ϊ͸��  
		CFont m_Font;
		m_Font.CreateFont(15, //���߼���λ��ʽָ������ĸ߶�  
			0, //���߼���λ��ʽָ���������ַ���ƽ�����  
			0, //ָ��ƫ�봹�ߺ�X������ʾ���ϵļнǣ���λ��0.1�ȣ�  
			0, //ָ���ַ������ߺ�X��֮��ļнǣ���λ��0.1�ȣ�  
			FW_SEMIBOLD, //ָ���������  
			FALSE, //�ǲ���б��  
			FALSE, //�Ӳ����»���  
			0, //ָ���Ƿ��������ַ�ͻ��  
			ANSI_CHARSET, //ָ��������ַ���  
			OUT_DEFAULT_PRECIS, //ָ��������������  
			CLIP_DEFAULT_PRECIS, //ָ������ļ�������  
			DEFAULT_QUALITY, //ָʾ������������  
			DEFAULT_PITCH | FF_SWISS, //ָ������ļ��ͼ���  
			_T("����") //ָ���������������  
		);
		pDC->SelectObject(&m_Font);//����Ϊ15������
		break;
	}

	case CTLCOLOR_EDIT:
	{
		pDC->SetTextColor(RGB(0, 0, 0)); //����������ɫ  
										 //pDC->SetBkColor(RGB(0,0,0));  
		pDC->SetBkColor(RGB(255, 255, 255));
		//pDC->SetBkMode(TRANSPARENT); //���ñ���Ϊ͸��  
		return hbr;
	}
	}

	pDC->SetBkMode(TRANSPARENT); //���ñ���Ϊ͸��  
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������  
	return m_secbg;
}


void CLOGIN::OnEnChangePassword()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


//�س����¼
BOOL CLOGIN::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:    // ���λس�
			CLOGIN::OnClickedLogin();
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
//���Ĭ������
BOOL CLOGIN::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetAutoRun(TRUE);//���ÿ����Զ�����  

	//���EDIT
	LPCSTR str = "TestStr";
	USES_CONVERSION;
	LPCWSTR pwcStr = A2CW(str);

	//��ɫ��ֵ
	m_DownColor = RGB(122, 103, 238);//��ťɫ
	m_UpColor = RGB(106, 90, 238);   //ǳ��ɫ
	m_TextColor = RGB(248, 248, 255);//��ɫ
	m_EditColor = RGB(10, 10, 10);   //��������
	m_brMine.CreateSolidBrush(m_TextColor); //�׵׺���
	b_login.Init(m_TextColor, m_UpColor, m_DownColor, m_DownColor, m_DownColor, _T("��   ¼"));

	m_user.Enable(true);
	m_user.SetClrBorder(m_DownColor, m_TextColor);
	m_user.SetClrText(RGB(10, 10, 10));  //����������ɫ
	//m_user.SetText("�������˺�����");
	m_pass.Enable(true);
	m_pass.SetClrBorder(m_DownColor, m_TextColor);
	m_pass.SetClrText(RGB(10, 10, 10));  //����������ɫ


	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
void CLOGIN::SetAutoRun(BOOL bAutoRun)
{
	HKEY hKey;
	CString strRegPath = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");//�ҵ�ϵͳ��������  
	if (bAutoRun)
	{
		if (RegOpenKeyEx(HKEY_CURRENT_USER, strRegPath, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS) //��������       
		{
			TCHAR szModule[_MAX_PATH];
			GetModuleFileName(NULL, szModule, _MAX_PATH);//�õ������������ȫ·��  
			RegSetValueEx(hKey,_T( "Client"), 0, REG_SZ, (const BYTE*)(LPCSTR)szModule, strlen((LPCSTR)szModule)); //���һ����Key,������ֵ��"Client"��Ӧ�ó������֣����Ӻ�׺.exe��  
			RegCloseKey(hKey); //�ر�ע���  
		}
		else
		{
			AfxMessageBox(_T("ϵͳ��������,������ϵͳ����"));
		}
	}
	else
	{
		if (RegOpenKeyEx(HKEY_CURRENT_USER, strRegPath, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
		{
			RegDeleteValue(hKey, _T("Client"));
			RegCloseKey(hKey);
		}
	}
}

void CLOGIN::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: �ڴ˴������Ϣ����������
	//InvalidateRect(true);
}
