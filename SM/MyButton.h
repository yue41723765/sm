#pragma once

class CMyButton : public CButton
{
		DECLARE_DYNAMIC(CMyButton)

	public:
		CMyButton();
		virtual ~CMyButton();

	protected:
		DECLARE_MESSAGE_MAP()
	public:
		//����������ɫ��������ɫ��������ɫ���޶�����ɫ�� ��ť����
		void Init(COLORREF clrFont, COLORREF clrBKFocus, COLORREF clrBKUnfocus, COLORREF clrBKSelected, COLORREF clrBKDisable, CString  clrText);
		void SetFont(CFont* pFont, BOOL bRedraw = TRUE);

		virtual BOOL Create(LPCTSTR lpszCaption, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
		virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	private:
		int         m_nCtrlState;
		CFont*      m_pFont;

		COLORREF    m_clrFont;
		COLORREF    m_clrBKFocus;
		COLORREF    m_clrBKUnfocus;
		COLORREF    m_clrBKSelected;
		COLORREF    m_clrBKDisable;
		COLORREF m_DownColor;//��ťɫ
		COLORREF m_UpColor;   //ǳ��ɫ
		COLORREF m_TextColor;//��ɫ
		CString     m_text;
		bool        m_bTracking;

		void        DrawRoundRect(CDC &dc, int radius, CRect rect, COLORREF clrBK);
	public:
		afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
		afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
		afx_msg void OnMouseHover(UINT nFlags, CPoint point);
		afx_msg void OnMouseLeave();
		afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	};
