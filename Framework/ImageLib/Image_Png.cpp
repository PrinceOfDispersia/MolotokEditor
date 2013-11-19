/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	Image_Png.cpp - png image loader
 *
 **/

#include <Platform/Common/ApplicationCommon.h>
#include "ImageLib.h"
#include <Libs/libpng/png.h>

size_t png_read_offset=0;
byte*png_read_buffer;

void Img_PngErrorFunc(png_structp png_ptr,png_const_charp error_msg)
{
	//_tprintf(_T("libpng error(): %s"),error_msg);
}

void Img_PngWarningFunc(png_structp png_ptr,png_const_charp warning_msg)
{
	//Con_Printf("libPng warning(): %s",warning_msg);
}

void Img_PngReadFunc(png_structp png_ptr, png_bytep data, png_size_t length)
{
	memcpy(data,&png_read_buffer[png_read_offset],length);
	png_read_offset+=length;
}

rawImage_t * Img_LoadPng(byte * pData,size_t bufferSize)
{
	png_read_offset=4;
	png_read_buffer=pData;

	bool IsValid=!png_sig_cmp(pData, 0, 4);

	if (!IsValid)
		return 0;

	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0,Img_PngErrorFunc,Img_PngWarningFunc);

	if (!png_ptr)
	{		
		return 0;
	}

	png_set_sig_bytes(png_ptr,4);

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		png_destroy_read_struct(&png_ptr,(png_infopp)NULL, (png_infopp)NULL);
		return 0;
	}

	png_infop end_info = png_create_info_struct(png_ptr);
	if (!end_info)
	{
		png_destroy_read_struct(&png_ptr, &info_ptr,(png_infopp)NULL);
		return 0;
	}

	if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_read_struct(&png_ptr, &info_ptr,&end_info);
		return 0;
	}

	png_set_keep_unknown_chunks(png_ptr, PNG_HANDLE_CHUNK_AS_DEFAULT,0,0);
	png_set_read_fn(png_ptr,0, Img_PngReadFunc);
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

	int format;
	int bpp = 0;

	switch(png_get_color_type(png_ptr,info_ptr))
	{
	case PNG_COLOR_TYPE_RGB:
		format=GL_RGB;
		bpp = 24;
		break;
	case PNG_COLOR_TYPE_RGB_ALPHA:
		format=GL_RGBA;
		bpp = 32;
		break;
	default:
		_tprintf(_T("Img_LoadPng(): Only 24/32 bpp images supported\n"));
		png_destroy_read_struct(&png_ptr, &info_ptr,&end_info);
		return 0;
	}

	rawImage_t*pRet=AllocRawImage(bpp/8*png_get_image_width(png_ptr,info_ptr)*png_get_image_height(png_ptr,info_ptr));


	unsigned char ** rows=png_get_rows(png_ptr,info_ptr);

	int row_size=(png_get_image_width(png_ptr,info_ptr)*bpp/8);

	for(size_t i=0;i<png_get_image_height(png_ptr,info_ptr);i++)
	{
		memcpy(&pRet->pBuffer[i*row_size],rows[png_get_image_height(png_ptr,info_ptr)-i-1],row_size);
	}	

	pRet->iWidth=png_get_image_width(png_ptr,info_ptr);
	pRet->iHeight=png_get_image_height(png_ptr,info_ptr);
	pRet->bpp = bpp / 8;
	

	png_destroy_read_struct(&png_ptr, &info_ptr,&end_info);

	return pRet;
}

