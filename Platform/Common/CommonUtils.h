/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	CommonUtils.h - common platform utils
 *
 **/

#ifndef COMMON_UTILS_H
#define COMMON_UTILS_H

#include <Framework/EnumFlags.h>

TCHAR *VA( const TCHAR *format, ... );

/*
 *	Safe wrappers for CRT function
 **/
size_t Sys_SafeSprintf(TCHAR * dest,size_t destSize,TCHAR * format,...);
bool Sys_SafeStrCopy(TCHAR * dest,TCHAR * source,size_t destSize);

/*
 *	Log file class, implementation up to platform code
 **/
namespace ME_Framework
{
	enum class eLogLevels
	{
		LOG_DEV = 1,
		LOG_SYSTEM = 2
	};
	
	class LogFile
	{
		eLogLevels m_LogLevel;
	public:
		LogFile(TCHAR * file,eLogLevels type);
		~LogFile();

		void Log(TCHAR * format,...);
	};
}

#endif