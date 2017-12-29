#include "stdafx.h"
#include "PicButton.h"
// PicButton.cpp : ʵ���ļ�
//

// CPicButton

IMPLEMENT_DYNAMIC(CPicButton, CButton)

CPicButton::CPicButton()
{
	m_bOver/*�����λ�ڰ�ť֮�ϣ�*/ = FALSE;
	m_bSelected/*����ť�����£�*/ = FALSE;
	m_bTracking/*������갴���ͷţ�*/ = FALSE;
	m_bFocus /*��ťΪ��ǰ����*/ = FALSE;
}

CPicButton::~CPicButton()
{
}


BEGIN_MESSAGE_MAP(CPicButton, CButton)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEHOVER()
END_MESSAGE_MAP()



// CPicButton ��Ϣ�������




void CPicButton::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE | TME_HOVER;
		tme.dwHoverTime = 100;//���ͣ�ڰ�ť�ϣ��ı�״̬��ʱ�䣬��1����Ϊ��λ
		m_bTracking = _TrackMouseEvent(&tme);
	}	CButton::OnMouseMove(nFlags, point);

}


void CPicButton::OnMouseLeave()
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_bOver = FALSE; //   ���ûλ�ڰ�ť֮��     
	m_bTracking = FALSE;           //����뿪��ť       
	InvalidateRect(NULL, FALSE);  //�ð�ť�ػ�

	CButton::OnMouseLeave();
}


void CPicButton::OnMouseHover(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

	m_bOver = TRUE;  //��������ڰ�ť�Ͽ�
	InvalidateRect(NULL); //�ػ���

	CButton::OnMouseHover(nFlags, point);
}


void CPicButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CRect rect = lpDrawItemStruct->rcItem;
	CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	int nSaveDC = pDC->SaveDC();
	UINT state = lpDrawItemStruct->itemState;
	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);       //��ť�ؼ�DC   
	//���ݰ�ť��״̬��ͼ

	//(��������ť״̬ͼ�������¼����������������������ƽ��������ͼƬһ��������)


	//���ͣ����ť��״̬
	if (m_bOver)
	{

		//�����°�ť��ͼ���������������Լ���ǰ������ͼ�ķ���ȥ����

		CDC MemDC;
		MemDC.CreateCompatibleDC(pDC);
		CBitmap *pOldBmp;
		pOldBmp = MemDC.SelectObject(&m_OverBitmap);//����ͣ��״̬
		pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &MemDC, 0, 0, SRCCOPY);
		CString strText =_T( "ȷ��");
		COLORREF clrOld = pDC->SetTextColor(RGB(115, 115, 115));
		int oldMode = pDC->SetBkMode(TRANSPARENT);
		pDC->DrawText(m_text, -1, &lpDrawItemStruct->rcItem, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		pDC->SetTextColor(RGB(115, 115, 115));
		pDC->SetBkMode(oldMode);
	}

	else
	{

		//��ʼ״̬
		CDC MemDC;
		MemDC.CreateCompatibleDC(pDC);
		CBitmap *pOldBmp;
		pOldBmp = MemDC.SelectObject(&m_NormalBitmap);//�������״̬		
		BITMAP bmp;
		m_NormalBitmap.GetObject(sizeof(bmp), &bmp);
		pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &MemDC, 0, 0, SRCCOPY);
		CString strText = _T("ȷ��");
		COLORREF clrOld = pDC->SetTextColor(m_Color);
		int oldMode = pDC->SetBkMode(TRANSPARENT);
		pDC->DrawText(m_text, -1, &lpDrawItemStruct->rcItem, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		pDC->SetTextColor(m_Color);
		pDC->SetBkMode(oldMode);
	}

	//��ť������(ѡ��)
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
		CString strText = _T("ȷ��");
		COLORREF clrOld = pDC->SetTextColor(m_Color);
		int oldMode = pDC->SetBkMode(TRANSPARENT);
		pDC->DrawText(m_text, -1, &lpDrawItemStruct->rcItem, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		pDC->SetTextColor(m_Color);
		pDC->SetBkMode(oldMode);
	}

	// TODO:  ������Ĵ����Ի���ָ����
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