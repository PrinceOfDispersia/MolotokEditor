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

XGUI_Widget::XGUI_Widget(xgRect_t & rect)
{
	m_vChilds.clear();
	m_vAlignOrderedChilds.clear();

	m_Rect.pos = rect.pos;
	m_Rect.ext = rect.ext;

	m_Color = g_pGUIManager->GuiSettings()->m_cDesktopBG;
	m_ZOrder = 0;
	m_Anchors = TAnchor::akNone;

	m_pParent = 0;

	for(int i = 0 ; i < ARRAY_SIZE(m_flTimers); i++)
	{
		m_flTimers[i] = 0;
	}

	m_Align = TAlign::alNone;
	SetRect(m_Rect);

	m_bVisible = true;
	m_bEnabled = true;

	m_strCaption = _T("");
	m_bDragged = false;

	m_vParentStart = ME_Math::Vector2D(0,0);

	m_nWidgetCounter = 0;
	m_nWidgetNumber = 0;

	

}

/*
*	Function recalculates widget rect corresponding to widget alignment
**/
void XGUI_Widget::RecalcRectWithAligment()
{
	switch(m_Align)
	{
	case TAlign::alClientArea:
		{
			// alClientArea has lowest priority
			xgRect_t pr = GetParentRect();
			if (!m_pParent)
				m_Rect = pr;
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

					wx1 = w->m_Rect.pos.x; wx2 = w->m_Rect.pos.x + w->m_Rect.ext.x;
					wy1 = w->m_Rect.pos.y; wy2 = w->m_Rect.pos.y + w->m_Rect.ext.y;

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

				m_Rect.pos.x = x1;
				m_Rect.pos.y = y1;
				m_Rect.ext.x = x2 - x1;
				m_Rect.ext.y = y2 - y1;

			}

		}		
		break;
	case TAlign::alTop:
		{
			xgRect_t pr = GetParentRect();

			if (!m_pParent)
			{
				m_Rect.pos = ME_Math::Vector2D(0,0);
				m_Rect.ext.x = pr.ext.x;
			}
			else
			{
				vec_t x1,x2,y1,y2;

				x1 = pr.pos.x; x2 = pr.pos.x + pr.ext.x;
				y1 = 0; y2 = y1 + m_Rect.ext.y;

				// alTop pushes widgets at bottom side
				for(XGUI_Widget * w : m_pParent->m_vAlignOrderedChilds)
				{
					if (w->m_Align == TAlign::alNone || w->m_Align == TAlign::alClientArea) continue;
					if (w == this) continue;
					if (w->m_AlignPriority < m_AlignPriority) continue;

					vec_t wx1,wx2,wy1,wy2;

					wx1 = w->m_Rect.pos.x; wx2 = w->m_Rect.pos.x + w->m_Rect.ext.x;
					wy1 = w->m_Rect.pos.y; wy2 = w->m_Rect.pos.y + w->m_Rect.ext.y;

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
							y1+=w->m_Rect.ext.y;
							y2+=w->m_Rect.ext.y;
						}
						break;
					}
				}

				m_Rect.pos.x = x1;
				m_Rect.pos.y = y1;
				m_Rect.ext.x = x2 - x1;
				m_Rect.ext.y = y2 - y1;
			}



		}
		break;
	case TAlign::alBottom:
		{
			xgRect_t pr = GetParentRect();
			if (!m_pParent)
			{
				m_Rect.pos = ME_Math::Vector2D(0,pr.ext.y - m_Rect.ext.y);
				m_Rect.ext.x = pr.ext.x;			
			}
			else
			{
				vec_t x1,x2,y1,y2;

				x1 = pr.pos.x; x2 = pr.pos.x + pr.ext.x;
				y1 = pr.ext.y - m_Rect.ext.y; y2 = y1 + m_Rect.ext.y;

				for(XGUI_Widget * w : m_pParent->m_vAlignOrderedChilds)
				{
					if (w->m_Align == TAlign::alNone || w->m_Align == TAlign::alClientArea) continue;
					if (w == this) continue;
					if (w->m_AlignPriority < m_AlignPriority) continue;

					vec_t wx1,wx2,wy1,wy2;

					wx1 = w->m_Rect.pos.x; wx2 = w->m_Rect.pos.x + w->m_Rect.ext.x;
					wy1 = w->m_Rect.pos.y; wy2 = w->m_Rect.pos.y + w->m_Rect.ext.y;

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
							y1-=w->m_Rect.ext.y;
							y2-=w->m_Rect.ext.y;
						}
						break;
					}
				}

				m_Rect.pos.x = x1;
				m_Rect.pos.y = y1;
				m_Rect.ext.x = x2 - x1;
				m_Rect.ext.y = y2 - y1;
			}

		}
		break;
	case TAlign::alLeft:
		{
			xgRect_t pr = GetParentRect();
			if (!m_pParent)
			{
				m_Rect.pos = ME_Math::Vector2D(0,0);
				m_Rect.ext.y = pr.ext.y;			
			}
			else
			{
				vec_t x1,x2,y1,y2;

				x1 = 0; x2 = m_Rect.ext.x;
				y1 = 0; y2 = y1 + pr.ext.y;

				// alTop pushes widgets at bottom side
				for(XGUI_Widget * w : m_pParent->m_vAlignOrderedChilds)
				{
					if (w->m_Align == TAlign::alNone || w->m_Align == TAlign::alClientArea) continue;
					if (w == this) continue;
					if (w->m_AlignPriority < m_AlignPriority) continue;

					vec_t wx1,wx2,wy1,wy2;

					wx1 = w->m_Rect.pos.x; wx2 = w->m_Rect.pos.x + w->m_Rect.ext.x;
					wy1 = w->m_Rect.pos.y; wy2 = w->m_Rect.pos.y + w->m_Rect.ext.y;

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
							x1+=w->m_Rect.ext.x;
							x2+=w->m_Rect.ext.x;
						}
					}
				}

				m_Rect.pos.x = x1;
				m_Rect.pos.y = y1;
				m_Rect.ext.x = x2 - x1;
				m_Rect.ext.y = y2 - y1;
			}

		}		
		break;
	case TAlign::alRight:
		{
			xgRect_t pr = GetParentRect();
			if (!m_pParent)
			{
				m_Rect.pos = ME_Math::Vector2D(pr.ext.x - m_Rect.ext.x,0);
				m_Rect.ext.y = pr.ext.y;		
			}
			else
			{
				vec_t x1,x2,y1,y2;

				x1 = pr.ext.x - m_Rect.ext.x; x2 = x1 + m_Rect.ext.x;
				y1 = 0; y2 = y1 + pr.ext.y;

				// alTop pushes widgets at bottom side
				for(XGUI_Widget * w : m_pParent->m_vAlignOrderedChilds)
				{
					if (w->m_Align == TAlign::alNone || w->m_Align == TAlign::alClientArea) continue;
					if (w == this) continue;
					if (w->m_AlignPriority < m_AlignPriority) continue;


					vec_t wx1,wx2,wy1,wy2;

					wx1 = w->m_Rect.pos.x; wx2 = w->m_Rect.pos.x + w->m_Rect.ext.x;
					wy1 = w->m_Rect.pos.y; wy2 = w->m_Rect.pos.y + w->m_Rect.ext.y;

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
							x1-=w->m_Rect.ext.x;
							x2-=w->m_Rect.ext.x;
						}
					}
				}

				m_Rect.pos.x = x1;
				m_Rect.pos.y = y1;
				m_Rect.ext.x = x2 - x1;
				m_Rect.ext.y = y2 - y1;
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
	m_Rect = rect;

	RecalcItemsRects();
}

/*
*	Recalculates items rects according to aligment
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
void XGUI_Widget::RecursiveNotifyEveryone(ME_Framework::appEvent_t & ev)
{
	for(TWidgetSharedPtr w : m_vChilds)
	{
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
		for(TWidgetSharedPtr c : m_vChilds)
		{
			RecursiveNotifyEveryone(ev);			
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
		child->m_vParentStart = m_vParentStart + m_Rect.pos;
		child->Render();
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

	glVertex2d(m_Rect.pos.x,m_Rect.pos.y);
	glVertex2d(m_Rect.pos.x + m_Rect.ext.x,m_Rect.pos.y);
	glVertex2d(m_Rect.pos.x + m_Rect.ext.x,m_Rect.pos.y + m_Rect.ext.y);
	glVertex2d(m_Rect.pos.x,m_Rect.pos.y + m_Rect.ext.y);

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

	// Update rect with aligment
	pWidget->SetRect(pWidget->m_Rect);	
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

	xgRect_t r = m_Rect;
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
		r.pos += m_pParent->m_Rect.pos;
		m_pParent->CalcClientRect(r);
	}
}

/*
*	Calculates point position relative to control rect
**/
void XGUI_Widget::PointToClient(ME_Math::Vector2D & v)
{
	XGUI_Widget * pParent = m_pParent;

	v-=m_Rect.pos;

	// Traverse up hierarchy
	//
	while(pParent && pParent->m_pParent)
	{
		pParent = pParent->m_pParent;
		v-=pParent->m_Rect.pos;
	}

}

/*
*	Calculates relative to screen point position on screen
**/
void XGUI_Widget::ClientToScreen(ME_Math::Vector2D & v)
{
	XGUI_Widget * pParent = m_pParent;

	v+=m_Rect.pos;

	// Traverse up hierarchy
	//
	while(pParent && pParent->m_pParent)
	{
		pParent = pParent->m_pParent;
		v+=pParent->m_Rect.pos;
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

		PointToClient(v2);			
		m_Rect.pos += v2;		

		// Update aligment
		SetRect(m_Rect);
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
		return m_pParent->m_Rect;
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
	return m_Rect;
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
