/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	ApplicationCommon.h - cross platform shared header file
 *
 **/

#include "..\Framework\Framework.h"

namespace ME_Editor
{
	class Exception
	{
	public:
		// TODO: Replace with my fancy string class
		char message[256];
	};
}

extern void ApplicationStart();
extern void ApplicationRun();
extern void ApplicationShutdown();
extern void ApplicationOnException(ME_Editor::Exception * pException);
