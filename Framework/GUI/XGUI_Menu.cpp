/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	XGUI_Menu.cpp - generic button class
 *
 **/

#include <Platform/Common/ApplicationCommon.h>

using namespace ME_Framework::ME_XGUI;
using namespace ME_Framework::ME_OpenGLBackend;

extern XGUI_Manager * g_pGUIManager;
extern XGUI_Tesselator * g_pTesselator;

/*
 *	Button constructor
 **/
XGUI_MenuItem::XGUI_MenuItem(xgRect_t & r): XGUI_Widget(r)
{
	m_Align = TAlign::alNone;

	m_Color.r = m_Color.g = m_Color.b = m_Color.a = 255;

	if (r.ext.Length() == 0)
	{
		r.ext.x = 170;
		r.ext.y = 20;
	}

	m_Rect.pos = r.pos;
	m_Rect.ext = r.ext;

	m_strCaption = _T("Сохранить...");
	m_bPressed = false;
}

/*
 *	Destructor
 **/
XGUI_MenuItem::~XGUI_MenuItem()
{

}

/*
 * Render button hover overlay
 **/
void XGUI_MenuItem::DrawHoverOverlay()
{
	float fadeOut = g_pPlatform->TimeElapsed() - m_flHoveroutTimer;
	fadeOut /= 0.3f;
	if (fadeOut > 1.0f) fadeOut = 1.0f;

	color32_t c;
	c.r = 255; c.g = 255; c.b = 255;
	c.a = (byte)(255 * (1-fadeOut));
	g_pTesselator->DefaultColor(c);

	if (fadeOut < 1)
		XGUI_DrawSheetGlyph(sprMenuItemHover,m_Rect);
}

/*
 *	Renders button text label
 **/
void XGUI_MenuItem::DrawTextLabel()
{
	// TODO: calc this once when resize\moved\label change?
	
	xgRect_t f = m_Rect;
	f.Implode(xAxis,16);
	
	m_pGuiFontNormal->SetTextColor(0,0,0,255);
	m_pGuiFontNormal->DrawAlignedText(m_strCaption,f,TTextHorizontalAligment::alhLeft,TTextVerticalAligment::alvCenter);
	
	xgRect_t r;
	r.pos = m_Rect.pos + m_vMargins[0].x;
	r.ext = m_Rect.ext;

	

	r.ext.x = sprMenuChecked->e[0];
	r.ext.y = sprMenuChecked->e[1];

	r.pos.y = m_Rect.pos.y  +  (m_Rect.ext.y / 2 - r.ext.y / 2);

	XGUI_DrawSheetGlyph(sprMenuChecked,r);



}

/*
 *	Renders whole button
 **/
void XGUI_MenuItem::DrawComponent()
{
	

	XGUI_DrawSheetGlyph(sprMenuItemBg,m_Rect);
	DrawHoverOverlay();	
	DrawTextLabel();
}

/*
 *	Event handler
 **/
void XGUI_MenuItem::HandleEvent(ME_Framework::appEvent_t & ev)
{
	switch(ev.eventid)
	{
	case eventTypes::EV_MOUSE_KEY_DOWN:
		if (ev.uParam1 == MKEY_LEFT) 
			m_bPressed = true;
		break;
	case eventTypes::EV_MOUSE_KEY_UP:
		m_bPressed = false;
		break;
	}
}