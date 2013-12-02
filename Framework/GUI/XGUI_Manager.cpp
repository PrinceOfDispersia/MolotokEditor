/*
*	Molotok Editor, 2013 (C) PrinceOfDispersia
*	XGUI_Manager.cpp - GUI Manager
*
**/

#include <Platform/Common/ApplicationCommon.h>

using namespace ME_Framework::ME_XGUI;
XGUI_Manager * g_pGUIManager;


mSheetGlyph_t * ME_XGUI::sprButtonHovered[9];
mSheetGlyph_t * ME_XGUI::sprButtonNormal[9];
mSheetGlyph_t * ME_XGUI::sprButtonPressed[9];

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

	if (!pBuffer) 
		return;

	m_pImagesSheet = new XGUI_Sheet(pBuffer,sz);

	g_pPlatform->FileSystem()->CloseFile(pBuffer);

	pBuffer = g_pPlatform->FileSystem()->LoadFile(_T("gui/fonts/segoeui9.ft2"),&sz);

	if (!pBuffer)
		return;

	m_pGuiFont = new XGUI_Font((dFontHdr_t*)pBuffer,sz);

	g_pGUIManager = this;

	LoadScalableSet(sprButtonNormal,"UI.ButtonBig.Normal");
	LoadScalableSet(sprButtonHovered,"UI.ButtonBig.Hovered");
	LoadScalableSet(sprButtonPressed,"UI.ButtonBig.Pressed");

	m_pGuiVars = new CConfigVarsManager(_T("configs/gui_default_scheme.xml"));
	LoadVars();
	
	xgRect_t r;
	m_pDesktop = new XGUI_Widget(&r);

	m_bInEditorMode = false;
}

/*
 *	Initializes values from config file
 **/
void XGUI_Manager::LoadVars()
{
	m_GuiSettings.m_cDesktopBG = m_pGuiVars->GetColorValue(m_pGuiVars->QueryVariable("gui_desktop_bg_col","[45 45 48 255]"));
	m_GuiSettings.m_cDesktopBG = m_pGuiVars->GetColorValue(m_pGuiVars->QueryVariable("gui_desktop_fg_col","[45 45 48 255]"));
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
	m_pDesktop->Render();
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
 *	Draw glyph from sheet
 **/
void ME_Framework::ME_XGUI::XGUI_DrawSheetGlyph(mSheetGlyph_t * pGlyph,xgRect_t & r)
{
	glBegin(GL_QUADS);
		glTexCoord2dv(pGlyph->c[0]);
		glVertex2d(r.pos.x,r.pos.y);
		glTexCoord2dv(pGlyph->c[1]);
		glVertex2d(r.pos.x + r.ext.x,r.pos.y);
		glTexCoord2dv(pGlyph->c[2]);
		glVertex2d(r.pos.x + r.ext.x,r.pos.y + r.ext.y);
		glTexCoord2dv(pGlyph->c[3]);
		glVertex2d(r.pos.x,r.pos.y + r.ext.y);
	glEnd();
}

/*
 * Renders fancy scalable glyph
 **/
void ME_Framework::ME_XGUI::XGUI_DrawScalableSheetGlyph(mSheetGlyph_t * pGlyphs[9],xgRect_t & r)
{
	pGlyphs[0]->sheet->Bind();

	ME_Math::Vector2D Vectors[9 * 4];
	
	int unscalable_top_x = (pGlyphs[0]->e[0] + pGlyphs[2]->e[0]);
	int unscalable_mid_y = (pGlyphs[0]->e[1] + pGlyphs[7]->e[1]);
	
	vec_t top_extend_x = (r.ext.x) - unscalable_top_x;
	vec_t top_extend_y = (r.ext.y) - unscalable_mid_y;

	// Top left

	Vectors[0].x = 0;										Vectors[0].y = 0;
	Vectors[1].x = pGlyphs[0]->e[0];						Vectors[1].y = 0;
	Vectors[2].x = pGlyphs[0]->e[0];						Vectors[2].y = pGlyphs[0]->e[1];
	Vectors[3].x = 0;										Vectors[3].y = pGlyphs[0]->e[1];

	// Top Middle

	Vectors[4].x = pGlyphs[0]->e[0];						Vectors[4].y = 0;
	Vectors[5].x = pGlyphs[0]->e[0] + top_extend_x;			Vectors[5].y = 0;
	Vectors[6].x = pGlyphs[0]->e[0] + top_extend_x;			Vectors[6].y = pGlyphs[1]->e[1];
	Vectors[7].x = pGlyphs[0]->e[0];						Vectors[7].y = pGlyphs[1]->e[1];

	// Top right

	Vectors[8].x =  Vectors[5].x;							Vectors[8].y = 0;
	Vectors[9].x =	Vectors[5].x + pGlyphs[2]->e[0];		Vectors[9].y = 0;
	Vectors[10].x = Vectors[5].x + pGlyphs[2]->e[0];		Vectors[10].y = pGlyphs[2]->e[1];
	Vectors[11].x = Vectors[5].x;							Vectors[11].y = pGlyphs[2]->e[1];
		
	// Middle left

	Vectors[12].x = 0;										Vectors[12].y = Vectors[3].y;
	Vectors[13].x = pGlyphs[3]->e[0];						Vectors[13].y = Vectors[3].y;
	Vectors[14].x = pGlyphs[3]->e[0];						Vectors[14].y = Vectors[3].y + top_extend_y;
	Vectors[15].x = 0;										Vectors[15].y = Vectors[3].y + top_extend_y;

	// Middle Middle

	Vectors[16].x = pGlyphs[3]->e[0];						Vectors[16].y = Vectors[3].y;
	Vectors[17].x = pGlyphs[3]->e[0] + top_extend_x;		Vectors[17].y = Vectors[3].y;
	Vectors[18].x = pGlyphs[3]->e[0] + top_extend_x;		Vectors[18].y = Vectors[3].y + top_extend_y;
	Vectors[19].x = pGlyphs[3]->e[0];						Vectors[19].y = Vectors[3].y + top_extend_y;

	// Middle right

	Vectors[20].x = Vectors[5].x;							Vectors[20].y = Vectors[3].y;
	Vectors[21].x =	Vectors[17].x + pGlyphs[5]->e[0];		Vectors[21].y = Vectors[3].y;
	Vectors[22].x = Vectors[17].x + pGlyphs[5]->e[0];		Vectors[22].y = Vectors[3].y  + top_extend_y;
	Vectors[23].x = Vectors[5].x;							Vectors[23].y = Vectors[3].y  + top_extend_y;

	// bottom left

	Vectors[24].x = 0;										Vectors[24].y = Vectors[23].y;
	Vectors[25].x = pGlyphs[6]->e[0];						Vectors[25].y = Vectors[23].y;
	Vectors[26].x = pGlyphs[6]->e[0];						Vectors[26].y = Vectors[23].y + pGlyphs[6]->e[1]; 
	Vectors[27].x = 0;										Vectors[27].y = Vectors[23].y + pGlyphs[6]->e[1];

	// bottom Middle

	Vectors[28].x = pGlyphs[6]->e[0];						Vectors[28].y = Vectors[23].y;
	Vectors[29].x = pGlyphs[6]->e[0] + top_extend_x;		Vectors[29].y = Vectors[23].y;
	Vectors[30].x = pGlyphs[6]->e[0] + top_extend_x;		Vectors[30].y = Vectors[23].y + pGlyphs[7]->e[1];
	Vectors[31].x = pGlyphs[6]->e[0];						Vectors[31].y = Vectors[23].y + pGlyphs[7]->e[1];

	// bottom right

	Vectors[32].x = Vectors[30].x;							Vectors[32].y = Vectors[23].y;
	Vectors[33].x =	Vectors[30].x + pGlyphs[8]->e[0];		Vectors[33].y = Vectors[23].y;
	Vectors[34].x = Vectors[30].x + pGlyphs[8]->e[0];		Vectors[34].y = Vectors[23].y + pGlyphs[8]->e[1];
	Vectors[35].x = Vectors[30].x;							Vectors[35].y = Vectors[23].y + pGlyphs[8]->e[1];


	glBegin(GL_QUADS);

	for(int i = 0 ; i < 36 ; i+=4)
	{
		//if (i / 4 == 4)  continue;

		glTexCoord2dv(pGlyphs[i / 4]->c[0]);
		glVertex2dv(Vectors[i] + r.pos);
		glTexCoord2dv(pGlyphs[i / 4]->c[1]);
		glVertex2dv(Vectors[i + 1]  + r.pos);
		glTexCoord2dv(pGlyphs[i / 4]->c[2]);
		glVertex2dv(Vectors[i + 2]  + r.pos);
		glTexCoord2dv(pGlyphs[i / 4]->c[3]);
		glVertex2dv(Vectors[i + 3]  + r.pos);
	}
	
	glEnd();
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
 *	Renders untextured rectangle
 **/
void ME_Framework::ME_XGUI::XGUI_DrawRect(xgRect_t & r)
{
	glBegin(GL_QUADS);
		
		glVertex2d(r.pos.x,r.pos.y);
		glVertex2d(r.pos.x + r.ext.x,r.pos.y);
		glVertex2d(r.pos.x + r.ext.x,r.pos.y + r.ext.y);
		glVertex2d(r.pos.x,r.pos.y + r.ext.y);

	glEnd();	
}
