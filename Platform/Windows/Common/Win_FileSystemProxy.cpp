/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	Win_FileSystemProxy.cpp - implementation of FS proxy for windows
 *
 **/

#include <windows.h>
#include <io.h>
#include <Platform/Common/ApplicationCommon.h>

MemoryPool * g_pFileSystemPool = 0;

/*
 *	Generic Disk provider
 **/
class CGenericDiskFSProvider: public IFileSystemProvider
{
	TCHAR* m_strWorkDir;
public:
	/*
	 *	Constructor
	 **/
	CGenericDiskFSProvider(TCHAR * strWorkDir)
	{
		m_strWorkDir = Sys_StrDup(strWorkDir);
	}

	/*
	 *	Destructor
	 **/
	virtual ~CGenericDiskFSProvider()
	{
		g_pPlatform->StringsPool()->Free(m_strWorkDir);
	}

	/*
	 *	Generic file read function
	 **/
	virtual byte* LoadFile(TCHAR * path,size_t * size)
	{
		TCHAR buffer[4096];
		Sys_SafeSprintf(buffer,ARRAY_SIZE(buffer),_T("%s\\%s"),m_strWorkDir,path);

		FILE * fp = _tfopen(buffer,_T("rb"));

		if (!fp) 
		{
			*size = 0;
			return 0;			
		}

		size_t sz = 0;
		fseek(fp,0,SEEK_END);
		sz = ftell(fp);
		fseek(fp,0,SEEK_SET);

		byte * pBuffer = (byte*)g_pFileSystemPool->Alloc(sz);

		fread(pBuffer,sz,1,fp);
		fclose(fp);

		*size = sz;

		return pBuffer;
		
	}

	virtual void CloseFile(byte * ptr)
	{
		g_pFileSystemPool->Free(ptr);
	}

};

/*
 *	File system class
 **/

/*
 *	Constructor
 **/
FileSystemProxy::FileSystemProxy(TCHAR * workDir)
{
	m_strWorkDir = Sys_StrDup(workDir);
	m_vProviders.push_back(new CGenericDiskFSProvider(m_strWorkDir));

	g_pFileSystemPool = g_pPlatform->MemoryPools()->AllocSubPool(_T("File system"));
}

/*
 *	Destructor
 **/
FileSystemProxy::~FileSystemProxy()
{
	g_pPlatform->StringsPool()->Free(m_strWorkDir);

	for(IFileSystemProvider * prov: m_vProviders)
	{
		delete prov;
	}

	m_vProviders.clear();
	m_vProviders.shrink_to_fit();

	g_pFileSystemPool->UnlinkFromParent();
	delete g_pFileSystemPool;
}

/*
 * Tries to load file with every provider available
 **/
byte * FileSystemProxy::LoadFile(TCHAR * path,size_t *sz)
{
	for(IFileSystemProvider * prov : m_vProviders)
	{
		byte * b = prov->LoadFile(path,sz);
		if (b)
			return b;
	}

	return 0;
}

/*
 *	Free allocated data for file
 **/
void FileSystemProxy::CloseFile(byte * ptr)
{
	g_pFileSystemPool->Free(ptr);
}

/*
 * Opens file by relative to program path
 **/
FILE * FileSystemProxy::OpenFileLocal(TCHAR * path,TCHAR * mode)
{
	TCHAR tmp[4096];
	Sys_SafeSprintf(tmp,ARRAY_SIZE(tmp),_T("%s/%s"),m_strWorkDir,path);

	FILE * fp = _tfopen(path,mode);

	return fp;
}