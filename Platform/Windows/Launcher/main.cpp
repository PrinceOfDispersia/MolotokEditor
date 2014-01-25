/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	main.cpp - entry point for windows builds
 *
 **/

#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <Platform/Common/ApplicationCommon.h>
#include "../Common/Win_Timer.h"
#include <vld.h>
#include "../Common/Win_OpenGL.h"

HINSTANCE g_hInstance;

using namespace ME_Framework;
PlatformEnvironment * ME_Framework::g_pPlatform;

/*
 *	Pre-parsed command line
 **/

TCHAR ** g_tArgv;
int g_tArgc;

/*
 *	Splits single line command line into separate strings
 **/
void ParseCommandLine(TCHAR * strOneline)
{
	String str = strOneline;
	Tokenizer t(str);
	

	// Assume that there is no more than MAX_COMMAND_LINE_ARGUMENTS arguments
	g_tArgv = (TCHAR**)g_pPlatform->MemoryPools()->Alloc(sizeof(TCHAR*) * MAX_COMMAND_LINE_ARGUMENTS);

	g_tArgc = 0;
	

	// Read tokens
	while(t.ReadNextToken())
	{
		if (g_tArgc > MAX_COMMAND_LINE_ARGUMENTS)
		{
			Sys_FatalError(VA(_T("Sorry but there is hard coded limit for maximum number of command line arguments - %d.\nIf you really need more - ask authors to increase that limit\n or change MAX_COMMAND_LINE_ARGUMENTS in /Platform/ApplicationConfig.h and rebuild application"),MAX_COMMAND_LINE_ARGUMENTS));
			// And since we've crashed - from this point i don't care about consequences :)
		}

		const String & strParam = t.GetTokenValue();
		g_tArgv[g_tArgc] = (TCHAR*)g_pPlatform->MemoryPools()->Alloc((strParam.Length() + 1) * sizeof(TCHAR));
		_tcscpy(g_tArgv[g_tArgc],strParam);
		g_tArgc++;
	}
	
}

/*
 *	Frees parsed command line
 **/
void FreeParsedCommandLine()
{
	for(int i = 0 ; i < g_tArgc ; i++)
	{
		g_pPlatform->MemoryPools()->Free(g_tArgv[i]);
	}

	g_pPlatform->MemoryPools()->Free(g_tArgv);
}

/*
 *	Search for give argument index, -1 if not found
 **/
const int PlatformEnvironment::FindCmdLineArgument(TCHAR * strParam) const
{
	for(int i = 0 ; i < g_tArgc ; i++)
	{
		if (!_tcscmp(strParam,g_tArgv[i]))
		{
			return i;
		}
	}

	return -1;
}

/*
 *	Returns string value of n-ish argument
 **/
const TCHAR * PlatformEnvironment::GetCmdLineArgument(int index) const 
{
	if (index < 0 || index > g_tArgc) 
		return _T("<none>");
		
	return g_tArgv[index];
}

/*
 *	Returns count of command line arguments
 **/
const int PlatformEnvironment::GetCmdLineArgumentsCount() const 
{
	return g_tArgc;
}

/*
 *	Returns client area extents
 **/
ME_Math::Vector2D PlatformEnvironment::GetClientAreaExtents()
{
	return g_pPlatform->GetOpenGLContext()->m_vWindowExtents;
}

/*
 *	Returns cursor pos within client area
 **/
ME_Math::Vector2D PlatformEnvironment::GetCursorPos()
{
	POINT pt;
	::GetCursorPos(&pt);
	ScreenToClient(((CWinOpenGLContext*)g_pPlatform->GetOpenGLContext())->m_hWnd,&pt);
	
	return  ME_Math::Vector2D(pt.x,pt.y);
}

/*
 *	Sets cursor position within client area
 **/
void PlatformEnvironment::SetCursorPos(ME_Math::Vector2D v)
{
	POINT pt;
	pt.x = (long)v.x; pt.y = (long)v.y;

	ClientToScreen(((CWinOpenGLContext*)g_pPlatform->GetOpenGLContext())->m_hWnd,&pt);

	::SetCursorPos(pt.x,pt.y);
}

/*
 *	Sets specified cursor
 **/
void PlatformEnvironment::SetCursor(eMouseCursors cur)
{
	
	//HWND wnd = ((CWinOpenGLContext*)g_pPlatform->GetOpenGLContext())->m_hWnd;

	HCURSOR hCur;

	switch (cur)
	{
	case mcNormal:
		hCur = LoadCursor(NULL,IDC_ARROW);
		break;
	case mcSizeSWNE:
		hCur = LoadCursor(NULL,IDC_SIZENESW);
		break;
	case mcSizeNWSE:
		hCur = LoadCursor(NULL,IDC_SIZENWSE);
		break;
	case mcSizeVertical:
		hCur = LoadCursor(NULL,IDC_SIZENS);
		break;
	case mcSizeHorizontal:
		hCur = LoadCursor(NULL,IDC_SIZEWE);
		break;
	case mcSizeCenter:		
		hCur = LoadCursor(NULL,IDC_SIZEALL);
		break;
	default:
		hCur = LoadCursor(NULL,IDC_ARROW);
		break;
	}

	::SetCursor(hCur);

	
}

/************************************************************************/
/*			Entry point for windows application			                */
/************************************************************************/
int __stdcall WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd )
{
	// Copy instance to global variable - we'll need it in OpenGL context creation
	g_hInstance = hInstance;

	// Start timer
	Sys_TimerStart();

	// Initialize console
	ME_Console::Start();

	// Initialize environment
	ME_Framework::g_pPlatform = new PlatformEnvironment;
	
	// Parse command line
	ParseCommandLine(GetCommandLine());
		
	g_pPlatform->SystemLog()->Log(_T("Platform environment initialized\n"));
	//ME_Console::Printf(_T("[\n"));

	// Initialize file system
	TCHAR workDir[4096];
	GetCurrentDirectory(4096,workDir);
	FileSystemProxy * pProxy = new FileSystemProxy(workDir);
	g_pPlatform->SetFileSystemProxy(pProxy);

	// Initialize OpenGL
	IOpenGLContext * pContext = new CWinOpenGLContext();
	g_pPlatform->SetOpenGLContext(pContext);
	pContext->MainLoop();		
	
	
	// Cleanup		
	ApplicationShutdown();
	FreeParsedCommandLine();

	ME_Console::Stop();

	delete ME_Framework::g_pPlatform;		

}