/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	XGUI_Dock.cpp - dock and dockable container classes
 *
 **/

#include <Platform/Common/ApplicationCommon.h>

using namespace ME_Framework::ME_XGUI;
using namespace ME_Framework::ME_OpenGLBackend;

/*
 *	Dock constructor
 **/
XGUI_Dock::XGUI_Dock(xgRect_t & r) : XGUI_Widget(r)
{
	m_bRenderOutline = false;
}

/*
 *	render debug dock border
 **/
void XGUI_Dock::DrawComponent()
{
	glLineWidth(4);
	glColor4f(0,1,0,1);

	if (m_bRenderOutline) 
		XGUI_DrawRectOutline(m_Rect);

	glLineWidth(1);
}

/*
 *	Think routine
 **/
void XGUI_Dock::Think()
{
	if (!m_pParent) return;

	const std::vector<XGUI_Widget*> & c = m_pParent->GetChilds();
	
	m_bRenderOutline = false;

	for(XGUI_Widget * w : c)
	{		
		if (!w->IsDragged()) continue;		
		if (!w->IsDockable()) continue;
		

		auto r = w->GetRect();
		if (!RectIntersection(m_Rect,r)) continue;
		
		w->SetDockWidget(this);
		m_bRenderOutline = true;
	}
}


/*
 *	Constructor
 **/
XGUI_DockWindow::XGUI_DockWindow(xgRect_t & r) : XGUI_Widget(r)
{
	
	r.pos = ME_Math::Vector2D(0,0);
	r.ext = ME_Math::Vector2D(10,26);
	m_pDragButton = new XGUI_GenericButton(r);


	mSheetGlyph_t *spr[] = {sprDragHandleDotsNormal,sprDragHandleDotsHovered};
	m_pDragButton->SetSkinSet(spr,2);

	AddChildWidget(m_pDragButton);

	m_pDragButton->SetButtonID(1);

	m_Orientation = TOrientation::orVertical;
	SetDockState(TDockState::dsFloating);

	m_bDockable = true;
}

/*
 *	Render dock window
 **/
void XGUI_DockWindow::DrawComponent()
{
	GL_DisableState(GLS_TEXTURE_2D); 
	GL_EnableState(GLS_BLEND);

	glColor4f(1,1,1,1);
	
	XGUI_DrawScalableSheetGlyph(sprFloatingPanel,m_Rect);
	GL_EnableState(GLS_TEXTURE_2D);
	
}

/*
 *	Event handler
 **/
void XGUI_DockWindow::HandleEvent(ME_Framework::appEvent_t & pEvent)
{
	if (pEvent.eventid == eventTypes::EV_CHILD_BUTTON_PUSHED 
							|| (pEvent.eventid == eventTypes::EV_MOUSE_KEY_DOWN && m_DockState == TDockState::dsFloating))
	{
		m_bDragged = true;
		m_vDragOrigin = g_pPlatform->GetCursorPos();
		
		if (m_DockState == TDockState::dsDocked)
		{
			m_Rect.ext = m_PreDockRect.ext;
			SetRect(m_Rect);
		}
		PointToClient(m_vDragOrigin);
		SetDockState(TDockState::dsFloating);
	}
	else if (pEvent.eventid == eventTypes::EV_CHILD_BUTTON_RELEASED 
							|| (pEvent.eventid == eventTypes::EV_MOUSE_KEY_UP))
	{
		if (m_pDockWidget && m_DockState == TDockState::dsFloating)
		{
 			xgRect_t r = m_pDockWidget->GetRect();
 			if (!RectIntersection(m_Rect,r))
 			{
								
 				SetDockState(TDockState::dsFloating);
 				m_Rect.ext = m_PreDockRect.ext;
 				m_bDragged = false;
 				return;
 			}

			m_PreDockRect = m_Rect;
			SetRect(m_Rect = m_pDockWidget->GetRect());			
			SetDockState(TDockState::dsDocked);
			
			switch(m_pDockWidget->GetAlign())
			{
			case TAlign::alTop:
			case TAlign::alBottom:
				m_Orientation = TOrientation::orHorizontal;
				break;
			case TAlign::alLeft:
			case TAlign::alRight:
				m_Orientation = TOrientation::orVertical;
				break;
			}

			return;
		}
		m_bDragged = false;
				
	}

}

/*
 *	
 **/
void XGUI_DockWindow::Think()
{
	m_pDragButton->SetVisibilty(m_DockState == TDockState::dsDocked);

	if (!m_pDragButton->GetVisibilty()) return;

	switch(m_Orientation)
	{
	case TOrientation::orVertical:
		{
			xgRect_t r;

			r.ext.x = 20;
			r.ext.y = 6;

			r.pos.x = m_Rect.ext.x / 2 - r.ext.x / 2;
			r.pos.y = 4;

			m_pDragButton->SetRect(r);
		}
		break;
	case TOrientation::orHorizontal:
		{
			xgRect_t r;

			r.ext.x = 6;
			r.ext.y = 20;

			r.pos.y = m_Rect.ext.y / 2 - r.ext.y / 2;
			r.pos.x = 4;

			m_pDragButton->SetRect(r);
		}
		break;
	}
}
