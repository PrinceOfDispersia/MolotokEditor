/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	Assert.h - assertion definitions
 *
 **/

#ifndef ASSERT_H
#define ASSERT_H

extern void Sys_FatalError(TCHAR * strMessage);
void Sys_Assert(TCHAR * strMessage,TCHAR * strFile,int iLine);

#ifdef _DEBUG
#define assert(exp) if (!(exp)) Sys_Assert(_T(#exp),_T(__FILE__),__LINE__)
#else
#define assert(exp)
#endif

#endif