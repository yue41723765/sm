
// SM.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "SM.h"
#include "SMDlg.h"
#include "LOGIN.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSMApp

BEGIN_MESSAGE_MAP(CSMApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CSMApp ����

CSMApp::CSMApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CSMApp ����

CSMApp theApp;


// CSMApp ��ʼ��

BOOL CSMApp::InitInstance()
{
	AfxEnableControlContainer();



	//��
	skinppLoadSkin(_T("XP-Metallic.ssk"));//blue.sskΪ�����ļ����µ�Ƥ���ļ�
								   //skinppLoadSkinFromRes();//��̬��Դ���أ� ��Ƥ��.h�ļ����к�����������������

 
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	CLOGIN logindlg;
	if (logindlg.DoModal()==IDOK)
	{
	     CSMDlg csmdlg;
	};

	if (pShellManager != NULL)
 	{
 		delete pShellManager;
 	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

