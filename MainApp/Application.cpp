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
	r.ext = ME_Math::Vector2D(150,24);

	XGUI_Button * pButton = new XGUI_Button(&r);
	pButton->SetCaption(String(_T("The quick brown fox jumped over a lazy dog")));

	pManager->AddWidget(pButton);

	tinyxml2::XMLDocument doc;
	
	tinyxml2::XMLElement * e = doc.NewElement("GUIScheme");
	tinyxml2::XMLElement * c = doc.NewElement("Colors");
	tinyxml2::XMLElement * c2 = doc.NewElement("Desktop");

	e->InsertFirstChild(c);
	c->InsertFirstChild(c2);
	
	c2->SetAttribute("r",255);
	c2->SetAttribute("g",0);
	c2->SetAttribute("b",0);

	doc.InsertFirstChild(e);

	FILE * fp = fopen("1.xml","wb");

	doc.SaveFile(fp,false);

	fclose(fp);
	

	
	
	

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
	// TODO: write here some code)
	// 	
	ShutdownImageLib();
}

/************************************************************************/
/*			Events handler                                              */
/************************************************************************/
void ApplicationPumpEvent(ME_Framework::appEvent_t & ev)
{
	//_tprintf(_T("ApplicationPumpEvent(): eventid = %d, uParam1 = %d\n"),ev.eventid,ev.uParam1);

	switch(ev.eventid)
	{
	case eventTypes::EV_MOUSE_KEY_DOWN:
	case eventTypes::EV_MOUSE_KEY_UP:
		pManager->HandleEvent(ev);
		break;
	}

}