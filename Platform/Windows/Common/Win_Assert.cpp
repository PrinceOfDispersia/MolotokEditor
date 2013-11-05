/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	Win_Assert.cpp - assert function for Windows platform
 *
 **/

#include "PlatformCommon.h"

void Sys_FatalError(TCHAR * strMessage)
{
	MessageBox(0,strMessage,_T("Fatal error"),MB_ICONERROR);
	exit(666);
}