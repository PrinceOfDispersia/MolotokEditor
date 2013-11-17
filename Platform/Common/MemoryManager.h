/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	MemoryManager.h - pool based memory allocator
 *
 **/

#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

namespace ME_Framework
{
	
	class MemoryPool
	{
		static const int m_iPoolNameSize = 128;

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
		
		
		void RemoveChildPool(const MemoryPool * pPool);
	public:
		void PerformCorruptionCheck(const bool bRecursive = false);

		MemoryPool * AllocSubPool(const TCHAR * strDbgName);

		MemoryPool(const TCHAR * strPoolName,MemoryPool * pOwner);
		~MemoryPool();

		void * Alloc(const size_t nSize,const TCHAR * strFile,int iLine);
		void Free(void * pPtr,const  TCHAR * strFile,int iLine);

		#define Alloc(n) Alloc(n,_T(__FILE__),__LINE__)
		#define Free(ptr) Free(ptr,_T(__FILE__),__LINE__)

	};
}

#endif