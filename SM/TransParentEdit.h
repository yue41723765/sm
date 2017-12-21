#if !defined(AFX_TRANSPARENTEDIT_H__75FDCA6B_92CC_4411_B410_77F410387373__INCLUDED_)
#define AFX_TRANSPARENTEDIT_H__75FDCA6B_92CC_4411_B410_77F410387373__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#define MSG_PROSLIDER_CURPOS_UPDATE					(WM_USER + 101)
#define MSG_SCROLLWND_UM_DESTMOVE					(WM_USER + 102)
#define MSG_EDIT_CHANGE								(WM_USER + 103)
#define WM_CXSHADE_RADIO							(WM_USER + 104)

//////////////////////////////////////////////////////////////////////////
//	Timer 101 �C 150
//////////////////////////////////////////////////////////////////////////
#define EV_BASE_SKIN								101
#define EV_SKIN_HSB_1								(EV_BASE_SKIN + 0)
#define EV_SKIN_HSB_2								(EV_BASE_SKIN + 1)
#define EV_SKIN_VSB_1								(EV_BASE_SKIN + 2)
#define EV_SKIN_VSB_2								(EV_BASE_SKIN + 3)
#define EV_SKIN_SB_NOTIFY							(EV_BASE_SKIN + 4)
#define EV_SKIN_SB_DELAY							(EV_BASE_SKIN + 5)
#define EV_SKIN_SW_UPDATE							(EV_BASE_SKIN + 6)


class _declspec(dllexport)  CTransparentEdit : public CEdit
{
public:
	CTransparentEdit();
	~CTransparentEdit();

	void				Enable(BOOL _b); //����Enable����
	void				SetClrBorder(COLORREF _outerclr, COLORREF _innerclr); //���ñ߿���ɫ
	void				SetClrText(COLORREF _clr);  //����������ɫ
	void				SetText(CString _str); //�����ı�
	void				SetTranparent(BOOL _bTransparent, COLORREF _clrBk = RGB(255, 255, 255));//�����Ƿ񱳾�͸��
	void				SetEditHide(bool isHide);												//���ñ༭��ؼ�����������ʾ
	void                SetFont(CFont* pFont, BOOL bRedraw = TRUE);
protected:
	//{{AFX_MSG(CTransparentEdit)
	afx_msg HBRUSH		CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void		OnChange();
	afx_msg void		OnKillFocus();
	afx_msg void		OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CFont * m_pFont;
	CBrush				m_brush;
	COLORREF			m_clrBorder_outer, m_clrBorder_inner, m_clrBK;
	COLORREF			m_clrText;
	BOOL				m_bTransparent;

	void				InvalidRcInP();
};

#endif
