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
		typedef struct mSheetGlyph_s
		{
			unsigned int hash;

			vec_t c[4];
			int e[2];
		}mSheetGlyph_t;

		class XGUI_Sheet
		{
			byte * m_pBuffer;
			size_t m_buffSize;

			dSheetHDR_t * m_pHeader;
			dSheetGlyph_t * m_pSheetGlyphs;

			std::vector<mSheetGlyph_t>;

		public:
			XGUI_Sheet(byte * pBuffer,size_t buffSize);
			~XGUI_Sheet();
		};
	}
}


#endif