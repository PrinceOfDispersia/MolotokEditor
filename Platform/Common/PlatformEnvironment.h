/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	PlatformEnvironment.h - platform environment
 *
 **/

namespace ME_Framework
{
	
#if 0
	class CCommandLineArgs
	{
		std::vector<String> m_vCommandArgsList;
	public:
		/*
		 *	Constructor
		 **/
		CCommandLineArgs(TCHAR * strSource)
		{
			Tokenizer tok(strSource);

			while(tok.ReadNextToken())
			{
				m_vCommandArgsList.push_back(tok.GetTokenValue());
			}
		}
		
		/*
		 * Destructor
		 **/
		~CCommandLineArgs()
		{
			m_vCommandArgsList.clear();
			m_vCommandArgsList.shrink_to_fit();
		}

		/*
		 *	Returns n-ish argument
		 **/
		const String & GetArgument(int index)
		{
			assert (index < 0 || index > m_vCommandArgsList.size());			
			return m_vCommandArgsList[index];
		}
		
		/*
		 *	Returns index of specified argument, -1 if not found
		 **/
		const int FindArgument(TCHAR)
		{
			for(int i = 0 ; i < m_vCommandArgsList.size() ; i++)
			{
				if (m_vCommandArgsList[i] == str)
					return i;
			}

			return -1;
		}
	};
#endif

	/*
	 *	Generic environment used by application
	 **/
	class PlatformEnvironment
	{
		// Common pools
		MemoryPool * m_pGlobalMemoryPool;
		MemoryPool * m_pStringsPool;

		// Command line
		//CCommandLineArgs * m_pCommandLine;
	public:
		/*
		 *	Main memory pool access
		 **/
		MemoryPool * MemoryPools()
		{
			return m_pGlobalMemoryPool;
		}
		
		/*
		 *	Strings pool access
		 **/
		MemoryPool * StringsPool()
		{
			return m_pStringsPool;
		}

		/*
		 *	Constructor
		 **/
		PlatformEnvironment()
		{
			m_pGlobalMemoryPool = new MemoryPool(_T("Global"),0);
			m_pStringsPool = m_pGlobalMemoryPool->AllocSubPool(_T("Strings"));
		}

		/*
		 * Destructor
		 **/
		~PlatformEnvironment()
		{
			delete m_pGlobalMemoryPool;
		}
	};

	// Instance, initialization is up to platform
	extern PlatformEnvironment * g_pPlatform;
}