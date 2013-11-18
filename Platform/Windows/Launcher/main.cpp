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
	

	// Assume that there is no more than 256 arguments
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


/************************************************************************/
/*			Entry point for windows application			                */
/************************************************************************/
int __stdcall WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd )
{
	// Copy instance to global variable - we'll need it in OpenGL context creation
	g_hInstance = hInstance;

	// Start timer
	Sys_TimerStart();

	// Initialize environment
	ME_Framework::g_pPlatform = new PlatformEnvironment;
	
	// Parse command line
	ParseCommandLine(GetCommandLine());
		

	// Alloc windows console, 
	// TODO: fix codepages issues - it probably works fine only with Russian and English
	// and will screw up on chinese or something like that
	AllocConsole();
	FILE * console = freopen( "CONOUT$", "w+t", stdout);

	// switch it to unicode if necceseary
	if (sizeof(TCHAR) == 2)
	{
		_setmode(_fileno(console), _O_U16TEXT);
	}
	
	g_pPlatform->SystemLog()->Log(_T("Platform envrionment initialized\n"));

	// Run
	try
	{
		// Initiliaze OpenGL
		IOpenGLContext * pContext = new CWinOpenGLContext();
		g_pPlatform->SetOpenGLContext(pContext);
		pContext->MainLoop();		
	}
	catch(ME_Framework::Exception * pException)
	{
			// Handle major errors here
			ApplicationOnException(pException);
	}

	
	// Cleanup		
	ApplicationShutdown();
	FreeParsedCommandLine();

	FreeConsole();
	fclose(console);

	delete ME_Framework::g_pPlatform;		
}