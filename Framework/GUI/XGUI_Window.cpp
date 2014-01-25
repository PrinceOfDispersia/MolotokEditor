/*
 *	Molotok Editor, 2014 (C) PrinceOfDispersia
 *	XGUI_Widget.cpp - basic gui window class
 *
 **/

#include <Platform/Common/ApplicationCommon.h>

using namespace ME_Framework::ME_XGUI;


//************************************
// Method:    HandleEvent
// FullName:  ME_Framework::ME_XGUI::XGUI_Window::HandleEvent
// Access:    virtual public 
// Returns:   void
// Qualifier: handles application event
// Parameter: ME_Framework::appEvent_t & pEvent
//************************************
void XGUI_Window::HandleEvent( ME_Framework::appEvent_t & pEvent )
{

}

//************************************
// Method:    DrawComponent
// FullName:  ME_Framework::ME_XGUI::XGUI_Window::DrawComponent
// Access:    virtual public 
// Returns:   void
// Qualifier:
//************************************
void XGUI_Window::DrawComponent()
{
	XGUI_DrawScalableSheetGlyph(sprDialogActive,m_Rect);
}

//************************************
// Method:    Think
// FullName:  ME_Framework::ME_XGUI::XGUI_Window::Think
// Access:    virtual public 
// Returns:   void
// Qualifier:
//************************************
void XGUI_Window::Think()
{

}

//************************************
// Method:    ~XGUI_Window
// FullName:  ME_Framework::ME_XGUI::XGUI_Window::~XGUI_Window
// Access:    public 
// Returns:   
// Qualifier:
//************************************
XGUI_Window::~XGUI_Window()
{

}

//************************************
// Method:    XGUI_Window
// FullName:  ME_Framework::ME_XGUI::XGUI_Window::XGUI_Window
// Access:    public 
// Returns:   
// Qualifier: : XGUI_Widget(r)
// Parameter: xgRect_t & r
//************************************
XGUI_Window::XGUI_Window( xgRect_t & r ): XGUI_Widget(r)
{

}
