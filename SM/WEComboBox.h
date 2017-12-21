#pragma once
// CWEComboBox

class CWEComboBox : public CComboBox
{
	DECLARE_DYNAMIC(CWEComboBox)

public:
	enum	STATE { NORMAL = 0, OVER, DOWN, DISABLE };

	CBitmap		m_bmpDownBtn;	//������ť��ͼƬ
	STATE		m_stateDownBtn;	//������ť��״̬

public:
	CWEComboBox();
	virtual ~CWEComboBox();

protected:

	/**
	*  DrawPicture(CDC* pDC, int nState, CRect rect)
	*					�˺����������ǻ�λͼ
	*  @param:			pDC		�ò��������ͼDC
	*  @param:			nState	�ò���ָ���˰�ť��״̬
	*  @param:			rect	�ò���ָ������ͼ������
	*  @return:        ����ֵΪ��
	**/
	void	DrawPicture(CDC* pDC, int nState, CRect rect);

	/**
	*  DrawShowText(CDC* pDC)
	*					�˺����������ǻ���ť����
	*  @param:			pDC		�ò��������ͼDC
	*  @return:        ����ֵΪ��
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
