/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	XGUI_Font.cpp - font class
 *
 **/

#include <Platform/Common/ApplicationCommon.h>

using namespace ME_XGUI;
using namespace ME_OpenGLBackend;

extern XGUI_Tesselator * g_pTesselator;

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

		if (sym == _T('\t'))
		{
			w+=30;
			continue;
		}

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



		auto * inf = &m_pGlyphs[idx];
		
		w+=(inf->orig_w);

		if (h < (inf->orig_h))
		{
			h = inf->orig_h; 
			o = inf->yoffs;
		}


	}

	rect->pos = ME_Math::Vector2D(0,o);
	rect->ext = ME_Math::Vector2D(w,h);
}

/*
 *	render string
 **/
void XGUI_Font::Draw(ME_Math::Vector2D pos,String str)
{
		vec_t w = 0;
		vec_t h = 0;
		
		size_t i = 0;

		const TCHAR * ptr = str.c_str();
		size_t sz = str.Length();

		if (m_bShouldApplyColoring)
		{
			g_pTesselator->DefaultColor(m_TextColor);
		}

		for(size_t i = 0 ; i < sz ; i++)
		{

			TCHAR sym = ptr[i];

			if (sym == _T('\t'))
			{
				w+=30;
				continue;
			}

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
			
			auto * inf = &m_pGlyphs[idx];
			CommitGlyph(inf,pos.x,pos.y,w);
			
			w+=(inf->orig_w);
						
			
		}

		if (m_bShouldApplyColoring)
		{
			g_pTesselator->ResetDefaultColor();
			m_bShouldApplyColoring = false;
		}
		
}

/*
 *	Constructor
 **/
XGUI_Font::XGUI_Font(dFontHdr_t * pHeader,size_t sz,vec_t ScaleFactor)
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

		dGlyphInfo_t * d_pGlyphs = (dGlyphInfo_t *)&pBits[pHeader->lumps[LUMP_FNT_GLYPHS].start];
		m_nGlyphs = pHeader->lumps[LUMP_FNT_GLYPHS].length / sizeof(dGlyphInfo_t);

		m_pGlyphs = (mGlyphInfo_t*)Mem_Alloc(sizeof(mGlyphInfo_t) * m_nGlyphs);

		byte * pFontImage = &pBits[pHeader->lumps[LUMP_FNT_IMAGE].start];
		size_t imgSize = pHeader->lumps[LUMP_FNT_IMAGE].length;

		//m_pFontImage = GL_LoadTexture(_T("FontImage"),pFontImage,imgSize,false);

		m_pSymbolMaps = &pBits[pHeader->lumps[LUMP_FNT_MAPS].start];
		m_nMaps = pHeader->lumps[LUMP_FNT_MAPS].length / sizeof(byte);

		m_pCodePages = (short*)&pBits[pHeader->lumps[LUMP_FNT_PAGES].start];

		m_bShouldApplyColoring = false;
		 
		m_flScaleFactor = ScaleFactor;


}

/*
 *	Destructor
 **/
XGUI_Font::~XGUI_Font()
{
	g_pPlatform->MemoryPools()->Free(m_pHeader);
	
	Mem_Free(m_pGlyphs);
	m_pFontImage.reset();
}

void XGUI_Font::SetAtlas(pgl_texture_t pAtlas,ME_Math::Vector2D offset)
{
	m_pFontImage = pAtlas;

	dGlyphInfo_t * d_pGlyphs = (dGlyphInfo_t *)&((byte*)m_pHeader)[m_pHeader->lumps[LUMP_FNT_GLYPHS].start];

	for(size_t i = 0 ; i < m_nGlyphs ; i++)
	{
		m_pGlyphs[i].xpos = d_pGlyphs[i].xpos;
		m_pGlyphs[i].ypos = d_pGlyphs[i].ypos;
		m_pGlyphs[i].width = d_pGlyphs[i].width;
		m_pGlyphs[i].height = d_pGlyphs[i].height;
		m_pGlyphs[i].xoffs = d_pGlyphs[i].xoffs;
		m_pGlyphs[i].yoffs = d_pGlyphs[i].yoffs;
		m_pGlyphs[i].orig_w = d_pGlyphs[i].orig_w;
		m_pGlyphs[i].orig_h = d_pGlyphs[i].orig_h;


	}

	for(size_t i = 0 ; i < m_nGlyphs ; i++)
	{
		m_pGlyphs[i].xpos += (short)(-m_vAtlasOffset.x + offset.x);
		m_pGlyphs[i].ypos += (short)(-m_vAtlasOffset.y + offset.y);

		float 	x =  (float)(m_pGlyphs[i].xpos);
		float	y =  (float)(m_pGlyphs[i].ypos);

		float * c = m_pGlyphs[i].c;

		c[0] = x / m_pFontImage->width;
		c[1] = 1 - (y / m_pFontImage->height);

		c[2] = (x + m_pGlyphs[i].width) / m_pFontImage->width;
		c[3] = 1 - ( (y + m_pGlyphs[i].height) / m_pFontImage->height);

		m_pGlyphs[i].width *= m_flScaleFactor;
		m_pGlyphs[i].height *= m_flScaleFactor;
		m_pGlyphs[i].orig_w *= m_flScaleFactor;
		m_pGlyphs[i].orig_h *= m_flScaleFactor;
		m_pGlyphs[i].xoffs *= m_flScaleFactor;
		m_pGlyphs[i].yoffs *= m_flScaleFactor;

	}

	m_vAtlasOffset = offset;





}

/*
 *	Draws text with carret in specified position
 **/
void XGUI_Font::DrawTextWithCarret(vec_t px,vec_t py,TCHAR * ptr,size_t carretOffset)
{
	vec_t w = 0;
	vec_t h = 0;

	size_t i = 0;
	
	if (m_bShouldApplyColoring)
	{
		m_bShouldApplyColoring = false;
		g_pTesselator->DefaultColor(m_TextColor);
	}

	while(*ptr)
	{
		if (i == (carretOffset))
		{
			color32_t c = {255,255,255,255};
			g_pTesselator->DefaultColor(c);

			xgRect_t r;
			r.pos.x = w + px;
			r.pos.y = py;

			r.ext.x = 1;
			r.ext.y = 16;

			XGUI_DrawSheetGlyph(sprWhite,r);

			g_pTesselator->ResetDefaultColor();
			//w+=4;
		}

		TCHAR sym = *ptr;

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

		auto * inf = &m_pGlyphs[idx];
		CommitGlyph(&m_pGlyphs[idx],px,py,w);
		
		if (sym == _T('\t'))
		{
			w+=30;
		}
		else 
			w+=(inf->orig_w);
		ptr++;
		i++;
	}

	if (i == (carretOffset))
	{
		color32_t c = {255,255,255,255};
		g_pTesselator->DefaultColor(c);

		xgRect_t r;
		r.pos.x = w + px;
		r.pos.y = py;

		r.ext.x = 1;
		r.ext.y = 16;

		XGUI_DrawSheetGlyph(sprWhite,r);

		g_pTesselator->ResetDefaultColor();
		//w+=4;
	}

	if (m_bShouldApplyColoring)
	{
		g_pTesselator->ResetDefaultColor();
		m_bShouldApplyColoring = false;
	}
}

void XGUI_Font::DrawMultilineTextInRect(xgRect_t & r,TCHAR * strBuffer)
{
	vec_t w = 0;
	vec_t h = 0;

	size_t i = 0;
	TCHAR * ptr = strBuffer;

	vec_t px = r.pos.x;
	vec_t py = r.pos.y;

	vec_t px2 = r.pos.x + r.ext.x;
	vec_t py2 = r.pos.y + r.ext.y;

	size_t charsInLine = 0;

	vec_t dh = 0;
	
	g_pTesselator->Flush();

	GL_SetScissor(r.pos.x,r.pos.y,r.ext.x,r.ext.y);
	GL_EnableScissorTest();
	
	if (m_bShouldApplyColoring)
	{
		m_bShouldApplyColoring = false;
		g_pTesselator->DefaultColor(m_TextColor);
	}

	while(*ptr)
	{
		if (*ptr == '\n')
		{
			w = 0;
			h += dh + 1;
			charsInLine = 0;
			ptr++;
			continue;
		}

		if (py + h > py2) break;

		TCHAR sym = *ptr;

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
		
		auto * inf = &m_pGlyphs[idx];

		CommitGlyph(inf,px,py,w);
		
		if (sym == _T('\t'))
		{
			w+=30;
		}
		else 
			w+=(inf->orig_w);
		ptr++;
		i++;
		charsInLine++;
	}

	if (m_bShouldApplyColoring)
	{
		g_pTesselator->ResetDefaultColor();
		m_bShouldApplyColoring = false;
	}

	g_pTesselator->Flush();
	GL_DisableScissorTest();

}

void XGUI_Font::SetTextColor(byte r,byte g,byte b,byte a)
{
	m_TextColor.r = r;
	m_TextColor.g = g;
	m_TextColor.b = b;
	m_TextColor.a = a;

	m_bShouldApplyColoring = true;
}

void XGUI_Font::DrawTextFittedInRect( xgRect_t & r,TCHAR * ptr )
{
	vec_t w = 0;
	vec_t h = 0;

	size_t i = 0;

	vec_t px = r.pos.x;
	vec_t py = r.pos.y;

	if (m_bShouldApplyColoring)
	{
		m_bShouldApplyColoring = false;
		g_pTesselator->DefaultColor(m_TextColor);
	}

	while(*ptr)
	{
		TCHAR sym = *ptr;

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


		auto * inf = &m_pGlyphs[idx];

		CommitGlyph(&m_pGlyphs[idx],px,py,w);

		if (sym == _T('\t'))
		{
			w+=30;
		}
		else 
			w+=(inf->orig_w);
		ptr++;
		i++;
	}


	if (m_bShouldApplyColoring)
	{
		g_pTesselator->ResetDefaultColor();
		m_bShouldApplyColoring = false;
	}	
}

/*
 *	Calculates count of symbols fitted to that string
 **/
size_t XGUI_Font::Calc_FittedSymbols( String & str,vec_t _w)
{
	vec_t w = 0;
	size_t ret = 0;

	const TCHAR * ptr = str.c_str();
	size_t sz = str.Length();

	for(size_t i = 0 ; i < sz ; i++)
	{
		TCHAR sym = ptr[i];

		if (sym == _T('\t'))
		{
			w+=30;
			continue;
		}

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
				
		auto * inf = &m_pGlyphs[idx];

		w+=(inf->orig_w);
		
		if (w > _w)
			return i;
	}

	return sz;

}

void XGUI_Font::DrawAlignedText(String & str, xgRect_t fitRect,THorizontalAligment horAligment,TVerticalAligment vertAligment )
{
	xgRect_t textRect;
	Calc_TextRect(str,&textRect);

	// fixme
	vec_t pad = 0.1;

	vec_t xt,yt;

	switch(horAligment)
	{
	case THorizontalAligment::alhLeft:
			xt = fitRect.pos.x +  pad;
		break;
	case THorizontalAligment::alhCenter:
			xt = fitRect.pos.x +  fitRect.ext.x / 2 - textRect.ext.x / 2;
		break;
	case THorizontalAligment::alhRight:
			xt = fitRect.Right() - textRect.ext.x - pad;
		break;
	}
	
	vec_t th = textRect.ext.y + pad;

	switch(vertAligment)
	{
	case TVerticalAligment::alvTop:
		yt = fitRect.pos.y +  pad;
		break;
	case TVerticalAligment::alvCenter:
		yt = fitRect.pos.y + (fitRect.ext.y - th) / 2;
		break;
	case TVerticalAligment::alvBottom:
		yt = fitRect.Bottom() - th - pad;
		break;
	}

	if (0) // debug
	{
		xgRect_t rrr;
		rrr.pos.x = xt;
		rrr.pos.y = yt;
		rrr.ext = textRect.ext;

		color32_t c;
		c.r = 255; c.g = 0; c.b = 0;
		c.a = 255;
		g_pTesselator->DefaultColor(c);
		XGUI_DrawSheetGlyph(sprWhite,rrr);
	}
	

	Draw(xt,yt,str);
	
	
}

void XGUI_Font::CommitGlyph( mGlyphInfo_s * inf,vec_t px,vec_t py,vec_t w )
{
	float* c = inf->c;
	float x,y;

	x =  (float)(inf->xpos);
	y =  (float)(inf->ypos);

	

	vec_t xofs = inf->xoffs;
	vec_t yofs = inf->yoffs; 

	vec_t x1 = w + xofs + px;
	vec_t x2 = x1 + (inf->width);

	vec_t y1 = yofs + py;
	vec_t y2 = y1 + (inf->height);

	g_pTesselator->Coord2(c[0],c[1]);
	g_pTesselator->Vertex2(x1,y1);
	g_pTesselator->Coord2(c[2],c[1]);
	g_pTesselator->Vertex2(x2,y1);
	g_pTesselator->Coord2(c[2],c[3]);
	g_pTesselator->Vertex2(x2,y2);
	g_pTesselator->Coord2(c[0],c[3]);
	g_pTesselator->Vertex2(x1,y2);
}
