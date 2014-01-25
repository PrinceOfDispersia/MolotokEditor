/*
*	Molotok Editor, 2013 (C) PrinceOfDispersia
*	Win_Log.cpp - log file implementation
*
**/

#include <Platform/Common/ApplicationCommon.h>


LogFile::LogFile(TCHAR * file,eLogLevels type)
{
	m_LogLevel = type;
}

LogFile::~LogFile()
{

}

void LogFile::Log(TCHAR * format,...)
{

	va_list args;
	//	size_t len;

	va_start( args, format );
	/*len = _vsctprintf( format, args ) + 1; // terminating '\0'
	len *= sizeof(TCHAR);*/

	
	TCHAR buffer[8192];
	
	_vsntprintf( buffer, 8192, format, args );
	buffer[8191] = 0;

	TCHAR * logTypeString = 0;

	switch(m_LogLevel)
	{
	case eLogLevels::LOG_DEV:
		logTypeString = _T("Developer");
		break;
	case eLogLevels::LOG_SYSTEM:
		logTypeString = _T("System");
		break;
	}
		
	ME_Console::Printf(_T("[%s]%s"),logTypeString,buffer);

	
}
