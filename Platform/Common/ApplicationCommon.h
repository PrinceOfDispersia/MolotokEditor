/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	ApplicationCommon.h - cross platform shared header file
 *
 **/

#ifndef APPLICATION_COMMON_H
#define APPLICATION_COMMON_H

// Include configuration header
#include <Platform/Common/ApplicationConfig.h>
#include <Framework/BasicMacros.h>
#include <Framework/BasicTypes.h>

// >=== PORTING NOTES: Change this header with appropriate for your platform
#include <Platform/Windows/Common/PlatformCommon.h>
// <=== PORTING NOTES END

#include <Platform/Common/Assert.h>
#include <Platform/Common/MemoryManager.h>
#include <Platform/Common/CommonUtils.h>

#include <Platform/Common/PlatformEnvironment.h>
#include <Framework/Framework.h>

#include <Platform/Common/OpenGLContext.h>
#include <Platform/Common/ApplicationEvents.h>


// TODO: move to separate header ?
namespace ME_Framework
{
	class Exception
	{
	public:
		String message;
	};
}



// Platform independent appication callbacks
extern void ApplicationStart();
extern void ApplicationRun(float flFrameDelta);
extern void ApplicationShutdown();
extern void ApplicationOnException(ME_Framework::Exception * pException);

using namespace ME_Framework;

#endif