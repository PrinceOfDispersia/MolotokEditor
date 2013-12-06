// GUISheetCompiler.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"


typedef unsigned char byte;

#include "..\..\Framework\GUI\GUISheet.h"

#pragma warning(disable: 4267)

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
	if (offset > buffsize)
		return 0;

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
	printf("GUI sheet compiler\n");
	printf("==========================================\n");
	printf("usage: GUISheetCompiler.exe skin.txt skin.png skin.bin\n");
	while (!_kbhit());
	
}


int _tmain(int argc, _TCHAR* argv[])
{
	if (argc != 4 )
	{
		Help();
		return 1;
	}

	// Load skin def
	size_t szSkinDef;
	byte * pSkinDef = LoadFile(argv[1],&szSkinDef);

	if (!szSkinDef || !pSkinDef)
	{
		_tprintf(_T("ERROR: file %s not found!\n"),argv[1]);
		while(!_kbhit());
		return 1;
	}

	// Load image
// 	size_t szImage;
// 	byte * pImage = LoadFile(argv[2],&szImage);
// 
// 	if (!szImage || !pImage)
// 	{
// 		_tprintf(_T("ERROR: file %s not found!\n"),argv[2]);
// 
// 		free(pImage);
// 
// 		while(!_kbhit());
// 		return 1;
// 	}

	FILE * fp = _tfopen(argv[3],_T("wb"));

	if (!fp)
	{
		_tprintf(_T("ERROR: can't open file %s for writing!\n"),argv[3]);
	
		free(pSkinDef);
		//free(pImage);

		while(!_kbhit());
		return 1;
	}

	dSheetHDR_t hdr;
	memset(&hdr,0,sizeof(hdr));

	fwrite(&hdr,sizeof(hdr),1,fp);

	size_t offset = 0;
	
	hdr.glyphs_offset = ftell(fp);

	while(char * line = ReadLine(pSkinDef,offset,szSkinDef))
	{
		dSheetGlyph_t g;
		memset(&g,0,sizeof(g));
		sscanf(line,"%d,%d,%d,%d %s",&g.x,&g.y,&g.w,&g.h,&g.name);
		fwrite(&g,sizeof(g),1,fp);
		hdr.glyphs_count++;
	}

	hdr.image_offset = 0;//ftell(fp);
	hdr.image_size = 0;//szImage;

	//fwrite(pImage,szImage,1,fp);

	fseek(fp,0,SEEK_SET);

	strncpy(hdr.magic,"SHEET",5);

	fwrite(&hdr,sizeof(hdr),1,fp);
	fclose(fp);

	//free(pImage);
	free(pSkinDef);

	return 0;
}


