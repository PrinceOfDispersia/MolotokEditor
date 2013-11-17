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

/************************************************************************/
/*          Safe, cross-platform, multi-charset version of sprintf      */
/*			formats string to "dest" buffer, "destSize" parameter specifies
 *			buffer size in characters									*/
/*			return value: number of used bytes in buffer				*/
/************************************************************************/
size_t Sys_SafeSprintf(TCHAR * dest,size_t destSize,TCHAR * format,...)
{
	va_list		argptr;
	
	TCHAR * s = dest;
	va_start( argptr, format );
	int written = _vsntprintf( s, destSize, format, argptr );
	va_end( argptr );

	if (written == -1)
	{
	#ifdef PARANOID
		// TODO: replace with console log, cause fatal error is too brutal
		Sys_FatalError(_T("Sys_SafeSprintf(): buffer overflow!"));
	#endif
		// null-terminate
		dest[destSize-1] = 0;
	}
	
	return written;	
}

/************************************************************************/
/*          Safe, cross-platform, multi-charset version of strcpy       */
/*			Copies source string to dest, ensuring null-termination of dest
 *			return value: true - no overflow, false - overflow happened
/************************************************************************/
bool Sys_SafeStrCopy(TCHAR * dest,TCHAR * source,size_t destSize)
{
	
	_tcsnccpy(dest,source,destSize);

	// Function didn't filled null-terminator, so we probably overflowed
	if (dest[destSize - 1] != 0)
	{
		#ifdef PARANOID
				// TODO: replace with console log, cause fatal error is too brutal
				Sys_FatalError(_T("Sys_SafeStrCopy(): destination overflow!"));
		#endif
		dest[destSize - 1] = 0;

		return false;
	}

	return true;
		
}


