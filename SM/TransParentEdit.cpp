// TransparentEdit.cpp : implementation file
//

#include "stdafx.h"
#include "TransParentEdit.h"	

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CLR_EDIT_DISABLE_BK							RGB(212, 208, 200)		//Disable时，编辑框的背景色
#define CLR_EDIT_DISABLE_BORDER_OUTER				RGB(150, 150, 150)		//Disable时，编辑框的外边框颜色
#define CLR_EDIT_DISABLE_BORDER_INNER				RGB(236, 233, 216)		//Disable时，编辑框的内边框颜色

CTransparentEdit::CTransparentEdit()
{
	m_bTransparent = FALSE;
	m_brush.CreateStockObject(NULL_BRUSH);
	m_clrBorder_outer = RGB(10, 92, 142);
	m_clrBorder_inner = RGB(255, 255, 255);
	m_clrBK = RGB(255, 255, 255);
	m_clrText = RGB(10, 92, 142);

	m_pFont = new CFont;
	m_pFont->CreateFont(15, //以逻辑单位方式指定字体的高度  
		0, //以逻辑单位方式指定字体中字符的平均宽度  
		0, //指定偏离垂线和X轴在显示面上的夹角（单位：0.1度）  
		0, //指定字符串基线和X轴之间的夹角（单位：0.1度）  
		FW_NORMAL, //指定字体磅数  
		FALSE, //是不是斜体  
		FALSE, //加不加下划线  
		0, //指定是否是字体字符突出  
		ANSI_CHARSET, //指定字体的字符集  
		OUT_DEFAULT_PRECIS, //指定所需的输出精度  
		CLIP_DEFAULT_PRECIS, //指定所需的剪贴精度  
		DEFAULT_QUALITY, //指示字体的输出质量  
		DEFAULT_PITCH | FF_SWISS, //指定字体的间距和家族  
		_T("宋体") //指定字体的字样名称  
	);
}

CTransparentEdit::~CTransparentEdit()
{
	m_brush.DeleteObject();
}

BEGIN_MESSAGE_MAP(CTransparentEdit, CEdit)
	//{{AFX_MSG_MAP(CTransparentEdit)
	ON_WM_CTLCOLOR_REFLECT()
	ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
	ON_CONTROL_REFLECT(EN_KILLFOCUS, OnKillFocus)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
HBRUSH CTransparentEdit::CtlColor(CDC* pDC, UINT nCtlColor)
{
	CRect rcCli;

	GetClientRect(rcCli);
	pDC->SetBkMode(TRANSPARENT);

	CBrush brBorder(m_clrBorder_outer);
	RECT boderRect(rcCli);
	boderRect.left = rcCli.left - 2;
	boderRect.top = rcCli.top - 2;
	boderRect.right = rcCli.right + 2;
	boderRect.bottom = rcCli.bottom + 2;
	pDC->FrameRect(&boderRect, &brBorder);	//设置外边框颜色
	brBorder.DeleteObject();

	boderRect.left = rcCli.left - 1;			//设置内边框颜色 
	boderRect.top = rcCli.top - 1;
	boderRect.right = rcCli.right + 1;
	boderRect.bottom = rcCli.bottom + 1;
	CBrush brInnerBorder(m_clrBorder_inner);
	pDC->FrameRect(&boderRect, &brInnerBorder);
	brInnerBorder.DeleteObject();
	CFont* pOldFont;
	if (m_pFont)
	{
		pOldFont = pDC->SelectObject(m_pFont);
	}
	if (IsWindowEnabled())
	{
		if (!m_bTransparent)
			pDC->FillSolidRect(&rcCli, m_clrBK);
		pDC->SetTextColor(m_clrText);		//设置字体颜色
	}
	else
	{
		pDC->FillSolidRect(&rcCli, CLR_EDIT_DISABLE_BK);

		CBrush brBorder(CLR_EDIT_DISABLE_BORDER_OUTER);
		RECT boderRect(rcCli);
		boderRect.left = rcCli.left - 2;
		boderRect.top = rcCli.top - 2;
		boderRect.right = rcCli.right + 2;
		boderRect.bottom = rcCli.bottom + 2;
		pDC->FrameRect(&boderRect, &brBorder); //设置外边框颜色
		brBorder.DeleteObject();

		boderRect.left = rcCli.left - 1;
		boderRect.top = rcCli.top - 1;
		boderRect.right = rcCli.right + 1;
		boderRect.bottom = rcCli.bottom + 1;
		CBrush brInnerBorder(CLR_EDIT_DISABLE_BORDER_INNER);
		pDC->FrameRect(&boderRect, &brInnerBorder);
		brInnerBorder.DeleteObject();
	}
	return m_brush;
}

void CTransparentEdit::OnChange()
{
	::PostMessage(GetParent()->m_hWnd, MSG_EDIT_CHANGE, (WPARAM)this, 0);
	InvalidRcInP();
}

void CTransparentEdit::OnKillFocus()
{
	InvalidRcInP();
}

void CTransparentEdit::OnLButtonDown(UINT nFlags, CPoint point)
{
	InvalidRcInP();
	CEdit::OnLButtonDown(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void CTransparentEdit::InvalidRcInP()
{
	CRect rcCliInP;

	GetClientRect(rcCliInP);

	ClientToScreen(rcCliInP);

	GetParent()->ScreenToClient(rcCliInP);

	GetParent()->InvalidateRect(&rcCliInP);
}

void CTransparentEdit::Enable(BOOL _b)
{
	EnableWindow(_b);
	InvalidRcInP();
}

void CTransparentEdit::SetTranparent(BOOL _bTransparent, COLORREF _clrBk)
{
	if (!(m_bTransparent = _bTransparent)) { m_clrBK = _clrBk; }
	InvalidRcInP();
}

void CTransparentEdit::SetClrBorder(COLORREF outerclr, COLORREF innerClr)
{
	m_clrBorder_outer = outerclr;
	m_clrBorder_inner = innerClr;
	InvalidRcInP();
}

void CTransparentEdit::SetClrText(COLORREF _clr)
{
	m_clrText = _clr;
	InvalidRcInP();
}

void CTransparentEdit::SetText(CString _str)
{
	SetWindowText(_str);
	InvalidRcInP();
}

void CTransparentEdit::SetEditHide(bool isHide)		//设置编辑框控件的隐藏与显示
{
	if (isHide)
	{
		this->ShowWindow(SW_HIDE);
	}
	else
	{
		this->ShowWindow(SW_SHOW);
	}
}
void CTransparentEdit::SetFont(CFont* pFont, BOOL bRedraw)
{
	if (m_pFont)
	{
		delete m_pFont;
	}
	m_pFont = pFont;

	if (bRedraw)
		Invalidate();
}//end func 