#include "stdafx.h"
#include "PicButton.h"
// PicButton.cpp : 实现文件
//

// CPicButton

IMPLEMENT_DYNAMIC(CPicButton, CButton)

CPicButton::CPicButton()
{
	m_bOver/*（鼠标位于按钮之上）*/ = FALSE;
	m_bSelected/*（按钮被按下）*/ = FALSE;
	m_bTracking/*（在鼠标按下释放）*/ = FALSE;
	m_bFocus /*按钮为当前焦点*/ = FALSE;
}

CPicButton::~CPicButton()
{
}


BEGIN_MESSAGE_MAP(CPicButton, CButton)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEHOVER()
END_MESSAGE_MAP()



// CPicButton 消息处理程序




void CPicButton::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE | TME_HOVER;
		tme.dwHoverTime = 100;//光标停在按钮上，改变状态的时间，以1毫秒为单位
		m_bTracking = _TrackMouseEvent(&tme);
	}	CButton::OnMouseMove(nFlags, point);

}


void CPicButton::OnMouseLeave()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	m_bOver = FALSE; //   鼠标没位于按钮之上     
	m_bTracking = FALSE;           //鼠标离开按钮       
	InvalidateRect(NULL, FALSE);  //让按钮重画

	CButton::OnMouseLeave();
}


void CPicButton::OnMouseHover(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	m_bOver = TRUE;  //鼠标盘旋在按钮上空
	InvalidateRect(NULL); //重画！

	CButton::OnMouseHover(nFlags, point);
}


void CPicButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CRect rect = lpDrawItemStruct->rcItem;
	CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	int nSaveDC = pDC->SaveDC();
	UINT state = lpDrawItemStruct->itemState;
	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);       //按钮控件DC   
	//根据按钮的状态贴图

	//(以下贴按钮状态图，分以下几种情况来贴，，，和我们平常贴背景图片一样的做法)


	//鼠标停留按钮的状态
	if (m_bOver)
	{

		//（以下按钮贴图，，可以用我们自己以前贴背景图的方法去做）

		CDC MemDC;
		MemDC.CreateCompatibleDC(pDC);
		CBitmap *pOldBmp;
		pOldBmp = MemDC.SelectObject(&m_OverBitmap);//加载停留状态
		pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &MemDC, 0, 0, SRCCOPY);
		CString strText =_T( "确定");
		COLORREF clrOld = pDC->SetTextColor(RGB(115, 115, 115));
		int oldMode = pDC->SetBkMode(TRANSPARENT);
		pDC->DrawText(m_text, -1, &lpDrawItemStruct->rcItem, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		pDC->SetTextColor(RGB(115, 115, 115));
		pDC->SetBkMode(oldMode);
	}

	else
	{

		//初始状态
		CDC MemDC;
		MemDC.CreateCompatibleDC(pDC);
		CBitmap *pOldBmp;
		pOldBmp = MemDC.SelectObject(&m_NormalBitmap);//加载最初状态		
		BITMAP bmp;
		m_NormalBitmap.GetObject(sizeof(bmp), &bmp);
		pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &MemDC, 0, 0, SRCCOPY);
		CString strText = _T("确定");
		COLORREF clrOld = pDC->SetTextColor(m_Color);
		int oldMode = pDC->SetBkMode(TRANSPARENT);
		pDC->DrawText(m_text, -1, &lpDrawItemStruct->rcItem, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		pDC->SetTextColor(m_Color);
		pDC->SetBkMode(oldMode);
	}

	//按钮被按下(选中)
	if (state & ODS_SELECTED)
	{
		CDC MemDC;
		MemDC.CreateCompatibleDC(pDC);
		CBitmap *pOldBmp;
		pOldBmp = MemDC.SelectObject(&m_PressedBitmap);
		BITMAP bmp;
		//pOldBmp->GetBitmap(&bmp);
		m_PressedBitmap.GetObject(sizeof(bmp), &bmp);
		pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &MemDC, 0, 0, SRCCOPY);
		CString strText = _T("确定");
		COLORREF clrOld = pDC->SetTextColor(m_Color);
		int oldMode = pDC->SetBkMode(TRANSPARENT);
		pDC->DrawText(m_text, -1, &lpDrawItemStruct->rcItem, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		pDC->SetTextColor(m_Color);
		pDC->SetBkMode(oldMode);
	}

	// TODO:  添加您的代码以绘制指定项
}
void CPicButton::SetBitmapId(int nOver, int nNormal, int nPressed, int nFocus, CString nText, COLORREF nColor)
{
	m_NormalBitmap.LoadBitmap(nNormal);
	m_PressedBitmap.LoadBitmap(nPressed);
	m_FocusBitmap.LoadBitmap(nFocus);
	m_OverBitmap.LoadBitmap(nOver);
	m_text = nText;
	m_Color=nColor;
}