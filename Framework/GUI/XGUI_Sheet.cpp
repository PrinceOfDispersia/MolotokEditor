/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	XGUI_Sheet.cpp - image sheet class
 *
 */

#include <Platform/Common/ApplicationCommon.h>

using namespace ME_Framework::ME_XGUI;



/*
 *	Sheet constructor
 **/
XGUI_Sheet::XGUI_Sheet(byte * pBuffer,size_t buffSize)
{
	if (memcmp(pBuffer,"SHEET",5)) 
	{
		m_pBuffer = 0;
		return;
	}

	m_pBuffer = (byte*)g_pPlatform->MemoryPools()->Alloc(buffSize);
	m_buffSize = buffSize;

	memcpy(m_pBuffer,pBuffer,buffSize);

	m_pHeader = (dSheetHDR_t *)m_pBuffer;

	byte * pImageSource = &m_pBuffer[m_pHeader->image_offset];
	size_t sz = m_pHeader->image_size;

	m_pImage = GL_LoadTexture(_T("GUISheet"),pImageSource,sz,false);

	dSheetGlyph_t * pGlyphs = (dSheetGlyph_t*)&m_pBuffer[m_pHeader->glyphs_offset];
	int nGlyphs = m_pHeader->glyphs_count;

	for(int i = 0 ; i < nGlyphs ; i++)
	{
		mSheetGlyph_t * mGlyph = (mSheetGlyph_t*)g_pPlatform->MemoryPools()->Alloc(sz);
		dSheetGlyph_t * dGlyph = &pGlyphs[i];
		
		// 0 0
		// 1 0
		// 1 1
		// 0 1
		// 
		
		mGlyph->c[0].x = (vec_t)dGlyph->x / m_pImage->width;
		mGlyph->c[0].y = 1 - (vec_t)dGlyph->y / m_pImage->height;

		mGlyph->c[1].x = (vec_t)(dGlyph->x + dGlyph->w) / m_pImage->width;
		mGlyph->c[1].y = 1 - (vec_t)dGlyph->y / m_pImage->height;

		mGlyph->c[2].x = (vec_t)(dGlyph->x + dGlyph->w) / m_pImage->width;
		mGlyph->c[2].y = 1 - (vec_t)(dGlyph->y + dGlyph->h) / m_pImage->height;

		mGlyph->c[3].x = (vec_t)(dGlyph->x) / m_pImage->width;
		mGlyph->c[3].y = 1 - (vec_t)(dGlyph->y + dGlyph->h) / m_pImage->height;

		mGlyph->e[0] = dGlyph->w;
		mGlyph->e[1] = dGlyph->h;

		strcpy(mGlyph->szName,dGlyph->name);
		mGlyph->sheet = this;

		m_vGlyphList.push_back(mGlyph);
	}
}

/*	
 *	Desctructor
 **/
XGUI_Sheet::~XGUI_Sheet()
{
	if (!m_pBuffer) return;

	g_pPlatform->MemoryPools()->Free(m_pBuffer);
	m_pImage.reset();

	for(mSheetGlyph_t* g: m_vGlyphList)
	{
		g_pPlatform->MemoryPools()->Free(g);
	}

	m_vGlyphList.clear();
	m_vGlyphList.shrink_to_fit();

}

/*
 *	Bind sheet image
 **/
void XGUI_Sheet::Bind()
{
	glBindTexture(GL_TEXTURE_2D,m_pImage->texID);
}


/*
 *	Find glyph with specified name
 **/
mSheetGlyph_t * XGUI_Sheet::FindGlyph(char * szName)
{
	for(mSheetGlyph_t * g : m_vGlyphList)
	{
		if (!_stricmp(g->szName,szName)) 
			return g;
	}

	 return 0;
}