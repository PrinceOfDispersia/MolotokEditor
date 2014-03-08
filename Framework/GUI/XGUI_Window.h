/*
 *	Molotok Editor, 2014 (C) PrinceOfDispersia
 *	XGUI_Widget.h - basic gui window class
 *
 **/

namespace ME_Framework
{
	namespace ME_XGUI
	{
		enum eWindowButtons
		{
			btnClose,
			btnMaximize,
			btnMinimize,
			btnRestore
		};

		/* Basic window class */
		class XGUI_Window: public XGUI_Widget
		{
			
			void CheckBorders();
		public:
			
			XGUI_Window(xgRect_t & r);
			~XGUI_Window();

			virtual void HandleEvent( ME_Framework::appEvent_t & pEvent );
			virtual void DrawComponent();
			virtual void Think();
		};
	}
}