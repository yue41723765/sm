#pragma once
#include "afxwin.h"
#include "afxbutton.h"
#include "MyButton.h"
// CLOGIN �Ի���

class CLOGIN : public CDialog
{
	DECLARE_DYNAMIC(CLOGIN)

public:
	CLOGIN(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLOGIN();

// �Ի�������
	enum { IDD = IDD_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()
	//afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
public:

	
	afx_msg void OnClickedLogin();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CString	m_password;
	CString	m_username;
	CBrush m_Brush;
	afx_msg void OnEnChangePassword();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CMyButton b_login;
	CBrush m_brMine;
	//������ɫ�ֱ�Ϊ���֣�Button Down�ı�����ɫ��Button Up�ı�����ɫ
	COLORREF m_TextColor, m_DownColor, m_UpColor,m_EditColor;
	virtual BOOL OnInitDialog();
};
