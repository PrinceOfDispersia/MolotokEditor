/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	CommonUtils.cpp - common platform utils
 *
 **/

#include "ApplicationCommon.h"

/************************************************************************/
/*			Formats string like printf function does, since result points
 *			to same static array, every call destroys old results, so
 *			use quickly or copy result
/************************************************************************/
TCHAR *VA( const TCHAR *format, ... )
{
	va_list		argptr;
	static TCHAR	string[8][1024], *s;
	static int	stringindex = 0;

	s = string[stringindex];
	stringindex = (stringindex + 1) & 7;
	va_start( argptr, format );
	_vsntprintf( s, sizeof( string[0] ), format, argptr );
	va_end( argptr );


	// Extra safety
	s[1023] = 0;

	return s;
}

