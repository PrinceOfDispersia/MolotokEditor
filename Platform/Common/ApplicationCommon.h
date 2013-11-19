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

#include <Platform/Common/FileSystemProxy.h>

#include <Platform/Common/Assert.h>
#include <Platform/Common/MemoryManager.h>
#include <Platform/Common/CommonUtils.h>

#include <Framework/MathLib/Math.h>
#include <Platform/Common/OpenGLContext.h>
#include <Platform/Common/PlatformEnvironment.h>
#include <Framework/Framework.h>


#include <Platform/Common/MouseKeys.h>
#include <Platform/Common/ApplicationEvents.h>

#include <Framework/GUI/XGUI.h>

// Platform independent appication callbacks
extern void ApplicationStart();
extern void ApplicationRun(float flFrameDelta);
extern void ApplicationShutdown();
extern void ApplicationPumpEvent(ME_Framework::appEvent_t & ev);


using namespace ME_Framework;

#endif