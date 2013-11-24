/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	XGUI_Sheet.h - image sheet class
 *
 **/

#ifndef XGUI_SHEET_H
#define XGUI_SHEET_H

#include "GUISheet.h"

namespace ME_Framework
{
	namespace ME_XGUI
	{

		class XGUI_Sheet;

		typedef struct mSheetGlyph_s
		{
			XGUI_Sheet * sheet;
			char szName[64];

			ME_Math::Vector2D c[4];
			int e[2];
		}mSheetGlyph_t;

		class XGUI_Sheet
		{
			byte * m_pBuffer;
			size_t m_buffSize;

			dSheetHDR_t * m_pHeader;
			
			std::vector<mSheetGlyph_t*> m_vGlyphList;
			gl_texture_t * m_pImage;
		public:
			XGUI_Sheet(byte * pBuffer,size_t buffSize);
			~XGUI_Sheet();

			void Bind();

			mSheetGlyph_t * FindGlyph(char * szName);
		};
	}
}


#endif