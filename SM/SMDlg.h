
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
#include "PicButton.h"

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
	void setData(CString m_name);
	void ListShow();
public:
	void ShowDialog();
	afx_msg void ClistDlg(NMHDR *pNMHDR, LRESULT *pResult);
	CListCtrlCl m_list_port;
	CComboBox m_chosecom;
	afx_msg void OnSelchangeCombo();
	afx_msg void OnClickedIdno();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CPicButton OKButton;
	CPicButton CancelBtn;
	CMFCButton btn_next;
	CMFCButton btn_previcous;
	afx_msg void OnBtnPageup();
	afx_msg void OnBtnPagedown();
public:
	int pageNo; //ҳ�����
	int pageSize; // ÿҳ��ʾ����
	int intStart; // ��ʼ��¼
	int intEnd; // ������¼
	int max_page; // ��¼���ҳ��
};

