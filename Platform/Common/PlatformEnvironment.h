/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	PlatformEnvironment.h - platform environment
 *
 **/

namespace ME_Editor
{
	class PlatformEnvironment
	{
		MemoryPool * m_pGlobalMemoryPool;
		MemoryPool * m_pStringsPool;
	public:
		MemoryPool * MemoryPools()
		{
			return m_pGlobalMemoryPool;
		}
		
		MemoryPool * StringsPool()
		{
			return m_pStringsPool;
		}

		PlatformEnvironment()
		{
			m_pGlobalMemoryPool = new MemoryPool(_T("Global"),0);
			m_pStringsPool = m_pGlobalMemoryPool->AllocSubPool(_T("Strings"));
		}

		~PlatformEnvironment()
		{
			delete m_pGlobalMemoryPool;
		}
	};

	extern PlatformEnvironment * g_pPlatform;
}