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
	
	class MemoryPool
	{
		TCHAR m_strPoolName[m_iPoolNameSize];
		MemoryPool * m_pOwner;
	
		typedef struct allocation_s
		{
			const TCHAR * strFile;
			int iLine;
			size_t size;
			void * ptr;
		}allocation_t;

		std::vector<allocation_t> m_vAllocationChains;
		std::vector<MemoryPool*> m_vChildrens;
		
		void PerformCorruptionCheck(const bool bRecursive = false);
		void RemoveChildPool(const MemoryPool * pPool);
	public:
		
		MemoryPool * AllocSubPool(const TCHAR * strDbgName);

		MemoryPool(const TCHAR * strPoolName,MemoryPool * pOwner);
		~MemoryPool();

		void * Alloc(const size_t nSize,const TCHAR * strFile,int iLine);
		void Free(void * pPtr,const  TCHAR * strFile,int iLine);

		#define Alloc(n) Alloc(n,__FILE__,__LINE__)
		#define Free(ptr) Free(ptr,__FILE__,__LINE__)

	};
}