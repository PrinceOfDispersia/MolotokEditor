/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	XGUI_Widget.cpp - basic gui widget class
 *
 **/

#include <Platform/Common/ApplicationCommon.h>

using namespace ME_Framework::ME_XGUI;
using namespace ME_Framework::ME_OpenGLBackend;

XGUI_Widget::XGUI_Widget(xgRect_t * rect)
{
	m_Rect.pos = rect->pos;
	m_Rect.ext = rect->ext;

	m_Color.r = 45;
	m_Color.g = 45;
	m_Color.b = 48;
	m_Color.a = 255;

	m_ZOrder = 0;

	
	m_Anchors = TAnchor::akNone;

	m_pParent = 0;

	for(int i = 0 ; i < ARRAY_SIZE(m_flTimers); i++)
	{
		m_flTimers[i] = 0;
	}

	m_Align = TAlign::alClientArea;
	SetSize(&m_Rect);
}

/*
 *	Resize widget accounting to aligment
 **/
void XGUI_Widget::SetSize(xgRect_t * rect)
{
	switch(m_Align)
	{
	case TAlign::alClientArea:
		m_Rect.pos = ME_Math::Vector2D(0,0);
		if (!m_pParent)
			m_Rect.ext = ME_Math::Vector2D(g_pPlatform->GetClientAreaExtents());
		else
			m_Rect.ext = m_pParent->m_Rect.ext;
		break;
	}
}


/*
 *	Handles system event
 **/
void XGUI_Widget::HandleEvent(ME_Framework::appEvent_t * pEvent)
{

}

/*
 *	Render widget and childrens
 **/
void XGUI_Widget::Render()
{
	DrawComponent();

	for(XGUI_Widget * child: m_vChilds)
	{
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

}

/*
 *	Property accessors
 **/

// Setters
void XGUI_Widget::SetParent(XGUI_Widget * pParent)
{
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

// Getters
XGUI_Widget * XGUI_Widget::GetParent()
{
	return m_pParent;
}

TAlign XGUI_Widget::GetAlign()
{
	return m_Align;
}

TAnchor XGUI_Widget::GetAnchors()
{
	return m_Anchors;
}

bool XGUI_Widget::GetVisibilty()
{
	return m_bVisible;
}

bool XGUI_Widget::GetEnableState()
{
	return m_bEnabled;
}

/*
 *	Property accessors end
 **/

/*
 *	Adds child widget
 **/
void XGUI_Widget::AddChildWidget(XGUI_Widget * pWidget)
{
	m_vChilds.push_back(pWidget);
}

/*
 *	Sorts childs by ZOrder
 **/
void XGUI_Widget::SortChilds()
{
	std::sort(m_vChilds.begin(),m_vChilds.end(),[] (XGUI_Widget* i,XGUI_Widget* j){return i->m_ZOrder < j->m_ZOrder;});
}

/*
 *	Marks child items under cursor or self if no childs marked
 **/
bool XGUI_Widget::MarkItemUnderCursor(ME_Math::Vector2D pt)
{
	for(XGUI_Widget * c: m_vChilds)
	{
		if (c->MarkItemUnderCursor(pt))
			return true;
	}

	xgRect_t r = m_Rect;
	CalcClientRect(r);

	if (pt.x >= r.pos.x && pt.x <= r.pos.x + r.ext.x)
		if (pt.y >= r.pos.y && pt.y <= r.pos.y + r.ext.y)
		{
			m_flHoveroutTimer = g_pPlatform->TimeElapsed();

			if (m_flHoverinTimer < 1)
				m_flHoverinTimer = m_flHoveroutTimer + 0.3f;

			return true;
		}
		
	return false;
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