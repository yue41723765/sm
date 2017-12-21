#pragma once
// CPicButton

class CPicButton : public CButton
{
	DECLARE_DYNAMIC(CPicButton)

public:
	CPicButton();
	virtual ~CPicButton();

	void SetBitmapId(int nOver, int nNormal, int nPressed, int nFocus, CString m_text,COLORREF m_Color);//设置图片状态
																	   //int nOver（鼠标在按钮上的状态）,int nNormal（初始状态）,int nPressed（按下状态）,int nFocus（焦点）

	BOOL m_bOver;	//鼠标位于按钮之上时该值为true，反之为flase
	BOOL m_bTracking;	//在鼠标按下没有释放时该值为true
	BOOL m_bSelected;	//按钮被按下是该值为true
	BOOL m_bFocus;	//按钮为当前焦点所在时该值为true

	CBitmap m_NormalBitmap;//加载初始状态的图片
	CBitmap m_PressedBitmap;//按下状态的图片
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


