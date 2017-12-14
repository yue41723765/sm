#pragma once


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
public:

	
	afx_msg void OnClickedLogin();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CString	m_password;
	CString	m_username;

	afx_msg void OnEnChangePassword();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
