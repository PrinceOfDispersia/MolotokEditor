/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	XGUI_Common.h - common GUI definitions
 *
 **/

#ifndef XGUI_COMMON_H
#define XGUI_COMMON_H

namespace ME_Framework
{
	namespace ME_XGUI
	{
		typedef struct xgRect_s 
		{
			ME_Math::Vector2D pos;
			ME_Math::Vector2D ext;
		}xgRect_t;
	}
}

#endif