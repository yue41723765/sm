#include "stdafx.h"
#include "MyButton.h"

#define CTRL_NOFOCUS  0x101    
#define CTRL_FOCUS    0x102  
#define CTRL_SELECTED 0x103  
// CMyButton  

IMPLEMENT_DYNAMIC(CMyButton, CButton)

CMyButton::CMyButton()
{
	m_pFont = new CFont;
	m_pFont->CreateFont(15, //���߼���λ��ʽָ������ĸ߶�  
		0, //���߼���λ��ʽָ���������ַ���ƽ�����  
		0, //ָ��ƫ�봹�ߺ�X������ʾ���ϵļнǣ���λ��0.1�ȣ�  
		0, //ָ���ַ������ߺ�X��֮��ļнǣ���λ��0.1�ȣ�  
		FW_NORMAL, //ָ���������  
		FALSE, //�ǲ���б��  
		FALSE, //�Ӳ����»���  
		0, //ָ���Ƿ��������ַ�ͻ��  
		ANSI_CHARSET, //ָ��������ַ���  
		OUT_DEFAULT_PRECIS, //ָ��������������  
		CLIP_DEFAULT_PRECIS, //ָ������ļ�������  
		DEFAULT_QUALITY, //ָʾ������������  
		DEFAULT_PITCH | FF_SWISS, //ָ������ļ��ͼ���  
		_T("����") //ָ���������������  
	);

	m_clrFont = RGB(255, 255, 255);
	m_bTracking = false;
}

CMyButton::~CMyButton()
{
	if (m_pFont != NULL)
	{
		m_pFont->DeleteObject();

		delete m_pFont;
		m_pFont = NULL;
	}
}


BEGIN_MESSAGE_MAP(CMyButton, CButton)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// CMyButton message handlers  




BOOL CMyButton::Create(LPCTSTR lpszCaption, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	return CButton::Create(lpszCaption, dwStyle, rect, pParentWnd, nID);
}


void CMyButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);       //��ť�ؼ�DC    

	COLORREF clrBK = RGB(255, 255, 255);


	if (IsWindowEnabled() == FALSE)
	{
		clrBK = m_clrBKDisable;
	}

	//draw background  
	CRect rect(lpDrawItemStruct->rcItem);
	//dc.FillSolidRect(&rect,clrBK);  
	DrawRoundRect(dc, 5, rect, clrBK);

	//draw text  
	CFont* pOldFont;
	if (m_pFont)
	{
		pOldFont = dc.SelectObject(m_pFont);
	}
	COLORREF clrOld = dc.SetTextColor(m_clrFont);
	CString strText;
	GetWindowText(strText);
	int oldMode = dc.SetBkMode(TRANSPARENT);
	dc.DrawText(strText, -1, &lpDrawItemStruct->rcItem, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	dc.SetBkMode(oldMode);
	dc.SetTextColor(clrOld);
	if (m_pFont)
	{
		dc.SelectObject(pOldFont);
	}
	if (lpDrawItemStruct->itemState&ODS_SELECTED)//�����°�ťʱ�Ĵ���
	{
		clrBK = m_clrBKUnfocus;
		//clrBK = RGB(255, 255, 255);
		DrawRoundRect(dc, 5, rect, clrBK);
		//draw text
		CFont* pOldFont;
		if (m_pFont)
		{
		pOldFont = dc.SelectObject(m_pFont);
		}
		COLORREF clrOld = dc.SetTextColor(m_clrFont);
		CString strText= m_text;
		int oldMode = dc.SetBkMode(TRANSPARENT);
		dc.DrawText(strText, -1, &lpDrawItemStruct->rcItem, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		dc.SetBkMode(oldMode);
		dc.SetTextColor(clrOld);
	}
	else
	{
		clrBK = m_clrBKFocus;
		//clrBK = RGB(0, 0, 255);
		DrawRoundRect(dc, 5, rect, clrBK);
		//draw text
		CFont* pOldFont;
		if (m_pFont)
		{
		pOldFont = dc.SelectObject(m_pFont);
		}
		COLORREF clrOld = dc.SetTextColor(m_clrFont);
		CString strText= m_text;
		int oldMode = dc.SetBkMode(TRANSPARENT);
		dc.DrawText(strText, -1, &lpDrawItemStruct->rcItem, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		dc.SetBkMode(oldMode);
		dc.SetTextColor(clrOld);
	}
	dc.Detach();
}


BOOL CMyButton::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
	//return CButton::OnEraseBkgnd(pDC);  
}

void CMyButton::Init(COLORREF clrFont,
	COLORREF clrBKFocus,
	COLORREF clrBKUnfocus,
	COLORREF clrBKSelected,
	COLORREF clrBKDisable,
	CString  clrText)
{
	SetButtonStyle(BS_OWNERDRAW);

	m_clrFont = clrFont;

	m_clrBKFocus = clrBKFocus;
	m_clrBKUnfocus = clrBKUnfocus;
	m_clrBKSelected = clrBKSelected;
	m_clrBKDisable = clrBKDisable;
	m_text = clrText;

	m_nCtrlState = CTRL_NOFOCUS;
}


void CMyButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_nCtrlState == CTRL_FOCUS)
	{
		m_nCtrlState = CTRL_SELECTED;
		Invalidate();
	}

	CButton::OnLButtonDown(nFlags, point);
}


void CMyButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_nCtrlState == CTRL_SELECTED)
	{
		m_nCtrlState = CTRL_FOCUS;
		Invalidate();
	}

	CButton::OnLButtonUp(nFlags, point);
}


void CMyButton::OnMouseHover(UINT nFlags, CPoint point)
{
	if (m_nCtrlState == CTRL_NOFOCUS)
	{
		m_nCtrlState = CTRL_FOCUS;
		Invalidate();
	}

	m_bTracking = FALSE;

	CButton::OnMouseHover(nFlags, point);
}


void CMyButton::OnMouseLeave()
{
	if (m_nCtrlState != CTRL_NOFOCUS)
	{
		m_nCtrlState = CTRL_NOFOCUS;
		Invalidate();
	}

	m_bTracking = FALSE;

	CButton::OnMouseLeave();
}


void CMyButton::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE | TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme) == TRUE ? true : false;
	}

	CButton::OnMouseMove(nFlags, point);
}

void CMyButton::DrawRoundRect(CDC &dc, int radius, CRect rect, COLORREF clrBK)
{
	POINT pt;
	pt.x = radius;
	pt.y = radius;

	int penSize = 1;
	CPen pen;
	pen.CreatePen(PS_SOLID, penSize, clrBK);
	CPen* hOldPen = dc.SelectObject(&pen);
	dc.RoundRect(&rect, pt);
	dc.SelectObject(hOldPen);
	rect.DeflateRect(CSize(penSize, penSize));//rect.DeflateRect(CSize(GetSystemMetrics(SM_CXEDGE), GetSystemMetrics(SM_CYEDGE)));  
	CBrush brush(clrBK);
	dc.FillRect(rect, &brush);
	brush.DeleteObject();
}

void CMyButton::SetFont(CFont* pFont, BOOL bRedraw)
{
	if (m_pFont)
	{
		delete m_pFont;
	}
	m_pFont = pFont;

	if (bRedraw)
		Invalidate();
}//end func 