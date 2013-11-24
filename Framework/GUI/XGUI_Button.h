/*
 *	Molotok Editor, 2013 (C) PrinceOfDispersia
 *	XGUI_Button.h - generic button class
 *
 **/

namespace ME_Framework
{

	namespace ME_XGUI
	{
		class XGUI_Button: public XGUI_Widget
		{
		public:
			XGUI_Button(xgRect_t * rect);
			~XGUI_Button();

			virtual void HandleEvent(ME_Framework::appEvent_t * pEvent);
			virtual void DrawComponent();

		};

	}

}