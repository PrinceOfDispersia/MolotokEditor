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

	FILE * fp = fopen("gui.png","rb");
	if (!fp) return;

	fseek(fp,0,SEEK_END);
	size_t sz = ftell(fp);
	fseek(fp,0,SEEK_SET);

	byte  * pBuffer = (byte*)g_pPlatform->MemoryPools()->Alloc(sz);
	fread(pBuffer,sz,1,fp);
	fclose(fp);

	gl_texture_t * pTexture = Img_Load(_T("sss"),pBuffer,sz,true);

	g_pPlatform->MemoryPools()->Free(pBuffer);
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
/*          Application exception handling routine                      */
/************************************************************************/
void ApplicationOnException(ME_Framework::Exception * pException)
{
	// TODO: show here message box with my hands, ask user to save documents
	// and shutdown editor
}

/************************************************************************/
/*			Events handler                                              */
/************************************************************************/
void ApplicationPumpEvent(ME_Framework::appEvent_t & ev)
{
	_tprintf(_T("ApplicationPumpEvent(): eventid = %d, uParam1 = %d\n"),ev.eventid,ev.uParam1);
}