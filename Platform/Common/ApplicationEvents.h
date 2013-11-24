/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	ApplicationEvents.h - event definitions
 *
 **/

#ifndef APPLICATION_EVENTS_H
#define APPLICATION_EVENTS_H

namespace ME_Framework
{
	enum class eventTypes
	{
		EV_KB_KEY_DOWN = 0,
		EV_KB_KEY_UP,
		EV_MOUSE_KEY_DOWN,
		EV_MOUSE_KEY_UP,
		EV_MOUSE_SCROLL,
		EV_WINDOW_RESIZE,

		
	};

	typedef struct  appEvent_s
	{
		eventTypes eventid;

		int iParam1;
		int iParam2;
		int iParam3;
		int iParam4;

		unsigned int uParam1;
		unsigned int uParam2;
		unsigned int uParam3;
		unsigned int uParam4;

		float fParam1;
		float fParam2;
		float fParam3;
		float fParam4;

		ME_Math::Vector3D vParam1;
		ME_Math::Vector3D vParam2;
		ME_Math::Vector3D vParam3;
		ME_Math::Vector3D vParam4;

		short sParam1;
		short sParam2;
		short sParam3;
		short sParam4;

		void * pParam1;
		void * pParam2;
		void * pParam3;
		void * pParam4;

	}appEvent_t;
}

#endif