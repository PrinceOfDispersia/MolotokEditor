/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	Win_OpenGL.h - windows OpenGL context implementation
 *	
 **/

class CWinOpenGLContext: public IOpenGLContext
{
	HWND m_hWnd;
	HGLRC m_hGLRC;
	HDC m_hDC;

	WNDCLASS m_Wc;

	void RegisterWindowClass();
	void SetupOpenGLContext();
public:
	CWinOpenGLContext();
	virtual ~CWinOpenGLContext();

	virtual void Activate();
	virtual void Resize(ME_Math::Vector2D vNewPos,ME_Math::Vector2D vNewExtents);
	virtual void MainLoop();

	LRESULT WindowProc(UINT uMsg,WPARAM wParam,LPARAM lParam);
};