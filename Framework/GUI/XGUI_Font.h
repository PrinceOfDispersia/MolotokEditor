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

			pgl_texture_t m_pFontImage;

			byte * m_pSymbolMaps;
			int m_nMaps;

			short * m_pCodePages;
			ME_Math::Vector2D m_vAtlasOffset;

			bool m_bShouldApplyColoring;
			color32_t m_TextColor;

		public:
			void SetTextColor(byte r,byte g,byte b,byte a);

			void SetAtlas(pgl_texture_t pAtlas,ME_Math::Vector2D offset);
			void Calc_TextRect(String & str,xgRect_t * rect);
			void Draw(ME_Math::Vector2D pos,String str);

			void Draw(vec_t x,vec_t y,String str)
			{
				Draw(ME_Math::Vector2D(x,y),str);
			}

			void DrawTextWithCarret(vec_t x,vec_t y,TCHAR * strBuffer,size_t carretOffset);
			void DrawMultilineTextInRect(xgRect_t & r,TCHAR * strBuffer);

			XGUI_Font(dFontHdr_t * pHeader,size_t sz);
			~XGUI_Font();
		};

	}
}

#endif