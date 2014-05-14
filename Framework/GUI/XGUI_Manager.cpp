/*
*	Molotok Editor, 2013 (C) PrinceOfDispersia
*	XGUI_Manager.cpp - GUI Manager
*
**/

#include <Platform/Common/ApplicationCommon.h>

using namespace ME_Framework::ME_XGUI;
using namespace ME_Framework::ME_OpenGLBackend;
XGUI_Manager * g_pGUIManager;
XGUI_Tesselator * g_pTesselator;

// Resources

mSheetGlyph_t * ME_XGUI::sprButtonHovered[9];
mSheetGlyph_t * ME_XGUI::sprButtonNormal[9];
mSheetGlyph_t * ME_XGUI::sprButtonPressed[9];

mSheetGlyph_t * ME_XGUI::sprFloatingPanel[9];
mSheetGlyph_t * ME_XGUI::sprDragHandleDotsNormal;
mSheetGlyph_t * ME_XGUI::sprDragHandleDotsHovered;

mSheetGlyph_t * ME_XGUI::sprDialogActive[9];
mSheetGlyph_t * ME_XGUI::sprDialogInActive[9];

mSheetGlyph_t * ME_XGUI::sprMinimize[3];
mSheetGlyph_t * ME_XGUI::sprMaximize[3];
mSheetGlyph_t * ME_XGUI::sprClose[3];
mSheetGlyph_t * ME_XGUI::sprRestore[3];


mSheetGlyph_t * ME_XGUI::sprWhite;
mSheetGlyph_t * ME_XGUI::sprBlank;

mSheetGlyph_t * ME_XGUI::sprMenuSelection;
mSheetGlyph_t * ME_XGUI::sprMenuChecked;

mSheetGlyph_t * ME_XGUI::sprMenuItemBg;
mSheetGlyph_t * ME_XGUI::sprMenuItemHover;


/*
 *	Function loads 3 by 3 set of glyphs for scalable widgets
 **/
void LoadScalableSprite(mSheetGlyph_t * sprites[9],char mask[])
{
	char tmp[128];
	
	// TOOD: make this safe way
	
	char xPos[][16] = {"Left","Middle","Right"};
	char yPos[][16] = {"Top","Middle","Bottom"};
	
	for(int y = 0 ; y < 3 ; y++)
	{
		for(int x = 0 ; x < 3 ; x++)
		{
			sprintf(tmp,"%s.%s%s",mask,yPos[y],xPos[x]);
			sprites[y * 3 + x] = g_pGUIManager->GetGUISheetGlyph(tmp);
		}
	}

}

/*
 *	Function loads spriteset for buttons
 **/
void LoadButtonSpriteSet(mSheetGlyph_t * dst[3],char mask[])
{
	char tmp[128];

	// TOOD: make this safe way

	char tags[][16] = {"Normal","Pressed","Hovered"};
	
	for(int x = 0 ; x < 3 ; x++)
	{
		sprintf(tmp,"%s.%s",mask,tags[x]);
		dst[x] = g_pGUIManager->GetGUISheetGlyph(tmp);
	}

}

/*
 *	Font loading routine
 **/
XGUI_Font * LoadFont(TCHAR * path,pgl_texture_t atlasTexture,ME_Math::Vector2D atlasOffset,vec_t ScaleFactor)
{
	size_t sz;
	byte * pBuffer = g_pPlatform->FileSystem()->LoadFile(path,&sz);

	if (!pBuffer)
		return 0;

	XGUI_Font * pRet;

	pRet = new XGUI_Font((dFontHdr_t*)pBuffer,sz,ScaleFactor);
	pRet->SetAtlas(atlasTexture,atlasOffset);

	g_pPlatform->FileSystem()->CloseFile(pBuffer);

	return pRet;
}

XGUI_Menu * g_pTestMenu;

XGUI_Menu* TestMenu(int call)
{
	if (call > 2) 
		return 0;

	XGUI_Menu * pMenu = new XGUI_Menu;

	for(int i = 0 ; i < 4 ; i++)
	{
		pMenu->AddItem(VA(_T("# %d Item # %d"),call,i),mikGeneric,0)->AssingSubmenu(TestMenu(call + 1));
	}

	return pMenu;
	
}

/*
 *	Constructor
 **/
XGUI_Manager::XGUI_Manager()
{
	size_t sz;
	byte * pBuffer = g_pPlatform->FileSystem()->LoadFile(_T("gui/skin.bin"),&sz);

	m_GuiAtlas = GL_LoadTextureFromFS(_T("gui/atlas_all.png"));
	m_pImagesSheet = nullptr;

	if (!pBuffer) 
		Sys_FatalError(_T("Can't open GUI skin file!"));

	//	0,0
	//	0,690
	//	256,690
	//
	

	m_pImagesSheet = new XGUI_Sheet(pBuffer,sz);
	m_pImagesSheet->SetAtlas(m_GuiAtlas,ME_Math::Vector2D(0,0));

	g_pPlatform->FileSystem()->CloseFile(pBuffer);

	

	// TODO: fix 
	m_pTesselator = new XGUI_Tesselator(65536);
	g_pTesselator = m_pTesselator;
	
	g_pGUIManager = this;

	m_pGuiFontNormal = LoadFont(_T("gui/fonts/Arial37.ft2"),m_GuiAtlas,ME_Math::Vector2D(0,680),0.25);
	m_pGuiFontSmall = LoadFont(_T("gui/fonts/Arial37.ft2"),m_GuiAtlas,ME_Math::Vector2D(0,680),0.1);

	LoadScalableSprite(sprButtonNormal,"UI.ButtonBig.Normal");
	LoadScalableSprite(sprButtonHovered,"UI.ButtonBig.Hovered");
	LoadScalableSprite(sprButtonPressed,"UI.ButtonBig.Pressed");
	LoadScalableSprite(sprFloatingPanel,"FloatingPanel");

	sprDragHandleDotsHovered = GetGUISheetGlyph("DragHandleDots.Hovered");
	sprDragHandleDotsNormal = GetGUISheetGlyph("DragHandleDots.Normal");

	sprMenuSelection = GetGUISheetGlyph("MenuCurrentSelection");
	sprMenuChecked = GetGUISheetGlyph("MenuCheckedItem");

	sprMenuItemBg = GetGUISheetGlyph("MenuItemBg");
	sprMenuItemHover = GetGUISheetGlyph("MenuItemHover");

	LoadScalableSprite(sprDialogActive,"dialogWnd.Active");
	LoadScalableSprite(sprDialogInActive,"dialogWnd.InActive");


	sprWhite = GetGUISheetGlyph("White");
	sprBlank = GetGUISheetGlyph("Blank");

	m_pGuiVars = new CConfigVarsManager(_T("configs/gui_default_scheme.xml"));
	
	LoadVars();
	
	xgRect_t r;
	m_pDesktop = new XGUI_Widget(r);
	m_pDesktop->SetAlign(TAlign::alClientArea);
	m_pDesktop->RecalcRectWithAligment();

	g_pTestMenu = TestMenu(0);
	RegisterMenu(g_pTestMenu);
	//t->Popup(ME_Math::Vector2D(320,240));


	LoadButtonSpriteSet(sprMinimize,"UI.Minimize");
	LoadButtonSpriteSet(sprMaximize,"UI.Maximize");
	LoadButtonSpriteSet(sprClose,"UI.Close");
	LoadButtonSpriteSet(sprRestore,"UI.Restore");

	for(int i = 0 ; i < 4 ; i++)
	{
		xgRect_t r;
		r.pos = ME_Math::Vector2D(400,400);
		r.ext = ME_Math::Vector2D(320,240);

		XGUI_Window * w = new XGUI_Window(r);
		w->SetCaption(String(_T("The quick brown fox jumped over a lazy dog")));
		AddWidget(w);

	}
	
	// Recalculate rects
	m_pDesktop->RecalcItemsRects();

	// dock sites setup end

	m_bInEditorMode = false;
	m_bCursorLocked = false;

	ME_Console::LoadGraphics();

	m_pFocusedWidget = 0;
	m_pCursorWidget = 0;

	m_pCurrentPopup = 0;
}

/*
 *	Initializes values from config file
 **/
void XGUI_Manager::LoadVars()
{
	m_GuiSettings.m_cDesktopBG = m_pGuiVars->GetColorValue(m_pGuiVars->QueryVariable("gui_desktop_bg_col","[45 45 48 255]"));	
	m_GuiSettings.m_cMenuDivider = m_pGuiVars->GetColorValue(m_pGuiVars->QueryVariable("menu_divider_color","[0 0 0 255]"));	
}


/*
 * Destructor	
 **/
XGUI_Manager::~XGUI_Manager()
{
	delete m_pDesktop;
	delete m_pImagesSheet;
	delete m_pGuiVars;
	delete m_pGuiFontNormal;
	delete m_pTesselator;
	delete m_pGuiFontSmall;
}

/*
 *	Render all the things!
 **/
void XGUI_Manager::Draw()
{
	m_pImagesSheet->Bind();

	GL_EnableState(GLS_TEXTURE_2D);
	GL_EnableState(GLS_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	m_pDesktop->Render();
	
	GL_DisableScissorTest();

	// 
	ME_Console::Draw();
	//

	m_pTesselator->Flush();
	Perform_WidgetRemoving();
}

/*
 *	Runs items logic
 **/
void XGUI_Manager::Think(float flDeltaTime)
{
	

	if (m_bCursorLocked)
	{
		ME_Math::Vector2D v = g_pPlatform->GetCursorPos();

		if (v.x < m_CursorLockRect.pos.x) v.x = m_CursorLockRect.pos.x;			
		if (v.x > (m_CursorLockRect.pos.x + m_CursorLockRect.ext.x))  v.x = m_CursorLockRect.pos.x + m_CursorLockRect.ext.x;

		if (v.y < m_CursorLockRect.pos.y) v.y = m_CursorLockRect.pos.y;			
		if (v.y > (m_CursorLockRect.pos.y + m_CursorLockRect.ext.y))  v.y = m_CursorLockRect.pos.y + m_CursorLockRect.ext.y;

		g_pPlatform->SetCursorPos(v);
	}


	m_pDesktop->SortChilds();
	m_pDesktop->UpdateTimers(flDeltaTime);
	m_pDesktop->DoThink();

	XGUI_Widget * w = m_pDesktop->WidgetUnderCursor(g_pPlatform->GetCursorPos());

	if (m_pCursorWidget && m_pCursorWidget != w)
	{
		ME_Framework::appEvent_t e;
		e.eventid = eventTypes::EV_CURSOR_LEAVED;
		m_pCursorWidget->HandleEvent(e);
	}

	if (w && m_pCursorWidget != w)
	{
		ME_Framework::appEvent_t e;
		e.eventid = eventTypes::EV_CURSOR_ENTERED;
		w->HandleEvent(e);
	}

	m_pCursorWidget = w;

	if (w)
	{		
		w->m_flHoveroutTimer = g_pPlatform->TimeElapsed();
	}
	
	if (w)
	{
		g_pPlatform->SetCursor(w->m_CurrentCursor);
	}

}

/*
 *	Searches for specified glyph in sheet
 **/
mSheetGlyph_t * XGUI_Manager::GetGUISheetGlyph(char * szName)
{
	if (m_pImagesSheet == nullptr) return 0;
	return m_pImagesSheet->FindGlyph(szName);
}

/*
 *	Adds widget to desktop
 **/
void XGUI_Manager::AddWidget(XGUI_Widget * pWidget)
{
	pWidget->m_pGuiFontNormal = m_pGuiFontNormal;
	pWidget->m_pGuiFontSmall = m_pGuiFontSmall;


	m_pDesktop->AddChildWidget(pWidget);
	
	// Recalc aligments
	m_pDesktop->SetRect(m_pDesktop->m_WorkRect);
}

/*
 *	Handles app event
 **/
void XGUI_Manager::HandleEvent(ME_Framework::appEvent_t & ev)
{
	switch(ev.eventid)
	{
		case eventTypes::EV_MOUSE_KEY_DOWN:
		{
			switch(ev.uParam1)
			{
			case MKEY_LEFT:
				{
					
					XGUI_Widget * w = m_pDesktop->WidgetUnderCursor(g_pPlatform->GetCursorPos());

					if (m_pCurrentPopup!=w)
					{
						if (m_pCurrentPopup)
						{
							if (w)
							{
								XGUI_MenuItem *menuItem = dynamic_cast<XGUI_MenuItem *>(w);
								if (NULL != menuItem)
								{
									if (!w->IsBelongsHeirarchy(m_pCurrentPopup))
										m_pCurrentPopup->SetVisibilty(false);

									if (!menuItem->SubMenu())
										m_pCurrentPopup->SetVisibilty(false);
								}
								else m_pCurrentPopup->SetVisibilty(false);
							}
							else 
								m_pCurrentPopup->SetVisibilty(false);
						}
					}

					if (w)
					{
						SetFocusedWidget(w);

						if (m_bInEditorMode)
						{
							ME_Math::Vector2D v = g_pPlatform->GetCursorPos();
							w->ScreenToClient(v);
							w->m_vDragOrigin = v;
							w->SetDragged(true);
						}
						else 
							w->HandleEvent(ev);
					}
				}
				break;
			case MKEY_RIGHT:
				{
					XGUI_Widget * w = m_pDesktop->WidgetUnderCursor(g_pPlatform->GetCursorPos());

					if (m_pCurrentPopup!=w)
					{
						if (m_pCurrentPopup)
							m_pCurrentPopup->SetVisibilty(false);
					}

					if (w && w->m_pContextMenu)
					{
						w->m_pContextMenu->Popup(g_pPlatform->GetCursorPos());
						m_pCurrentPopup = w->m_pContextMenu;
					}

				}

				break;
			}

		}
		break;
		case eventTypes::EV_MOUSE_KEY_UP:
		{
			if (m_bInEditorMode)
			{
				XGUI_Widget * w = m_pDesktop->WidgetUnderCursor(g_pPlatform->GetCursorPos());
				if (w)
				{
					ME_Math::Vector2D v = g_pPlatform->GetCursorPos();
					w->ScreenToClient(v);					
					w->SetDragged(false);
				}
			}
			m_pDesktop->HandleEvent(ev);
		}
		break;
		case eventTypes::EV_WINDOW_RESIZE:
		{
			xgRect_t r;
			r.ext = g_pPlatform->GetClientAreaExtents();
			r.pos = ME_Math::Vector2D(0,0);
			m_pDesktop->SetRect(r);
		}
		break;
	}
}

/*
 *	Tesselator constructor allocates buffers
 **/
XGUI_Tesselator::XGUI_Tesselator(int elementsCount)
{
	m_pColors = (color32_t*)g_pPlatform->MemoryPools()->Alloc(elementsCount * sizeof(color32_t));
	m_pVertexes = (ME_Math::Vector2D*)g_pPlatform->MemoryPools()->Alloc(elementsCount * sizeof(ME_Math::Vector2D));
	m_pUVMapping = (ME_Math::Vector2D*)g_pPlatform->MemoryPools()->Alloc(elementsCount * sizeof(ME_Math::Vector2D));

	m_nElements = elementsCount;
	m_nUsedElements = 0;
	m_vTranslation = ME_Math::Vector2D(0,0);
	m_cDefault.r = 255;
	m_cDefault.g = 255;
	m_cDefault.b = 255;
	m_cDefault.a = 255;

	m_vScaling.x = m_vScaling.y = 1;
}

/*
 *	Tesselator destructor - free buffers
 **/
XGUI_Tesselator::~XGUI_Tesselator()
{
	g_pPlatform->MemoryPools()->Free(m_pColors);
	g_pPlatform->MemoryPools()->Free(m_pVertexes);
	g_pPlatform->MemoryPools()->Free(m_pUVMapping);
}

/*
 *	Sets UV coordinates for current vertex
 **/
void XGUI_Tesselator::Coord2v(ME_Math::Vector2D & uv)
{
	m_pUVMapping[m_nUsedElements] = uv;
}

void XGUI_Tesselator::Coord2(float u,float v)
{
	m_pUVMapping[m_nUsedElements].x = u;
	m_pUVMapping[m_nUsedElements].y = v;
}


/*
 *	Sets color for current vertex
 **/
void XGUI_Tesselator::Color32(color32_t & c)
{
	m_pColors[m_nUsedElements] = c;
}

/*
 *	Pushes vertex
 **/
void XGUI_Tesselator::Vertex2v(ME_Math::Vector2D & vec)
{
	m_pVertexes[m_nUsedElements] = (vec + m_vTranslation) * m_vScaling;
	m_nUsedElements++;

	if (m_nUsedElements == (m_nElements)) 
		Flush();

	if (m_nUsedElements < m_nElements)
		m_pColors[m_nUsedElements] = m_cDefault;


}

/*
 *	Sets translation of vertex
 **/
void XGUI_Tesselator::SetTranslation(ME_Math::Vector2D & vOrigin)
{
	m_vTranslation = vOrigin;
}

/*
 *	Render buffers
 **/
void XGUI_Tesselator::Flush()
{
	if (m_nUsedElements == 0) return;

	m_vTranslation = ME_Math::Vector2D(0,0);
	
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	
	glVertexPointer(2,GL_VECT,0,m_pVertexes);
	glTexCoordPointer(2,GL_VECT,0,m_pUVMapping);
	glColorPointer(4,GL_UNSIGNED_BYTE,0,m_pColors);

	glDrawArrays(GL_QUADS,0,m_nUsedElements);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	
	m_nUsedElements = 0;
	ResetDefaultColor();
}

/*
 *	Pushes vertex specified by separate coords
 **/
void XGUI_Tesselator::Vertex2(vec_t x,vec_t y)
{
	m_pVertexes[m_nUsedElements].x = x + m_vTranslation.x;
	m_pVertexes[m_nUsedElements].y = y + m_vTranslation.y;
	
	m_pVertexes[m_nUsedElements] *= m_vScaling;

	m_nUsedElements++;

	if (m_nUsedElements == m_nElements) 
		Flush();
	
	if (m_nUsedElements < m_nElements)
		m_pColors[m_nUsedElements] = m_cDefault;
}

/*
 *	Pushes array of coords
 **/
void XGUI_Tesselator::Coord2a(ME_Math::Vector2D * pUV,int count)
{
	if (m_nUsedElements + count >= m_nElements)
		Flush();

	memcpy(&m_pUVMapping[m_nUsedElements],pUV,sizeof(ME_Math::Vector2D) * count);
}

/*
 *	Pushes array of colors
 **/
void XGUI_Tesselator::Color32a(color32_t * c,int count)
{
	if (m_nUsedElements + count >= m_nElements)
		Flush();

	memcpy(&m_pColors[m_nUsedElements],c,sizeof(color32_t) * count);
}

void XGUI_Tesselator::Vertex2a(ME_Math::Vector2D * pVecs,int count)
{
	if (m_nUsedElements + count >= m_nElements)
		Flush();
	
	memcpy(&m_pVertexes[m_nUsedElements],pVecs,count * sizeof(ME_Math::Vector2D));
	m_nUsedElements+=count;
	
}

/*
 * Sets default color for verts
 **/
void XGUI_Tesselator::DefaultColor(color32_t  c)
{
	m_cDefault = c;
	m_pColors[m_nUsedElements] = m_cDefault;
}

/*
 *	Enables cursor lock
 **/
void XGUI_Manager::LockCursor(xgRect_t rect)
{
	m_bCursorLocked = true;
	m_CursorLockRect = rect;
}

/*
 *	Disables cursor lock
 **/
void XGUI_Manager::UnlockCursor()
{
	m_bCursorLocked = false;
}

/*
 *	Locks cursor within desktop area
 **/
void XGUI_Manager::LockCursorInDesktop()
{
	LockCursor(m_pDesktop->GetRect());
}

/*
 *	Renders string with default font
 **/

void XGUI_Manager::Gui_Printf(vec_t x,vec_t y,TCHAR * string)
{
	m_pGuiFontNormal->Draw(ME_Math::Vector2D(x,y),string);
}

/*
 *	Returns specified font pointer
 **/
XGUI_Font * XGUI_Manager::Get_GuiFont(TGuiFontTypes fontType)
{
	switch(fontType)
	{
	case TGuiFontTypes::gfSmall:
			return m_pGuiFontSmall;
		break;
	case TGuiFontTypes::gfNormal:
			return m_pGuiFontNormal;
		break;
	}

	return 0;
}

/*
 *	Removes all widgets queryied to be removed
 **/
void XGUI_Manager::Perform_WidgetRemoving()
{
	for(widgetReferenceRemoveQuery_t q : m_vWidgetsToRemove)
	{
		auto it = std::find(q.container.begin(),q.container.end(),q.ptr);

		if (*it)
		{
			//q.container.erase(it);
		}
	}

	m_vWidgetsToRemove.clear();
}


/*
 *	Adds widget remove query to list
 **/
void XGUI_Manager::Safe_QueryWidgetRemove(TWidgetSharedPtr ptr,TWidgetVector & vec)
{
	assert("...");
	//widgetReferenceRemoveQuery_s q(ptr,vec);
	//m_vWidgetsToRemove.push_back(q);
}

void XGUI_Manager::SetFocusedWidget( XGUI_Widget * w )
{
	if (m_pFocusedWidget)
	{
		m_pFocusedWidget->m_bFocused = false;
		m_pFocusedWidget->SetZOrder(m_pFocusedWidget->m_PreFocusedZOrder);
	}

	m_pFocusedWidget = w;
	m_pFocusedWidget->m_bFocused = true;
	m_pFocusedWidget->m_PreFocusedZOrder = m_pFocusedWidget->GetZOrder();
	m_pFocusedWidget->SetZOrder(XGUI_Widget::zFocused);

}

XGUI_Widget * XGUI_Manager::CachedWidgetUnderCursor()
{
	return m_pCursorWidget;
}


void XGUI_Manager::RegisterMenu( XGUI_Menu * pMenu )
{
	AddWidget(pMenu);

	for(auto r: pMenu->m_vChilds)
	{
		if (XGUI_Widget * w = r.get())
		{
			XGUI_MenuItem *item = dynamic_cast<XGUI_MenuItem *>(w);
			if (NULL != item)
			{
				if (item->SubMenu())
					RegisterMenu(item->SubMenu());
			}
		}
	}
}

void XGUI_Manager::RemoveMenu( XGUI_Menu * pMenu )
{

}

void XGUI_Tesselator::SetScaling( vec_t x,vec_t y )
{
	m_vScaling.x = x;
	m_vScaling.y = y;
}

void XGUI_Tesselator::ResetScaling()
{
	m_vScaling.x = m_vScaling.y = 1;
}
