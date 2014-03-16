/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	XGUI_Button.cpp - generic button class
 *
 **/

#include <Platform/Common/ApplicationCommon.h>

using namespace ME_Framework::ME_XGUI;
using namespace ME_Framework::ME_OpenGLBackend;

extern XGUI_Manager * g_pGUIManager;
extern XGUI_Tesselator * g_pTesselator;

extern XGUI_Menu * g_pTestMenu;

/*
 *	Button constructor
 **/
XGUI_Button::XGUI_Button(xgRect_t & r): XGUI_Widget(r)
{
	m_Align = TAlign::alNone;

	m_Color.r = m_Color.g = m_Color.b = m_Color.a = 255;

	if (r.ext.Length() == 0)
	{
		r.ext.x = 70;
		r.ext.y = 24;
	}

	m_Rect.pos = r.pos;
	m_Rect.ext = r.ext;

	//m_strCaption = _T("");
	m_bPressed = false;

	m_pContextMenu = g_pTestMenu;
}

/*
 *	Destructor
 **/
XGUI_Button::~XGUI_Button()
{

}

/*
 * Render button hover overlay
 **/
void XGUI_Button::DrawHoverOverlay()
{
	float fadeOut = g_pPlatform->TimeElapsed() - m_flHoveroutTimer;
	fadeOut /= 0.3f;
	if (fadeOut > 1.0f) fadeOut = 1.0f;

	color32_t c;
	c.r = 255; c.g = 255; c.b = 255;
	c.a = (byte)(255 * (1-fadeOut));
	g_pTesselator->DefaultColor(c);

	if (fadeOut < 1)
		XGUI_DrawScalableSheetGlyph(sprButtonHovered,m_Rect);
}

/*
 *	Renders button text label
 **/
void XGUI_Button::DrawTextLabel()
{
	// TODO: calc this once when resize\moved\label change?
	
	m_pGuiFontNormal->SetTextColor(0,0,0,255);
	m_pGuiFontNormal->DrawAlignedText(m_strCaption,m_Rect,THorizontalAligment::alhCenter,TVerticalAligment::alvCenter);
	
}

/*
 *	Renders whole button
 **/
void XGUI_Button::DrawComponent()
{
	g_pTesselator->ResetDefaultColor();

	if (!m_bPressed)
	{
		
		XGUI_DrawScalableSheetGlyph(sprButtonNormal,m_Rect);
		DrawHoverOverlay();
	}
	else
	{
		XGUI_DrawScalableSheetGlyph(sprButtonPressed,m_Rect);
	}
	
	g_pTesselator->ResetDefaultColor();
	
	DrawTextLabel();

}

/*
 *	Event handler
 **/
void XGUI_Button::HandleEvent(ME_Framework::appEvent_t & ev)
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