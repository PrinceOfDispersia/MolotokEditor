/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	XGUI_Font.cpp - font class
 *
 **/

#include <Platform/Common/ApplicationCommon.h>

using namespace ME_XGUI;

/*
 *	Calculates area used when printing string
 **/
void XGUI_Font::Calc_TextRect(String & str,xgRect_t * rect)
{
	memset(rect,0,sizeof(xgRect_t));

	// TODO: implement me!
}

/*
 *	render string
 **/
void XGUI_Font::Draw(ME_Math::Vector2D pos,String str)
{
		// TODO: implement hash maps
		// Cleanup, and move code to Calc_TextRect;

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,m_pFontImage->texID);
		glTranslatef(100,100,0);

		glPushMatrix();

		float w = 0;
		float h = 0;
		float yoff = 0;
		for(size_t i = 0 ; i < str.Length() ; i++)
		{

			size_t j = 0;
			for(j = 0 ; j < m_nGlyphs ; j++)
			{
				if (m_pGlyphs[j].sym == str[i])
					break;
			}

			dGlyphInfo_t * inf = &m_pGlyphs[j];

			float c[4];

			float x,y;

			x =  (float)inf->xpos;
			y =  (float)inf->ypos;

			c[0] = x / m_pFontImage->width;
			c[1] = 1 - (y / m_pFontImage->height);

			c[2] = (x + inf->width) / m_pFontImage->width;
			c[3] = 1 - ( (y + inf->height) / m_pFontImage->height);

			

			const float scale = 2.0f+ (sin(g_pPlatform->TimeElapsed()) + 1) * 10;
			
			glTranslatef(inf->xoffs * scale,inf->yoffs * scale,0);
			

			glColor4f(1,1,1,1);
			glBegin(GL_QUADS);
			glTexCoord2f(c[0],c[1]);
			glVertex2f(0,0);
			glTexCoord2f(c[2],c[1]);
			glVertex2f(inf->width * scale,0);
			glTexCoord2f(c[2],c[3]);
			glVertex2f(inf->width * scale,inf->height * scale);
			glTexCoord2f(c[0],c[3]);
			glVertex2f(0,inf->height * scale);
			glEnd();

			glTranslatef(-inf->xoffs * scale,-inf->yoffs * scale,0);
			
			glTranslatef((inf->orig_w) * scale,0,0);
			w+=(inf->orig_w) * scale;
			if (inf->height*scale > h)
			{
				h = inf->height*scale;
				yoff = inf->yoffs*scale;
			}
			
			

		}
		
		glPopMatrix();

		glDisable(GL_TEXTURE_2D);
		glColor4f(0.7f,0,0.7f,1);
		glBegin(GL_QUADS);
		glVertex2f(0,yoff);
		glVertex2f(w,yoff);
		glVertex2f(w,h + yoff);
		glVertex2f(0,h + yoff);
		glEnd();

		glPushMatrix();

	
		glEnable(GL_TEXTURE_2D);

		for(size_t i = 0 ; i < str.Length() ; i++)
		{

			size_t j = 0;
			for(j = 0 ; j < m_nGlyphs ; j++)
			{
				if (m_pGlyphs[j].sym == str[i])
					break;
			}

			dGlyphInfo_t * inf = &m_pGlyphs[j];

			float c[4];

			float x,y;

			x =  (float)inf->xpos;
			y =  (float)inf->ypos;

			c[0] = x / m_pFontImage->width;
			c[1] = 1 - (y / m_pFontImage->height);

			c[2] = (x + inf->width) / m_pFontImage->width;
			c[3] = 1 - ( (y + inf->height) / m_pFontImage->height);



			const float scale = 2.0f + (sin(g_pPlatform->TimeElapsed()) + 1) * 10;

			glTranslatef(inf->xoffs * scale,inf->yoffs * scale,0);


			glColor4f(1,1,1,1);
			glBegin(GL_QUADS);
			glTexCoord2f(c[0],c[1]);
			glVertex2f(0,0);
			glTexCoord2f(c[2],c[1]);
			glVertex2f(inf->width * scale,0);
			glTexCoord2f(c[2],c[3]);
			glVertex2f(inf->width * scale,inf->height * scale);
			glTexCoord2f(c[0],c[3]);
			glVertex2f(0,inf->height * scale);
			glEnd();

			glTranslatef(-inf->xoffs * scale,-inf->yoffs * scale,0);

			glTranslatef((inf->orig_w) * scale,0,0);
			w+=inf->orig_w * scale;
			

		}

		glPopMatrix();

		

		


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
}

/*
 *	Destructor
 **/
XGUI_Font::~XGUI_Font()
{
	g_pPlatform->MemoryPools()->Free(m_pHeader);
	GL_FreeTexture(m_pFontImage);
}

