/*
*	Molotok Editor, 2013 (C) PrinceOfDispersia
*	XGUI_DrawUtils.cpp - entry point for cross-platform application
*
**/

#include <Platform/Common/ApplicationCommon.h>

extern ME_XGUI::XGUI_Tesselator * g_pTesselator;

/*
 *	Draw glyph from sheet
 **/
void ME_Framework::ME_XGUI::XGUI_DrawSheetGlyph(mSheetGlyph_t * pGlyph,xgRect_t & r)
{
	g_pTesselator->Coord2v(pGlyph->c[0]);		
	g_pTesselator->Vertex2(r.pos.x,r.pos.y);

	g_pTesselator->Coord2v(pGlyph->c[1]);
	g_pTesselator->Vertex2(r.pos.x + r.ext.x,r.pos.y);

	g_pTesselator->Coord2v(pGlyph->c[2]);
	g_pTesselator->Vertex2(r.pos.x + r.ext.x,r.pos.y + r.ext.y);

	g_pTesselator->Coord2v(pGlyph->c[3]);
	g_pTesselator->Vertex2(r.pos.x,r.pos.y + r.ext.y);
	
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

	
	

	for(int i = 0 ; i < 36 ; i+=4)
	{
		g_pTesselator->Coord2v(pGlyphs[i / 4]->c[0]);
		g_pTesselator->Vertex2v(Vectors[i] + r.pos);
		g_pTesselator->Coord2v(pGlyphs[i / 4]->c[1]);
		g_pTesselator->Vertex2v(Vectors[i + 1]  + r.pos);
		g_pTesselator->Coord2v(pGlyphs[i / 4]->c[2]);
		g_pTesselator->Vertex2v(Vectors[i + 2]  + r.pos);
		g_pTesselator->Coord2v(pGlyphs[i / 4]->c[3]);
		g_pTesselator->Vertex2v(Vectors[i + 3]  + r.pos);
	}
	
	
}

/*
 *	Renders untextured rectangle
 **/
void ME_Framework::ME_XGUI::XGUI_DrawRect(xgRect_t & r)
{
	g_pTesselator->Vertex2(r.pos.x,r.pos.y);
	g_pTesselator->Vertex2(r.pos.x + r.ext.x,r.pos.y);
	g_pTesselator->Vertex2(r.pos.x + r.ext.x,r.pos.y + r.ext.y);
	g_pTesselator->Vertex2(r.pos.x,r.pos.y + r.ext.y);
}

/*
 *	Renders untextured rectangle
 **/
void ME_Framework::ME_XGUI::XGUI_DrawRectOutline(xgRect_t & r)
{
	glBegin(GL_LINE_LOOP);
		
		glVertex2d(r.pos.x,r.pos.y);
		glVertex2d(r.pos.x + r.ext.x,r.pos.y);
		glVertex2d(r.pos.x + r.ext.x,r.pos.y + r.ext.y);
		glVertex2d(r.pos.x,r.pos.y + r.ext.y);

	glEnd();	
}

