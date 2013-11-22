/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	XGUI_Font.h - font class
 *
 **/

#ifndef XGUI_FONT_H
#define XGUI_FONT_H

#include "Font_FT2.h"

namespace ME_Framework
{
	namespace ME_XGUI
	{
		class XGUI_Font
		{
			dFontHdr_t * m_pHeader;

			dGlyphInfo_t *	  m_pGlyphs;
			dKerningPairs_t * m_pKerningPairs;

			size_t m_nGlyphs;
			size_t m_nKerningPairs;

			gl_texture_t * m_pFontImage;

			byte * m_pSymbolMaps;
			int m_nMaps;

			short * m_pCodePages;
			
		public:
			void Calc_TextRect(String & str,xgRect_t * rect);
			void Draw(ME_Math::Vector2D pos,String str);

			XGUI_Font(dFontHdr_t * pHeader,size_t sz);
			~XGUI_Font();
		};

	}
}

#endif