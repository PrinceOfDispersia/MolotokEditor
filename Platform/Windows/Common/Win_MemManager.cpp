/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	Win_MemManager.cpp - memory manager implementation
 *
 **/

#include <Platform\Common\ApplicationCommon.h>
#include "PlatformCommon.h"

using namespace ME_Editor;

// Undefine tracking macroses for this file
#undef Alloc
#undef Free

// Define guard bytes
byte Guard1[4] = {0xBA,0xAD,0xF0,0x0D};
byte Guard2[4] = {0xDE,0xAD,0xBE,0xEF};

/************************************************************************/
/*		Allocates nSize bytes and tracks information about caller       */
/************************************************************************/
void* MemoryPool::Alloc(const size_t nSize,const  TCHAR * strFile,const int iLine)
{
	#ifdef PARANOID
		PerformCorruptionCheck();
	#endif

	// Simply alloc and guard around for now
	byte * ptr = (byte*)malloc(nSize + 8);
	memcpy(ptr,Guard1,4);
	memcpy(ptr+nSize,Guard2,4);

	// Add allocation to list
	allocation_t a;
	a.strFile = strFile;
	a.iLine = iLine;
	a.size = nSize;
	a.ptr = ptr+4;

	m_vAllocationChains.push_back(a);

	// return pointer
	return ptr+4;
}

/************************************************************************/
/*		Frees data at pointer, removing data from stat					*/	 
/************************************************************************/
void MemoryPool::Free(void * pPtr,const TCHAR * strFile,const int iLine)
{
	// Shift to guard fences

	#ifdef PARANOID
		PerformCorruptionCheck();
	#endif

	// 1) Find allocation

	// I'm so fucking love new C++
	auto it = m_vAllocationChains.begin();
	for(; it != m_vAllocationChains.end() ; ++it)
	{
		if ((*it).ptr == pPtr)
		{
			break;
		}
		
	}

	// 2) Erase if found one and free memory
	if (it != m_vAllocationChains.end())
	{
		pPtr = ((byte*)pPtr)-4;
		m_vAllocationChains.erase(it);
		free(pPtr);
	}
	else
	{
		Sys_FatalError(VA(_T("MemoryPool::Free(): freeing pointer from another pool @ %s on line %d"),strFile,iLine));
	}
	
}

/************************************************************************/
/*		 Allocates sub-pool												*/
/************************************************************************/
MemoryPool * MemoryPool::AllocSubPool(const TCHAR * strDbgName)
{
	MemoryPool * pPool = new MemoryPool(strDbgName,this);
	m_vChildrens.push_back(pPool);
	return pPool;
}

/************************************************************************/
/*        Checks guard bytes for all tracked allocations                */
/************************************************************************/
void MemoryPool::PerformCorruptionCheck(bool bRecursive)
{
	TCHAR msg[]=
	_T("MemoryPool::PerformCorruptionCheck(): damaged Guard%d for allocation at %s on line %d in memory pool %s");

	for(auto it = m_vAllocationChains.begin(); it != m_vAllocationChains.end() ; ++it)
	{
		allocation_t a = (*it);
		byte * ptr = (byte*)a.ptr-4;

		if (memcmp(ptr,Guard1,4) != 0)			
		{
			Sys_FatalError(VA(msg,1,(*it).strFile,(*it).iLine,m_strPoolName));
		}

		if (memcmp(ptr+a.size,Guard2,4) !=0)			
		{
			Sys_FatalError(VA(msg,2,(*it).strFile,(*it).iLine,m_strPoolName));
		}
	}	
}

/************************************************************************/
/*        Constructor													*/
/************************************************************************/
MemoryPool::MemoryPool(const TCHAR * strPoolName,MemoryPool * pOwner)
{
	_tcsnccpy_s(m_strPoolName,strPoolName,sizeof(m_strPoolName));
	m_pOwner = pOwner;
	
}

/************************************************************************/
/*        Removes pool from child list                                  */
/************************************************************************/
void MemoryPool::RemoveChildPool(const MemoryPool * pPool)
{
	auto it = m_vChildrens.begin();
	for(; it != m_vChildrens.end() ; ++it)
	{
		if ((*it) == pPool)
		{
			break;
		}

	}

	// 2) Erase if found one and free memory
	if ((*it) == pPool)
	{
		m_vChildrens.erase(it);		
	}
	else
	{
		Sys_FatalError(VA(_T("MemoryPool::RemoveChildPool(): pool %s is not member of pool %s"),pPool->m_strPoolName,m_strPoolName));
	}
}

/************************************************************************/
/*        Destructor													*/
/************************************************************************/
MemoryPool::~MemoryPool()
{
	#ifdef PARANOID
		PerformCorruptionCheck(false);
	#endif

	for(auto it = m_vAllocationChains.begin(); it != m_vAllocationChains.end() ; ++it)
	{
		byte * ptr = ((byte*)(*it).ptr)-4;
		free(ptr);
	}	

	/*if (m_pOwner)
	{
		m_pOwner->RemoveChildPool(this);
	}*/

	for(auto it = m_vChildrens.begin(); it != m_vChildrens.end() ; ++it)
	{
		delete *it;
	}

	m_vChildrens.shrink_to_fit();
	m_vAllocationChains.shrink_to_fit();
	
}

