#pragma once
// CWEComboBox

class CWEComboBox : public CComboBox
{
	DECLARE_DYNAMIC(CWEComboBox)

public:
	enum	STATE { NORMAL = 0, OVER, DOWN, DISABLE };

	CBitmap		m_bmpDownBtn;	//下拉按钮的图片
	STATE		m_stateDownBtn;	//下拉按钮的状态

public:
	CWEComboBox();
	virtual ~CWEComboBox();

protected:

	/**
	*  DrawPicture(CDC* pDC, int nState, CRect rect)
	*					此函数的作用是画位图
	*  @param:			pDC		该参数传入绘图DC
	*  @param:			nState	该参数指定了按钮的状态
	*  @param:			rect	该参数指定欲绘图的区域
	*  @return:        返回值为空
	**/
	void	DrawPicture(CDC* pDC, int nState, CRect rect);

	/**
	*  DrawShowText(CDC* pDC)
	*					此函数的作用是画按钮文字
	*  @param:			pDC		该参数传入绘图DC
	*  @return:        返回值为空
	**/
	void	DrawShowText(CDC* pDC);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCbnSelchange();
};
