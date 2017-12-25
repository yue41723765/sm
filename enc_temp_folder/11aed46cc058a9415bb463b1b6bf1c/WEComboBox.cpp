#include "stdafx.h"
#include "WEComboBox.h"
#include "SMDlg.h"

// CWEComboBox

IMPLEMENT_DYNAMIC(CWEComboBox, CComboBox)
CWEComboBox::CWEComboBox()
{
	m_bmpDownBtn.LoadBitmap(IDB_DOWN);

	m_stateDownBtn = NORMAL;
}

CWEComboBox::~CWEComboBox()
{
}

void CWEComboBox::DrawPicture(CDC* pDC, int nState, CRect rect)
{
	CBitmap		*pOldBitmap;
	CDC*		pMemDC = new CDC;

	//读位图到内存
	pMemDC->CreateCompatibleDC(pDC);
	pOldBitmap = pMemDC->SelectObject(&m_bmpDownBtn);
	pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), pMemDC, rect.Width()*nState, 0, SRCCOPY);
	pMemDC->SelectObject(pOldBitmap);

	//释放内存
	ReleaseDC(pMemDC);
	delete	pMemDC;
}

void CWEComboBox::DrawShowText(CDC* pDC)
{
	//获取显示文字
	TCHAR	strText[MAX_PATH + 1];
	::GetWindowText(m_hWnd, strText, MAX_PATH);

	if (strText == NULL)
		return;

	CFont*		hOldFont = pDC->SelectObject(GetFont());
	CSize		szExtent = pDC->GetTextExtent(strText, lstrlen(strText));
	int			nMode = pDC->SetBkMode(TRANSPARENT);
	CPoint		pt(2, 2);
	pDC->DrawState(pt, szExtent, strText, DSS_NORMAL, TRUE, 0, (HBRUSH)NULL);
	pDC->SelectObject(hOldFont);
	pDC->SetBkMode(nMode);
}

BEGIN_MESSAGE_MAP(CWEComboBox, CComboBox)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnCbnSelchange)
END_MESSAGE_MAP()



// CWEComboBox 消息处理程序


void CWEComboBox::OnPaint()
{
	CPaintDC		dc(this);
	CRect			rectCombo;
	CBrush			brushInser, brushOuter;
	COMBOBOXINFO	comboInfo;
	CString			strShow;

	//获取控件信息
	GetWindowText(strShow);
	comboInfo.cbSize = sizeof(COMBOBOXINFO);
	GetComboBoxInfo(&comboInfo);

	if (comboInfo.stateButton == 8)
	{
		m_stateDownBtn = DOWN;
	}
	else if (m_stateDownBtn != OVER)
	{
		m_stateDownBtn = NORMAL;
	}

	//设置重绘区域
	GetClientRect(rectCombo);

	//创建画刷
	brushInser.CreateSolidBrush(RGB(222, 240, 216));
	brushOuter.CreateSolidBrush(RGB(222, 240, 216));

	//重绘
	dc.FillRect(rectCombo, &brushInser);
	dc.FrameRect(rectCombo, &brushOuter);
	DrawPicture(&dc, m_stateDownBtn, comboInfo.rcButton);
	DrawShowText(&dc);
}

void CWEComboBox::OnMouseMove(UINT nFlags, CPoint point)
{
	TRACKMOUSEEVENT tme;

	tme.cbSize = sizeof(tme);
	tme.hwndTrack = m_hWnd;
	tme.dwHoverTime = 1;
	tme.dwFlags = TME_LEAVE | TME_HOVER;

	_TrackMouseEvent(&tme);

	CComboBox::OnMouseMove(nFlags, point);
}

LRESULT CWEComboBox::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
	COMBOBOXINFO	comboInfo;
	CRect			rectBtn;
	CPoint			point;

	//获取控件信息
	comboInfo.cbSize = sizeof(COMBOBOXINFO);
	GetComboBoxInfo(&comboInfo);
	//获取按钮区域
	rectBtn = comboInfo.rcButton;
	ClientToScreen(rectBtn);
	//获取鼠标位置
	GetCursorPos(&point);

	if (rectBtn.PtInRect(point))
	{
		m_stateDownBtn = OVER;
	}
	else
	{
		m_stateDownBtn = DOWN;
	}

	Invalidate(FALSE);

	return 1;
}

LRESULT CWEComboBox::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_stateDownBtn = NORMAL;
	Invalidate();

	return 1;
}

void CWEComboBox::OnCbnSelchange()
{
	Invalidate();
}
