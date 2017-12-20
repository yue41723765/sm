#pragma once
#include "afxwin.h"
#include "afxbutton.h"
#include "MyButton.h"
// CLOGIN 对话框

class CLOGIN : public CDialog
{
	DECLARE_DYNAMIC(CLOGIN)

public:
	CLOGIN(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLOGIN();

// 对话框数据
	enum { IDD = IDD_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
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
	//三种颜色分别为文字，Button Down的背景颜色，Button Up的背景颜色
	COLORREF m_TextColor, m_DownColor, m_UpColor,m_EditColor;
	virtual BOOL OnInitDialog();
};
