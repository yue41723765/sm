#pragma once
// CPicButton

class CPicButton : public CButton
{
	DECLARE_DYNAMIC(CPicButton)

public:
	CPicButton();
	virtual ~CPicButton();

	void SetBitmapId(int nOver, int nNormal, int nPressed, int nFocus, CString m_text,COLORREF m_Color);//����ͼƬ״̬
																	   //int nOver������ڰ�ť�ϵ�״̬��,int nNormal����ʼ״̬��,int nPressed������״̬��,int nFocus�����㣩

	BOOL m_bOver;	//���λ�ڰ�ť֮��ʱ��ֵΪtrue����֮Ϊflase
	BOOL m_bTracking;	//����갴��û���ͷ�ʱ��ֵΪtrue
	BOOL m_bSelected;	//��ť�������Ǹ�ֵΪtrue
	BOOL m_bFocus;	//��ťΪ��ǰ��������ʱ��ֵΪtrue

	CBitmap m_NormalBitmap;//���س�ʼ״̬��ͼƬ
	CBitmap m_PressedBitmap;//����״̬��ͼƬ
	CBitmap m_FocusBitmap;//....
	CBitmap m_OverBitmap;//......
	CString m_text;
	COLORREF m_Color;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
};


