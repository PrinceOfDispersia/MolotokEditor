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

			mGlyphInfo_s *	  m_pGlyphs;
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

			void CommitGlyph( mGlyphInfo_t * inf,vec_t px,vec_t py,vec_t w );

			vec_t m_flScaleFactor;

		public:
			void SetTextColor(byte r,byte g,byte b,byte a);

			void SetAtlas(pgl_texture_t pAtlas,ME_Math::Vector2D offset);
			void Calc_TextRect(String & str,xgRect_t * rect);
			size_t Calc_FittedSymbols(String & str,vec_t w);
			

			void Draw(vec_t x,vec_t y,String str)
			{
				Draw(ME_Math::Vector2D(x,y),str);
			}

			void Draw(ME_Math::Vector2D pos,String str);
			void DrawTextWithCarret(vec_t x,vec_t y,TCHAR * strBuffer,size_t carretOffset);
			void DrawMultilineTextInRect(xgRect_t & r,TCHAR * strBuffer);			
			void DrawTextFittedInRect(xgRect_t & targetRect,TCHAR * str);
			void DrawAlignedText(String & str,xgRect_t fitRect,THorizontalAligment horAligment,TVerticalAligment vertAligment);
			

			XGUI_Font(dFontHdr_t * pHeader,size_t sz,vec_t ScaleFactor);
			~XGUI_Font();
		};

	}
}

#endif