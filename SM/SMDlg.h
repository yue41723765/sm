
// SMDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include <iostream>
#include "resource.h"
#include "ListCtrlCl.h"
#include "afxbutton.h"
#include "MyButton.h"
#define MAX_PATH 260
using namespace std;

// CSMDlg �Ի���
class CSMDlg : public CDialogEx
{
// ����
public:
	CSMDlg(CWnd* pParent = NULL);	// ��׼���캯��
// �Ի�������
	enum { IDD = IDD_SM_DIALOG};
	CTime time;
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
// ʵ��
protected:
	HICON m_hIcon;
	void initData();
	void ToTray();
	void DeleteTray();
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg LRESULT OnShowTask(WPARAM wParam, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	
	void setData();
public:
	void ShowDialog();
	afx_msg void ClistDlg(NMHDR *pNMHDR, LRESULT *pResult);
	CListCtrlCl m_list_port;
	CComboBox m_chosecom;
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnClickedIdno();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CMyButton OKButton;
	CMyButton CancelBtn;
};

