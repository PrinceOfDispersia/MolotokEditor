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
	// TODO: initialize OpenGL here

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
}

/************************************************************************/
/*          Application exception handling routine                      */
/************************************************************************/
void ApplicationOnException(ME_Framework::Exception * pException)
{
	// TODO: show here message box with my hands, ask user to save documents
	// and shutdown editor
}