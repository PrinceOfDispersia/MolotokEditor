/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	PlatformEnvironment.h - platform environment
 *
 **/

#ifndef PLATFORM_ENVIRONMENT_H
#define PLATFORM_ENVIRONMENT_H

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

		LogFile * m_pDeveloperLog;
		LogFile * m_pSystemLog;

		IOpenGLContext * m_pOpenGLContext;
		FileSystemProxy * m_pFileSystemProxy;
				
	public:
		/*
		 *	OpenGL context getters-setters
		 **/
		IOpenGLContext * GetOpenGLContext()
		{
			return m_pOpenGLContext;
		}

		void SetOpenGLContext(IOpenGLContext * pContext)
		{
			m_pOpenGLContext = pContext;
		}

		/*
		 *	FileSystem getters-setters
		 **/
		FileSystemProxy * FileSystem()
		{
			return m_pFileSystemProxy;
		}

		void SetFileSystemProxy(FileSystemProxy * p)
		{
			m_pFileSystemProxy = p;
		}

		/*
		 *	Log access
		 **/
		LogFile * SystemLog()
		{
			return m_pSystemLog;
		}

		LogFile * DeveloperLog()
		{
			return m_pDeveloperLog;
		}

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

			m_pSystemLog = new LogFile(_T("system.log"),eLogLevels::LOG_SYSTEM);
			m_pDeveloperLog = new LogFile(_T("developer.log"),eLogLevels::LOG_DEV);

			m_pOpenGLContext = 0;
			m_pFileSystemProxy = 0;
		}

		/*
		 * Destructor
		 **/
		~PlatformEnvironment()
		{
			
			delete m_pSystemLog;
			delete m_pDeveloperLog;
			
			if (m_pOpenGLContext) delete m_pOpenGLContext;
			if (m_pFileSystemProxy) delete m_pFileSystemProxy;

			delete m_pGlobalMemoryPool;
		}

		// Implementation up to platform
		const int FindCmdLineArgument(TCHAR * strName) const;
		const int GetCmdLineArgumentsCount() const;
		const TCHAR * GetCmdLineArgument(int index) const;

		const float TimeElapsed();
		
		float m_flFrameTime;

		ME_Math::Vector2D GetCursorPos();
		void SetCursorPos(ME_Math::Vector2D  v);
		void SetCursor(eMouseCursors cur);

		ME_Math::Vector2D GetClientAreaExtents();
	};

	// Instance, initialization is up to platform
	extern PlatformEnvironment * g_pPlatform;
}



#endif