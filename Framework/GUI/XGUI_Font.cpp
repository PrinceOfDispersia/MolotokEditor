/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	XGUI_Font.cpp - font class
 *
 **/

#include <Platform/Common/ApplicationCommon.h>

using namespace ME_XGUI;
using namespace ME_OpenGLBackend;

/*
 *	Calculates area used when printing string
 **/
void XGUI_Font::Calc_TextRect(String & str,xgRect_t * rect)
{
	memset(rect,0,sizeof(xgRect_t));

	size_t i = 0;

	vec_t w,h,o;

	w = 0;
	h = 0;
	o = 0;


	const TCHAR * ptr = str.c_str();
	size_t sz = str.Length();

	for(size_t i = 0 ; i < sz ; i++)
	{

		TCHAR sym = ptr[i];

		int idx = -1;
		byte page = sym >> 8;

		for(int j = 0 ; j < m_nMaps ; j++)
		{
			if (m_pSymbolMaps[j] == page)
			{
				idx = j;
				break;
			}
		}

		if (idx == -1) 
			continue;

		byte b = sym & 0x00FF;
		idx = m_pCodePages[(idx * 256) + b];



		dGlyphInfo_t * inf = &m_pGlyphs[idx];
		
		w+=(inf->orig_w);

		if (h < (inf->height))
		{
			h = inf->height; 
			o = inf->yoffs;
		}


	}

	rect->pos = ME_Math::Vector2D(0,0);
	rect->ext = ME_Math::Vector2D(w,h);
}

/*
 *	render string
 **/
void XGUI_Font::Draw(ME_Math::Vector2D pos,String str)
{
// 		glDisable(GL_TEXTURE_2D);
// 		xgRect_t r;
// 		Calc_TextRect(str,&r);
// 
// 		glColor4f(0,.5,0,1);
// 		glBegin(GL_QUADS);
// 		glVertex2d(pos.x,			pos.y + r.pos.y);
// 		glVertex2d(pos.x + r.ext.x,	pos.y + r.pos.y);
// 		glVertex2d(pos.x + r.ext.x,	pos.y + r.ext.y + r.pos.y);
// 		glVertex2d(pos.x,			pos.y + r.ext.y + r.pos.y);
// 		glEnd();
// 

		GL_EnableState(GLS_TEXTURE_2D);
		GL_EnableState(GLS_BLEND);

		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

 		glBindTexture(GL_TEXTURE_2D,m_pFontImage->texID);
		

		vec_t w = 0;
		vec_t h = 0;
						
		glBegin(GL_QUADS);
		
		size_t i = 0;

		const TCHAR * ptr = str.c_str();
		size_t sz = str.Length();

		for(size_t i = 0 ; i < sz ; i++)
		{

			TCHAR sym = ptr[i];

			int idx = -1;
			byte page = sym >> 8;

			for(int j = 0 ; j < m_nMaps ; j++)
			{
				if (m_pSymbolMaps[j] == page)
				{
					idx = j;
					break;
				}
			}

			if (idx == -1) continue;

			byte b = sym & 0x00FF;
			idx = m_pCodePages[(idx * 256) + b];
			
			

			dGlyphInfo_t * inf = &m_pGlyphs[idx];
				
		
			float c[4];

			float x,y;

			x =  (float)inf->xpos;
			y =  (float)inf->ypos;

			c[0] = x / m_pFontImage->width;
			c[1] = 1 - (y / m_pFontImage->height);

			c[2] = (x + inf->width) / m_pFontImage->width;
			c[3] = 1 - ( (y + inf->height) / m_pFontImage->height);

			

			
			
			vec_t xofs = inf->xoffs;
			vec_t yofs = inf->yoffs; 
			
			vec_t x1 = w + xofs + pos.x;
			vec_t x2 = x1 + (inf->width);

			vec_t y1 = yofs + pos.y;
			vec_t y2 = y1 + (inf->height);

			

 			glTexCoord2f(c[0],c[1]);
 			glVertex2d(x1,y1);
 			glTexCoord2f(c[2],c[1]);
 			glVertex2d(x2,y1);
 			glTexCoord2f(c[2],c[3]);
 			glVertex2d(x2,y2);
 			glTexCoord2f(c[0],c[3]);
 			glVertex2d(x1,y2);
			
			w+=(inf->orig_w);
						
			
		}
		glEnd();
				
		GL_DisableState(GLS_BLEND);

}

/*
 *	Constructor
 **/
XGUI_Font::XGUI_Font(dFontHdr_t * pHeader,size_t sz)
{
		m_pHeader = (dFontHdr_t*)g_pPlatform->MemoryPools()->Alloc(sz);

		if (memcmp(pHeader,"F0nT",4))
		{
			g_pPlatform->SystemLog()->Log(_T("XGUI_Font(): invalid buffer magic!\n"));
			memset(m_pHeader,0,sz);
		}
		else memcpy(m_pHeader,pHeader,sz);

		byte * pBits = (byte*)m_pHeader;

		m_pKerningPairs = (dKerningPairs_t *)&pBits[pHeader->lumps[LUMP_FNT_KERNING_PAIRS].start];
		m_nKerningPairs = pHeader->lumps[LUMP_FNT_KERNING_PAIRS].length / sizeof(dKerningPairs_t);

		m_pGlyphs = (dGlyphInfo_t *)&pBits[pHeader->lumps[LUMP_FNT_GLYPHS].start];
		m_nGlyphs = pHeader->lumps[LUMP_FNT_GLYPHS].length / sizeof(dGlyphInfo_t);

		byte * pFontImage = &pBits[pHeader->lumps[LUMP_FNT_IMAGE].start];
		size_t imgSize = pHeader->lumps[LUMP_FNT_IMAGE].length;

		m_pFontImage = GL_LoadTexture(_T("FontImage"),pFontImage,imgSize,false);

		m_pSymbolMaps = &pBits[pHeader->lumps[LUMP_FNT_MAPS].start];
		m_nMaps = pHeader->lumps[LUMP_FNT_MAPS].length / sizeof(byte);

		m_pCodePages = (short*)&pBits[pHeader->lumps[LUMP_FNT_PAGES].start];
}

/*
 *	Destructor
 **/
XGUI_Font::~XGUI_Font()
{
	g_pPlatform->MemoryPools()->Free(m_pHeader);
	
	m_pFontImage.reset();
}

