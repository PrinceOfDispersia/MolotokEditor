/*
*	Molotok Editor, 2013 (C) PrinceOfDispersia
*	Application.cpp - entry point for cross-platform application
*
**/

#include <Platform/Common/ApplicationCommon.h>

ME_XGUI::XGUI_Font * pFont;

/************************************************************************/
/*			Application startup routine, 
*			Called when all platform related subsystem initialized		
*			e.g. command line parsed, memory manager initialized, e.t.c.*/
/************************************************************************/
void ApplicationStart()
{
	// Start image library
	InitImageLib();

	dFontHdr_t * pHeader;
	size_t sz;

	pHeader = (dFontHdr_t*)g_pPlatform->FileSystem()->LoadFile(_T("gui/fonts/segoeui.ft2"),&sz);
	pFont = new ME_XGUI::XGUI_Font(pHeader,sz);
	

	g_pPlatform->FileSystem()->CloseFile((byte*)pHeader);
		
}

/************************************************************************/
/*          Application main cycle routine, called right after startup
*          by platform abstraction layer								*/
/************************************************************************/

float d = 0;
int fps = 0;
int rfps = 0;

void ApplicationRun(float flFrameDelta)
{
	// TODO: load here graphics and additional resources, while showing	
	// fancy loading box to user, then run main logic cycle
	
	if (d > 1.0f)
	{
		rfps = fps;
		fps = 0;
		d = 0;
	}

	fps++;
	d+=flFrameDelta;
	
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