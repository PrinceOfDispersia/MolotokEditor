///*
//*	Molotok Editor, 2013 (C) PrinceOfDispersia
//*	XGUI_Dock.cpp - dock and dockable container classes
//*
//**/
//
///*
//#include <Platform/Common/ApplicationCommon.h>
//
//using namespace ME_Framework::ME_XGUI;
//using namespace ME_Framework::ME_OpenGLBackend;
//extern XGUI_Tesselator * g_pTesselator;
//extern XGUI_Manager * g_pGUIManager;
//
///*
//*	Dock constructor
//**/
//XGUI_Dock::XGUI_Dock(xgRect_t & r) : XGUI_Widget(r)
//{
//	m_bRenderOutline = false;
//}
//
///*
//*	render debug dock border
//**/
//void XGUI_Dock::DrawComponent()
//{
//	glLineWidth(4);
//	
//
//	glColor4f(0,1,1,1);
//	XGUI_DrawRectOutline(m_Rect);
//
//	glColor4f(0,1,0,1);
//
//	if (m_bRenderOutline) 
//		XGUI_DrawRectOutline(m_OutlineRect);
//
//	glLineWidth(1);
//}
//
///*
//*	Think routine
//**/
//void XGUI_Dock::Think()
//{
//	if (!m_pParent) return;
//
//	auto & c = m_pParent->GetChilds();
//
//	m_bRenderOutline = false;
//
//	for(auto w : c)
//	{		
//		if (!w->IsDragged()) continue;		
//		if (!w->IsDockable()) continue;
//
//
//		auto r = w->GetRect();
//		if (!RectIntersection(m_Rect,r)) 
//				continue;
//
//		w->SetDockWidget(this);
//		m_bRenderOutline = true;
//
//		m_OutlineRect = m_Rect;
//
//		if (m_vChilds.size() > 0)
//		{
//			switch(m_Align)
//			{
//			case TAlign::alTop:
//				{
//					m_OutlineRect.ext[1] += 32;
//				}
//				break;
//			case TAlign::alBottom:
//				{				
//					m_OutlineRect.ext[1] += 32;
//					m_OutlineRect.pos[1] -= 32;
//				}
//				break;
//			case TAlign::alLeft:
//				{
//					m_OutlineRect = m_Rect;
//					m_OutlineRect.ext[0] += 32;				
//				}
//			case TAlign::alRight:
//				{
//					m_OutlineRect.ext[0] += 32;
//					m_OutlineRect.pos[0] -= 32;
//				}
//				break;
//			}
//		}
//		
//	}
//}
//
///*
//*	Event handler
//**/
//void XGUI_Dock::HandleEvent(ME_Framework::appEvent_t & pEvent)
//{
//	if (pEvent.eventid == eventTypes::EV_ALIGN_PERFORMED)
//	{
//		switch(m_Align)
//		{
//		case TAlign::alTop:
//		case TAlign::alBottom:
//			//RecalcRectUsingContents();
//			RealignDockedItems(TItemAlignOrder::iaUpDownLeftToRight);
//			break;
//		case TAlign::alLeft:
//		case TAlign::alRight:
//			//RecalcRectUsingContents();
//			RealignDockedItems(TItemAlignOrder::iaLeftToRightUpToDown);
//			break;
//		}
//	}
//}
//
//
///*
//*	Constructor
//**/
//XGUI_DockWindow::XGUI_DockWindow(xgRect_t & r) : XGUI_Widget(r)
//{
//
//	r.pos = ME_Math::Vector2D(0,4);
//	r.ext = ME_Math::Vector2D(10,26);
//	m_pDragButton = new XGUI_GenericButton(r);
//
//	mSheetGlyph_t *spr[] = {sprDragHandleDotsNormal,sprDragHandleDotsHovered};
//	m_pDragButton->SetSkinSet(spr,2);
//
//	AddChildWidget(m_pDragButton);
//
//	m_pDragButton->SetButtonID(1);
//
//	m_Orientation = TOrientation::orVertical;
//	SetDockState(TDockState::dsFloating);
//
//	m_bDockable = true;
//
//	m_vMargins[0].x =  sprFloatingPanel[0]->e[0];
//	m_vMargins[0].y =  sprFloatingPanel[0]->e[1];
//	
//	m_vMargins[1].x =  sprFloatingPanel[8]->e[0];
//	m_vMargins[1].y =  sprFloatingPanel[8]->e[1];
//
//	m_Orientation = TOrientation::orHorizontal;
//}
//
///*
//*	Render dock window
//**/
//void XGUI_DockWindow::DrawComponent()
//{
//	XGUI_DrawScalableSheetGlyph(sprFloatingPanel,m_Rect);
//
//	//g_pTesselator->DefaultColor(g_pGUIManager->GuiSettings()->m_cDesktopBG);
//
//	//XGUI_DrawSheetGlyph(sprWhite,m_Rect);
//
//	if (m_DockState == TDockState::dsFloating)
//	{		
//
//		xgRect_t cl = m_Rect;
//		CalcClientRect(cl);
//
//		cl.pos.x += sprFloatingPanel[0]->e[0];
//
//		color32_t c;
//		c.r = 0; c.g = 0; c.b = 0; c.a = 255;
//		g_pTesselator->DefaultColor(c);
//		m_pGuiFontNormal->Draw(cl.pos,m_strCaption);
//
//		
//	}
//	g_pTesselator->ResetDefaultColor();
//}
//
///*
//*	Event handler
//**/
//void XGUI_DockWindow::HandleEvent(ME_Framework::appEvent_t & pEvent)
//{
//	if (pEvent.eventid == eventTypes::EV_CHILD_BUTTON_PUSHED 
//		|| (pEvent.eventid == eventTypes::EV_MOUSE_KEY_DOWN && m_DockState == TDockState::dsFloating))
//	{
//		SetDragged(true);
//		m_vDragOrigin = g_pPlatform->GetCursorPos();
//		
//		PointToClient(m_vDragOrigin);
//		
//		bool bUndocked = (m_DockState == TDockState::dsDocked);
//		SetDockState(TDockState::dsFloating);
//		
//		if (bUndocked)
//		{
//			m_Rect.ext = m_PreDockRect.ext;	
//			RecalcItemPositions();
//			
//			SetRect(m_Rect);
//		}
//
//		
//	}
//	else if (pEvent.eventid == eventTypes::EV_CHILD_BUTTON_RELEASED 
//		|| (pEvent.eventid == eventTypes::EV_MOUSE_KEY_UP))
//	{
//		if (m_pDockWidget && m_DockState == TDockState::dsFloating)
//		{
//			xgRect_t r = m_pDockWidget->GetRect();
//			if (!RectIntersection(m_Rect,r))
//			{			
//				m_pDockWidget = 0;
//				SetDockState(TDockState::dsFloating);
//				m_Rect.ext = m_PreDockRect.ext;
//				SetDragged(false);
//				RecalcItemPositions();
//				return;
//			}
//
//			m_PreDockRect = m_Rect;
//
//			switch(m_pDockWidget->GetAlign())
//			{
//			case TAlign::alTop:
//			case TAlign::alBottom:
//				m_Orientation = TOrientation::orHorizontal;
//				m_Rect.ext.x = m_pDockWidget->GetRect().ext.x;
//				break;
//			case TAlign::alLeft:
//			case TAlign::alRight:
//				m_Orientation = TOrientation::orVertical;
//				m_Rect.ext.y = m_pDockWidget->GetRect().ext.y;
//				break;
//			}
//
//			
//			
//			
//			
//			SetRect(m_Rect);			
//			CompressRect(m_Orientation);
//			SetDockState(TDockState::dsDocked);
//			
//
//			RecalcItemPositions();
//			return;
//		}
//
//		SetDragged(false);
//
//	}
//	else if (pEvent.eventid == eventTypes::EV_ALIGN_PERFORMED)
//	{
//		RecalcItemPositions();
//	}
//
//}
//
///*
//*	Think routine	
//**/
//void XGUI_DockWindow::Think()
//{
//	m_pDragButton->SetVisibilty(m_DockState == TDockState::dsDocked);
//
//	if (!m_pDragButton->GetVisibilty()) return;
//
//	switch(m_Orientation)
//	{
//	case TOrientation::orVertical:
//		{
//			xgRect_t r;
//
//			r.ext.x = 20;
//			r.ext.y = 6;
//
//			r.pos.x = m_Rect.ext.x / 2 - r.ext.x / 2;
//			r.pos.y = 4;
//
//			m_pDragButton->SetRect(r);
//		}
//		break;
//	case TOrientation::orHorizontal:
//		{
//			xgRect_t r;
//
//			r.ext.x = 6;
//			r.ext.y = 20;
//
//			r.pos.y = m_Rect.ext.y / 2 - r.ext.y / 2 + 4;
//			r.pos.x = 4;
//
//			m_pDragButton->SetRect(r);
//		}
//		break;
//	}
//}
//
///*
//*	Flows items according orientation
//**/
//void XGUI_DockWindow::RecalcItemPositions()
//{
//		
//	switch(m_Orientation)
//	{
//	case TOrientation::orHorizontal:	
//		{
//			m_Rect.ext.y = 0;
//			vec_t ox = m_vMargins[0].x + m_pDragButton->GetRect().ext.x;
//			vec_t oy = m_vMargins[0].y;
//
//			for(auto w : m_vChilds)
//			{
//				if (w.get() == m_pDragButton) 
//					continue;
//				xgRect_t r = w->GetRect();
//
//				if ((ox+r.ext.x) > (m_Rect.ext.x - m_vMargins[1].x))
//				{
//					oy+=r.ext.y;
//					m_Rect.ext.y += r.ext.y;
//					ox = m_vMargins[0].x + m_pDragButton->GetRect().ext.x;
//				}
//
//				r.pos.x = ox;
//				r.pos.y = oy;
//
//				ox += (int)r.ext.x;		
//				
//				w->SetRect(r);				
//
//				if (r.Bottom() > m_Rect.ext.y + m_vMargins[1].y)
//					m_Rect.ext.y = r.Bottom() + m_vMargins[1].y;
//			}
//		}
//	break;
//	case TOrientation::orVertical:	
//		{
//			//m_Rect.ext.x = 0;
//			vec_t ox = m_vMargins[0].x;
//			vec_t oy = m_vMargins[0].y + m_pDragButton->GetRect().ext.y;
//			for(auto w : m_vChilds)
//			{
//				if (w.get() == m_pDragButton) 
//					continue;
//				xgRect_t r = w->GetRect();
//
//				r.pos.x = ox;
//				r.pos.y = oy;
//
//				oy += (int)r.ext.y;		
//
//				//if (oy > (m_Rect.ext.y - m_vMargins[1].y))
//				//	m_Rect.ext.y = oy + m_vMargins[1].y;
//
//				w->SetPos(ox,oy);				
//
//				if (r.Right() > m_Rect.ext.x + m_vMargins[1].x)
//					m_Rect.ext.x = r.Right() + m_vMargins[1].x;
//			}
//		}
//	break;
//	}
//}
//
///*
//*	Handler of widget docking to this widget
//**/
//void XGUI_Dock::OnWidgetDocked(TWidgetSharedPtr w)
//{
//	//m_vChilds.push_back(w);
//	//w->SetParent(this);
//
//	switch(m_Align)
//	{
//	case TAlign::alTop:
//		ME_Console::Printf(_T("Docked: alTop %f\n"),w->GetRect().ext.y);
//		m_Rect.ext.y += w->GetRect().ext.y;
//		RealignDockedItems(TItemAlignOrder::iaUpDownLeftToRight);
//		break;
//	case TAlign::alBottom:
//		ME_Console::Printf(_T("Docked: alBottom\n"));
//		m_Rect.pos.y -= w->GetRect().ext.y;
//		m_Rect.ext.y += w->GetRect().ext.y;
//		RealignDockedItems(TItemAlignOrder::iaUpDownLeftToRight);
//		break;
//	case TAlign::alLeft:
//		ME_Console::Printf(_T("Docked: alLeft\n"));
//		m_Rect.ext.x += w->GetRect().ext.x;
//		RealignDockedItems(TItemAlignOrder::iaLeftToRightUpToDown);
//		break;
//	case TAlign::alRight:
//		ME_Console::Printf(_T("Docked: alRight\n"));
//		m_Rect.ext.x += w->GetRect().ext.x;
//		m_Rect.pos.x -= w->GetRect().ext.x;
//		RealignDockedItems(TItemAlignOrder::iaLeftToRightUpToDown);
//		break;
//
//	}
//	
//	RecalcRectUsingContents();
//	SetRect(m_Rect);
//	m_pParent->SetRect(m_pParent->GetRect());
//
//}
//
//#define dockMinExtents 26
//
///*
//*	Handler of widget undocking to this widget
//**/
//void XGUI_Dock::OnWidgetUndocked(TWidgetSharedPtr w)
//{
////	auto it = std::remove(m_vChilds.begin(),m_vChilds.end(),TWidgetWeakPtr(w));
//	
//	//w->SetParent(m_pParent);
//
//	switch(m_Align)
//	{
//	case TAlign::alTop:
//		ME_Console::Printf(_T("Undocked: alTop %f\n"),w->GetRect().ext.y);
//		m_Rect.ext.y -= w->GetRect().ext.y;
//		if (m_Rect.ext.y < dockMinExtents) m_Rect.ext.y = dockMinExtents;
//		RealignDockedItems(TItemAlignOrder::iaUpDownLeftToRight);
//		break;
//	case TAlign::alBottom:
//		ME_Console::Printf(_T("Undocked: alBottom\n"));
//		m_Rect.pos.y += w->GetRect().ext.y;
//		m_Rect.ext.y -= w->GetRect().ext.y;
//
//		if (m_Rect.pos.y > (g_pPlatform->GetClientAreaExtents().y - dockMinExtents))
//		{
//			m_Rect.pos.y = g_pPlatform->GetClientAreaExtents().y - dockMinExtents;
//			m_Rect.ext.y = dockMinExtents;
//		}
//
//		RealignDockedItems(TItemAlignOrder::iaUpDownLeftToRight);
//		break;
//	case TAlign::alLeft:
//		ME_Console::Printf(_T("Undocked: alLeft\n"));
//		m_Rect.ext.x -= w->GetRect().ext.x;
//
//		if (m_Rect.ext.x < dockMinExtents) m_Rect.ext.x = dockMinExtents;		
//		
//		RealignDockedItems(TItemAlignOrder::iaLeftToRightUpToDown);
//		break;
//	case TAlign::alRight:
//		ME_Console::Printf(_T("Undocked: alRight\n"));
//		m_Rect.ext.x -= w->GetRect().ext.x;
//		m_Rect.pos.x += w->GetRect().ext.x;
//
//		if (m_Rect.pos.x > (g_pPlatform->GetClientAreaExtents().x - dockMinExtents))
//		{
//			m_Rect.pos.x = g_pPlatform->GetClientAreaExtents().x - dockMinExtents;
//			m_Rect.ext.x = dockMinExtents;
//		}
//
//		RealignDockedItems(TItemAlignOrder::iaLeftToRightUpToDown);
//		break;
//	}
//	
//	RecalcRectUsingContents();
//	SetRect(m_Rect);
//	m_pParent->SetRect(m_pParent->GetRect());
//}
//
///*
// *	Recalculates dock rect according items
// **/
//void XGUI_Dock::RecalcRectUsingContents()
//{
//	if (m_vChilds.empty()) return;
//
//	switch(m_Align)
//	{
//	case TAlign::alTop:
//		m_Rect.ext.y = 0;
//		for(auto w: m_vChilds)
//		{
//			m_Rect.ext.y += w->GetRect().ext.y;
//		}
//		break;
//	case TAlign::alLeft:
//		m_Rect.ext.x = 0;
//		for(auto w: m_vChilds)
//		{
//			m_Rect.ext.x += w->GetRect().ext.x;
//		}
//		break;
//	case TAlign::alRight:
//		{
//		vec_t ext = 0;
//
//		for(auto w: m_vChilds)
//		{
//			ext += w->GetRect().ext.x;
//		}
//
//		m_Rect.pos.x -= ext;
//		m_Rect.pos.x += m_Rect.ext.x;
//		m_Rect.ext.x = ext;
//		}
//		break;
//	case TAlign::alBottom:
//		vec_t ext = 0;
//		
//		for(auto w: m_vChilds)
//		{
//			ext += w->GetRect().ext.y;
//		}
//
//		m_Rect.pos.y -= ext;
//		m_Rect.pos.y += m_Rect.ext.y;
//		m_Rect.ext.y = ext;
//
//		break;
//	}
//}
//*/
//*/