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
		g_pPlatform->DeveloperLog()->Log(_T("Sys_SafeSprintf(): buffer overflow!"));
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
				g_pPlatform->DeveloperLog()->Log(_T("Sys_SafeStrCopy(): destination overflow!"));
		#endif
		dest[destSize - 1] = 0;

		return false;
	}

	return true;
	
}

/************************************************************************/
/*			Allocates buffer for copy of string and copies it		    
 *			Uses platform memory manager
/************************************************************************/
TCHAR * Sys_StrDup(TCHAR * source)
{
	size_t s = _tcsclen(source) + 1;
	s*=sizeof(TCHAR);

	TCHAR * pRet = (TCHAR*)g_pPlatform->StringsPool()->Alloc(s);
	_tcscpy(pRet,source);

	return pRet;
}

/************************************************************************/
/*			Disassembles string into array of substrings
 *			last element contains part remaining part of source string.
 *			Modifies original string
/************************************************************************/
size_t Sys_DisassembleStringToTokenArray(TCHAR * strPtr,TCHAR divisor,TCHAR ** destArray,size_t destSize)
{
	size_t curr_array_pos = 0;

	size_t startOfs = 0;
	size_t currOfs = 0;

	while(true)
	{
		if (strPtr[currOfs] == divisor)
		{

			destArray[curr_array_pos] = &strPtr[startOfs];

			curr_array_pos++;

			if (curr_array_pos < destSize)
			{
				while(strPtr[currOfs] == divisor)
				{
					strPtr[currOfs] = 0;
					currOfs++;					
				}
			}
			else 
				return destSize;
			
			
			startOfs = currOfs;
			
		}
			
		
		currOfs++;

		if (!strPtr[currOfs]) 
		{
			destArray[curr_array_pos] = &strPtr[startOfs];
			return curr_array_pos+1;
		}

	}

}

/************************************************************************/
/*			Converts one byte characters to corresponding project settings
 *			character width, allocates new buffer
 /************************************************************************/
TCHAR* Sys_CharToTChar(const char * src)
{
	if (sizeof(TCHAR) == sizeof(char))
		return Sys_StrDup((TCHAR*)src);

	size_t len = strlen(src);

	TCHAR * ptr = (TCHAR*)Mem_Alloc(len * 2 + sizeof(TCHAR));

	unsigned char * p = (unsigned char*)src;
	TCHAR * dest = ptr;
	while(*p)
	{
		if (*p > 127)
			*dest++=' ',p++;
		else *dest++ = *p++;
	}

	*dest = 0;

	return ptr;
}