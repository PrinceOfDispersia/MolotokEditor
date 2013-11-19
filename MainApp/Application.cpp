/*
*	Molotok Editor, 2013 (C) PrinceOfDispersia
*	Application.cpp - entry point for cross-platform application
*
**/

#include <Platform/Common/ApplicationCommon.h>

/************************************************************************/
/*			Application startup routine, 
*			Called when all platform related subsystem initialized		
*			e.g. command line parsed, memory manager initialized, e.t.c.*/
/************************************************************************/
void ApplicationStart()
{
	// Start image library
	InitImageLib();

	size_t sz;
	byte * pBuffer = g_pPlatform->FileSystem()->LoadFile(_T("gui.png"),&sz);
	gl_texture_t * pTexture = Img_Load(_T("sss"),pBuffer,sz,false);

	if (pBuffer) g_pPlatform->FileSystem()->CloseFile(pBuffer);
		
}

/************************************************************************/
/*          Application main cycle routine, called right after startup
*          by platform abstraction layer								*/
/************************************************************************/
void ApplicationRun(float flFrameDelta)
{
	// TODO: load here graphics and additional resources, while showing	
	// fancy loading box to user, then run main logic cycle

}

/************************************************************************/
/*          Application shutdown routine
*          called when application shuts down, free resource here	 	*/
/************************************************************************/
void ApplicationShutdown()
{
	// TODO: write here some code)
	// 	
	ShutdownImageLib();
}

/************************************************************************/
/*			Events handler                                              */
/************************************************************************/
void ApplicationPumpEvent(ME_Framework::appEvent_t & ev)
{
	_tprintf(_T("ApplicationPumpEvent(): eventid = %d, uParam1 = %d\n"),ev.eventid,ev.uParam1);
}