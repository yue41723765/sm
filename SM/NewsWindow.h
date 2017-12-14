#pragma once

//û��MFC�������,ʹ��ATL���CString
#ifndef __AFX_H__
#include <atlstr.h>
#endif

#define WM_CONTROLCLICK WM_APP+106

class CNewsWindow
{
public:
	CNewsWindow();
	~CNewsWindow();
public:
	HWND m_hWnd;
public:
	// ����Ƥ��(λͼ��Դ����,�������ı���ɫ)
	BOOL SetSkin(LPCTSTR lpSkinBitmapName, COLORREF CaptionColor = 0x000000);
	// ��������
	BOOL Create(LPCTSTR lpWindowName, int nWidth = 250, int nHeight = 180);
	BOOL Create(LPCTSTR lpClassName, LPCTSTR lpWindowName, int nWidth, int nHeight);
	// �������ű��⡢���ݡ�����
	BOOL SetNews(LPCTSTR lpNewsTitle, LPCTSTR lpNewsContent, LPCTSTR lpNewsURL);
	// ��ʾ����
	void Show();
	// ����Ϊ������,���������ٺ���˳���Ϣѭ��
	void SetMainWindow(BOOL bMainWindow = TRUE);
	// �����Զ��ر�,���Ϊ��,������Ӻ󴰿��Զ��ر�
	void SetAutoClose(BOOL bAutoClose = TRUE);
	// ���ùر�
	void SetWindowClose(BOOL bAutoClose = TRUE);
public:
	BOOL IsWindow();
	BOOL DestroyWindow();
private:
#define NCT_CLOSE	0
#define NCT_VIEW	1
#define NCT_TITLE	2
#define NCT_CONTENT	3

	typedef struct tagNEWSCONTROL
	{
		CString		strText;
		int			nType;
		int			x;
		int			y;
		int			nWidth;
		int			nHeight;
		RECT		Rect;
	}NEWSCONTROL, *LPNEWSCONTROL;
private:
	LPNEWSCONTROL m_pControls;
	int m_nControlCount;
private:
	HCURSOR m_hArrowCursor;
	HCURSOR m_hHandCursor;
	HCURSOR m_hCurCursor;

	HICON m_hAppSmallIcon;

	HDC m_hSkinDC;
	HDC m_hCacheDC;
	HBITMAP m_hSkinBitmap;
	HBITMAP m_hSkinOldBitmap;
	HBITMAP m_hCacheBitmap;
	HBITMAP m_hCacheOldBitmap;
	HFONT m_hFont;
	HFONT m_hBoldFont;
	COLORREF m_CaptionColor;
	int m_nHoverIndex;
	int m_nDownIndex;
	BOOL m_bMainWindow;
	BOOL m_bAutoClose;
	BOOL m_bTracking;
	CString m_strURL;
private:
	BOOL DrawWindow();
	BOOL DrawWindowEx();
	void DrawButton(HDC hDC, LPNEWSCONTROL pControl);
	void DrawStatic(HDC hDC, LPNEWSCONTROL pControl);
	void DrawNineRect(HDC hdcDest, RECT DestRect, RECT SrcRect, RECT NineRect, UINT crTransparent = 0xFF00FF);
	RECT CreateRect(LONG left, LONG top, LONG right, LONG bottom);
	void CreateControl(LPNEWSCONTROL pControl, int nType, int x, int y, int nWidth, int nHeight, CString strText = _T(""));
	int ControlFromPoint(POINT pt);
	int ControlFromPoint(LPARAM lParam);
	void SetCursor(HCURSOR hCursor);
public:
	LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT OnEraseBkgnd(UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT OnPaint(UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT OnMouseMove(UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT OnMouseHover(UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT OnMouseLeave(UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT OnLButtonDown(UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT OnLButtonUp(UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT OnControlClick(UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT OnDestroy(UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT OnSetCursor(UINT message, WPARAM wParam, LPARAM lParam);
};

