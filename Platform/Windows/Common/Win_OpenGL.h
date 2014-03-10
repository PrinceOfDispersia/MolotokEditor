/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	Win_OpenGL.h - windows OpenGL context implementation
 *	
 **/

class CWinOpenGLContext: public IOpenGLContext
{
	friend PlatformEnvironment;
	HWND m_hWnd;
	HGLRC m_hGLRC;
	HDC m_hDC;

	WNDCLASS m_Wc;

	void RegisterWindowClass();
	void SetupOpenGLContext();


	bool TranslateToEvent(UINT uMsg,WPARAM wParam,LPARAM lParam);
	ME_Framework::appEvent_t m_tmpEvent;
	bool m_bAppLaunched;
public:
	CWinOpenGLContext();
	virtual ~CWinOpenGLContext();

	virtual void Activate();
	virtual void Resize(ME_Math::Vector2D vNewPos,ME_Math::Vector2D vNewExtents);
	virtual void MainLoop();

	LRESULT WindowProc(UINT uMsg,WPARAM wParam,LPARAM lParam);
};