
// SMDlg.h : 头文件
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

// CSMDlg 对话框
class CSMDlg : public CDialogEx
{
// 构造
public:
	CSMDlg(CWnd* pParent = NULL);	// 标准构造函数
// 对话框数据
	enum { IDD = IDD_SM_DIALOG};
	CTime time;
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
// 实现
protected:
	HICON m_hIcon;
	void initData();
	void ToTray();
	void DeleteTray();
	// 生成的消息映射函数
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
	int pageNo; //页数编号
	int pageSize; // 每页显示条数
	int intStart; // 开始记录
	int intEnd; // 结束记录
	int max_page; // 记录最大页数
};

