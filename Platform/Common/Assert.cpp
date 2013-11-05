/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	Assert.cpp - assertion definitions
 *
 **/

#include "ApplicationCommon.h"

void Sys_Assert(TCHAR * strMessage,TCHAR * strFile,int iLine)
{
	// TODO: ensure that buffer is big enough
	TCHAR tmp[1024];

	_sntprintf(tmp,ARRAY_SIZE(tmp),_T("Assertion \"%s\" failed at %s on line %d"),strMessage,strFile,iLine);

	Sys_FatalError(tmp);
} 