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

	m_WorkRect.pos = r.pos;
	m_WorkRect.ext = r.ext;

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
		XGUI_DrawSheetGlyph(sprMenuItemHover,m_WorkRect);
}

/*
 *	Renders button text label
 **/
void XGUI_MenuItem::DrawTextLabel()
{
	// TODO: calc this once when resize\moved\label change?
	
	xgRect_t f = m_WorkRect;
	f.Implode(xAxis,m_WorkRect.ext.y);
	
	m_pGuiFontNormal->SetTextColor(0,0,0,255);
	m_pGuiFontNormal->DrawAlignedText(m_strCaption,f,THorizontalAligment::alhLeft,TVerticalAligment::alvCenter);
	
	xgRect_t r;
	r.pos = m_WorkRect.pos;
	r.ext = ME_Math::Vector2D(m_WorkRect.ext.y,m_WorkRect.ext.y);

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
		xgRect_t r = m_WorkRect;
		r.Implode(xAxis,4);
		XGUI_DrawSheetGlyphAligned(sprMenuSelection,r,THorizontalAligment::alhRight,TVerticalAligment::alvCenter);
	}
}

/*
 *	Renders whole button
 **/
void XGUI_MenuItem::DrawComponent()
{
	//XGUI_DrawSheetGlyph(sprMenuItemBg,m_Rect);

	if (m_strCaption.c_str()[0] != _T('-'))
	{		
		DrawHoverOverlay();	
		DrawTextLabel();
	}
	else
	{
		//XGUI_DrawSheetGlyph(sprMenuItemBg,m_Rect);
		xgRect_t r = m_WorkRect;
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
				
				m_pSubMenu->m_WorkRect.pos.x = pMenu->m_WorkRect.Right();
				m_pSubMenu->m_WorkRect.pos.y = pMenu->m_WorkRect.Top() + m_WorkRect.Top();
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

XGUI_MenuItem * XGUI_Menu::AddItem( String strName,TMenuItemKinds kind,int group, bool bChecked)
{
	xgRect_t r;
	r.pos.x = 0;
	r.pos.y = m_WorkRect.ext.y;

	r.ext.Init();

	XGUI_MenuItem * pRet = new XGUI_MenuItem(r);
	pRet->SetCaption(strName);
	AddChildWidget(pRet);
		

	if (m_WorkRect.ext.x < pRet->GetRect().ext.x)
			m_WorkRect.ext.x = pRet->GetRect().ext.x;
	if (strName.c_str()[0] == _T('-'))
		pRet->GetRect().ext.y = 1;

	m_WorkRect.ext.y += pRet->GetRect().ext.y;

	pRet->SetKind(kind);
	pRet->SetSelectionGroup(group);
	pRet->SetToggleState(bChecked);

	return pRet;
}

void XGUI_Menu::Popup( ME_Math::Vector2D pos )
{
	m_WorkRect.pos = pos;
	SetVisibilty(true);
	SetZOrder(zFocused+1);
}

void XGUI_MenuItem::SetKind( TMenuItemKinds kind )
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

void XGUI_Menu::DrawComponent()
{
	XGUI_DrawSheetGlyph(sprMenuItemBg,m_WorkRect);
}

XGUI_MenuBar::XGUI_MenuBar(xgRect_t r): XGUI_Widget(r)
{
	SetAlign(TAlign::alTop);
	GetRect().ext.y = 20;
}

XGUI_MenuBar::~XGUI_MenuBar()
{

}

void XGUI_MenuBar::AddItem( String strName,TWidgetSharedPtr pMenu )
{
	vec_t pad = 2.0;
	vec_t offset = pad;

	for(TWidgetSharedPtr widget: m_vChilds)
	{
		offset += widget->GetRect().ext.x + pad;
	}
		
	vec_t w = 0;
	xgRect_t r;

	
	g_pGUIManager->Get_GuiFont(TGuiFontTypes::gfNormal)->Calc_TextRect(strName,&r);

	r.pos.y = 0;
	r.pos.x = offset;
	r.ext.y = 20;
	r.ext.x+=15;

	

	XGUI_MenuButton * pButton = new XGUI_MenuButton(r);

	mSheetGlyph_t* skinArr[] = {sprBlank,sprBlank,sprMenuItemHover};
	pButton->SetCaption(strName);
	AddChildWidget(pButton);
}

void XGUI_MenuBar::DrawComponent()
{

}

XGUI_MenuButton::XGUI_MenuButton( xgRect_t & r ):XGUI_Widget(r)
{
	m_bPressed = false;
}

void XGUI_MenuButton::DrawComponent()
{
	float fadeOut = g_pPlatform->TimeElapsed() - m_flHoveroutTimer;
	fadeOut /= 0.3f;
	if (fadeOut > 1.0f) fadeOut = 1.0f;

	color32_t c;
	c.r = c.g = c.b = 255;
	c.a = (byte)((1-fadeOut) * 255);

	g_pTesselator->DefaultColor(c);

	if (fadeOut < 1)
	{
		XGUI_DrawSheetGlyph(sprMenuItemHover,m_WorkRect);			
	}

	g_pTesselator->ResetDefaultColor();

	m_pGuiFontNormal->SetTextColor(0,0,0,255);
	m_pGuiFontNormal->DrawAlignedText(m_strCaption,m_WorkRect,THorizontalAligment::alhCenter,TVerticalAligment::alvCenter);
}

void XGUI_MenuButton::HandleEvent( ME_Framework::appEvent_t & e )
{
	if (e.eventid == eventTypes::EV_MOUSE_KEY_DOWN)
	{
		XGUI_MenuBar * m_pBar = dynamic_cast<XGUI_MenuBar*>(m_pParent);

		assert(m_pBar);

		m_pBar->m_bActive = true;


	}
	else if (e.eventid == eventTypes::EV_NOTIFY_COMPONENT_CLICKED)
	{
		MessageBeep(0xFFFFFFFF);
	}

		
	
}
