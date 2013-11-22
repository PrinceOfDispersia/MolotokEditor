// FontCompiler.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"

typedef unsigned char byte;

#include "..\..\Framework\GUI\Font_FT2.h"


// Common types
typedef unsigned char byte;

/*
 *	Loads file to buffer
 **/
byte * LoadFile(TCHAR * path,size_t * size)
{
	FILE * fp = 0;
	_tfopen_s(&fp,path,_T("rb"));
	if (!fp) return 0;

	fseek(fp,0,SEEK_END);
	*size = ftell(fp);
	fseek(fp,0,SEEK_SET);

	byte * pBuffer = (byte*)malloc(*size);

	fread(pBuffer,*size,1,fp);
	fclose(fp);

	return pBuffer;
}

char * ReadLine(byte * pBuffer,size_t & offset,size_t buffsize)
{
	static char buffer[32768];

	char * pString = (char*)&pBuffer[offset];
	char * a = buffer;

	while(*pString && *pString!='\n' && offset < buffsize)
	{
		offset++;
		*a++ = *pString++;
	}

	offset++;

	*a = 0;

	return buffer;
}

/*
 *	Prints help
 **/
void Help()
{
	printf("==========================================\n");
	printf("UBFG Font compiler\n");
	printf("==========================================\n");
	printf("usage: FontCompiler.exe font.fnt font.ft2\n");
	while (!_kbhit());
	
}



int _tmain(int argc, _TCHAR* argv[])
{
	// Check for arguments
	if (argc != 3)
	{
		Help();
		return 1;
	}

	size_t sz;
	byte * pSource = LoadFile(argv[1],&sz);
	size_t offset  = 0;
	
	if (!pSource)
	{
		printf("File not found!\n");
		while (!_kbhit());
		return 1;
	}
	
	char * line = ReadLine(pSource,offset,sz);
	char szTextureName[256];

	sscanf(line,"textures: %s",&szTextureName);

	std::vector<dGlyphInfo_t> vGlyphs;
	std::vector<dKerningPairs_t> vKernings;

	line = ReadLine(pSource,offset,sz);

	
	while((offset < sz) && (line = ReadLine(pSource,offset,sz)))
	{
		if (!_strnicmp(line,"kerning",strlen("kerning")))	
			break;
		//32	0	0	0	0	6	28	5	28

		int arr[9];

		sscanf(line,"%d %d %d %d %d %d %d %d %d",&arr[0],&arr[1],&arr[2],&arr[3],&arr[4],&arr[5],&arr[6],&arr[7],&arr[8]);

		dGlyphInfo_t inf;
		
		inf.sym = arr[0];
		inf.xpos = arr[1];
		inf.ypos = arr[2];
		inf.width = arr[3];
		inf.height = arr[4];
		inf.xoffs = arr[5];
		inf.yoffs = arr[6];
		inf.orig_w = arr[7];
		inf.orig_h = arr[8];

		vGlyphs.push_back(inf);
	}
	

	while((offset < sz) && (line = ReadLine(pSource,offset,sz)))
	{
		int arr[2];
		float f;
		sscanf(line,"%d %d %f",&arr[0],&arr[1],&f);		
		dKerningPairs_t p;
		p.s1 = arr[0];
		p.s2 = arr[1];
		p.value = f;

		vKernings.push_back(p);
	}
	



	FILE * fp = _tfopen(argv[2],_T("wb"));

	dFontHdr_t hdr;
	memset(&hdr,0,sizeof(hdr));

	hdr.magic[0] = 'F';
	hdr.magic[1] = '0';
	hdr.magic[2] = 'n';
	hdr.magic[3] = 'T';

	fwrite(&hdr,sizeof(hdr),1,fp);

	// write glyphs

	hdr.lumps[LUMP_FNT_GLYPHS].start = ftell(fp);
	hdr.lumps[LUMP_FNT_GLYPHS].length = vGlyphs.size() * sizeof(dGlyphInfo_t);

	fwrite(&vGlyphs[0],sizeof(dGlyphInfo_t),vGlyphs.size(),fp);
	
	// write kernings
	hdr.lumps[LUMP_FNT_KERNING_PAIRS].start = ftell(fp);
	hdr.lumps[LUMP_FNT_KERNING_PAIRS].length = vKernings.size() * sizeof(dKerningPairs_t);

	fwrite(&vKernings[0],sizeof(dKerningPairs_t),vKernings.size(),fp);

	// write image
		FILE * fImage = fopen(szTextureName,"rb");

		if (!fImage)
		{
			printf("Image not found!");
			while (!_kbhit());
			return 1;
		}

		fseek(fImage,0,SEEK_END);
		size_t szImage = ftell(fImage);
		fseek(fImage,0,SEEK_SET);

		byte * pImage = (byte*)malloc(szImage);
		fread(pImage,szImage,1,fImage);
		fclose(fImage);
	// 

	hdr.lumps[LUMP_FNT_IMAGE].start = ftell(fp);
	hdr.lumps[LUMP_FNT_IMAGE].length = szImage;

	fwrite(pImage,szImage,1,fp);

	// Codepages
	byte pages[256];
	unsigned short codepages[256][256];
	memset(&codepages,0,sizeof(codepages));
	int used_pages = 0;

	for(int i = 0 ; i < vGlyphs.size() ; i++)
	{
		short s = vGlyphs[i].sym;
		byte page = (s >> 8);

		bool bAdd = true;
		int page_index = -1;

		int j;
		for(j = 0 ; j < used_pages ; j++)
		{
			if (pages[j] == page)
			{
				page_index = j;
				bAdd = false;
				break;
			}
		}

		if (bAdd)
		{
			page_index = used_pages;
			pages[used_pages++] = page;			
			
		}

		byte b = s & 0x00FF;

		codepages[page_index][b] = i;
		
	}

	
	hdr.lumps[LUMP_FNT_PAGES].start = ftell(fp);
	hdr.lumps[LUMP_FNT_PAGES].length = used_pages * 256 * sizeof(short);

	fwrite(codepages,256 * sizeof(short),used_pages,fp);

	hdr.lumps[LUMP_FNT_MAPS].start = ftell(fp);
	hdr.lumps[LUMP_FNT_MAPS].length = used_pages * sizeof(byte);

	fwrite(pages,sizeof(byte),used_pages,fp);
	

	fseek(fp,0,SEEK_SET);
	fwrite(&hdr,sizeof(hdr),1,fp);
	fclose(fp);
	


	vGlyphs.clear();
	vKernings.clear();
	vGlyphs.shrink_to_fit();
	vKernings.shrink_to_fit();

	free(pSource);
	free(pImage);

	return 0;
}

