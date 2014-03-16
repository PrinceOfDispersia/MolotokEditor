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

	m_pSubMenu = 0;
	m_SelectionGroup = -1;
	m_MenuKind = mikGeneric;
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
	f.Implode(xAxis,m_Rect.ext.y);
	
	m_pGuiFontNormal->SetTextColor(0,0,0,255);
	m_pGuiFontNormal->DrawAlignedText(m_strCaption,f,THorizontalAligment::alhLeft,TVerticalAligment::alvCenter);
	
	xgRect_t r;
	r.pos = m_Rect.pos;
	r.ext = ME_Math::Vector2D(m_Rect.ext.y,m_Rect.ext.y);

	if (0)
	{
		color32_t c;
		c.r = 255;
		c.g = 0;
		c.b = 0;
		c.a = 255;
		g_pTesselator->DefaultColor(c);
		XGUI_DrawSheetGlyph(sprWhite,r);
	}

	if (m_bPressed)
	{
		switch (m_MenuKind)
		{
		case ME_XGUI::mikGeneric:
			break;
		case ME_XGUI::mikToggle:
			XGUI_DrawSheetGlyphAligned(sprMenuChecked,r,THorizontalAligment::alhCenter,TVerticalAligment::alvCenter);
			break;
		case ME_XGUI::mikSelection:
			XGUI_DrawSheetGlyphAligned(sprMenuSelection,r,THorizontalAligment::alhCenter,TVerticalAligment::alvCenter);
			break;
		default:
			break;
		}		
	}

	if (m_pSubMenu)
	{
		xgRect_t r = m_Rect;
		r.Implode(xAxis,4);
		XGUI_DrawSheetGlyphAligned(sprMenuSelection,r,THorizontalAligment::alhRight,TVerticalAligment::alvCenter);
	}
}

/*
 *	Renders whole button
 **/
void XGUI_MenuItem::DrawComponent()
{
	XGUI_DrawSheetGlyph(sprMenuItemBg,m_Rect);

	if (m_strCaption.c_str()[0] != _T('-'))
	{		
		DrawHoverOverlay();	
		DrawTextLabel();
	}
	else
	{
		XGUI_DrawSheetGlyph(sprMenuItemBg,m_Rect);
		xgRect_t r = m_Rect;
		r.Implode(xAxis,20);
		r.ext.x += 18;

		g_pTesselator->DefaultColor(g_pGUIManager->GuiSettings()->m_cMenuDivider);
		XGUI_DrawSheetGlyph(sprWhite,r);
		g_pTesselator->ResetDefaultColor();		
	
	}
	
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
		{
			switch (m_MenuKind)
			{
			case ME_Framework::ME_XGUI::mikGeneric:
				// execute command here
				break;
			case ME_Framework::ME_XGUI::mikToggle:
					m_bPressed = !m_bPressed;
				break;
			case ME_Framework::ME_XGUI::mikSelection:
				{
					for (auto r  : m_pParent->GetChilds())
					{						
						if (XGUI_Widget * w = r.get())
						{
							if (w == this) continue;

							XGUI_MenuItem *item = dynamic_cast<XGUI_MenuItem *>(w);
							if (NULL != item)
							{
								if (item->m_SelectionGroup != -1)
									item->m_bPressed = false;
							}
						}
					}

					m_bPressed = true;
				}
				break;
			default:
				break;
			}
			
		}
		break;
	case eventTypes::EV_MOUSE_KEY_UP:
		
		break;
	case eventTypes::EV_CURSOR_ENTERED:
		
		if (m_pSubMenu && !m_pSubMenu->GetVisibilty())
		{
			XGUI_Menu *pMenu = dynamic_cast<XGUI_Menu *>(m_pParent);
			if (NULL != pMenu)
			{
				pMenu->PopupSubmenu(m_pSubMenu);
				
				m_pSubMenu->m_Rect.pos.x = pMenu->m_Rect.Right();
				m_pSubMenu->m_Rect.pos.y = pMenu->m_Rect.Top() + m_Rect.Top();
			}
		}
		else if (m_pSubMenu)
		{
			XGUI_Menu *pMenu = dynamic_cast<XGUI_Menu *>(m_pParent);
			if (NULL != pMenu)
			{
				pMenu->m_iSubMenuHideCounter++;				
			}
		}
		else 
		{
			XGUI_Menu *pMenu = dynamic_cast<XGUI_Menu *>(m_pParent);
			if (NULL != pMenu && pMenu->m_pCurrentPopedUpSubmenu)
			{
				pMenu->m_iSubMenuHideCounter--;
				if (pMenu->m_iSubMenuHideCounter == 0)
				{
					pMenu->m_pCurrentPopedUpSubmenu->SetVisibilty(false);
					pMenu->m_pCurrentPopedUpSubmenu = 0;
				}
			}
		}

		break;
	case eventTypes::EV_CURSOR_LEAVED:
		//m_strCaption = _T("2");
		break;
	default:
		break;
	}
}

void XGUI_MenuItem::AssingSubmenu(XGUI_Menu * pMenu)
{
	if (!pMenu) return;
	m_pSubMenu = pMenu;	
	m_pSubMenu->m_pSubmenuOwner = this;
	

	pMenu->SetVisibilty(false);
}

XGUI_Menu::XGUI_Menu(): XGUI_Widget()
{
	m_pCurrentPopedUpSubmenu = nullptr;
	m_iSubMenuHideCounter = 0;
	SetVisibilty(false);
	m_pSubmenuOwner = 0;
}

XGUI_Menu::~XGUI_Menu()
{

}

XGUI_MenuItem * XGUI_Menu::AddItem( String strName,eMenuItemKinds kind,int group, bool bChecked)
{
	xgRect_t r;
	r.pos.x = 0;
	r.pos.y = m_Rect.ext.y;

	r.ext.Init();

	XGUI_MenuItem * pRet = new XGUI_MenuItem(r);
	pRet->SetCaption(strName);
	AddChildWidget(pRet);
		

	if (m_Rect.ext.x < pRet->GetRect().ext.x)
			m_Rect.ext.x = pRet->GetRect().ext.x;
	if (strName.c_str()[0] == _T('-'))
		pRet->GetRect().ext.y = 1;

	m_Rect.ext.y += pRet->GetRect().ext.y;

	pRet->SetKind(kind);
	pRet->SetSelectionGroup(group);
	pRet->SetToggleState(bChecked);

	return pRet;
}

void XGUI_Menu::Popup( ME_Math::Vector2D pos )
{
	m_Rect.pos = pos;
	SetVisibilty(true);
	SetZOrder(zFocused+1);
}

void XGUI_MenuItem::SetKind( eMenuItemKinds kind )
{
	m_MenuKind = kind;
}

void XGUI_Menu::PopupSubmenu( XGUI_Menu * pMenu )
{
	if (m_pCurrentPopedUpSubmenu)
		m_pCurrentPopedUpSubmenu->SetVisibilty(false);

	m_iSubMenuHideCounter = 5;

	m_pCurrentPopedUpSubmenu = pMenu;
	m_pCurrentPopedUpSubmenu->SetVisibilty(false);
	m_pCurrentPopedUpSubmenu->SetVisibilty(true);
	m_pCurrentPopedUpSubmenu->SetZOrder(GetZOrder() + 1);
}

void XGUI_Menu::OnVisibilityChange( bool bNewState )
{
	if (!m_pParent) return;

	if (!bNewState)
	{
		for (auto r  : GetChilds())
		{						
			if (XGUI_Widget * w = r.get())
			{
				if (w == this) continue;

				XGUI_MenuItem *item = dynamic_cast<XGUI_MenuItem *>(w);
				if (NULL != item && item->m_pSubMenu && item->m_pSubMenu->GetVisibilty())
				{
					item->m_pSubMenu->SetVisibilty(bNewState);
				}
			}
		}
	}

	
}

bool XGUI_MenuItem::IsBelongsHeirarchy( XGUI_Widget * w )
{
	XGUI_Menu *parent = dynamic_cast<XGUI_Menu *>(m_pParent);
	if (NULL != parent)
	{
		if (parent == w)
			return true;

		if (parent->m_pSubmenuOwner)
			return parent->m_pSubmenuOwner->IsBelongsHeirarchy(w);
		else return false;
	}
	else 
		return false;
}
