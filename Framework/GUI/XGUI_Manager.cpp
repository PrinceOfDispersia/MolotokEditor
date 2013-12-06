/*
*	Molotok Editor, 2013 (C) PrinceOfDispersia
*	XGUI_Manager.cpp - GUI Manager
*
**/

#include <Platform/Common/ApplicationCommon.h>

using namespace ME_Framework::ME_XGUI;
XGUI_Manager * g_pGUIManager;
XGUI_Tesselator * g_pTesselator;

// Resources

mSheetGlyph_t * ME_XGUI::sprButtonHovered[9];
mSheetGlyph_t * ME_XGUI::sprButtonNormal[9];
mSheetGlyph_t * ME_XGUI::sprButtonPressed[9];

mSheetGlyph_t * ME_XGUI::sprFloatingPanel[9];
mSheetGlyph_t * ME_XGUI::sprDragHandleDotsNormal;
mSheetGlyph_t * ME_XGUI::sprDragHandleDotsHovered;

/*
 *	Function loads 3 by 3 set of glyphs for scalable widgets
 **/
void LoadScalableSet(mSheetGlyph_t * sprites[9],char mask[])
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
 *	Constructor
 **/
XGUI_Manager::XGUI_Manager()
{
	size_t sz;
	byte * pBuffer = g_pPlatform->FileSystem()->LoadFile(_T("gui/skin.bin"),&sz);

	m_pImagesSheet = nullptr;

	// TODO: fix 
	m_pTesselator = new XGUI_Tesselator(65536);
	g_pTesselator = m_pTesselator;

	if (!pBuffer) 
		Sys_FatalError(_T("Can't open GUI skin file!"));

	//	0,0
	//	0,690
	//	256,690
	//
	m_GuiAtlas = GL_LoadTextureFromFS(_T("gui/atlas_all.png"));

	m_pImagesSheet = new XGUI_Sheet(pBuffer,sz);
	m_pImagesSheet->SetAtlas(m_GuiAtlas,ME_Math::Vector2D(0,0));

	g_pPlatform->FileSystem()->CloseFile(pBuffer);

	pBuffer = g_pPlatform->FileSystem()->LoadFile(_T("gui/fonts/segoeui9.ft2"),&sz);

	if (!pBuffer)
		return;

	m_pGuiFont = new XGUI_Font((dFontHdr_t*)pBuffer,sz);
	m_pGuiFont->SetAtlas(m_GuiAtlas,ME_Math::Vector2D(0,690));

	g_pGUIManager = this;

	LoadScalableSet(sprButtonNormal,"UI.ButtonBig.Normal");
	LoadScalableSet(sprButtonHovered,"UI.ButtonBig.Hovered");
	LoadScalableSet(sprButtonPressed,"UI.ButtonBig.Pressed");
	LoadScalableSet(sprFloatingPanel,"FloatingPanel");

	sprDragHandleDotsHovered = GetGUISheetGlyph("DragHandleDots.Hovered");
	sprDragHandleDotsNormal = GetGUISheetGlyph("DragHandleDots.Normal");

	m_pGuiVars = new CConfigVarsManager(_T("configs/gui_default_scheme.xml"));
	
	LoadVars();
	
	xgRect_t r;
	m_pDesktop = new XGUI_Widget(r);
	m_pDesktop->SetAlign(TAlign::alClientArea);
	m_pDesktop->RecalcRectWithAligment();


	// Setup dock sites

	TAlign a[] = {TAlign::alLeft,TAlign::alRight,TAlign::alTop,TAlign::alBottom};
	int priority[] = {9998,9998,9999,9999};

	r.ext = ME_Math::Vector2D(32,32);
	r.pos = ME_Math::Vector2D(0,0);

	for(int i = 0 ; i < 4 ; i++)
	{
		XGUI_Dock * pDock = new XGUI_Dock(r);

		pDock->SetAlign(a[i]);
		pDock->SetAlignPriority(priority[i]);

		m_pDesktop->AddChildWidget(pDock);
	}

	r.pos = ME_Math::Vector2D(320,240);
	r.ext = ME_Math::Vector2D(150,80);
	XGUI_DockWindow * pTestWindow = new XGUI_DockWindow(r);

	pTestWindow->SetZOrder(1);
	
	m_pDesktop->AddChildWidget(pTestWindow);

	// Recalculate rects
	m_pDesktop->RecalcItemsRects();

	// dock sites setup end

	m_bInEditorMode = false;
}

/*
 *	Initializes values from config file
 **/
void XGUI_Manager::LoadVars()
{
	m_GuiSettings.m_cDesktopBG = m_pGuiVars->GetColorValue(m_pGuiVars->QueryVariable("gui_desktop_bg_col","[45 45 48 255]"));	
}


/*
 * Destructor	
 **/
XGUI_Manager::~XGUI_Manager()
{
	delete m_pDesktop;
	delete m_pImagesSheet;
	delete m_pGuiVars;
	delete m_pGuiFont;
}

/*
 *	Render all the things!
 **/
void XGUI_Manager::Draw()
{
	m_pImagesSheet->Bind();
	m_pDesktop->Render();
	m_pTesselator->Flush();
}

/*
 *	Runs items logic
 **/
void XGUI_Manager::Think(float flDeltaTime)
{
	m_pDesktop->SortChilds();
	m_pDesktop->UpdateTimers(flDeltaTime);
	m_pDesktop->DoThink();

	XGUI_Widget * w = m_pDesktop->WidgetUnderCursor(g_pPlatform->GetCursorPos());

	if (w)
	{
		w->m_flHoveroutTimer = g_pPlatform->TimeElapsed();
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
	pWidget->m_pGuiFont = m_pGuiFont;
	m_pDesktop->AddChildWidget(pWidget);
	
	// Recalc aligments
	m_pDesktop->SetRect(m_pDesktop->m_Rect);
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
			XGUI_Widget * w = m_pDesktop->WidgetUnderCursor(g_pPlatform->GetCursorPos());
			if (w)
			{
				if (m_bInEditorMode)
				{
					ME_Math::Vector2D v = g_pPlatform->GetCursorPos();
					w->PointToClient(v);
					w->m_vDragOrigin = v;
					w->m_bDragged = true;
				}
				else 
					w->HandleEvent(ev);
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
					w->PointToClient(v);					
					w->m_bDragged = false;
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
	m_pVertexes[m_nUsedElements] = vec + m_vTranslation;
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
}

/*
 *	Pushes vertex specified by separate coords
 **/
void XGUI_Tesselator::Vertex2(vec_t x,vec_t y)
{
	m_pVertexes[m_nUsedElements].x = x + m_vTranslation.x;
	m_pVertexes[m_nUsedElements].y = y + m_vTranslation.y;
	
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
}