/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	Application.cpp - entry point for cross-platform application
 *
 **/

#include <Platform/Common/ApplicationCommon.h>

/************************************************************************/
/*			Application startup routine, called when
 *			Called when all platform related subsystem initialized		
 *			e.g. command line parsed, memory manager initialized, e.t.c.*/
/************************************************************************/
void ApplicationStart()
{
	// TODO: call here OpenGL initialization

	byte * b[2048];
	for(int i = 0 ; i < 2048 ; i++)
	{
		b[i] = (byte*)g_pPlatform->MemoryPools()->Alloc(2048);
	}

	for(int i = 0 ; i < 2048 ; i++)	
		g_pPlatform->MemoryPools()->Free(b[i]);
}

/************************************************************************/
/*          Application main cycle routine, called right after startup
 *          by platform abstraction layer								*/
/************************************************************************/
void ApplicationRun()
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
}

/************************************************************************/
/*          Application exception handling routine                      */
/************************************************************************/
void ApplicationOnException(ME_Editor::Exception * pException)
{
	// TODO: show here message box with my hands, ask user to save documents
	// and shutdown editor
}