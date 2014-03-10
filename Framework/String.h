/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	String.h - high level strings implementation
 *
 **/

#ifndef STRING_H
#define STRING_H

#include <functional>

namespace ME_Framework
{
	
	class String
	{
		TCHAR * m_strData; // Data pointer
		size_t m_szLength; // String length in symbols
		size_t m_uiHash; // String hash
		size_t m_szBufferSize; // String buffer size in bytes
				

		// Hashing func implementation
		void RSHash()
		{
			unsigned int b    = 378551;
			unsigned int a    = 63689;
			m_uiHash = 0;

			for(size_t i = 0; i < m_szLength; i++)
			{
				m_uiHash = m_uiHash * a + m_strData[i];
				a    = a * b;
			}

			m_uiHash = (m_uiHash & 0x7FFFFFFF);
		}

		void Init(TCHAR * strSource)
		{
			m_szLength = (_tcsclen(strSource));
			size_t size = (m_szLength + 1) * sizeof(TCHAR);

			m_strData = (TCHAR*)g_pPlatform->StringsPool()->Alloc(size);
			
			m_szBufferSize = size;

			_tcscpy(m_strData,strSource);
			RSHash();
		}
	public:
		/*
		 *	Effective data length in characters
		 **/ 
		const size_t Length() const
		{
			return m_szLength;
		}

		void SetLength(size_t l)
		{
			m_szLength = l;
		}

		String & operator =(const String & a)
		{

			Init(a.m_strData);
			return (*this);
		}
		
		String operator=(TCHAR * other)
		{
			size_t newLen = _tcsclen(other) + 1;			
			newLen *= 2;

			if (m_strData)
			{
				if (m_szBufferSize < newLen)
				{					
					g_pPlatform->StringsPool()->Free(m_strData);
					m_strData = (TCHAR*)g_pPlatform->StringsPool()->Alloc(newLen);
					m_szBufferSize = newLen;
				}
			}
			else
			{
				m_strData = (TCHAR*)g_pPlatform->StringsPool()->Alloc(newLen);				
			}

			_tcscpy(m_strData,other);
			m_szLength = (_tcsclen(m_strData));
			RSHash();
			m_szBufferSize = newLen;

			return (*this);
		}

		/*
		 *	Comparison operator
		 **/
		bool operator==(String  other)
		{
			return m_uiHash == other.m_uiHash;
		}

		/*
		 *	Default constructor
		 **/
		String()
		{
			m_strData = 0;
			m_szLength = 0;
			m_uiHash = 0;
			m_szBufferSize = 0;
		}

		/*
		 *	Array access
		 **/
		TCHAR operator[](size_t i)
		{
			assert(i >= 0 && i<= m_szLength);

			return m_strData[i];

		}

		TCHAR* operator *()
		{
			return m_strData;
		}
		
		/*
		 *	Formated print function
		 **/
		void Sprintf(TCHAR * format,...)
		{
			va_list args;
			size_t len;
			
			va_start( args, format );
			len = _vsctprintf( format, args ) + 1; // terminating '\0'
			len *= sizeof(TCHAR);
			
			if (m_strData)
			{
				if (m_szBufferSize < len)
				{
					g_pPlatform->StringsPool()->Free(m_strData);
					m_strData = (TCHAR*)g_pPlatform->StringsPool()->Alloc(len);
					
					m_szBufferSize = len;
				}
			}
			else
			{
					m_strData = (TCHAR*)g_pPlatform->StringsPool()->Alloc(len);
					
			}
			
			_vstprintf( m_strData, format, args );

			m_szLength = (_tcsclen(m_strData));			
			RSHash();
		}

		/*
		 *	Increment operator
		 **/
		String & operator +=(String strOther)
		{
			size_t newLen = m_szLength + strOther.m_szLength + 1;			
			newLen *= 2;
			
			if (m_strData)
			{
				if (m_szBufferSize < newLen)
				{
					TCHAR * tmp = m_strData;

					m_strData = (TCHAR*)g_pPlatform->StringsPool()->Alloc(newLen);
					
					m_szBufferSize = newLen;

					_tcscpy(m_strData,tmp);
					g_pPlatform->StringsPool()->Free(tmp);
				}
			}
			else
			{
				m_strData = (TCHAR*)g_pPlatform->StringsPool()->Alloc(strOther.m_szBufferSize);				
			}

			_tcscat(m_strData,strOther.m_strData);
			m_szLength = (_tcsclen(m_strData));
			RSHash();

			return (*this);
		}

		String & operator +=(TCHAR c)
		{
			if ((m_szLength + 2) * sizeof(TCHAR) > m_szBufferSize)
			{
				TCHAR * tmp = m_strData;
				m_szBufferSize *= 2;
				m_strData = (TCHAR*)g_pPlatform->StringsPool()->Alloc(m_szBufferSize);
				_tcscpy(m_strData,tmp);

				
				g_pPlatform->StringsPool()->Free(tmp);
				m_szLength = (_tcsclen(m_strData));
				
			}

			m_strData[m_szLength] = c;
			m_szLength++;
			m_strData[m_szLength] = 0;

			RSHash();

			return (*this);
		}

		/*
		 *	Concatenation
		 **/
		String operator +(String & other)
		{
			String n = String(*this);
			n+=other;
			return n;
		}

		/*
		 *	Constructor
		 **/
		String(TCHAR * strSource)
		{
			Init(strSource);
		}

		/*
		 * Copy constructor
		 **/
		String(const String & other)
		{
			Init(other.m_strData);
		}

		/*
		 *	Destructor
		 **/
		~String()
		{
 			if (m_strData)
 				g_pPlatform->StringsPool()->Free(m_strData);
		}

		/*
		 * number constructor
		 **/
		String(const int & i)
		{
			Sprintf(_T("%d"),i);
		}

		/*
		 * number constructor
		 **/
		String(const double & d)
		{
			Sprintf(_T("%lf"),d);
		}

		/*
		 *	C-style string
		 **/
		const TCHAR* c_str()
		{
			return m_strData;
		}

		operator const TCHAR*() const
		{
			return m_strData;
		}

	};

	class Tokenizer
	{
		String m_strSource;
		String m_strTmpToken;
		
		size_t m_strOffset;		
	public:
		/*
		 * Constructor
		 **/
		Tokenizer(String source)
		{
			m_strSource = source;
			
			m_strOffset = 0;
		}

		/*
		 *	Gets current token value
		 **/
		const String & GetTokenValue()
		{
			return m_strTmpToken;
		}

		/*
		 *	Reads next token from string
		 **/
		bool ReadNextToken(TCHAR * tokenDelimeters = nullptr)
		{
			// Skip whitespace before
			while(m_strOffset < m_strSource.Length() && m_strSource[m_strOffset] < 33)
				m_strOffset++;

			// End of string
			if (m_strOffset == m_strSource.Length())
				return false;

			// Clear temporary token buffer
			m_strTmpToken = _T("");

			bool inQuotes = false;

			while(m_strOffset < m_strSource.Length())
			{
				if (m_strSource[m_strOffset] == _T('"'))
				{
					inQuotes = !inQuotes;
					m_strOffset++;
					continue;
				}

				// Grab everything
				if (inQuotes)
				{
					
					m_strTmpToken+=m_strSource[m_strOffset];
					g_pPlatform->StringsPool()->PerformCorruptionCheck();
					m_strOffset++;
					continue;
				}
				else
				{
					// whitespace
					if (m_strSource[m_strOffset] < 33)
					{
						return true;
					}
					else
					{
						m_strTmpToken+=m_strSource[m_strOffset];
						g_pPlatform->StringsPool()->PerformCorruptionCheck();
						m_strOffset++;
					}
				}

				
			}

			return true;

		}


	};
}

#endif 
