/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	XGUI_GenericSkinnedButton.cpp - general purpose button for gui components
 *
 **/

#include <Platform/Common/ApplicationCommon.h>

using namespace ME_Framework::ME_XGUI;
using namespace ME_Framework::ME_OpenGLBackend;
extern XGUI_Tesselator * g_pTesselator;
/*
 *	Constructor
 **/
XGUI_GenericButton::XGUI_GenericButton(xgRect_t & r) : XGUI_Widget(r)
{
	for(int i = 0 ; i < 3 ; i++)
		m_pSkinsSet[0] = 0;

	m_bPressed = false;
}

/*
 *	Render function
 **/
void XGUI_GenericButton::DrawComponent()
{
	switch (m_nUsedSkins)
	{
	case 1:
		{
			XGUI_DrawSheetGlyph(m_pSkinsSet[0],m_Rect);
		}
		break;
	case 2:
		{
			if (m_bPressed)
			{
				XGUI_DrawSheetGlyph(m_pSkinsSet[1],m_Rect);
			}
			else
			{
				XGUI_DrawSheetGlyph(m_pSkinsSet[0],m_Rect);
			}
		}
		break;
	case 3:
		{
			if (m_bPressed)
			{
				XGUI_DrawSheetGlyph(m_pSkinsSet[1],m_Rect);
			}
			else
			{
				XGUI_DrawSheetGlyph(m_pSkinsSet[0],m_Rect);
				
				float fadeOut = g_pPlatform->TimeElapsed() - m_flHoveroutTimer;
				fadeOut /= 0.3f;
				if (fadeOut > 1.0f) fadeOut = 1.0f;

				color32_t c;
				c.r = c.g = c.b = 255;
				c.a = (byte)(1-fadeOut) * 255;

				g_pTesselator->DefaultColor(c);
				
				if (fadeOut < 1)
					XGUI_DrawSheetGlyph(m_pSkinsSet[2],m_Rect);

				g_pTesselator->ResetDefaultColor();
			}
		}
		break;
	default:
		break;
	}
}


/*
 *	Sets button id
 **/
void XGUI_GenericButton::SetButtonID(int id)
{
	m_iButtonID = m_iButtonID;
}

/*
 * Gets button id
 **/
int XGUI_GenericButton::GetButotnID()
{
	return m_iButtonID;
}

/*
 * Sets skins
 **/
void XGUI_GenericButton::SetSkinSet(mSheetGlyph_t ** pSpritesSet,int sprCount)
{
	for(int i = 0 ; i < sprCount ; i++)
	{
		m_pSkinsSet[i] = pSpritesSet[i];
	}

	m_nUsedSkins = sprCount;
}

/*
 *	Button event handler
 **/
void XGUI_GenericButton::HandleEvent(ME_Framework::appEvent_t & e)
{
	switch(e.eventid)
	{
	case eventTypes::EV_MOUSE_KEY_DOWN:
		{
			m_bPressed = true;
			appEvent_t eParent;
			eParent.eventid = eventTypes::EV_CHILD_BUTTON_PUSHED;
			eParent.iParam1 = m_iButtonID;	
			m_pParent->HandleEvent(eParent);
		}		
		break;
	case eventTypes::EV_MOUSE_KEY_UP:
		{
			m_bPressed = false;
			appEvent_t eParent;
			eParent.eventid = eventTypes::EV_CHILD_BUTTON_RELEASED;
			eParent.iParam1 = m_iButtonID;
			m_pParent->HandleEvent(eParent);			
		}
		break;
	}
}