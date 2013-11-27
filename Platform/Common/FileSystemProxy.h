/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	FileSystemProxy.h - file system abstraction layer
 *
 **/

#ifndef FILESYSTEMPROXY_H
#define FILESYSTEMPROXY_H

namespace ME_Framework
{
	/*
	 *	FS provider - loads files by local path
	 **/
	class IFileSystemProvider
	{
	public:
	
		virtual ~IFileSystemProvider() {};

		virtual byte * LoadFile(TCHAR * path,size_t * size) = 0;
		virtual void CloseFile(byte * ptr) = 0;

	};

	/*
	 *	Virtual file system proxy
	 **/
	class FileSystemProxy
	{
		std::vector<IFileSystemProvider*> m_vProviders;
		TCHAR * m_strWorkDir;
	public:
		FileSystemProxy(TCHAR * workDir);
		~FileSystemProxy();

		FILE * OpenFileLocal(TCHAR * path,TCHAR * mode);

		byte* LoadFile(TCHAR * path,size_t *sz);
		void CloseFile(byte * ptr);
	};
}
#endif