/*
*	Molotok Editor, 2013 (C) PrinceOfDispersia
*	ImageLib.h - image loader definitions
*
**/

#ifndef IMAGELIB_H
#define IMAGELIB_H

/*
 *	Raw image descriptor
 **/
typedef struct rawImage_s 
{
	byte * pBuffer;
	int iWidth,iHeight;
	int bpp;
}rawImage_t;

/*
 *	GL Texture descriptor
 **/
typedef struct gl_texture_s
{
	TCHAR strTextureName[64];
	GLuint texID;
	GLuint format;
	GLuint internal_format;
	int flags;
	rawImage_t * source;
	int width,height;
}gl_texture_t;

void InitImageLib();
void ShutdownImageLib();

gl_texture_t * Img_Load(TCHAR * szName,byte * pSource,size_t bufferSize,bool keepRaw);
rawImage_t * AllocRawImage(int sz);


#endif