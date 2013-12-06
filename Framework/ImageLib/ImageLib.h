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

typedef std::shared_ptr<gl_texture_t> pgl_texture_t;

pgl_texture_t GL_LoadTexture(TCHAR * szName,byte * pSource,size_t bufferSize,bool keepRaw);
pgl_texture_t GL_LoadTextureFromFS(TCHAR * szName);

rawImage_t * AllocRawImage(int sz);


#endif