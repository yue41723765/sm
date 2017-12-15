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
	//, m_password(_T(""))
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
}


BEGIN_MESSAGE_MAP(CLOGIN, CDialog)
	ON_BN_CLICKED(IDC_LOGIN, &CLOGIN::OnClickedLogin)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_EN_CHANGE(IDC_PASSWORD, &CLOGIN::OnEnChangePassword)
END_MESSAGE_MAP()


// CLOGIN ��Ϣ�������


void CLOGIN::OnClickedLogin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		CSMDlg Dlg3;           // ģ̬����Ի�����CTipDlg��ʵ��   
		Dlg3.DoModal();
	}
	if (ress == 0)
	{
		MessageBox("�û������������");
	}

}

//����ͼƬ����bug
BOOL CLOGIN::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	/*CBitmap bitmap; //����λͼ
	bitmap.LoadBitmap(IDB_BITMAP1); //����λͼ
	BITMAP bmp; //����ṹ��
	bitmap.GetBitmap(&bmp); //��λͼ��Ϣ���ṹ��
	CDC dcCompatible; //CDC��ļ���dc
	dcCompatible.CreateCompatibleDC(pDC);
	dcCompatible.SelectObject(&bitmap); //��λͼѡ���豸������
	CRect rect;
	GetClientRect(&rect);
	//pDC->BitBlt(0,0,rect.Width(),rect.Height(),&dcCompatible,.0,0,SRCCOPY);//1:1��ʾ
	pDC->StretchBlt(0,0,rect.Width(),rect.Height(),&dcCompatible,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);//����ȫ����ʾ
	*/
	return TRUE;//��һ���������ǣ�����Ĭ�ϵ�return
	//return CDialog::OnEraseBkgnd(pDC);
}


HBRUSH CLOGIN::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if(nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_BTN || nCtlColor ==  CTLCOLOR_STATIC)//���ô��ڱ���͸��Ч����������ɫ

	{   
		pDC->SetBkMode(TRANSPARENT);     //͸��
		pDC->SetTextColor(RGB(0,0,0));//��̬�ı���������ɫ
// 		CFont * cFont=new CFont;  
// 		cFont->CreateFont(8,0,0,0,FW_SEMIBOLD,FALSE,FALSE,0,  ANSI_CHARSET,OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY, DEFAULT_PITCH&FF_SWISS,"Arial"); 
// 		pWnd->SetFont(cFont);

	}   
	// TODO: Return a different brush if the default is not desired

	
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return   HBRUSH(GetStockObject(HOLLOW_BRUSH));
	//return hbr;
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
