/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	ApplicationCommon.h - cross platform shared header file
 *
 **/

#include <Framework/BasicTypes.h>
// >=== PORTING NOTES: Change this header with appropriate for your platform
#include <Platform/Windows/Common/PlatformCommon.h>
// <=== PORTING NOTES END
#include <Platform/Common/MemoryManager.h>
#include <Framework/Framework.h>

// TODO: move to separate header ?
namespace ME_Editor
{
	class Exception
	{
	public:
		// TODO: Replace with my fancy string class
		TCHAR message[256];
	};


}

// Platform independent appication callbacks
extern void ApplicationStart();
extern void ApplicationRun();
extern void ApplicationShutdown();
extern void ApplicationOnException(ME_Editor::Exception * pException);
