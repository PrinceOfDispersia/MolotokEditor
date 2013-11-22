/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	GUI_Sheet.h - image sheet definitions
 *
 **/

#ifndef GUI_SHEET_H
#define GUI_SHEET_H

#pragma pack(push,1)

typedef struct dSheetHDR_s
{
	char magic[5]; // SHEET
	int image_offset;
	int image_size;

	int glyphs_offset;
	int glyphs_count;
}dSheetHDR_t;

typedef struct dSheetGlyph_s 
{
	char name[64];
	int x,y;
	int w,h;
}dSheetGlyph_t;

#pragma pack(pop)

#endif

