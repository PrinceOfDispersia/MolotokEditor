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
	xgRect_t strRect;
	m_pGuiFont->Calc_TextRect(m_strCaption,&strRect);

	xgRect_t cl = m_Rect;
	CalcClientRect(cl);

	vec_t xt,yt;

	vec_t yofs = strRect.pos.y;

	vec_t w = strRect.ext.x;
	vec_t h = strRect.ext.y - yofs;

	strRect.pos += cl.pos + ME_Math::Vector2D((cl.ext.x / 2 - w / 2),(cl.ext.y / 2 - h / 2) - h / 4);

	xt = strRect.pos.x; yt = strRect.pos.y - yofs;

	
	glColor4f(0.0f,0.0f,0.0f,1);

	m_pGuiFont->Draw(ME_Math::Vector2D(xt,yt),m_strCaption);
}

/*
 *	Renders whole button
 **/
void XGUI_Button::DrawComponent()
{
	GL_EnableState(GLS_TEXTURE_2D);
	GL_EnableState(GLS_BLEND);
	GL_DisableState(GLS_DEPTH_TEST);
	
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glColor4ubv((GLubyte*)&m_Color);
	
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
	g_pTesselator->Flush();

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