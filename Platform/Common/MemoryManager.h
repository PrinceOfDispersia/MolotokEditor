/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	MemoryManager.h - pool based memory allocator
 *
 **/

namespace ME_Editor
{
	const byte Guard1[4] = {0xBA,0xAD,0xF0,0x0D};
	const byte Guard2[4] = {0xDE,0xAD,0xBE,0xEF};

	const int m_iPoolNameSize = 128;

	class MemoryManager;

	class MemoryPool
	{
		TCHAR m_strPoolName[m_iPoolNameSize];
		MemoryManager * m_pOwner;
	
		friend MemoryManager;

		void PerformCorruptionCheck();

		MemoryPool(TCHAR * strPoolName,MemoryManager * pOwner);
	public:
		
		~MemoryPool();

		void * Alloc(size_t nSize);
		void Free(void * pPtr);
				

	};
}