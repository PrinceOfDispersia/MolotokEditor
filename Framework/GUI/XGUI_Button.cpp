/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	XGUI_Button.cpp - generic button class
 *
 **/

#include <Platform/Common/ApplicationCommon.h>

using namespace ME_Framework::ME_XGUI;
using namespace ME_Framework::ME_OpenGLBackend;

mSheetGlyph_t * g_pButtonNormalArr[9] = {nullptr};
mSheetGlyph_t * g_pButtonNormal =  0;
mSheetGlyph_t * g_pButtonHovered = 0;
mSheetGlyph_t * g_pButtonPressed = 0;

extern XGUI_Manager * g_pGUIManager;

XGUI_Button::XGUI_Button(xgRect_t * r): XGUI_Widget(r)
{
	m_Align = TAlign::alNone;

	if (g_pButtonNormalArr[0] == nullptr)
	{
		g_pButtonNormalArr[0] = g_pGUIManager->GetGUISheetGlyph("UI.ButtonBig.Pressed.TopLeft");
		g_pButtonNormalArr[1] = g_pGUIManager->GetGUISheetGlyph("UI.ButtonBig.Pressed.TopMiddle");
		g_pButtonNormalArr[2] = g_pGUIManager->GetGUISheetGlyph("UI.ButtonBig.Pressed.TopRight");

		g_pButtonNormalArr[3] = g_pGUIManager->GetGUISheetGlyph("UI.ButtonBig.Pressed.MiddleLeft");
		g_pButtonNormalArr[4] = g_pGUIManager->GetGUISheetGlyph("UI.ButtonBig.Pressed.MiddleMiddle");
		g_pButtonNormalArr[5] = g_pGUIManager->GetGUISheetGlyph("UI.ButtonBig.Pressed.MiddleRight");

		g_pButtonNormalArr[6] = g_pGUIManager->GetGUISheetGlyph("UI.ButtonBig.Pressed.BottomLeft");
		g_pButtonNormalArr[7] = g_pGUIManager->GetGUISheetGlyph("UI.ButtonBig.Pressed.BottomMiddle");
		g_pButtonNormalArr[8] = g_pGUIManager->GetGUISheetGlyph("UI.ButtonBig.Pressed.BottomRight");

		g_pButtonNormal = g_pGUIManager->GetGUISheetGlyph("UI.ButtonBig.Normal");

		g_pButtonHovered = g_pGUIManager->GetGUISheetGlyph("UI.ButtonBig.Hovered");
		g_pButtonPressed = g_pGUIManager->GetGUISheetGlyph("UI.ButtonBig.Pressed");
	}

	m_Color.r = m_Color.g = m_Color.b = m_Color.a = 255;

	if (r->ext.Length() == 0)
	{
		r->ext.x = g_pButtonNormal->e[0];
		r->ext.y = g_pButtonNormal->e[1];
	}

	m_Rect.pos = r->pos;
	m_Rect.ext = r->ext + ME_Math::Vector2D(100,100);

	m_strCaption = _T("Button17");
}

XGUI_Button::~XGUI_Button()
{

}

void XGUI_Button::DrawComponent()
{
	assert(g_pButtonNormal);

	GL_EnableState(GLS_TEXTURE_2D);
	GL_EnableState(GLS_BLEND);
	
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glColor4ubv((GLubyte*)&m_Color);
	
	g_pButtonNormal->sheet->Bind();


	//XGUI_DrawSheetGlyph(g_pButtonNormal,m_Rect);
	
	float fadeOut = g_pPlatform->TimeElapsed() - m_flHoveroutTimer;
	fadeOut /= 0.3f;
	if (fadeOut > 1.0f) fadeOut = 1.0f;

	float fadeIn = (m_flHoverinTimer - g_pPlatform->TimeElapsed()) / 0.3f;
	if (fadeIn < 0) fadeIn = 0;
	

	glColor4f(1,1,1,1-fadeOut);
	
	glColor4f(1,1,1,1);
	//XGUI_DrawSheetGlyph(g_pButtonHovered,m_Rect);
	XGUI_DrawScalableSheetGlyph(g_pButtonNormalArr,m_Rect);

	glColor4f(0,0,0,1);

	xgRect_t strRect;
	m_pGuiFont->Calc_TextRect(m_strCaption,&strRect);

	xgRect_t cl = m_Rect;
	CalcClientRect(cl);

	float xt = cl.pos.x + ((cl.ext.x / 2) - strRect.ext.x/2) + strRect.pos.x;
	float yt = cl.pos.y + ((cl.ext.y / 2) - strRect.ext.y/2) - strRect.pos.y;

	strRect.pos += cl.pos;


	glColor4f(0.2,0.2,0.2,1);

	m_pGuiFont->Draw(ME_Math::Vector2D(xt,yt),m_strCaption);
}

void XGUI_Button::HandleEvent(ME_Framework::appEvent_t * pEvent)
{

}