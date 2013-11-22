/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	XGUI_Sheet.cpp - image sheet class
 *
 */

#include <Platform/Common/ApplicationCommon.h>

using namespace ME_Framework::ME_XGUI;

/*
 *	Sheet constructor
 **/
XGUI_Sheet::XGUI_Sheet(byte * pBuffer,size_t buffSize)
{
	m_pBuffer = (byte*)g_pPlatform->MemoryPools()->Alloc(buffSize);
	m_buffSize = buffSize;

	memcpy(m_pBuffer,pBuffer,buffSize);



}

/*	
 *	Desctructor
 **/
XGUI_Sheet::~XGUI_Sheet()
{

}
