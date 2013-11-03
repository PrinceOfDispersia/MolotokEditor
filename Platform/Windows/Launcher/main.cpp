/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	main.cpp - entry point for windows builds
 *
 **/

#include <windows.h>
#include <Platform/Common/ApplicationCommon.h>

int __stdcall WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd )
{
	
	try
	{
		ApplicationStart();
		ApplicationRun();
	}
	catch(ME_Editor::Exception * pException)
	{
			// Handle major errors here
			ApplicationOnException(pException);
	}
		
	ApplicationShutdown();
		
}