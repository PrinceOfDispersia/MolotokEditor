/*
*	Molotok Editor, 2013 (C) PrinceOfDispersia
*	ImageLib.cpp - image loader definitions
*
**/

#include <Platform/Common/ApplicationCommon.h>
#include "ImageLib.h"
#include "Image_Png.h"

std::vector<gl_texture_t*> g_TexturesPool;

enum eImageTypes
{
	IMG_UNK = 0,
	IMG_PNG 
};

/*
 *	Loader params
 **/
GLuint g_loaderWrap_s;
GLuint g_loaderWrap_t;
GLuint g_loaderFilterMin;
GLuint g_loaderFilterMag;
GLuint g_loaderAnisotropy;

/*
 *	Initializes default loader params
 **/
void SetupDefaultLoaderParams()
{
	g_loaderWrap_s=GL_REPEAT;
	g_loaderWrap_t=GL_REPEAT;

	g_loaderFilterMin=GL_LINEAR_MIPMAP_LINEAR;
	g_loaderFilterMag=GL_LINEAR;
	
}


/*
 *	Determines image type by first signature bytes
 **/
eImageTypes DetermineImageType(byte * pBuffer)
{
	byte png_sig[] = {137,80,78,71,13,10,26,10};
	if (!memcmp(pBuffer,png_sig,8))	
		return IMG_PNG;

	return IMG_UNK;
}

MemoryPool * g_pImgMemoryPool;

/*
 *	Allocates buffer for raw image
 **/
rawImage_t * AllocRawImage(int sz)
{
	rawImage_t*pRet=(rawImage_t*)g_pImgMemoryPool->Alloc(sizeof(rawImage_t));
	pRet->pBuffer=(byte*)g_pImgMemoryPool->Alloc(sz);
	return pRet;
}

/*
 *	Frees data used by raw image
 **/
void FreeRawImage(rawImage_t * p)
{
	g_pImgMemoryPool->Free(p->pBuffer);
	g_pImgMemoryPool->Free(p);
}

/*
 *	Allocates new GL texture
 **/
gl_texture_t*AllocGLTexture(int pref_id = -1)
{
	gl_texture_t*pRet=(gl_texture_t*)g_pImgMemoryPool->Alloc(sizeof(gl_texture_t));

	if (pref_id==-1)
	{
		glGenTextures(1,&pRet->texID);
	}
	else pRet->texID=pref_id;
		

	g_TexturesPool.push_back(pRet);

	return pRet;
}

/*
 *	Initiliazes image lib
 **/
void InitImageLib()
{
	g_pImgMemoryPool = g_pPlatform->MemoryPools()->AllocSubPool(_T("ImageLib"));
	SetupDefaultLoaderParams();
}

/*
 *	Shuts down image lib
 **/
void ShutdownImageLib()
{
	/*
	 *	Free textures pool here
	 **/

	g_pImgMemoryPool->UnlinkFromParent();
	delete g_pImgMemoryPool;
}

/*
 *	Loads image
 **/
gl_texture_t * Img_Load(TCHAR * szName,byte * pSource,size_t bufferSize,bool keepRaw)
{
	eImageTypes type = DetermineImageType(pSource);

	rawImage_t * pImage = nullptr;
	switch(type)
	{
	case IMG_PNG:
		pImage = Img_LoadPng(pSource,bufferSize);
		break;
	}

	if (pImage == nullptr) 
		return nullptr;

	gl_texture_t * pTexture = AllocGLTexture();
	Sys_SafeStrCopy(pTexture->strTextureName,szName,ARRAY_SIZE(pTexture->strTextureName));
	pTexture->width = pImage->iWidth;
	pTexture->height = pImage->iHeight;
	pTexture->format = pImage->bpp == 4 ? GL_RGBA : GL_RGB;
	pTexture->source = pImage;
	pTexture->internal_format = pTexture->format;


	glBindTexture(GL_TEXTURE_2D,pTexture->texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, g_loaderWrap_s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, g_loaderWrap_t);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, g_loaderFilterMag);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, g_loaderFilterMin); 

	if (g_loaderFilterMin == GL_LINEAR_MIPMAP_LINEAR)
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);

	glTexImage2D(GL_TEXTURE_2D,0,pTexture->internal_format,pTexture->width,pTexture->height,0,pTexture->format,GL_UNSIGNED_BYTE,pImage->pBuffer);


	if (!keepRaw)
	{
		FreeRawImage(pImage);
		pTexture->source = 0;
	}

	return pTexture;

}

