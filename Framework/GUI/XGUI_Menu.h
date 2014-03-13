/*
*	Molotok Editor, 2014 (C) PrinceOfDispersia
*	XGUI_Menu.h - menu classes
*
**/

namespace ME_Framework
{

	namespace ME_XGUI
	{
		class XGUI_MenuItem: public XGUI_Widget
		{
			bool m_bPressed;


			void DrawHoverOverlay();
			void DrawTextLabel();
		public:
			XGUI_MenuItem(xgRect_t & rect);
			~XGUI_MenuItem();

			virtual void HandleEvent(ME_Framework::appEvent_t & ev);
			virtual void DrawComponent();
		};

	}

}