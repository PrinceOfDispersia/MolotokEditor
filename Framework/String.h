/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	String.h - high level strings implementation
 *
 **/

#ifndef STRING_H
#define STRING_H

namespace ME_Editor
{
	class String
	{
		TCHAR * m_strData;
		size_t m_szLength;
		unsigned int  m_uiHash;
	public:

		String(TCHAR * strSource);
		~String();
	};
}

#endif 
