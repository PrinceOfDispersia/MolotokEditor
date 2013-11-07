/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	CommonUtils.h - common platform utils
 *
 **/

#ifndef COMMON_UTILS_H
#define COMMON_UTILS_H

TCHAR *VA( const TCHAR *format, ... );
size_t Sys_SafeSprintf(TCHAR * dest,size_t destSize,TCHAR * format,...);

#endif