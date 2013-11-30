/*
*	Molotok Editor, 2013 (C) PrinceOfDispersia
*	Application.cpp - entry point for cross-platform application
*
**/

#include <Platform/Common/ApplicationCommon.h>

using namespace ME_Framework::ME_XGUI;

XGUI_Manager * pManager;

/************************************************************************/
/*			Application startup routine, 
*			Called when all platform related subsystem initialized		
*			e.g. command line parsed, memory manager initialized, e.t.c.*/
/************************************************************************/
void ApplicationStart()
{
	// Start image library
	InitImageLib();

	pManager = new XGUI_Manager;

	xgRect_t r;
	r.pos = ME_Math::Vector2D(100,100);
	r.ext = ME_Math::Vector2D(20,24);



	for(int i = 0 ; i < 4 ; i++)
	{
		TAlign a[4] = {TAlign::alLeft,TAlign::alRight,TAlign::alTop,TAlign::alBottom};
		int order[4] = {0,2,3,3};
		TCHAR str[][16] = {_T("alLeft"),_T("alRight"),_T("alTop"),_T("alBottom")};

		for(int j = 0 ; j < 3 ; j++)
		{
			XGUI_Button * pButton = new XGUI_Button(&r);		
			pButton->SetCaption(String(VA(_T("%s"),str[i])));
			pButton->SetAlign(a[i]);
			pButton->SetAlignPriority(order[i]);
			pManager->AddWidget(pButton);
		}

		
	}
	

	XGUI_Button * pButton = new XGUI_Button(&r);
	pButton->SetCaption(String(_T("alClientArea")));	
	pButton->SetAlign(TAlign::alClientArea);

	pManager->AddWidget(pButton);
	
	
}

/************************************************************************/
/*          Application main cycle routine, called right after startup
*          by platform abstraction layer								*/
/************************************************************************/
void ApplicationRun(float flFrameDelta)
{
	// TODO: load here graphics and additional resources, while showing	
	// fancy loading box to user, then run main logic cycle
	
	pManager->Think(flFrameDelta);
	pManager->Draw();


}

/************************************************************************/
/*          Application shutdown routine
*          called when application shuts down, free resource here	 	*/
/************************************************************************/
void ApplicationShutdown()
{
	ShutdownImageLib();	
	delete pManager;
	
}

/************************************************************************/
/*			Events handler                                              */
/************************************************************************/
void ApplicationPumpEvent(ME_Framework::appEvent_t & ev)
{
	//_tprintf(_T("ApplicationPumpEvent(): eventid = %d, uParam1 = %d\n"),ev.eventid,ev.uParam1);

	if (flags(ev.eventid & eventTypes::EV_GUI_EV_MASK))
	{
		pManager->HandleEvent(ev);
	}
}