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

	// Remove me: doing some testing of classes and trying out fancy "auto" :)
	ME_Math::Vector2D test1(100);
	test1+=ME_Math::Vector2D(500,100);
	test1*=2.0f;
	auto test2 = test1/2.4f;
	
	// TODO: make null termination checks
	TCHAR tmp[2];
	_sntprintf(tmp,ARRAY_SIZE(tmp),_T("%f %f"),test1.x,test1.y);

	MemoryPool * pPool = new MemoryPool(_T("Global"),0);

	MemoryPool * pPool2 = pPool->AllocSubPool(_T("Strings"));
	pPool->Alloc(100);
	pPool2->Alloc((size_t)8192*1024);

	delete pPool;
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