/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	main.cpp - entry point for windows builds
 *
 **/

#include <windows.h>
#include <Platform/Common/ApplicationCommon.h>

using namespace ME_Framework;
PlatformEnvironment * ME_Framework::g_pPlatform;

/************************************************************************/
/*			Entry point for windows application			                */
/************************************************************************/
int __stdcall WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd )
{
	
	// Initialize environment
	ME_Framework::g_pPlatform = new PlatformEnvironment;
	
	try
	{
		ApplicationStart();
		ApplicationRun();
	}
	catch(ME_Framework::Exception * pException)
	{
			// Handle major errors here
			ApplicationOnException(pException);
	}

	
		
	ApplicationShutdown();
	delete ME_Framework::g_pPlatform;	
}