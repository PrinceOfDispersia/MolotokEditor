/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	Font_FT2.h - FT2 font definitions
 *
 **/
#ifndef FONT_FT2_H
#define FONT_FT2_H

#pragma pack(push,1)
typedef struct dGlyphInfo_s
{
	unsigned short sym;
	short xpos;
	short ypos;
	short width;
	short height;
	short xoffs;
	short yoffs;
	short orig_w;
	short orig_h;
}dGlyphInfo_t;

typedef struct mGlyphInfo_s 
{
	float xpos;
	float ypos;
	float width;
	float height;
	float xoffs;
	float yoffs;
	float orig_w;
	float orig_h;
	
	float c[4];
}mGlyphInfo_t;

typedef struct dKerningPairs_s
{
	unsigned short s1;
	unsigned short s2;

	float value;
}dKerningPairs_t;

typedef struct lump_s
{
	int start,length; // Use ints on disk structures, since size_t varies from build to build
}lump_t;

enum eFT2Lumps 
{
	LUMP_FNT_GLYPHS = 0,
	LUMP_FNT_KERNING_PAIRS,
	LUMP_FNT_IMAGE,
	LUMP_FNT_MAPS,
	LUMP_FNT_PAGES,
	LUMP_FNT_MAX
};

typedef struct dFontHdr_s
{
	char magic[4]; // FONT	
	lump_t lumps[LUMP_FNT_MAX];
}dFontHdr_t;



#pragma pack(pop)

#endif