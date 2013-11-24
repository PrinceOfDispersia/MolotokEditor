/*
*	Molotok Editor, 2013 (C) PrinceOfDispersia
*	XGUI_Manager.cpp - GUI Manager
*
**/

#include <Platform/Common/ApplicationCommon.h>

using namespace ME_Framework::ME_XGUI;
XGUI_Manager * g_pGUIManager;

/*
 *	Constructor
 **/
XGUI_Manager::XGUI_Manager()
{
	xgRect_t r;
	m_pDesktop = new XGUI_Widget(&r);
	

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
}

/*
 * Destructor	
 **/
XGUI_Manager::~XGUI_Manager()
{
	delete m_pDesktop;
	delete m_pImagesSheet;
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

	m_pDesktop->MarkItemUnderCursor(g_pPlatform->GetCursorPos());
}

mSheetGlyph_t * XGUI_Manager::GetGUISheetGlyph(char * szName)
{
	if (m_pImagesSheet == nullptr) return 0;

	return m_pImagesSheet->FindGlyph(szName);
}

void XGUI_Manager::AddWidget(XGUI_Widget * pWidget)
{
	pWidget->m_pGuiFont = m_pGuiFont;
	m_pDesktop->AddChildWidget(pWidget);
}

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

void ME_Framework::ME_XGUI::XGUI_DrawScalableSheetGlyph(mSheetGlyph_t * pGlyphs[9],xgRect_t & r)
{
	ME_Math::Vector2D Vectors[9 * 4];

	r.ext.x = (sin(g_pPlatform->TimeElapsed())+1) * 500;
	r.ext.y = (sin(g_pPlatform->TimeElapsed())+1) * 500;

	int unscalable_top_x = (pGlyphs[0]->e[0] + pGlyphs[2]->e[0]);
	int unscalable_mid_y = (pGlyphs[0]->e[1] + pGlyphs[2]->e[1]);
	
	float top_extend_x = (r.ext.x) - unscalable_top_x;
	float top_extend_y = (r.ext.y) - unscalable_mid_y;

	// Top left

	Vectors[0].x = 0;										Vectors[0].y = 0;
	Vectors[1].x = pGlyphs[0]->e[0];						Vectors[1].y = 0;
	Vectors[2].x = pGlyphs[0]->e[0];						Vectors[2].y = pGlyphs[0]->e[1];
	Vectors[3].x = 0;										Vectors[3].y = pGlyphs[0]->e[1];

	// Top Middle

	Vectors[4].x = pGlyphs[0]->e[0];						Vectors[4].y = 0;
	Vectors[5].x = pGlyphs[1]->e[0] + top_extend_x;			Vectors[5].y = 0;
	Vectors[6].x = pGlyphs[1]->e[0] + top_extend_x;			Vectors[6].y = pGlyphs[1]->e[1];
	Vectors[7].x = pGlyphs[0]->e[0];						Vectors[7].y = pGlyphs[1]->e[1];

	// Top right

	Vectors[8].x =  Vectors[5].x;							Vectors[8].y = 0;
	Vectors[9].x =	Vectors[5].x + pGlyphs[2]->e[0];		Vectors[9].y = 0;
	Vectors[10].x = Vectors[5].x + pGlyphs[2]->e[0];		Vectors[10].y = pGlyphs[2]->e[1];
	Vectors[11].x = Vectors[5].x;							Vectors[11].y = pGlyphs[2]->e[1];
		
	// Middle left

	Vectors[12].x = 0;										Vectors[12].y = Vectors[3].y;
	Vectors[13].x = pGlyphs[3]->e[0];						Vectors[13].y = Vectors[3].y;
	Vectors[14].x = pGlyphs[3]->e[0];						Vectors[14].y = pGlyphs[3]->e[1] + top_extend_y;
	Vectors[15].x = 0;										Vectors[15].y = pGlyphs[3]->e[1] + top_extend_y;

	// Middle Middle

	Vectors[16].x = pGlyphs[3]->e[0];						Vectors[16].y = Vectors[3].y;
	Vectors[17].x = pGlyphs[4]->e[0] + top_extend_x;		Vectors[17].y = Vectors[3].y;
	Vectors[18].x = pGlyphs[4]->e[0] + top_extend_x;		Vectors[18].y = pGlyphs[4]->e[1] + top_extend_y;
	Vectors[19].x = pGlyphs[3]->e[0];						Vectors[19].y = pGlyphs[4]->e[1] + top_extend_y;

	// Middle right

	Vectors[20].x =  Vectors[5].x;							Vectors[20].y = Vectors[3].y;
	Vectors[21].x =	Vectors[5].x + pGlyphs[5]->e[0];		Vectors[21].y = Vectors[3].y;
	Vectors[22].x = Vectors[5].x + pGlyphs[5]->e[0];		Vectors[22].y = pGlyphs[5]->e[1]  + top_extend_y;
	Vectors[23].x = Vectors[5].x;							Vectors[23].y = pGlyphs[5]->e[1]  + top_extend_y;

	// bottom left

	Vectors[24].x = 0;										Vectors[24].y = Vectors[23].y;
	Vectors[25].x = pGlyphs[6]->e[0];						Vectors[25].y = Vectors[23].y;
	Vectors[26].x = pGlyphs[6]->e[0];						Vectors[26].y = Vectors[23].y + pGlyphs[6]->e[1]; 
	Vectors[27].x = 0;										Vectors[27].y = Vectors[23].y + pGlyphs[6]->e[1];

	// bottom Middle

	Vectors[28].x = pGlyphs[6]->e[0];						Vectors[28].y = Vectors[23].y;
	Vectors[29].x = pGlyphs[7]->e[0] + top_extend_x;		Vectors[29].y = Vectors[23].y;
	Vectors[30].x = pGlyphs[7]->e[0] + top_extend_x;		Vectors[30].y = Vectors[23].y + pGlyphs[7]->e[1];
	Vectors[31].x = pGlyphs[6]->e[0];						Vectors[31].y = Vectors[23].y + pGlyphs[7]->e[1];

	// bottom right

	Vectors[32].x = Vectors[30].x;							Vectors[32].y = Vectors[23].y;
	Vectors[33].x =	Vectors[30].x + pGlyphs[8]->e[0];		Vectors[33].y = Vectors[23].y;
	Vectors[34].x = Vectors[30].x + pGlyphs[8]->e[0];		Vectors[34].y = Vectors[23].y + pGlyphs[8]->e[1];
	Vectors[35].x = Vectors[30].x;							Vectors[35].y = Vectors[23].y + pGlyphs[8]->e[1];


	glBegin(GL_QUADS);

	for(int i = 0 ; i < 36 ; i+=4)
	{
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

void XGUI_Manager::HandleEvent(ME_Framework::appEvent_t & ev)
{

}
