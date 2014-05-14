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
		m_DragKind = TWindowDragKinds::wdkNone;
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
	XGUI_DrawScalableSheetGlyph(ActiveSkin(),m_WorkRect);
	
	size_t cSymbols = m_pGuiFontNormal->Calc_FittedSymbols(m_strCaption,m_WorkRect.ext.x - 54);

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

	xgRect_t fitRect;
	fitRect.pos = m_WorkRect.pos;
	fitRect.ext = m_WorkRect.ext;

	fitRect.Implode(xAxis,ActiveSkin()[0]->e[0]);
	
	
	fitRect.ext.y = ActiveSkin()[0]->e[1];

	m_pGuiFontNormal->SetTextColor(0,0,0,255);
	m_pGuiFontNormal->DrawAlignedText(strFitted,fitRect,THorizontalAligment::alhCenter,TVerticalAligment::alvCenter);
	
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
	m_DragKind = TWindowDragKinds::wdkNone;	

	ME_Math::Vector2D clientAreaStart;
	clientAreaStart.x = ActiveSkin()[0]->e[0];
	clientAreaStart.y = ActiveSkin()[0]->e[1];

	
	vec_t h = ActiveSkin()[1]->e[1];

	r.pos = ME_Math::Vector2D(m_WorkRect.ext.x - 50,h / 2 - 9);
	r.ext = ME_Math::Vector2D(18,18);

	XGUI_GenericButton * pMaximize = new XGUI_GenericButton(r);
	pMaximize->SetSkinSet(sprMaximize,3);
	pMaximize->SetAnchors(TAnchor::akTop | TAnchor::akRight);
	AddChildWidget(pMaximize);

	r.pos = ME_Math::Vector2D(m_WorkRect.ext.x - 30,h / 2 - 9);
	r.ext = ME_Math::Vector2D(18,18);

	XGUI_GenericButton * pClose = new XGUI_GenericButton(r);
	pClose->SetSkinSet(sprClose,3);
	pClose->SetAnchors(TAnchor::akTop | TAnchor::akRight);
	AddChildWidget(pClose);


	r.pos.Init();r.ext.Init();
	XGUI_MenuBar * pBar = new XGUI_MenuBar(r);

	pBar->AddItem(String(_T("Ololo")),0);
	pBar->AddItem(String(_T("Ololo")),0);
	pBar->AddItem(String(_T("Ololo")),0);

	AddChildWidget(pBar);


	
//	MessageBoxA(0,info().members()[0].name,"",MB_ICONERROR);
	
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
	m_DragKind = TWindowDragKinds::wdkNone;

	if (c.x < thickness && c.x > 0)
	{
		if (c.y < thickness && c.y > 0)
		{
			m_CurrentCursor = eMouseCursors::mcSizeNWSE;
			g_pPlatform->SetCursor(m_CurrentCursor);
			m_DragKind = TWindowDragKinds::wdkXPos | TWindowDragKinds::wdkYPos;
		}
		else if (c.y > thickness && c.y < (m_WorkRect.ext.y - thickness))
		{
			m_CurrentCursor = eMouseCursors::mcSizeHorizontal;
			g_pPlatform->SetCursor(m_CurrentCursor);
			m_DragKind = TWindowDragKinds::wdkXPos;
		}
		else if (c.y > (m_WorkRect.ext.y - thickness) && c.y < m_WorkRect.ext.y)
		{
			m_CurrentCursor = eMouseCursors::mcSizeSWNE;
			g_pPlatform->SetCursor(m_CurrentCursor);
			m_DragKind = TWindowDragKinds::wdkXPos | TWindowDragKinds::wdkYExt;
		}
	}
	else if (c.x > thickness && c.x < (m_WorkRect.ext.x - thickness))
	{
		if (c.y < thickness && c.y > 0)
		{
			m_CurrentCursor = eMouseCursors::mcSizeVertical;
			g_pPlatform->SetCursor(m_CurrentCursor);
			m_DragKind = TWindowDragKinds::wdkYPos;
		}
		else if (c.y > thickness && c.y < ActiveSkin()[1]->e[1])
		{
			m_CurrentCursor = eMouseCursors::mcSizeCenter;
			g_pPlatform->SetCursor(m_CurrentCursor);
			m_DragKind = TWindowDragKinds::wdkPos;
		}
		else if (c.y > (m_WorkRect.ext.y - thickness) && c.y < m_WorkRect.ext.y)
		{
			m_CurrentCursor = eMouseCursors::mcSizeVertical;
			g_pPlatform->SetCursor(m_CurrentCursor);

			m_DragKind = TWindowDragKinds::wdkYExt;
		}
	}
	else if (c.x > (m_WorkRect.ext.x - thickness) && c.x < m_WorkRect.ext.x)
	{
		if (c.y < thickness && c.y > 0)
		{
			m_CurrentCursor = eMouseCursors::mcSizeSWNE;
			g_pPlatform->SetCursor(m_CurrentCursor);

			m_DragKind = TWindowDragKinds::wdkXExt | TWindowDragKinds::wdkYPos;
		}
		else if (c.y > thickness && c.y < (m_WorkRect.ext.y - thickness))
		{
			m_CurrentCursor = eMouseCursors::mcSizeHorizontal;
			g_pPlatform->SetCursor(m_CurrentCursor);

			m_DragKind = TWindowDragKinds::wdkXExt;
		}
		else if (c.y > (m_WorkRect.ext.y - thickness) && c.y < m_WorkRect.ext.y)
		{
			m_CurrentCursor = eMouseCursors::mcSizeNWSE;
			g_pPlatform->SetCursor(m_CurrentCursor);

			m_DragKind = TWindowDragKinds::wdkXExt | TWindowDragKinds::wdkYExt;
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

	xgRect_t old = m_WorkRect;

	if (m_DragKind == TWindowDragKinds::wdkPos)
	{
		ScreenToClient(v2);			
		m_WorkRect.pos += v2;		
		SetRect(m_WorkRect);
	}
	else
	{
		ME_Math::Vector2D vOldPos = m_WorkRect.pos;
		ME_Math::Vector2D vOldExt = m_WorkRect.ext;

		if (flags(m_DragKind & TWindowDragKinds::wdkXPos))
		{
			vec_t oldRight = m_WorkRect.Right();
			vec_t oldPos = m_WorkRect.pos.x;
			m_WorkRect.pos.x = v2.x;
			m_WorkRect.ext.x = oldRight - m_WorkRect.pos.x;

			if (m_WorkRect.ext.x < 16) 
			{
				m_WorkRect.pos.x = oldPos;
				m_WorkRect.ext.x = oldRight - m_WorkRect.pos.x;				
			}

		}

		if (flags(m_DragKind & TWindowDragKinds::wdkYPos))
		{
			vec_t oldBottom = m_WorkRect.Bottom();
			vec_t oldPos = m_WorkRect.pos.y;
			m_WorkRect.pos.y = v2.y;
			m_WorkRect.ext.y = oldBottom - m_WorkRect.pos.y;

			vec_t minHeight = ActiveSkin()[1]->e[1] + ActiveSkin()[6]->e[1];

			if (m_WorkRect.ext.y < minHeight) 
			{
				m_WorkRect.pos.y = oldPos;
				m_WorkRect.ext.y = oldBottom - m_WorkRect.pos.y;				
			}

		}
		
		if (flags(m_DragKind & TWindowDragKinds::wdkXExt))
		{
			ME_Math::Vector2D v = g_pPlatform->GetCursorPos();
			ScreenToClient(v);			
			m_WorkRect.ext.x = v.x;

			if (m_WorkRect.ext.x < 60) m_WorkRect.ext.x = 60;
		}

		if (flags(m_DragKind & TWindowDragKinds::wdkYExt))
		{
			ME_Math::Vector2D v = g_pPlatform->GetCursorPos();
			ScreenToClient(v);			
			m_WorkRect.ext.y = v.y;

			vec_t minHeight = ActiveSkin()[1]->e[1] + ActiveSkin()[6]->e[1];

			if (m_WorkRect.ext.y < minHeight)
				m_WorkRect.ext.y = minHeight;
		}

		SetRect(m_WorkRect);
 		
		int rf = ExpandRectByContents();
 		if (rf & rfLockXPos)
 		{
 			m_WorkRect.pos.x = vOldPos.x;
 			m_WorkRect.ext.x = vOldExt.x;
 			
 		}
 		if (rf & rfLockYPos)
 		{
 			m_WorkRect.pos.y = vOldPos.y;
 			m_WorkRect.ext.y = vOldExt.y;
 		}
		
		SetRect(m_WorkRect);
		
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

int XGUI_Window::ExpandRectByContents()
{
	int ret_flags = 0;
	vec_t max_cont_w = 0;
	vec_t max_cont_h = 0;

	for(TWidgetSharedPtr w : m_vChilds)
	{
		if (auto widget = w.get())
		{
			if (!flags(widget->GetAnchors() & TAnchor::akLeft) && !flags(widget->GetAnchors() & TAnchor::akTop))
				continue;

			if (widget->GetRect().Right()  > max_cont_w)
			{
				max_cont_w = widget->GetRect().Right();
			}
			if (widget->GetRect().Bottom()  > max_cont_h)
			{
				max_cont_h = widget->GetRect().Bottom();				
			}
		}

	}

	if (max_cont_w > (GetRect().ext.x - m_vMargins[1].x))
	{
		GetRect().ext.x = max_cont_w + m_vMargins[1].x;
		if (flags(m_DragKind & TWindowDragKinds::wdkXPos))
			ret_flags |= rfLockXPos;
	}
	if (max_cont_h > (GetRect().ext.y - m_vMargins[1].y))
	{
		GetRect().ext.y = max_cont_h + m_vMargins[1].y;
		if (flags(m_DragKind & TWindowDragKinds::wdkYPos))
			ret_flags |= rfLockYPos;
	}

	return ret_flags;
}
