/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	Win_Assert.cpp - assert function for Windows platform
 *
 **/

#include "PlatformCommon.h"

void Sys_FatalError(TCHAR * strMessage)
{
	int result = MessageBox(0,strMessage,_T("Fatal error - choose YES to switch to debugger"),MB_ICONERROR | MB_YESNO);

	if (result == IDYES)
	{
		__debugbreak();
	}

	exit(666);
}