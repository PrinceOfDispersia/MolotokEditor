/*
 *	Molotok Editor, 2014 (C) PrinceOfDispersia
 *	XGUI_Widget.cpp - basic gui window class
 *
 **/

#include <Platform/Common/ApplicationCommon.h>

using namespace ME_Framework::ME_XGUI;
extern XGUI_Tesselator * g_pTesselator;
extern XGUI_Manager * g_pGUIManager;


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
	if (pEvent.eventid == eventTypes::EV_MOUSE_KEY_DOWN)
	{
		if (pEvent.uParam1 == MKEY_LEFT)
		{
			m_bDragged = true;
			ME_Math::Vector2D cursor = g_pPlatform->GetCursorPos();
			ScreenToClient(cursor);
			m_vDragOrigin = cursor;
		}
	}
	else if (pEvent.eventid == eventTypes::EV_MOUSE_KEY_UP)
	{
		m_bDragged = false;
		m_DragKind = eWindowDragKinds::wdkNone;
	}
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
	XGUI_DrawScalableSheetGlyph(ActiveSkin(),m_Rect);

	color32_t c = {0,0,0,255};
	g_pTesselator->DefaultColor(c);

	size_t cSymbols = m_pGuiFontNormal->Calc_FittedSymbols(m_strCaption,m_Rect.ext.x - 54);

	String strFitted = m_strCaption;

	if (cSymbols < strFitted.Length())
	{
		((TCHAR*)strFitted.c_str())[cSymbols] = 0;
		strFitted.SetLength(cSymbols);
		
		
		size_t c = 0;

		while(c < 3 && cSymbols > 0)
		{
			((TCHAR*)strFitted.c_str())[--cSymbols] = _T('.');
			c++;
		}		
	}

	xgRect_t sz;	
	m_pGuiFontNormal->Calc_TextRect(strFitted,&sz);
	

	ME_Math::Vector2D textPos;
		
	textPos.x = m_Rect.pos.x + ((m_Rect.ext.x-54) / 2 - sz.ext.x / 2);
	textPos.y = m_Rect.pos.y + (((sz.ext.y-sz.pos.x) / 2)) + 1.5;

	m_pGuiFontNormal->Draw(textPos,strFitted);
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
	SetMarginsFromSkinset(ActiveSkin());
	m_DragKind = eWindowDragKinds::wdkNone;	

	ME_Math::Vector2D clientAreaStart;
	clientAreaStart.x = ActiveSkin()[0]->e[0];
	clientAreaStart.y = ActiveSkin()[0]->e[1];

	
	vec_t h = ActiveSkin()[1]->e[1];

	r.pos = ME_Math::Vector2D(m_Rect.ext.x - 50,h / 2 - 9);
	r.ext = ME_Math::Vector2D(18,18);

	XGUI_GenericButton * pMaximize = new XGUI_GenericButton(r);
	pMaximize->SetSkinSet(sprMaximize,3);
	pMaximize->SetAnchors(TAnchor::akTop | TAnchor::akRight);
	AddChildWidget(pMaximize);

	r.pos = ME_Math::Vector2D(m_Rect.ext.x - 30,h / 2 - 9);
	r.ext = ME_Math::Vector2D(18,18);

	XGUI_GenericButton * pClose = new XGUI_GenericButton(r);
	pClose->SetSkinSet(sprClose,3);
	pClose->SetAnchors(TAnchor::akTop | TAnchor::akRight);
	AddChildWidget(pClose);


	r.pos = m_vMargins[0];
	r.ext = ME_Math::Vector2D(150,27);

	XGUI_Button * pButton = new XGUI_Button(r);
	pButton->SetCaption(String(_T("akLeft | akTop")));
	pButton->SetAnchors(TAnchor::akTop | TAnchor::akLeft);
	AddChildWidget(pButton);

	r.pos = ME_Math::Vector2D(100,100);
	pButton = new XGUI_Button(r);
	pButton->SetCaption(String(_T("akBottom | akRight")));
	pButton->SetAnchors(TAnchor::akBottom | TAnchor::akRight);
	AddChildWidget(pButton);

	
	
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
	if (g_pGUIManager->CachedWidgetUnderCursor() != this) return;

	if (m_bDragged) 
	{
		g_pPlatform->SetCursor(m_CurrentCursor);
		return;
	}

	ME_Math::Vector2D c;

	c = g_pPlatform->GetCursorPos();
	ScreenToClient(c);
		

	// Left middle border
	vec_t  thickness = ActiveSkin()[3]->e[0];

	m_CurrentCursor = eMouseCursors::mcNormal;
	m_DragKind = eWindowDragKinds::wdkNone;

	if (c.x < thickness && c.x > 0)
	{
		if (c.y < thickness && c.y > 0)
		{
			m_CurrentCursor = eMouseCursors::mcSizeNWSE;
			g_pPlatform->SetCursor(m_CurrentCursor);
			m_DragKind = eWindowDragKinds::wdkXPos | eWindowDragKinds::wdkYPos;
		}
		else if (c.y > thickness && c.y < (m_Rect.ext.y - thickness))
		{
			m_CurrentCursor = eMouseCursors::mcSizeHorizontal;
			g_pPlatform->SetCursor(m_CurrentCursor);
			m_DragKind = eWindowDragKinds::wdkXPos;
		}
		else if (c.y > (m_Rect.ext.y - thickness) && c.y < m_Rect.ext.y)
		{
			m_CurrentCursor = eMouseCursors::mcSizeSWNE;
			g_pPlatform->SetCursor(m_CurrentCursor);
			m_DragKind = eWindowDragKinds::wdkXPos | eWindowDragKinds::wdkYExt;
		}
	}
	else if (c.x > thickness && c.x < (m_Rect.ext.x - thickness))
	{
		if (c.y < thickness && c.y > 0)
		{
			m_CurrentCursor = eMouseCursors::mcSizeVertical;
			g_pPlatform->SetCursor(m_CurrentCursor);
			m_DragKind = eWindowDragKinds::wdkYPos;
		}
		else if (c.y > thickness && c.y < ActiveSkin()[1]->e[1])
		{
			m_CurrentCursor = eMouseCursors::mcSizeCenter;
			g_pPlatform->SetCursor(m_CurrentCursor);
			m_DragKind = eWindowDragKinds::wdkPos;
		}
		else if (c.y > (m_Rect.ext.y - thickness) && c.y < m_Rect.ext.y)
		{
			m_CurrentCursor = eMouseCursors::mcSizeVertical;
			g_pPlatform->SetCursor(m_CurrentCursor);

			m_DragKind = eWindowDragKinds::wdkYExt;
		}
	}
	else if (c.x > (m_Rect.ext.x - thickness) && c.x < m_Rect.ext.x)
	{
		if (c.y < thickness && c.y > 0)
		{
			m_CurrentCursor = eMouseCursors::mcSizeSWNE;
			g_pPlatform->SetCursor(m_CurrentCursor);

			m_DragKind = eWindowDragKinds::wdkXExt | eWindowDragKinds::wdkYPos;
		}
		else if (c.y > thickness && c.y < (m_Rect.ext.y - thickness))
		{
			m_CurrentCursor = eMouseCursors::mcSizeHorizontal;
			g_pPlatform->SetCursor(m_CurrentCursor);

			m_DragKind = eWindowDragKinds::wdkXExt;
		}
		else if (c.y > (m_Rect.ext.y - thickness) && c.y < m_Rect.ext.y)
		{
			m_CurrentCursor = eMouseCursors::mcSizeNWSE;
			g_pPlatform->SetCursor(m_CurrentCursor);

			m_DragKind = eWindowDragKinds::wdkXExt | eWindowDragKinds::wdkYExt;
		}
	}
	
}

//************************************
// Method:    RecalcDrag
// FullName:  ME_Framework::ME_XGUI::XGUI_Window::RecalcDrag
// Access:    virtual private 
// Returns:   void
// Qualifier:
//************************************
void XGUI_Window::RecalcDrag()
{
	if (!m_bDragged) return;
	
	ME_Math::Vector2D v = g_pPlatform->GetCursorPos();
	ME_Math::Vector2D clip = g_pPlatform->GetClientAreaExtents();
	
	if (v.x < 0) v.x = 0;
	if (v.y < 0) v.y = 0;
	if (v.x > clip.x) v.x = clip.x;
	if (v.y > clip.y) v.y = clip.y;
	
	ME_Math::Vector2D v2 = v - m_vDragOrigin;

	xgRect_t old = m_Rect;

	if (m_DragKind == eWindowDragKinds::wdkPos)
	{
		ScreenToClient(v2);			
		m_Rect.pos += v2;		
		SetRect(m_Rect);
	}
	else
	{
		if (flags(m_DragKind & eWindowDragKinds::wdkXPos))
		{
			vec_t oldRight = m_Rect.Right();
			vec_t oldPos = m_Rect.pos.x;
			m_Rect.pos.x = v2.x;
			m_Rect.ext.x = oldRight - m_Rect.pos.x;

			if (m_Rect.ext.x < 16) 
			{
				m_Rect.pos.x = oldPos;
				m_Rect.ext.x = oldRight - m_Rect.pos.x;				
			}

		}

		if (flags(m_DragKind & eWindowDragKinds::wdkYPos))
		{
			vec_t oldBottom = m_Rect.Bottom();
			vec_t oldPos = m_Rect.pos.y;
			m_Rect.pos.y = v2.y;
			m_Rect.ext.y = oldBottom - m_Rect.pos.y;

			vec_t minHeight = ActiveSkin()[1]->e[1] + ActiveSkin()[6]->e[1];

			if (m_Rect.ext.y < minHeight) 
			{
				m_Rect.pos.y = oldPos;
				m_Rect.ext.y = oldBottom - m_Rect.pos.y;				
			}

		}
		
		if (flags(m_DragKind & eWindowDragKinds::wdkXExt))
		{
			ME_Math::Vector2D v = g_pPlatform->GetCursorPos();
			ScreenToClient(v);			
			m_Rect.ext.x = v.x;

			if (m_Rect.ext.x < 60) m_Rect.ext.x = 60;
		}

		if (flags(m_DragKind & eWindowDragKinds::wdkYExt))
		{
			ME_Math::Vector2D v = g_pPlatform->GetCursorPos();
			ScreenToClient(v);			
			m_Rect.ext.y = v.y;

			vec_t minHeight = ActiveSkin()[1]->e[1] + ActiveSkin()[6]->e[1];

			if (m_Rect.ext.y < minHeight)
				m_Rect.ext.y = minHeight;
		}

		ExpandRectByContents();
		SetRect(m_Rect);
	}


		
}

//************************************
// Method:    ActiveSkin
// FullName:  ME_Framework::ME_XGUI::XGUI_Window::ActiveSkin
// Access:    private 
// Returns:   mSheetGlyph_t **
// Qualifier:
//************************************
mSheetGlyph_t ** XGUI_Window::ActiveSkin()
{
	return IsFocused() ? sprDialogActive : sprDialogInActive;
}

void XGUI_Window::ExpandRectByContents()
{
	for(TWidgetSharedPtr w : m_vChilds)
	{
		if (auto widget = w.get())
		{
			if (widget->GetRect().Right()  > (GetRect().ext.x - m_vMargins[1].x))
				GetRect().ext.x = widget->GetRect().Right() + m_vMargins[1].x;
			if (widget->GetRect().Bottom()  > (GetRect().ext.y - m_vMargins[1].y))
				GetRect().ext.y = widget->GetRect().Bottom() + m_vMargins[1].y;
		}

	}
}
