/*
*	Molotok Editor, 2013 (C) PrinceOfDispersia
*	XGUI_Widget.cpp - basic gui widget class
*
**/

#include <Platform/Common/ApplicationCommon.h>

using namespace ME_Framework::ME_XGUI;
using namespace ME_Framework::ME_OpenGLBackend;

extern XGUI_Manager * g_pGUIManager;
extern XGUI_Tesselator * g_pTesselator;
extern XGUI_Menu * g_pTestMenu;

XGUI_Widget::XGUI_Widget(xgRect_t & rect)
{
	m_vChilds.clear();
	m_vAlignOrderedChilds.clear();

	m_WorkRect.pos = rect.pos;
	m_WorkRect.ext = rect.ext;

	m_Color = g_pGUIManager->GuiSettings()->m_cDesktopBG;
	m_ZOrder = 1;
	m_Anchors = TAnchor::akNone;

	m_pParent = 0;

	for(int i = 0 ; i < ARRAY_SIZE(m_flTimers); i++)
	{
		m_flTimers[i] = 0;
	}

	m_Align = TAlign::alNone;
	SetRect(m_WorkRect);

	m_bVisible = true;
	m_bEnabled = true;

	m_strCaption = _T("");
	m_bDragged = false;

	m_vParentStart = ME_Math::Vector2D(0,0);

	m_nWidgetCounter = 0;
	m_nWidgetNumber = 0;

	m_CurrentCursor = eMouseCursors::mcNormal;

	m_pGuiFontNormal = g_pGUIManager->Get_GuiFont(TGuiFontTypes::gfNormal);
	m_pGuiFontSmall = g_pGUIManager->Get_GuiFont(TGuiFontTypes::gfSmall);

	m_bFocused = false;

	m_pParent = 0;
		
}

XGUI_Widget::XGUI_Widget()
{
	m_vChilds.clear();
	m_vAlignOrderedChilds.clear();

	m_WorkRect.pos.Init();
	m_WorkRect.ext.Init();

	m_Color = g_pGUIManager->GuiSettings()->m_cDesktopBG;
	m_ZOrder = 1;
	m_Anchors = TAnchor::akNone;

	m_pParent = 0;

	for(int i = 0 ; i < ARRAY_SIZE(m_flTimers); i++)
	{
		m_flTimers[i] = 0;
	}

	m_Align = TAlign::alNone;
	SetRect(m_WorkRect);

	m_bVisible = true;
	m_bEnabled = true;

	m_strCaption = _T("");
	m_bDragged = false;

	m_vParentStart = ME_Math::Vector2D(0,0);

	m_nWidgetCounter = 0;
	m_nWidgetNumber = 0;

	m_CurrentCursor = eMouseCursors::mcNormal;

	m_pGuiFontNormal = g_pGUIManager->Get_GuiFont(TGuiFontTypes::gfNormal);
	m_pGuiFontSmall = g_pGUIManager->Get_GuiFont(TGuiFontTypes::gfSmall);

	m_bFocused = false;
	m_pContextMenu = g_pTestMenu;


	
}

/*
*	Function recalculates widget rect corresponding to widget alignment
**/
void XGUI_Widget::RecalcRectWithAligment()
{
	xgRect_t pr = GetParentRect();

	if (m_pParent)
	{
		/*if (flags(m_Anchors & TAnchor::akTop))
		{
			vec_t newY = m_AnchorsCoefs[ackTop] * pr.ext.y;
			m_Rect.pos.y = newY;
		}

		if (flags(m_Anchors & TAnchor::akLeft))
		{
			vec_t newX = m_AnchorsCoefs[ackLeft] * pr.ext.x;
			m_Rect.pos.x = newX;
		}*/

		if (flags(m_Anchors & TAnchor::akRight))
		{
			vec_t newRight = pr.ext.x - m_AnchorsCoefs[ackRight];
			vec_t newX = newRight - m_WorkRect.ext.x;
			
			if (!flags(m_Anchors & TAnchor::akLeft))
			{
				m_WorkRect.pos.x = newX;
			}
			else 
			{
				m_WorkRect.ext.x = newRight - m_WorkRect.pos.x;
			}

			if (m_WorkRect.pos.x < m_pParent->m_vMargins[0].x) 
				m_WorkRect.pos.x = m_pParent->m_vMargins[0].x;

			// Move left if overlaps with edge of parent

			/*if (m_Rect.ext.x < 0)
			{				
				m_Rect.pos.x -= m_Rect.ext.x * -2;				
				m_Rect.ext.x = -m_Rect.ext.x;				
			}*/
		}

		if (flags(m_Anchors & TAnchor::akBottom))
		{
			vec_t newBottom = pr.ext.y - m_AnchorsCoefs[ackBottom];
			vec_t newY = newBottom - m_WorkRect.ext.y;

			if (!flags(m_Anchors & TAnchor::akTop))
			{
				m_WorkRect.pos.y = newY;
			}
			else 
			{
				m_WorkRect.ext.y = newBottom - m_WorkRect.pos.y;
			}

			if (m_WorkRect.pos.y < m_pParent->m_vMargins[0].y) 
					m_WorkRect.pos.y = m_pParent->m_vMargins[0].y;

			/*if (m_Rect.ext.y < 0)
			{				
				m_Rect.pos.y -= m_Rect.ext.y * -2;				
				m_Rect.ext.y = -m_Rect.ext.y;				
			}*/
		}
	}


	switch(m_Align)
	{
	case TAlign::alClientArea:
		{
			// alClientArea has lowest priority
			
			if (!m_pParent)
				m_WorkRect = pr;
			else
			{
				vec_t x1,x2,y1,y2;

				x1 = pr.pos.x; x2 = pr.pos.x + pr.ext.x;
				y1 = pr.pos.y; y2 = pr.pos.y + pr.ext.y;

				for(XGUI_Widget * w : m_pParent->m_vAlignOrderedChilds)
				{
					if (w == this) continue;
					if (w->m_Align == TAlign::alNone) continue;

					vec_t wx1,wx2,wy1,wy2;

					wx1 = w->m_WorkRect.pos.x; wx2 = w->m_WorkRect.pos.x + w->m_WorkRect.ext.x;
					wy1 = w->m_WorkRect.pos.y; wy2 = w->m_WorkRect.pos.y + w->m_WorkRect.ext.y;

					switch(w->m_Align)
					{
					case TAlign::alTop:
						if (wy2 > y1)	y1 = wy2;						
						break;
					case TAlign::alBottom:
						if (wy1 < y2)	y2 = wy1;													
						break;
					case TAlign::alLeft:
						if (wx2 > x1)	x1 = wx2;							
						break;
					case TAlign::alRight:
						if (wx1 < x2)	x2 = wx1;							
						break;
					}
				}

				m_WorkRect.pos.x = x1;
				m_WorkRect.pos.y = y1;
				m_WorkRect.ext.x = x2 - x1;
				m_WorkRect.ext.y = y2 - y1;

			}

		}		
		break;
	case TAlign::alTop:
		{
			

			if (!m_pParent)
			{
				m_WorkRect.pos = ME_Math::Vector2D(0,0);
				m_WorkRect.ext.x = pr.ext.x;
			}
			else
			{
				vec_t x1,x2,y1,y2;

				x1 = (m_pParent)->m_vMargins[0][0]; x2 = pr.ext.x - (m_pParent)->m_vMargins[1][0];
				y1 = (m_pParent)->m_vMargins[0][1]; y2 = y1 + m_UnAlignedRect.ext.y;

				if (y2 > (pr.ext.y - (m_pParent)->m_vMargins[1][1])) 
				{
					y2 = (pr.ext.y - (m_pParent)->m_vMargins[1][1]);
				}

				// alTop pushes widgets at bottom side
				for(XGUI_Widget * w : m_pParent->m_vAlignOrderedChilds)
				{
					if (w->m_Align == TAlign::alNone || w->m_Align == TAlign::alClientArea) continue;
					if (w == this) continue;
					if (w->m_AlignPriority < m_AlignPriority) continue;

					vec_t wx1,wx2,wy1,wy2;

					wx1 = w->m_WorkRect.pos.x; wx2 = w->m_WorkRect.pos.x + w->m_WorkRect.ext.x;
					wy1 = w->m_WorkRect.pos.y; wy2 = w->m_WorkRect.pos.y + w->m_WorkRect.ext.y;

					switch (w->m_Align)
					{
					case TAlign::alLeft:
						if (x1 < wx2) x1 = wx2;
						break;
					case TAlign::alRight:
						if (x2 > wx1) x2 = wx1;
						break;
					case TAlign::alTop:
						if (w->m_nWidgetNumber < m_nWidgetNumber)
						{
							y1+=w->m_WorkRect.ext.y;
							y2+=w->m_WorkRect.ext.y;
						}
						break;
					}
				}

				m_WorkRect.pos.x = x1;
				m_WorkRect.pos.y = y1;
				m_WorkRect.ext.x = x2 - x1;
				m_WorkRect.ext.y = y2 - y1;
			}



		}
		break;
	case TAlign::alBottom:
		{
			
			if (!m_pParent)
			{
				m_WorkRect.pos = ME_Math::Vector2D(0,pr.ext.y - m_WorkRect.ext.y);
				m_WorkRect.ext.x = pr.ext.x;			
			}
			else
			{
				vec_t x1,x2,y1,y2;

				x1 = pr.pos.x; x2 = pr.pos.x + pr.ext.x;
				y1 = pr.ext.y - m_WorkRect.ext.y; y2 = y1 + m_WorkRect.ext.y;

				for(XGUI_Widget * w : m_pParent->m_vAlignOrderedChilds)
				{
					if (w->m_Align == TAlign::alNone || w->m_Align == TAlign::alClientArea) continue;
					if (w == this) continue;
					if (w->m_AlignPriority < m_AlignPriority) continue;

					vec_t wx1,wx2,wy1,wy2;

					wx1 = w->m_WorkRect.pos.x; wx2 = w->m_WorkRect.pos.x + w->m_WorkRect.ext.x;
					wy1 = w->m_WorkRect.pos.y; wy2 = w->m_WorkRect.pos.y + w->m_WorkRect.ext.y;

					switch (w->m_Align)
					{
					case TAlign::alLeft:
						if (x1 < wx2) x1 = wx2;
						break;
					case TAlign::alRight:
						if (x2 > wx1) x2 = wx1;
						break;
					case TAlign::alBottom:
						if (w->m_nWidgetNumber < m_nWidgetNumber)
						{
							y1-=w->m_WorkRect.ext.y;
							y2-=w->m_WorkRect.ext.y;
						}
						break;
					}
				}

				m_WorkRect.pos.x = x1;
				m_WorkRect.pos.y = y1;
				m_WorkRect.ext.x = x2 - x1;
				m_WorkRect.ext.y = y2 - y1;
			}

		}
		break;
	case TAlign::alLeft:
		{
			
			if (!m_pParent)
			{
				m_WorkRect.pos = ME_Math::Vector2D(0,0);
				m_WorkRect.ext.y = pr.ext.y;			
			}
			else
			{
				vec_t x1,x2,y1,y2;

				x1 = 0; x2 = m_WorkRect.ext.x;
				y1 = 0; y2 = y1 + pr.ext.y;

				// alTop pushes widgets at bottom side
				for(XGUI_Widget * w : m_pParent->m_vAlignOrderedChilds)
				{
					if (w->m_Align == TAlign::alNone || w->m_Align == TAlign::alClientArea) continue;
					if (w == this) continue;
					if (w->m_AlignPriority < m_AlignPriority) continue;

					vec_t wx1,wx2,wy1,wy2;

					wx1 = w->m_WorkRect.pos.x; wx2 = w->m_WorkRect.pos.x + w->m_WorkRect.ext.x;
					wy1 = w->m_WorkRect.pos.y; wy2 = w->m_WorkRect.pos.y + w->m_WorkRect.ext.y;

					switch (w->m_Align)
					{
					case TAlign::alTop:
						if (y1 < wy2) y1 = wy2;
						break;
					case TAlign::alBottom:
						if (y2 > wy1) y2 = wy1;
						break;
					case TAlign::alLeft:
						if (w->m_nWidgetNumber < m_nWidgetNumber)
						{
							x1+=w->m_WorkRect.ext.x;
							x2+=w->m_WorkRect.ext.x;
						}
					}
				}

				m_WorkRect.pos.x = x1;
				m_WorkRect.pos.y = y1;
				m_WorkRect.ext.x = x2 - x1;
				m_WorkRect.ext.y = y2 - y1;
			}

		}		
		break;
	case TAlign::alRight:
		{
			
			if (!m_pParent)
			{
				m_WorkRect.pos = ME_Math::Vector2D(pr.ext.x - m_WorkRect.ext.x,0);
				m_WorkRect.ext.y = pr.ext.y;		
			}
			else
			{
				vec_t x1,x2,y1,y2;

				x1 = pr.ext.x - m_WorkRect.ext.x; x2 = x1 + m_WorkRect.ext.x;
				y1 = 0; y2 = y1 + pr.ext.y;

				// alTop pushes widgets at bottom side
				for(XGUI_Widget * w : m_pParent->m_vAlignOrderedChilds)
				{
					if (w->m_Align == TAlign::alNone || w->m_Align == TAlign::alClientArea) continue;
					if (w == this) continue;
					if (w->m_AlignPriority < m_AlignPriority) continue;


					vec_t wx1,wx2,wy1,wy2;

					wx1 = w->m_WorkRect.pos.x; wx2 = w->m_WorkRect.pos.x + w->m_WorkRect.ext.x;
					wy1 = w->m_WorkRect.pos.y; wy2 = w->m_WorkRect.pos.y + w->m_WorkRect.ext.y;

					switch (w->m_Align)
					{
					case TAlign::alTop:
						if (y1 < wy2) y1 = wy2;
						break;
					case TAlign::alBottom:
						if (y2 > wy1) y2 = wy1;
						break;
					case TAlign::alRight:
						if (w->m_nWidgetNumber < m_nWidgetNumber)
						{
							x1-=w->m_WorkRect.ext.x;
							x2-=w->m_WorkRect.ext.x;
						}
					}
				}

				m_WorkRect.pos.x = x1;
				m_WorkRect.pos.y = y1;
				m_WorkRect.ext.x = x2 - x1;
				m_WorkRect.ext.y = y2 - y1;
			}

		}
		break;
	}

	ME_Framework::appEvent_t e;
	e.eventid = eventTypes::EV_ALIGN_PERFORMED;
	HandleEvent(e);
}

/*
*	Resize widget accounting to aligment
**/
void XGUI_Widget::SetRect(xgRect_t & rect)
{
	m_WorkRect = rect;
	m_UnAlignedRect = rect;

	RecalcItemsRects();
}

/*
*	Recalculates items rects according to aligment and anchors
**/
void XGUI_Widget::RecalcItemsRects()
{
	RecalcRectWithAligment();

	for(XGUI_Widget * w : m_vAlignOrderedChilds)
	{
		w->RecalcItemsRects();
	}
}

/*
*	Notifies every component in gui hierarchy with event
**/
void XGUI_Widget::RecursiveNotifyEveryone(ME_Framework::appEvent_t & ev,XGUI_Widget * filter)
{
	for(TWidgetSharedPtr w : m_vChilds)
	{
		if (w.get() == filter) continue;

		if (w->m_bVisible == false)  
			continue;
		w->HandleEvent(ev);
		w->RecursiveNotifyEveryone(ev);
	}
}

/*
*	Handles system event
**/
void XGUI_Widget::HandleEvent(ME_Framework::appEvent_t & ev)
{
	switch(ev.eventid)
	{
	case eventTypes::EV_MOUSE_KEY_UP:
		for(TWidgetSharedPtr c : g_pGUIManager->m_pDesktop->GetChilds())
		{
			auto e = ev; 
			e.eventid = eventTypes::EV_NOTIFY_MOUSE_UP;
			c->RecursiveNotifyEveryone(e);			
		}
		break;
	case eventTypes::EV_MOUSE_KEY_DOWN:
		{
			auto e = ev;
			e.eventid = eventTypes::EV_NOTIFY_COMPONENT_CLICKED;
			for(TWidgetSharedPtr c : g_pGUIManager->m_pDesktop->GetChilds())
			{
				c->RecursiveNotifyEveryone(e,this);			
			}
		}
		break;
	}
}

/*
*	Render widget and childrens
**/
void XGUI_Widget::Render()
{
	g_pTesselator->SetTranslation(m_vParentStart);
	DrawComponent();

	for(TWidgetSharedPtr child: m_vChilds)
	{
		if (child->m_bVisible == false) continue;
		child->m_vParentStart = m_vParentStart + m_WorkRect.pos;

		xgRect_t r = m_WorkRect;
		CalcClientRect(r);

		GL_SetScissor(r.Left(),r.Top(),r.ext.x,r.ext.y);
		GL_EnableScissorTest();

		child->Render();

		g_pTesselator->Flush();
	}

	
}

/*
*	Draws widget
*
**/
void XGUI_Widget::DrawComponent()
{
	GL_DisableState(GLS_TEXTURE_2D);
	GL_DisableState(GLS_BLEND);

	glColor4ubv((GLubyte*)&m_Color);
	glBegin(GL_QUADS);

	glVertex2d(m_WorkRect.pos.x,m_WorkRect.pos.y);
	glVertex2d(m_WorkRect.pos.x + m_WorkRect.ext.x,m_WorkRect.pos.y);
	glVertex2d(m_WorkRect.pos.x + m_WorkRect.ext.x,m_WorkRect.pos.y + m_WorkRect.ext.y);
	glVertex2d(m_WorkRect.pos.x,m_WorkRect.pos.y + m_WorkRect.ext.y);

	glEnd();

	GL_EnableState(GLS_TEXTURE_2D);
	GL_EnableState(GLS_BLEND);
}

/*
*	Updates timers of widget
**/
void XGUI_Widget::UpdateTimers(float flDelta)
{
	for(int i = 0 ; i < ARRAY_SIZE(m_flTimers); i++)
	{
		m_flTimers[i] += flDelta;
	}
}

/*
*	Destructor 
**/
XGUI_Widget::~XGUI_Widget()
{
	for(TWidgetSharedPtr w : m_vChilds)
	{
		w.reset();
	}

	m_vChilds.clear();
	m_vChilds.shrink_to_fit();

	m_vAlignOrderedChilds.clear();
	m_vAlignOrderedChilds.shrink_to_fit();
}

/*
*	Property accessors
**/

// Setters
void XGUI_Widget::SetParent(XGUI_Widget * pParent)
{
	if (m_pParent != nullptr)
	{
		auto it = std::find(m_pParent->m_vChilds.begin(),m_pParent->m_vChilds.end(),shared_from_this());

		if (*it)
		{
			g_pGUIManager->Safe_QueryWidgetRemove(*it,m_pParent->m_vChilds);
		}
	}

	m_pParent = pParent;
}

void XGUI_Widget::SetAlign(TAlign align)
{
	m_Align = align;
}

void XGUI_Widget::SetAnchors(TAnchor anchors)
{
	m_Anchors = anchors;
}

void XGUI_Widget::SetVisibilty(bool bVisible)
{
	m_bVisible = bVisible;
	OnVisibilityChange(bVisible);
}

void XGUI_Widget::SetEnableState(bool bEnabled)
{
	m_bEnabled = bEnabled;
}

void XGUI_Widget::SetCaption(String & str)
{
	m_strCaption = str;
}

// Getters
XGUI_Widget * XGUI_Widget::GetParent() const
{
	return m_pParent;
}

TAlign XGUI_Widget::GetAlign() const
{
	return m_Align;
}

TAnchor XGUI_Widget::GetAnchors() const
{
	return m_Anchors;
}

bool XGUI_Widget::GetVisibilty() const
{
	return m_bVisible;
}

bool XGUI_Widget::GetEnableState() const
{
	return m_bEnabled;
}

String & XGUI_Widget::GetCaption()
{
	return m_strCaption;
}

/*
*	Property accessors end
**/

/*
*	Adds child widget
**/
void XGUI_Widget::AddChildWidget(XGUI_Widget * pWidget)
{
	m_vAlignOrderedChilds.push_back(pWidget);
	m_vChilds.push_back(TWidgetSharedPtr(pWidget));
	pWidget->m_pParent = this;

	pWidget->OnAddToParent(this);

	// Update rect with aligment
	pWidget->SetRect(pWidget->m_WorkRect);	
	pWidget->m_nWidgetNumber = m_nWidgetCounter++;	


	SortChildsByAlignOrder();
	
}

/*
*	Sorts children by ZOrder
**/
void XGUI_Widget::SortChilds()
{
	std::sort(m_vChilds.begin(),m_vChilds.end(),[] (TWidgetSharedPtr i,TWidgetSharedPtr j){return i->m_ZOrder < j->m_ZOrder;});
}

/*
*	Sorts children by align order
**/
void XGUI_Widget::SortChildsByAlignOrder()
{
	std::sort(m_vAlignOrderedChilds.begin(),m_vAlignOrderedChilds.end(),[] (XGUI_Widget* i,XGUI_Widget* j){return i->m_AlignPriority > j->m_AlignPriority;});
}

/*
*	Marks child items under cursor or self if no childs marked
**/
XGUI_Widget* XGUI_Widget::WidgetUnderCursor(ME_Math::Vector2D pt)
{

	xgRect_t r = m_WorkRect;
	CalcClientRect(r);

	if (!(pt.x >= r.pos.x && pt.x <= r.pos.x + r.ext.x)
		|| !(pt.y >= r.pos.y && pt.y <= r.pos.y + r.ext.y))
	{
		return 0;
	}

	for(auto it = m_vChilds.rbegin() ; it < m_vChilds.rend() ; it++)
	{
		XGUI_Widget * c = (*it).get();

		if (c->m_bVisible == false) 
			continue;

		XGUI_Widget * w = c->WidgetUnderCursor(pt);
		if (w)
			return w;
	}


	if (pt.x >= r.pos.x && pt.x <= r.pos.x + r.ext.x)
		if (pt.y >= r.pos.y && pt.y <= r.pos.y + r.ext.y)
		{
			return this;
		}

		return 0;
}

/*
* Calculates client area rect
**/
void XGUI_Widget::CalcClientRect(xgRect_t & r)
{
	if (m_pParent)
	{
		r.pos += m_pParent->m_WorkRect.pos;
		m_pParent->CalcClientRect(r);
	}
}

/*
*	Calculates point position relative to control rect
**/
void XGUI_Widget::ScreenToClient(ME_Math::Vector2D & v)
{
	XGUI_Widget * pParent = m_pParent;

	v-=m_WorkRect.pos;

	// Traverse up hierarchy
	//
	while(pParent && pParent->m_pParent)
	{
		pParent = pParent->m_pParent;
		v-=pParent->m_WorkRect.pos;
	}

}

/*
*	Calculates relative to screen point position on screen
**/
void XGUI_Widget::ClientToScreen(ME_Math::Vector2D & v)
{
	XGUI_Widget * pParent = m_pParent;

	v+=m_WorkRect.pos;

	// Traverse up hierarchy
	//
	while(pParent && pParent->m_pParent)
	{
		pParent = pParent->m_pParent;
		v+=pParent->m_WorkRect.pos;
	}

}

/*
*	Calls think procedure of self and childs
**/
void XGUI_Widget::DoThink()
{
	RecalcDrag();
	Think();

	for(TWidgetSharedPtr c: m_vChilds)
	{
		c->DoThink();
	}	
}

/*
*	Performs widget dragging if necceseary
**/
void XGUI_Widget::RecalcDrag()
{
	if (m_bDragged)
	{
		ME_Math::Vector2D v = g_pPlatform->GetCursorPos();
		ME_Math::Vector2D v2 = v - m_vDragOrigin;

		ScreenToClient(v2);			
		m_WorkRect.pos += v2;		

		// Update aligment
		SetRect(m_WorkRect);
	}
}

/*
*	Widget think function
**/
void XGUI_Widget::Think()
{

}

/*
*	Returns rect of widget's parent, or client area rect if widget has no parent
**/
xgRect_t XGUI_Widget::GetParentRect()
{
	if (m_pParent)
	{
		return m_pParent->m_WorkRect;
	}
	else
	{
		xgRect_t r;
		r.pos = ME_Math::Vector2D(0,0);
		r.ext = g_pPlatform->GetClientAreaExtents();

		return r;
	}
}

/*
*	Sets zorder property of widget, bigger value = closer to viewer
**/
void XGUI_Widget::SetZOrder(int order)
{
	m_ZOrder = order;
}

/*
*	Sets align priority
**/
void XGUI_Widget::SetAlignPriority(int alignPriority)
{
	m_AlignPriority = alignPriority;
}

/*
*	returns ZOrder of widget
**/
int XGUI_Widget::GetZOrder() const
{
	return m_ZOrder;
}

/*
*	returns align priority of widget
**/
int XGUI_Widget::GetAlignPriority() const
{
	return m_AlignPriority;
}

/*
* returns widget rect
**/
xgRect_t & XGUI_Widget::GetRect()
{
	return m_WorkRect;
}

/*
*	returns array of widget children
**/
const TWidgetVector & XGUI_Widget::GetChilds()
{
	return m_vChilds;
}


/*
*	returns drag state
**/
bool XGUI_Widget::IsDragged() const 
{
	return m_bDragged; 
}

/*
*	Toggles element dragged and blocks cursor from getting out of app window
**/
void XGUI_Widget::SetDragged(bool val) 
{ 
	if (val)
	{
		g_pGUIManager->LockCursorInDesktop();
	}
	else g_pGUIManager->UnlockCursor();

	m_bDragged = val; 
}

void XGUI_Widget::GrabFocus()
{
	g_pGUIManager->SetFocusedWidget(this);
}

bool XGUI_Widget::IsFocused()
{
	return m_bFocused;
}


void XGUI_Widget::OnAddToParent(XGUI_Widget * pParent)
{
	ME_Math::Vector2D  p_ext = pParent->m_WorkRect.ext;

	if (flags(m_Anchors & TAnchor::akTop))
	{
		m_AnchorsCoefs[ackTop] =  m_WorkRect.Top();
	}
	if (flags(m_Anchors & TAnchor::akLeft))
	{
		m_AnchorsCoefs[ackLeft] =  m_WorkRect.Left();
	}
	if (flags(m_Anchors & TAnchor::akRight))
	{
		m_AnchorsCoefs[ackRight] =  pParent->m_WorkRect.ext.x - m_WorkRect.Right();
	}
	if (flags(m_Anchors & TAnchor::akBottom))
	{
		m_AnchorsCoefs[ackBottom] =  pParent->m_WorkRect.ext.y - m_WorkRect.Bottom();
	}	
}

void XGUI_Widget::SetMarginsFromSkinset( mSheetGlyph_t * spr[9] )
{
	m_vMargins[0].x = spr[0]->e[0];
	m_vMargins[0].y = spr[1]->e[1];

	m_vMargins[1].x = spr[8]->e[0];
	m_vMargins[1].y = spr[8]->e[1];
}

bool XGUI_Widget::IsBelongsHeirarchy( XGUI_Widget * w )
{
	if (m_pParent == w)
		return true;

	if (!m_pParent)
		return false;

	return m_pParent->IsBelongsHeirarchy(w);
}


void XGUI_Widget::DoHandleEvent( ME_Framework::appEvent_t & pEvent )
{
	XGUI_Widget::HandleEvent(pEvent);
	HandleEvent(pEvent);
}
