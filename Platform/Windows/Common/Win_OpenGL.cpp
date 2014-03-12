/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	Win_OpenGL.cpp - OpenGL context implementation
 *	
 **/

#include <Platform/Common/ApplicationCommon.h>
#include <winuser.h>
#include "Win_OpenGL.h"
#include "Win_Timer.h"

#pragma comment(lib,"opengl32.lib")

extern HINSTANCE g_hInstance;

#define WND_CLASSNAME "Molotok_WinGL"

bool g_bWinGLClassRegistered = false;

using namespace ME_Framework::ME_OpenGLBackend;

// I know what I'm doing
#pragma warning(disable:4244)

/*
 *	Window procedure for our context
 **/
LRESULT _stdcall WinGL_WindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{

	CWinOpenGLContext * pCont = (CWinOpenGLContext*)GetWindowLongPtr(hwnd,GWLP_USERDATA);

	if (pCont)
	{
		return pCont->WindowProc(uMsg,wParam,lParam);
	}else 
		return DefWindowProc(hwnd,uMsg,wParam,lParam);
	
	return 0;
}

/*
 *	Class registration routine
 **/
void CWinOpenGLContext::RegisterWindowClass()
{
	m_Wc.style                = CS_HREDRAW |  CS_OWNDC;
	m_Wc.lpfnWndProc          = (WNDPROC)WinGL_WindowProc;
	m_Wc.cbClsExtra           = 0;
	m_Wc.cbWndExtra           = 0;
	m_Wc.hInstance            = g_hInstance;
	m_Wc.hIcon                = NULL;
	m_Wc.hCursor              = NULL;//LoadCursor(NULL, IDC_ARROW);
	m_Wc.hbrBackground        = NULL;
	m_Wc.lpszMenuName         = NULL;
	m_Wc.lpszClassName        = _T(WND_CLASSNAME);

	if (!RegisterClass(&m_Wc))
	{ 
		Sys_FatalError(_T("Failed to register OpenGL window class!"));
	}

	g_bWinGLClassRegistered = true;
}

/*
 *	Creates OpenGL context
 **/
void CWinOpenGLContext::SetupOpenGLContext()
{
	PIXELFORMATDESCRIPTOR pfd;

	ZeroMemory( &pfd, sizeof( pfd ) );

	pfd.nSize = sizeof( pfd );
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;

	pfd.cColorBits = 32;//params->flags & IE_RGB32 ? 32 : 16;
	pfd.cDepthBits = 32;///params->flags & IE_DEPTH32 ? 32 : 32;
	pfd.cStencilBits = 0;//params->flags & IE_STENCIL ? 8 : 0;

	int iFormat = ChoosePixelFormat( m_hDC, &pfd );
	SetPixelFormat( m_hDC, iFormat, &pfd );

	g_pPlatform->SystemLog()->Log(_T("GL Context created\n"));
	m_hGLRC = wglCreateContext(m_hDC);
		

	Activate();

	glClearColor(0,0,0,1);
	glDepthFunc(GL_LESS);
	glClearDepth(1.0f);
}

/*
 *	Context constructor
 **/
CWinOpenGLContext::CWinOpenGLContext()
{
	m_bAppLaunched = false;

	if (!g_bWinGLClassRegistered)
				RegisterWindowClass();	

	DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	DWORD dwStyle = WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX|WS_MAXIMIZEBOX|WS_THICKFRAME;
	
	const int width = 640;
	const int height = 480;


	// Center around desktop
	RECT rect;

	GetClientRect(GetDesktopWindow(), &rect);
	rect.left = (rect.right/2) - (width/2);
	rect.top = (rect.bottom/2) - (height/2);

	rect.bottom = rect.top + height;
	rect.right = rect.left + width;

	AdjustWindowRectEx(&rect,dwStyle,FALSE,dwExStyle);

	m_hWnd = ::CreateWindowEx(dwExStyle,_T(WND_CLASSNAME),
		_T("Molotok Editor"),
		dwStyle,
		rect.left,rect.top,
		rect.right - rect.left, rect.bottom - rect.top,
		NULL,
		NULL,
		g_hInstance,
		NULL);

	m_hGLRC = 0;
	m_hDC = GetDC(m_hWnd);
	
	SetWindowLongPtr(m_hWnd,GWLP_USERDATA,(LONG_PTR)this);	
	g_pPlatform->SystemLog()->Log(_T("GL Window created\n"));

	SetupOpenGLContext();


	g_pPlatform->SystemLog()->Log(_T("Showing window...\n"));
	ShowWindow(m_hWnd,SW_MAXIMIZE);

	
}

/*
 *	Destructor
 **/
CWinOpenGLContext::~CWinOpenGLContext()
{
	if (m_hGLRC) wglDeleteContext(m_hGLRC);	
}

/*
 *	Activates context 
 **/
void CWinOpenGLContext::Activate()
{
	wglMakeCurrent(GetDC(m_hWnd),m_hGLRC);
}

/*
 *	Resize handler for context
 **/
void CWinOpenGLContext::Resize(ME_Math::Vector2D vNewPos,ME_Math::Vector2D vNewExtents)
{
	m_vWindowExtents = vNewExtents;
	m_vWindowPosition = vNewPos;
}

/*
 *	Window procedure handler
 **/
LRESULT CWinOpenGLContext::WindowProc(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	

	if (TranslateToEvent(uMsg,wParam,lParam))
	{
		ApplicationPumpEvent(m_tmpEvent);
		return TRUE;
	}

	switch(uMsg)
	{
	case WM_SIZE:
		{
			RECT r;
			GetClientRect(m_hWnd,&r);

			
			Resize(ME_Math::Vector2D(r.left,r.top),ME_Math::Vector2D(r.right - r.left,r.bottom - r.top));

			appEvent_t e;
			e.eventid = eventTypes::EV_WINDOW_RESIZE;
			
			if (m_bAppLaunched) 
				ApplicationPumpEvent(e);

			return TRUE;
		}
	case WM_CLOSE:
		PostQuitMessage(0);
		
		break;
			
	}

	return DefWindowProc(m_hWnd,uMsg,wParam,lParam);
}

bool g_bMouseKeys[3];


/*
 *	Main loop of "Graphical" application
 **/
void CWinOpenGLContext::MainLoop()
{
	MSG             msg;
	
	// Synch backend
	GL_SynchBackend();


	// Start application
	FlagApplicationStart();
	ApplicationStart();
	
	float flPrevTime = Sys_TimeElapsed();
	float flFrameTime = 0;

	g_pPlatform->SystemLog()->Log(_T("App started\n"));

	while (1)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (GetMessage(&msg, NULL, 0, 0))
			{
				if (msg.message == WM_QUIT || msg.message == WM_CLOSE)
				{					
					return;
				}

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else 
			{
				return;
			}
		}

		

		flFrameTime = Sys_TimeElapsed() - flPrevTime;	
		flPrevTime = Sys_TimeElapsed();

		g_pPlatform->m_flFrameTime = flFrameTime;

		
		glClear(GL_COLOR_BUFFER_BIT);
			
		// Remove me
		
		// 2D viewport setup =================>
		glMatrixMode (GL_PROJECTION);
		glLoadIdentity ();

		
		glViewport(0,0,m_vWindowExtents.x,m_vWindowExtents.y);
		glOrtho (0, m_vWindowExtents.x, m_vWindowExtents.y, 0, 0, 1);

		glMatrixMode (GL_MODELVIEW);
		glLoadIdentity();

		// <============= 2D Viewport setup

		ApplicationRun(flFrameTime);						

		SwapBuffers(m_hDC);
		

	}
}

/*
 *	Windows message to event translator,
 *	returns true if successes translation and false if not
 **/
bool CWinOpenGLContext::TranslateToEvent(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	// we've fine here
	#pragma  warning(disable:4244)
	if (uMsg == WM_KEYDOWN)
	{
		m_tmpEvent.eventid = eventTypes::EV_KB_KEY_DOWN;
		m_tmpEvent.uParam1 = wParam;
		return true;
	}
	else if (uMsg == WM_KEYUP)
	{
		m_tmpEvent.eventid = eventTypes::EV_KB_KEY_UP;
		m_tmpEvent.uParam1 = wParam;
		return true;
	}
	
	if (uMsg == WM_CHAR)
	{
		m_tmpEvent.eventid = eventTypes::EV_KB_CHAR;
		m_tmpEvent.uParam1 = wParam;
		return true;
	}

	else if (uMsg == WM_LBUTTONDOWN || uMsg == WM_MBUTTONDOWN || uMsg == WM_RBUTTONDOWN)
	{
		m_tmpEvent.eventid = eventTypes::EV_MOUSE_KEY_DOWN;
		
		switch(uMsg)
		{
		case WM_LBUTTONDOWN:
			m_tmpEvent.uParam1 = MKEY_LEFT;
			break;
		case WM_MBUTTONDOWN:
			m_tmpEvent.uParam1 = MKEY_MIDDLE;
			break;
		case WM_RBUTTONDOWN:
			m_tmpEvent.uParam1 = MKEY_RIGHT;
			break;
		}
		
		return true;
	}
	else if (uMsg == WM_LBUTTONUP || uMsg == WM_MBUTTONUP || uMsg == WM_RBUTTONUP)
	{
		m_tmpEvent.eventid = eventTypes::EV_MOUSE_KEY_UP;

		switch(uMsg)
		{
		case WM_LBUTTONUP:
			m_tmpEvent.uParam1 = MKEY_LEFT;
			break;
		case WM_MBUTTONUP:
			m_tmpEvent.uParam1 = MKEY_MIDDLE;
			break;
		case WM_RBUTTONUP:
			m_tmpEvent.uParam1 = MKEY_RIGHT;
			break;
		}

		return true;
	}

	return false;
}

void CWinOpenGLContext::FlagApplicationStart()
{
	m_bAppLaunched = true;
}

void CWinOpenGLContext::FlagApplicationShutdown()
{
	m_bAppLaunched = false;
}
