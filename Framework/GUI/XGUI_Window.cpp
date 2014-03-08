/*
 *	Molotok Editor, 2014 (C) PrinceOfDispersia
 *	XGUI_Widget.cpp - basic gui window class
 *
 **/

#include <Platform/Common/ApplicationCommon.h>

using namespace ME_Framework::ME_XGUI;
extern XGUI_Tesselator * g_pTesselator;


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

	color32_t c = {0,0,0,255};
	g_pTesselator->DefaultColor(c);

	xgRect_t sz;
	ME_Framework::String str = _T("The quick brow fox jumped over a lazy dog");
	m_pGuiFontNormal->Calc_TextRect(str,&sz);
		
	ME_Math::Vector2D textPos;

	int h = sprDialogActive[0]->e[1];

	textPos.x = m_Rect.pos.x + (m_Rect.ext.x / 2 - sz.ext.x / 2);
	textPos.y = m_Rect.pos.y + (((sz.ext.y-sz.pos.x) / 2));

	m_pGuiFontNormal->Draw(textPos,str);
	
	XGUI_DrawRectOutline(sz + textPos);
	g_pTesselator->ResetDefaultColor();
	
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
	CheckBorders();	
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

//************************************
// Method:    CheckBorders
// FullName:  ME_Framework::ME_XGUI::XGUI_Window::CheckBorders
// Access:    private 
// Returns:   void
// Qualifier:
//************************************
void XGUI_Window::CheckBorders()
{
	ME_Math::Vector2D c;

	c = g_pPlatform->GetCursorPos();
	ScreenToClient(c);

	

	// Left middle border
	vec_t  thickness = sprDialogActive[3]->e[0];

	eMouseCursors newCursor =  eMouseCursors::mcNormal;

	if (c.x < thickness)
	{
		if (c.y < thickness && c.y > 0)
		{
			newCursor = eMouseCursors::mcSizeNWSE;
			g_pPlatform->SetCursor(newCursor);
		}
		else if (c.y > thickness && c.y < (m_Rect.ext.y - thickness))
		{
			newCursor = eMouseCursors::mcSizeHorizontal;
			g_pPlatform->SetCursor(newCursor);
		}
		else if (c.y > (m_Rect.ext.y - thickness) && c.y < m_Rect.ext.y)
		{
			newCursor = eMouseCursors::mcSizeSWNE;
			g_pPlatform->SetCursor(newCursor);
		}
	}
	else if (c.x > thickness && c.x < (m_Rect.ext.x - thickness))
	{
		if (c.y < thickness && c.y > 0)
		{
			newCursor = eMouseCursors::mcSizeVertical;
			g_pPlatform->SetCursor(newCursor);
		}
		else if (c.y > (m_Rect.ext.y - thickness) && c.y < m_Rect.ext.y)
		{
			newCursor = eMouseCursors::mcSizeVertical;
			g_pPlatform->SetCursor(newCursor);
		}
	}
	else if (c.x > (m_Rect.ext.x - thickness) && c.x < m_Rect.ext.x)
	{
		if (c.y < thickness && c.y > 0)
		{
			newCursor = eMouseCursors::mcSizeSWNE;
			g_pPlatform->SetCursor(newCursor);
		}
		else if (c.y > thickness && c.y < (m_Rect.ext.y - thickness))
		{
			newCursor = eMouseCursors::mcSizeHorizontal;
			g_pPlatform->SetCursor(newCursor);
		}
		else if (c.y > (m_Rect.ext.y - thickness) && c.y < m_Rect.ext.y)
		{
			newCursor = eMouseCursors::mcSizeNWSE;
			g_pPlatform->SetCursor(newCursor);
		}
	}

	

}
